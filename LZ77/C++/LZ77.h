#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <string>
#include <fstream>

class LZ77 {
public:
    using U8  = uint8_t;
    using U16 = uint16_t;
    using U32 = uint32_t;

    static constexpr int EXCESS       = 16;
    static constexpr int WINDOW_BITS  = 17;
    static constexpr int WINDOW_SIZE  = 1 << WINDOW_BITS;
    static constexpr int WINDOW_MASK  = WINDOW_SIZE - 1;
    static constexpr int MIN_MATCH    = 4;
    static constexpr int HASH_BITS    = 18;
    static constexpr int HASH_SIZE    = 1 << HASH_BITS;
    static constexpr int NIL          = -1;

    static constexpr U32 MAGIC        = 0x215A4C55;
    static constexpr size_t BLOCK_SIZE = 1 << 24;   

private:
    int Head[HASH_SIZE] = {};
    int Tail[WINDOW_SIZE] = {};

    template<typename T>
    static T Min(T a, T b) { return a < b ? a : b; }

    template<typename T>
    static T Max(T a, T b) { return a > b ? a : b; }

    static U16 UnalignedLoad16(const void* p) { return *static_cast<const U16*>(p); }
    static U32 UnalignedLoad32(const void* p) { return *static_cast<const U32*>(p); }

    static void UnalignedStore16(void* p, const U16 x) { *static_cast<U16*>(p) = x; }
    static void UnalignedCopy32(void* d, const void* s) { *static_cast<U32*>(d) = UnalignedLoad32(s); }

    static void WildCopy(U8* d, const U8* s, const int n) {
        UnalignedCopy32(d, s);
        UnalignedCopy32(d + 4, s + 4);
        for (int i = 8; i < n; i += 8) {
            UnalignedCopy32(d + i, s + i);
            UnalignedCopy32(d + i + 4, s + i + 4);
        }
    }

    static U32 Hash32(const void* p) {
        return UnalignedLoad32(p) * 0x9E3779B9u >> (32 - HASH_BITS);
    }

    static void EncodeMod(U8*& p, U32 x) {
        while (x >= 128) {
            x -= 128;
            *p++ = 128 + (x & 127);
            x >>= 7;
        }
        *p++ = x;
    }

    static U32 DecodeMod(U8*& p) {
        U32 x = 0;
        for (int i = 0; i <= 28; i += 7) {
            const int c = *p++;
            x += c << i;
            if (c < 128) break;
        }
        return x;
    }

public:
    size_t compressBuffer(const U8* in, const size_t in_len, U8* out, const int level = 4) {
        const int max_chain = (level < 8) ? 1 << level : WINDOW_SIZE;
        U8* op = out;

        for (int i = 0; i < HASH_SIZE; ++i) Head[i] = NIL;

        int run = 0;
        size_t p = 0;

        while (p < in_len) {
            int best_len = MIN_MATCH - 1;
            int dist = 0;

            if (const size_t max_match = in_len - p; max_match >= MIN_MATCH) {
                const int limit = Max(static_cast<int>(p) - WINDOW_SIZE, NIL);
                int chain_len = max_chain;
                int s = Head[Hash32(&in[p])];

                while (s > limit) {
                    if (in[s + best_len] == in[p + best_len] &&
                        UnalignedLoad32(&in[s]) == UnalignedLoad32(&in[p])) {
                        int len = MIN_MATCH;
                        while (len < static_cast<int>(max_match) && in[s + len] == in[p + len]) ++len;
                        if (len > best_len) { best_len = len; dist = static_cast<int>(p - s); }
                    }
                    if (!--chain_len) break;
                    s = Tail[s & WINDOW_MASK];
                }
            }

            if (best_len >= MIN_MATCH) {
                const int len = best_len - MIN_MATCH;
                const int tmp = (dist >> 12 & 16) + Min(len, 15);

                if (run) {
                    if (run >= 7) { *op++ = (7 << 5) + tmp; EncodeMod(op, run - 7); }
                    else *op++ = (run << 5) + tmp;
                    WildCopy(op, &in[p - run], run);
                    op += run;
                    run = 0;
                } else *op++ = tmp;

                if (len >= 15) EncodeMod(op, len - 15);
                UnalignedStore16(op, dist); op += 2;

                while (best_len--) {
                    const U32 h = Hash32(&in[p]);
                    Tail[p & WINDOW_MASK] = Head[h];
                    Head[h] = static_cast<int>(p++);
                }
            } else {
                ++run;
                const U32 h = Hash32(&in[p]);
                Tail[p & WINDOW_MASK] = Head[h];
                Head[h] = static_cast<int>(p++);
            }
        }

        if (run) {
            if (run >= 7) { *op++ = 7 << 5; EncodeMod(op, run - 7); }
            else *op++ = run << 5;
            WildCopy(op, &in[p - run], run);
            op += run;
        }

        return op - out;
    }

    static size_t decompressBuffer(const U8* in, const size_t in_len, U8* out, const size_t out_len) {
        U8* op = out;
        const U8* ip = in;
        const U8* ip_end = ip + in_len;
        const U8* op_end = op + out_len;

        while (ip < ip_end) {
            const int tag = *ip++;
            if (tag >= 32) {
                int run = tag >> 5;
                if (run == 7) run += DecodeMod(const_cast<U8*&>(ip));
                if (op + run > op_end) return -1;
                WildCopy(op, ip, run); op += run; ip += run;
                if (ip >= ip_end) break;
            }

            int len = (tag & 15) + MIN_MATCH;
            if (len == 15 + MIN_MATCH) len += DecodeMod(const_cast<U8*&>(ip));
            if (op + len > op_end) return -1;

            const int dist = ((tag & 16) << 12) + UnalignedLoad16(ip);
            ip += 2;
            const U8* cp = op - dist;
            if (cp < out) return -1;

            if (dist >= 4) { WildCopy(op, cp, len); op += len; }
            else { while (len--) *op++ = *cp++; }
        }
        return ip == ip_end ? op - out : -1;
    }

    void compressFile(const std::string& inFile, const std::string& outFile, int level = 4) {
        std::ifstream in(inFile, std::ios::binary);
        std::ofstream out(outFile, std::ios::binary);
        if (!in || !out) throw std::runtime_error("File open failed");

        U32 magic = MAGIC;
        out.write(reinterpret_cast<char*>(&magic), sizeof(magic));

        std::vector<U8> inBuf(BLOCK_SIZE + EXCESS);
        std::vector<U8> outBuf(BLOCK_SIZE * 2);

        while (in) {
            in.read(reinterpret_cast<char*>(inBuf.data()), BLOCK_SIZE);
            size_t raw_len = static_cast<size_t>(in.gcount());
            if (raw_len == 0) break;
            size_t comp_len = compressBuffer(inBuf.data(), raw_len, outBuf.data(), level);
            out.write(reinterpret_cast<char*>(&comp_len), sizeof(comp_len));
            out.write(reinterpret_cast<char*>(outBuf.data()), comp_len);
        }
    }

    static void decompressFile(const std::string& inFile, const std::string& outFile) {
        std::ifstream in(inFile, std::ios::binary);
        std::ofstream out(outFile, std::ios::binary);
        if (!in || !out) throw std::runtime_error("File open failed");

        U32 magic;
        in.read(reinterpret_cast<char*>(&magic), sizeof(magic));
        if (magic != MAGIC) throw std::runtime_error("Not ULZ format");

        std::vector<U8> inBuf(BLOCK_SIZE + EXCESS);
        std::vector<U8> outBuf(BLOCK_SIZE + EXCESS);

        while (in) {
            int comp_len;
            in.read(reinterpret_cast<char*>(&comp_len), sizeof(comp_len));
            if (!in) break;
            in.read(reinterpret_cast<char*>(inBuf.data()), comp_len);
            size_t out_len = decompressBuffer(inBuf.data(), comp_len, outBuf.data(), BLOCK_SIZE);
            if (out_len == static_cast<size_t>(-1)) throw std::runtime_error("Corrupt stream");
            out.write(reinterpret_cast<char*>(outBuf.data()), out_len);
        }
    }
};
