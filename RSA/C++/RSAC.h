#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

class RSA
{
public:
    explicit RSA(const unsigned bits = 512)
    {
      generate_keys(bits);
    }

    static void generate_keys(unsigned bits = 512)
    {
        if (bits < 64)
        {
            bits = 64;
        }

        p = generate_prime(bits / 2);
        q = generate_prime(bits - bits / 2);

        n = p * q;

        size_t phi = (p - 1) * (q - 1);

        e = 65537;

        if (gcd(e, phi) != 1)
        {
            e = 3;
            while (gcd(e, phi) != 1)
            {
                e += 2;
            }
        }

        d = modinv(e, phi);
        if (d == 0)
        {
            throw std::runtime_error("failed to compute modular inverse for d");
        }

        block_bytes = static_cast<size_t>((msb(n) - 1) / 8);
    }

    void set_public(const size_t& N, const size_t& E)
    {
        n = N;
        e = E;
        block_bytes = std::max<size_t>(1, (msb(n) - 1) / 8);
    }

    void set_private(const size_t& N, const size_t& D)
    {
        n = N;
        d = D;
        block_bytes = std::max<size_t>(1, (msb(n) - 1) / 8);
    }

    std::string encode(const std::string& plaintext) const
    {
        if (n == 0 || e == 0)
        {
            throw std::runtime_error("public key not set");
        }

        std::ostringstream out;
        bool first = true;

        for (size_t i = 0; i < plaintext.size(); i += block_bytes)
        {
            const size_t len = std::min(block_bytes, plaintext.size() - i);
            std::string chunk = plaintext.substr(i, len);

            size_t m = string_to_int(chunk);
            if (m >= n)
            {
                throw std::runtime_error("message block >= modulus");
            }

            size_t c = powmod(m, e, n);
            if (!first)
            {
                out << ' ';
            }

            out << to_hex(c);
            first = false;
        }
        return out.str();
    }

    std::string decode(const std::string& ciphertext) const
    {
        if (n == 0 || d == 0)
        {
            throw std::runtime_error("private key not set");
        }

        std::istringstream iss(ciphertext);

        std::string token;
        std::string result;

        while (iss >> token)
        {
            size_t c = from_hex(token);
            size_t m = powmod(c, d, n);

            std::string chunk = int_to_string(m, block_bytes);
            result += chunk;
        }
        return result;
    }

    size_t get_n() const
    {
        return n;
    }
    size_t get_e() const
    {
        return e;
    }
    size_t get_d() const
    {
        return d;
    }

private:
    cpp_int p{0}, q{0}, n{0}, e{0}, d{0};
    size_t block_bytes = 1;

    // Compute number of bits (msb position)
    static unsigned msb(const cpp_int& x) {
        if (x == 0) return 0;
        return boost::multiprecision::msb(x) + 1;
    }

    // Modular exponentiation
    static cpp_int powmod(cpp_int base, cpp_int exp, const cpp_int& mod) {
        cpp_int res = 1;
        base %= mod;
        while (exp > 0) {
            if ((exp & 1) != 0) res = (res * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        return res;
    }

    // GCD
    static cpp_int gcd(cpp_int a, cpp_int b) {
        while (b != 0) {
            cpp_int r = a % b;
            a = b;
            b = r;
        }
        return a >= 0 ? a : -a;
    }

    // Extended GCD
    static cpp_int egcd(cpp_int a, cpp_int b, cpp_int& x, cpp_int& y) {
        if (b == 0) { x = 1; y = 0; return a; }
        cpp_int x1, y1;
        cpp_int g = egcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - (a / b) * y1;
        return g;
    }

    // Modular inverse
    static cpp_int modinv(cpp_int a, const cpp_int& m) {
        cpp_int x, y;
        cpp_int g = egcd(a, m, x, y);
        if (g != 1 && g != -1) return 0;
        x %= m;
        if (x < 0) x += m;
        return x;
    }

    // Miller-Rabin probabilistic prime test
    bool is_probable_prime(const cpp_int& nval, int iterations = 12) const {
        if (nval < 2) return false;
        static const int small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
        for (int p : small_primes) {
            if (nval == p) return true;
            if (nval % p == 0) return false;
        }

        cpp_int d = nval - 1;
        unsigned s = 0;
        while ((d & 1) == 0) { d >>= 1; ++s; }

        std::mt19937_64 rng(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<uint64_t> dist;

        for (int i = 0; i < iterations; ++i) {
            cpp_int a = cpp_int(dist(rng)) % (nval - 3) + 2;
            cpp_int x = powmod(a, d, nval);
            if (x == 1 || x == nval - 1) continue;
            bool composite = true;
            for (unsigned r = 1; r < s; ++r) {
                x = (x * x) % nval;
                if (x == nval - 1) { composite = false; break; }
            }
            if (composite) return false;
        }
        return true;
    }

    // Generate a probable prime of `bits` length
    cpp_int generate_prime(unsigned bits) const {
        if (bits < 2) bits = 2;
        std::mt19937_64 rng(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<uint64_t> dist;

        while (true) {
            cpp_int x = 0;
            unsigned produced = 0;
            while (produced < bits) {
                x <<= 64;
                x += dist(rng);
                produced += 64;
            }
            unsigned extra = produced - bits;
            if (extra > 0) x >>= extra;
            x |= (cpp_int(1) << (bits - 1));
            x |= 1;
            if (is_probable_prime(x, 16)) return x;
        }
    }

    // Convert raw bytes string to integer
    static cpp_int string_to_int(const std::string& s) {
        cpp_int x = 0;
        for (unsigned char c : std::vector<unsigned char>(s.begin(), s.end())) {
            x <<= 8;
            x += c;
        }
        return x;
    }

    // Convert integer to raw bytes string of fixed length (pad with leading zeros)
    static std::string int_to_string(cpp_int x, size_t bytes) {
        std::vector<unsigned char> v;
        while (x > 0) {
            unsigned char b = static_cast<unsigned char>(x & 0xFF);
            v.push_back(b);
            x >>= 8;
        }
        while (v.size() < bytes) v.push_back(0);
        std::reverse(v.begin(), v.end());
        return std::string(v.begin(), v.end());
    }

    // Integer to hex string (big-endian)
    static std::string to_hex(cpp_int x) {
        if (x == 0) return "0";
        std::ostringstream oss;
        std::vector<unsigned char> v;
        while (x > 0) { v.push_back(static_cast<unsigned char>(x & 0xFF)); x >>= 8; }
        for (auto it = v.rbegin(); it != v.rend(); ++it) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(*it);
        }
        return oss.str();
    }

    // Hex string to integer
    static cpp_int from_hex(const std::string& hex) {
        cpp_int res = 0;
        std::string h = hex;
        if (h.size() % 2 == 1) h = "0" + h;
        for (size_t i = 0; i < h.size(); i += 2) {
            std::string byte = h.substr(i, 2);
            unsigned int val = 0;
            std::istringstream iss(byte);
            iss >> std::hex >> val;
            res <<= 8;
            res += val;
        }
        return res;
    }
};



