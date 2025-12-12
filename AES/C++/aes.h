#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>

#if __cplusplus >= 202002L
  #include <span>
  using std::span;
#else
  template<typename T>
  class span
  {
  public:
    span() : data_(nullptr), size_(0) {}
    span(T* p, const size_t n) : data_(p), size_(n) {}
    explicit span(const std::vector<T>& v) : data_(v.empty() ? nullptr : const_cast<T*>(v.data())), size_(v.size()) {}

    T* data() const
    {
        return data_;
    }
    size_t size() const
    {
        return size_;
    }
    bool empty() const
    {
        return size_ == 0;
    }
    T& operator[](size_t i) const
    {
        return data_[i];
    }
  private:
    T* data_;
    size_t size_;
  };
#endif

namespace aes
{

using u8  = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;

static constexpr size_t AES_BLOCK_SIZE = 16;

inline void secure_zero(void* p, size_t n) noexcept
{
    auto vp = static_cast<volatile u8*>(p);
    while (n--)
    {
        *vp++ = 0;
    }
}

static const std::array<u8, 256> SBOX = {
  0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
  0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
  0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
  0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
  0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
  0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
  0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
  0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
  0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
  0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
  0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
  0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
  0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
  0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
  0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
  0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

static const std::array<u8, 256> INV_SBOX = {
  0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
  0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
  0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
  0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
  0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
  0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
  0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
  0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
  0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
  0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
  0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
  0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
  0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
  0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
  0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
  0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

inline u8 xtime(const u8 x)
{
    return static_cast<u8>(x << 1 ^ (x & 0x80 ? 0x1b : 0x00));
}

inline u8 mul(u8 a, u8 b)
{
    u8 res = 0;
    while (b)
    {
        if (b & 1)
        {
            res ^= a;
        }

        a = xtime(a);
        b >>= 1;
    }
    return res;
}

struct Tables
{
    std::array<u32, 256> Te0, Te1, Te2, Te3;
    std::array<u32, 256> Td0, Td1, Td2, Td3;
    std::array<u8, 256>  Te4;
    std::array<u8, 256>  Td4;
    bool ready = false;

    void init()
    {
        if (ready)
        {
            return;
        }

        for (int i = 0; i < 256; ++i)
        {
            const u8 s = SBOX[i];
            const u8 s2 = mul(s, 2);
            const u8 s3 = static_cast<u8>(s2 ^ s);

            Te0[i] = static_cast<u32>(s2) << 24 | static_cast<u32>(s) << 16 | static_cast<u32>(s) << 8 | static_cast<u32>(s3);
            Te1[i] = static_cast<u32>(s3) << 24 | static_cast<u32>(s2) << 16 | static_cast<u32>(s) << 8 | static_cast<u32>(s);
            Te2[i] = static_cast<u32>(s) << 24 | static_cast<u32>(s3) << 16 | static_cast<u32>(s2) << 8 | static_cast<u32>(s);
            Te3[i] = static_cast<u32>(s) << 24 | static_cast<u32>(s) << 16 | static_cast<u32>(s3) << 8 | static_cast<u32>(s2);

            const u8 is = INV_SBOX[i];
            const u8 i9  = mul(is, 9);
            const u8 i11 = mul(is, 11);
            const u8 i13 = mul(is, 13);
            const u8 i14 = mul(is, 14);

            Td0[i] = (static_cast<u32>(i14) << 24) | (static_cast<u32>(i9) << 16) | (static_cast<u32>(i13) << 8) | (static_cast<u32>(i11));
            Td1[i] = (static_cast<u32>(i11) << 24) | (static_cast<u32>(i14) << 16) | (static_cast<u32>(i9) << 8) | (static_cast<u32>(i13));
            Td2[i] = (static_cast<u32>(i13) << 24) | (static_cast<u32>(i11) << 16) | (static_cast<u32>(i14) << 8) | (static_cast<u32>(i9));
            Td3[i] = (static_cast<u32>(i9) << 24) | (static_cast<u32>(i13) << 16) | (static_cast<u32>(i11) << 8) | (static_cast<u32>(i14));

            Te4[i] = SBOX[i];
            Td4[i] = INV_SBOX[i];
        }
        ready = true;
    }
};

inline Tables &global_tables()
{
    static Tables t;
    if (!t.ready)
    {
        t.init();
    }
    return t;
}


inline u32 rotr32(const u32 x, const int r)
{
    return (x >> r) | (x << (32 - r));
}
inline u32 rotl32(const u32 x, const int r)
{
    return (x << r) | (x >> (32 - r));
}

inline u32 be_load32(const u8* b) {
    return (static_cast<u32>(b[0]) << 24) |
           (static_cast<u32>(b[1]) << 16) |
           (static_cast<u32>(b[2]) << 8) |
           (static_cast<u32>(b[3]));
}
inline void be_store32(u8* b, u32 v) {
    b[0] = static_cast<u8>(v >> 24);
    b[1] = static_cast<u8>(v >> 16);
    b[2] = static_cast<u8>(v >> 8);
    b[3] = static_cast<u8>(v);
}

inline u32 rcon(unsigned int i) {
    u8 c = 1;
    while (i-- > 1) c = xtime(c);
    return static_cast<u32>(c) << 24;
}

template<unsigned int Nk, unsigned int Nr>
class AES_Core {
    static_assert(Nk == 4 || Nk == 6 || Nk == 8, "Nk must be 4,6,8");
public:
    static constexpr unsigned int Nb = 4;
    static constexpr unsigned int KEY_WORDS = Nb * (Nr + 1);

    AES_Core() = delete;
    AES_Core(const span<const u8> key_bytes) {
        if (key_bytes.size() != Nk * 4) {
            throw std::length_error("Invalid key length for this AES variant");
        }
        global_tables();
        key_expansion(key_bytes);
    }

    AES_Core(const AES_Core&) = delete;
    AES_Core& operator=(const AES_Core&) = delete;
    AES_Core(AES_Core&&) = delete;
    AES_Core& operator=(AES_Core&&) = delete;

    ~AES_Core() noexcept {
        secure_zero(round_key_.data(), round_key_.size() * sizeof(round_key_[0]));
    }

    inline void encrypt_block(const u8 in[AES_BLOCK_SIZE], u8 out[AES_BLOCK_SIZE]) const {
        const Tables& T = global_tables();
        u32 s0 = be_load32(in + 0);
        u32 s1 = be_load32(in + 4);
        u32 s2 = be_load32(in + 8);
        u32 s3 = be_load32(in + 12);

        u32 t0 = s0 ^ round_key_[0];
        u32 t1 = s1 ^ round_key_[1];
        u32 t2 = s2 ^ round_key_[2];
        u32 t3 = s3 ^ round_key_[3];

        unsigned int rk = 4;
        for (unsigned int round = 1; round < Nr; ++round) {
            u32 a0 = T.Te0[(t0 >> 24) & 0xFF] ^ T.Te1[(t1 >> 16) & 0xFF] ^ T.Te2[(t2 >> 8) & 0xFF] ^ T.Te3[(t3 >> 0) & 0xFF] ^ round_key_[rk+0];
            u32 a1 = T.Te0[(t1 >> 24) & 0xFF] ^ T.Te1[(t2 >> 16) & 0xFF] ^ T.Te2[(t3 >> 8) & 0xFF] ^ T.Te3[(t0 >> 0) & 0xFF] ^ round_key_[rk+1];
            u32 a2 = T.Te0[(t2 >> 24) & 0xFF] ^ T.Te1[(t3 >> 16) & 0xFF] ^ T.Te2[(t0 >> 8) & 0xFF] ^ T.Te3[(t1 >> 0) & 0xFF] ^ round_key_[rk+2];
            u32 a3 = T.Te0[(t3 >> 24) & 0xFF] ^ T.Te1[(t0 >> 16) & 0xFF] ^ T.Te2[(t1 >> 8) & 0xFF] ^ T.Te3[(t2 >> 0) & 0xFF] ^ round_key_[rk+3];
            t0 = a0; t1 = a1; t2 = a2; t3 = a3;
            rk += 4;
        }

        u8 b0 = T.Te4[(t0 >> 24) & 0xFF];
        u8 b1 = T.Te4[(t1 >> 16) & 0xFF];
        u8 b2 = T.Te4[(t2 >> 8) & 0xFF];
        u8 b3 = T.Te4[(t3 >> 0) & 0xFF];

        u8 c0 = T.Te4[(t1 >> 24) & 0xFF];
        u8 c1 = T.Te4[(t2 >> 16) & 0xFF];
        u8 c2 = T.Te4[(t3 >> 8) & 0xFF];
        u8 c3 = T.Te4[(t0 >> 0) & 0xFF];

        u32 rk0 = round_key_[rk+0];
        u32 rk1 = round_key_[rk+1];
        u32 rk2 = round_key_[rk+2];
        u32 rk3 = round_key_[rk+3];

        out[0]  = static_cast<u8>(b0 ^ (rk0 >> 24));
        out[1]  = static_cast<u8>(b1 ^ (rk0 >> 16));
        out[2]  = static_cast<u8>(b2 ^ (rk0 >> 8));
        out[3]  = static_cast<u8>(b3 ^ (rk0 >> 0));

        out[4]  = static_cast<u8>(c0 ^ (rk1 >> 24));
        out[5]  = static_cast<u8>(c1 ^ (rk1 >> 16));
        out[6]  = static_cast<u8>(c2 ^ (rk1 >> 8));
        out[7]  = static_cast<u8>(c3 ^ (rk1 >> 0));

        out[8]  = static_cast<u8>(T.Te4[(t2 >> 24) & 0xFF] ^ (rk2 >> 24));
        out[9]  = static_cast<u8>(T.Te4[(t3 >> 16) & 0xFF] ^ (rk2 >> 16));
        out[10] = static_cast<u8>(T.Te4[(t0 >> 8) & 0xFF] ^ (rk2 >> 8));
        out[11] = static_cast<u8>(T.Te4[(t1 >> 0) & 0xFF] ^ (rk2 >> 0));

        out[12] = static_cast<u8>(T.Te4[(t3 >> 24) & 0xFF] ^ (rk3 >> 24));
        out[13] = static_cast<u8>(T.Te4[(t0 >> 16) & 0xFF] ^ (rk3 >> 16));
        out[14] = static_cast<u8>(T.Te4[(t1 >> 8) & 0xFF] ^ (rk3 >> 8));
        out[15] = static_cast<u8>(T.Te4[(t2 >> 0) & 0xFF] ^ (rk3 >> 0));
    }

    inline void decrypt_block(const u8 in[AES_BLOCK_SIZE], u8 out[AES_BLOCK_SIZE]) const {
        const Tables& T = global_tables();
        u32 s0 = be_load32(in + 0);
        u32 s1 = be_load32(in + 4);
        u32 s2 = be_load32(in + 8);
        u32 s3 = be_load32(in + 12);

        unsigned int rk = 4 * Nr;
        u32 t0 = s0 ^ round_key_[rk+0];
        u32 t1 = s1 ^ round_key_[rk+1];
        u32 t2 = s2 ^ round_key_[rk+2];
        u32 t3 = s3 ^ round_key_[rk+3];
        rk -= 4;

        for (unsigned int round = Nr - 1; round > 0; --round) {
            u32 a0 = T.Td0[(t0 >> 24) & 0xFF] ^ T.Td1[(t3 >> 16) & 0xFF] ^ T.Td2[(t2 >> 8) & 0xFF] ^ T.Td3[(t1 >> 0) & 0xFF] ^ round_key_[rk+0];
            u32 a1 = T.Td0[(t1 >> 24) & 0xFF] ^ T.Td1[(t0 >> 16) & 0xFF] ^ T.Td2[(t3 >> 8) & 0xFF] ^ T.Td3[(t2 >> 0) & 0xFF] ^ round_key_[rk+1];
            u32 a2 = T.Td0[(t2 >> 24) & 0xFF] ^ T.Td1[(t1 >> 16) & 0xFF] ^ T.Td2[(t0 >> 8) & 0xFF] ^ T.Td3[(t3 >> 0) & 0xFF] ^ round_key_[rk+2];
            u32 a3 = T.Td0[(t3 >> 24) & 0xFF] ^ T.Td1[(t2 >> 16) & 0xFF] ^ T.Td2[(t1 >> 8) & 0xFF] ^ T.Td3[(t0 >> 0) & 0xFF] ^ round_key_[rk+3];
            t0 = a0; t1 = a1; t2 = a2; t3 = a3;
            rk -= 4;
        }

        u32 rk0 = round_key_[0], rk1 = round_key_[1], rk2 = round_key_[2], rk3 = round_key_[3];
        out[0]  = static_cast<u8>(INV_SBOX[(t0 >> 24) & 0xFF] ^ (rk0 >> 24));
        out[1]  = static_cast<u8>(INV_SBOX[(t3 >> 16) & 0xFF] ^ (rk0 >> 16));
        out[2]  = static_cast<u8>(INV_SBOX[(t2 >> 8)  & 0xFF] ^ (rk0 >> 8));
        out[3]  = static_cast<u8>(INV_SBOX[(t1 >> 0)  & 0xFF] ^ (rk0 >> 0));

        out[4]  = static_cast<u8>(INV_SBOX[(t1 >> 24) & 0xFF] ^ (rk1 >> 24));
        out[5]  = static_cast<u8>(INV_SBOX[(t0 >> 16) & 0xFF] ^ (rk1 >> 16));
        out[6]  = static_cast<u8>(INV_SBOX[(t3 >> 8)  & 0xFF] ^ (rk1 >> 8));
        out[7]  = static_cast<u8>(INV_SBOX[(t2 >> 0)  & 0xFF] ^ (rk1 >> 0));

        out[8]  = static_cast<u8>(INV_SBOX[(t2 >> 24) & 0xFF] ^ (rk2 >> 24));
        out[9]  = static_cast<u8>(INV_SBOX[(t1 >> 16) & 0xFF] ^ (rk2 >> 16));
        out[10] = static_cast<u8>(INV_SBOX[(t0 >> 8)  & 0xFF] ^ (rk2 >> 8));
        out[11] = static_cast<u8>(INV_SBOX[(t3 >> 0)  & 0xFF] ^ (rk2 >> 0));

        out[12] = static_cast<u8>(INV_SBOX[(t3 >> 24) & 0xFF] ^ (rk3 >> 24));
        out[13] = static_cast<u8>(INV_SBOX[(t2 >> 16) & 0xFF] ^ (rk3 >> 16));
        out[14] = static_cast<u8>(INV_SBOX[(t1 >> 8)  & 0xFF] ^ (rk3 >> 8));
        out[15] = static_cast<u8>(INV_SBOX[(t0 >> 0)  & 0xFF] ^ (rk3 >> 0));
    }

    std::vector<u8> encrypt_ecb(const std::vector<u8>& in) const {
        if (in.size() % AES_BLOCK_SIZE != 0) throw std::length_error("ECB: plaintext must be multiple of 16 bytes");
        std::vector<u8> out(in.size());
        u8 tmp_in[16], tmp_out[16];
        for (size_t i = 0; i < in.size(); i += 16) {
            std::memcpy(tmp_in, in.data() + i, 16);
            encrypt_block(tmp_in, tmp_out);
            std::memcpy(out.data() + i, tmp_out, 16);
        }
        return out;
    }

    std::vector<u8> decrypt_ecb(const std::vector<u8>& in) const {
        if (in.size() % AES_BLOCK_SIZE != 0) throw std::length_error("ECB: ciphertext must be multiple of 16 bytes");
        std::vector<u8> out(in.size());
        u8 tmp_in[16], tmp_out[16];
        for (size_t i = 0; i < in.size(); i += 16) {
            std::memcpy(tmp_in, in.data() + i, 16);
            decrypt_block(tmp_in, tmp_out);
            std::memcpy(out.data() + i, tmp_out, 16);
        }
        return out;
    }

    std::vector<u8> encrypt_cbc(const std::vector<u8>& plaintext, const std::array<u8,16>& iv) const {
        size_t psize = plaintext.size();
        size_t padlen = AES_BLOCK_SIZE - (psize % AES_BLOCK_SIZE);
        std::vector<u8> buffer;
        buffer.reserve(psize + padlen);
        buffer.insert(buffer.end(), plaintext.begin(), plaintext.end());
        buffer.insert(buffer.end(), padlen, static_cast<u8>(padlen));

        std::vector<u8> out(buffer.size());
        std::array<u8,16> state = iv;
        u8 tmp_block[16], xored[16], enc[16];
        for (size_t i = 0; i < buffer.size(); i += 16) {
            for (size_t j = 0; j < 16; ++j) xored[j] = static_cast<u8>(buffer[i+j] ^ state[j]);
            encrypt_block(xored, enc);
            std::memcpy(out.data() + i, enc, 16);
            std::memcpy(state.data(), enc, 16);
        }
        return out;
    }

    std::vector<u8> decrypt_cbc(const std::vector<u8>& ciphertext, const std::array<u8,16>& iv) const {
        if (ciphertext.size() % AES_BLOCK_SIZE != 0) throw std::length_error("CBC: ciphertext size invalid");
        std::vector<u8> tmp(ciphertext.size());
        std::array<u8,16> prev = iv;
        u8 dec[16], xored[16];
        for (size_t i = 0; i < ciphertext.size(); i += 16) {
            decrypt_block(ciphertext.data() + i, dec);
            for (size_t j = 0; j < 16; ++j) xored[j] = static_cast<u8>(dec[j] ^ prev[j]);
            std::memcpy(tmp.data() + i, xored, 16);
            std::memcpy(prev.data(), ciphertext.data() + i, 16);
        }
        if (tmp.empty()) return tmp;
        u8 pad = tmp.back();
        if (pad == 0 || pad > 16) throw std::runtime_error("Invalid PKCS#7 padding");
        for (size_t i = tmp.size() - pad; i < tmp.size(); ++i) if (tmp[i] != pad) throw std::runtime_error("Invalid PKCS#7 padding");
        tmp.resize(tmp.size() - pad);
        return tmp;
    }

    std::vector<u8> encrypt_cfb(const std::vector<u8>& plaintext, const std::array<u8,16>& iv) const {
        if (plaintext.empty()) return {};
        std::vector<u8> out(plaintext.size());
        std::array<u8,16> reg = iv;
        u8 stream[16];
        for (size_t i = 0; i < plaintext.size(); i += 16) {
            encrypt_block(reg.data(), stream);
            size_t chunk = std::min<size_t>(16, plaintext.size() - i);
            for (size_t j = 0; j < chunk; ++j) {
                out[i+j] = static_cast<u8>(plaintext[i+j] ^ stream[j]);
            }
            if (chunk == 16) {
                std::memcpy(reg.data(), out.data() + i, 16);
            } else {
                std::array<u8,16> newreg{};
                size_t rem = 16 - chunk;
                for (size_t k = 0; k < rem; ++k) newreg[k] = reg[k+chunk];
                for (size_t k = 0; k < chunk; ++k) newreg[rem + k] = out[i+k];
                reg = newreg;
            }
        }
        return out;
    }

    std::vector<u8> decrypt_cfb(const std::vector<u8>& ciphertext, const std::array<u8,16>& iv) const {
        if (ciphertext.empty()) return {};
        std::vector<u8> out(ciphertext.size());
        std::array<u8,16> reg = iv;
        u8 stream[16];
        for (size_t i = 0; i < ciphertext.size(); i += 16) {
            encrypt_block(reg.data(), stream);
            size_t chunk = std::min<size_t>(16, ciphertext.size() - i);
            for (size_t j = 0; j < chunk; ++j) {
                out[i+j] = static_cast<u8>(ciphertext[i+j] ^ stream[j]);
            }
            if (chunk == 16) {
                std::memcpy(reg.data(), ciphertext.data() + i, 16);
            } else {
                std::array<u8,16> newreg{};
                size_t rem = 16 - chunk;
                for (size_t k = 0; k < rem; ++k) newreg[k] = reg[k+chunk];
                for (size_t k = 0; k < chunk; ++k) newreg[rem + k] = ciphertext[i+k];
                reg = newreg;
            }
        }
        return out;
    }

    std::vector<u8> xor_stream_ofb(const std::vector<u8>& input, std::array<u8,16> iv) const {
        if (input.empty()) return {};
        std::vector<u8> out(input.size());
        u8 stream[16];
        for (size_t i = 0; i < input.size(); i += 16) {
            encrypt_block(iv.data(), stream);
            std::memcpy(iv.data(), stream, 16);
            size_t chunk = std::min<size_t>(16, input.size() - i);
            for (size_t j = 0; j < chunk; ++j) out[i+j] = static_cast<u8>(input[i+j] ^ stream[j]);
        }
        return out;
    }

    std::vector<u8> xor_stream_ctr(const std::vector<u8>& input, std::array<u8,16> nonce_counter) const {
        if (input.empty()) return {};
        std::vector<u8> out(input.size());
        u8 stream[16];
        for (size_t i = 0; i < input.size(); i += 16) {
            encrypt_block(nonce_counter.data(), stream);
            size_t chunk = std::min<size_t>(16, input.size() - i);
            for (size_t j = 0; j < chunk; ++j) out[i+j] = static_cast<u8>(input[i+j] ^ stream[j]);
            for (int k = 15; k >= 0; --k) {
                if (++nonce_counter[k] != 0) break;
            }
        }
        return out;
    }

private:
    std::array<u32, KEY_WORDS> round_key_{};

    void key_expansion(const span<const u8> key) {
        unsigned int key_words = Nk;
        for (unsigned int i = 0; i < key_words; ++i) {
            round_key_[i] = be_load32(key.data() + 4*i);
        }
        unsigned int i = key_words;
        while (i < KEY_WORDS) {
            u32 temp = round_key_[i-1];
            if (i % key_words == 0) {
                temp = (static_cast<u32>(SBOX[(temp >> 16) & 0xFF]) << 24)
                     | (static_cast<u32>(SBOX[(temp >> 8) & 0xFF]) << 16)
                     | (static_cast<u32>(SBOX[(temp >> 0) & 0xFF]) << 8)
                     | (static_cast<u32>(SBOX[(temp >> 24) & 0xFF]) << 0);
                temp ^= rcon(i / key_words);
            } else if (key_words > 6 && (i % key_words) == 4) {
                temp = (static_cast<u32>(SBOX[(temp >> 24) & 0xFF]) << 24)
                     | (static_cast<u32>(SBOX[(temp >> 16) & 0xFF]) << 16)
                     | (static_cast<u32>(SBOX[(temp >> 8) & 0xFF]) << 8)
                     | (static_cast<u32>(SBOX[(temp >> 0) & 0xFF]) << 0);
            }
            round_key_[i] = round_key_[i - key_words] ^ temp;
            ++i;
        }
    }
};

using AES128 = AES_Core<4, 10>;
using AES192 = AES_Core<6, 12>;
using AES256 = AES_Core<8, 14>;

class AES
{
public:
    enum class Variant
    {
        AES128, AES192, AES256
    };

    explicit AES(const std::vector<u8>& key, const Variant v = Variant::AES256)
    {
        switch (v)
        {
            case Variant::AES128:
                if (key.size() != 16) throw std::length_error("AES-128 key must be 16 bytes");
                core128_ = std::make_unique<AES128>(span<const u8>(key));
                break;
            case Variant::AES192:
                if (key.size() != 24) throw std::length_error("AES-192 key must be 24 bytes");
                core192_ = std::make_unique<AES192>(span<const u8>(key));
                break;
            case Variant::AES256:
                if (key.size() != 32) throw std::length_error("AES-256 key must be 32 bytes");
                core256_ = std::make_unique<AES256>(span<const u8>(key));
                break;
        }
        variant_ = v;
    }

    ~AES() noexcept
    {

    }

    AES(const AES&) = delete;
    AES& operator=(const AES&) = delete;

    std::vector<u8> encrypt_ecb(const std::vector<u8>& in) const
    {
        return dispatch([&](auto& core)
        {
            return core.encrypt_ecb(in);
        });
    }
    std::vector<u8> decrypt_ecb(const std::vector<u8>& in) const
    {
        return dispatch([&](auto& core)
        {
            return core.decrypt_ecb(in);
        });
    }

    std::vector<u8> encrypt_cbc(const std::vector<u8>& in, const std::array<u8,16>& iv) const
    {
        return dispatch([&](auto& core)
        {
            return core.encrypt_cbc(in, iv);
        });
    }
    std::vector<u8> decrypt_cbc(const std::vector<u8>& in, const std::array<u8,16>& iv) const
    {
        return dispatch([&](auto& core)
        {
            return core.decrypt_cbc(in, iv);
        });
    }

    std::vector<u8> encrypt_cfb(const std::vector<u8>& in, const std::array<u8,16>& iv) const
    {
        return dispatch([&](auto& core)
        {
            return core.encrypt_cfb(in, iv);
        });
    }

    std::vector<u8> decrypt_cfb(const std::vector<u8>& in, const std::array<u8,16>& iv) const
    {
        return dispatch([&](auto& core)
        {
            return core.decrypt_cfb(in, iv);
        });
    }

    std::vector<u8> ofb_xor(const std::vector<u8>& in, const std::array<u8,16>& iv) const
    {
        return dispatch([&](auto& core)
        {
            return core.xor_stream_ofb(in, iv);
        });
    }

    std::vector<u8> ctr_xor(const std::vector<u8>& in, const std::array<u8,16>& nonce_counter) const
    {
        return dispatch([&](auto& core)
        {
            return core.xor_stream_ctr(in, nonce_counter);
        });
    }

private:
    Variant variant_ = Variant::AES256;
    std::unique_ptr<AES128> core128_;
    std::unique_ptr<AES192> core192_;
    std::unique_ptr<AES256> core256_;

    template<typename Fn>
    auto dispatch(Fn f) const -> decltype(f(*core256_))
    {
        switch (variant_)
        {
            case Variant::AES128: return f(*core128_);
            case Variant::AES192: return f(*core192_);
            default: return f(*core256_);
        }
    }
};

}

