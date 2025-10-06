#ifndef ECC_MATH_H_
#define ECC_MATH_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define secp128r1 16
#define secp192r1 24
#define secp256r1 32
#define secp384r1 48

#ifndef ECC_CURVE
    #define ECC_CURVE secp256r1
#endif

#if (ECC_CURVE != secp128r1 && ECC_CURVE != secp192r1 && ECC_CURVE != secp256r1 && ECC_CURVE != secp384r1)
    #error "ECC_CURVE must be one of secp128r1, secp192r1, secp256r1, secp384r1"
#endif

#define ECC_BYTES ECC_CURVE
#define NUM_ECC_DIGITS (ECC_BYTES/8)
#define MAX_TRIES 16

typedef unsigned int ecc_uint;

#if defined(__SIZEOF_INT128__) || (defined(__clang_major__) && ((__clang_major__ * 100 + __clang_minor__) >= 302))
    #define ECC_SUPPORTS_INT128 1
#else
    #define ECC_SUPPORTS_INT128 0
#endif

#if ECC_SUPPORTS_INT128
    typedef unsigned __int128 ecc_uint128_t;
#else
    typedef struct { uint64_t low; uint64_t high; } ecc_uint128_t;
#endif

#define CURVE_P_16 {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFDFFFFFFFFULL}
#define CURVE_P_24 {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFEULL, 0xFFFFFFFFFFFFFFFFULL}
#define CURVE_P_32 {0xFFFFFFFFFFFFFFFFULL, 0x00000000FFFFFFFFULL, 0x0000000000000000ULL, 0xFFFFFFFF00000001ULL}
#define CURVE_P_48 {0x00000000FFFFFFFFULL, 0xFFFFFFFF00000000ULL, 0xFFFFFFFFFFFFFFFEULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL}

#define CURVE_B_16 {0xD824993C2CEE5ED3ULL, 0xE87579C11079F43DULL}
#define CURVE_B_24 {0xFEB8DEECC146B9B1ULL, 0x0FA7E9AB72243049ULL, 0x64210519E59C80E7ULL}
#define CURVE_B_32 {0x3BCE3C3E27D2604BULL, 0x651D06B0CC53B0F6ULL, 0xB3EBBD55769886BCULL, 0x5AC635D8AA3A93E7ULL}
#define CURVE_B_48 {0x2A85C8EDD3EC2AEFULL, 0xC656398D8A2ED19DULL, 0x0314088F5013875AULL, 0x181D9C6EFE814112ULL, 0x988E056BE3F82D19ULL, 0xB3312FA7E23EE7E4ULL}

#define CURVE_G_16 { \
    {0x0C28607CA52C5B86ULL, 0x161FF7528B899B2DULL}, \
    {0xC02DA292DDED7A83ULL, 0xCF5AC8395BAFEB13ULL} }

#define CURVE_G_24 { \
    {0xF4FF0AFD82FF1012ULL, 0x7CBF20EB43A18800ULL, 0x188DA80EB03090F6ULL}, \
    {0x73F977A11E794811ULL, 0x631011ED6B24CDD5ULL, 0x07192B95FFC8DA78ULL} }

#define CURVE_G_32 { \
    {0xF4A13945D898C296ULL, 0x77037D812DEB33A0ULL, 0xF8BCE6E563A440F2ULL, 0x6B17D1F2E12C4247ULL}, \
    {0xCBB6406837BF51F5ULL, 0x2BCE33576B315ECEULL, 0x8EE7EB4A7C0F9E16ULL, 0x4FE342E2FE1A7F9BULL} }

#define CURVE_G_48 { \
    {0x3A545E3872760AB7ULL, 0x5502F25DBF55296CULL, 0x59F741E082542A38ULL, 0x6E1D3B628BA79B98ULL, 0x8EB1C71EF320AD74ULL, 0xAA87CA22BE8B0537ULL}, \
    {0x7A431D7C90EA0E5FULL, 0x0A60B1CE1D7E819DULL, 0xE9DA3113B5F0B8C0ULL, 0xF8F41DBD289A147CULL, 0x5D9E98BF9292DC29ULL, 0x3617DE4A96262C6FULL} }

#define CURVE_N_16 {0x75A30D1B9038A115ULL, 0xFFFFFFFE00000000ULL}
#define CURVE_N_24 {0x146BC9B1B4D22831ULL, 0xFFFFFFFF99DEF836ULL, 0xFFFFFFFFFFFFFFFFULL}
#define CURVE_N_32 {0xF3B9CAC2FC632551ULL, 0xBCE6FAADA7179E84ULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFF00000000ULL}
#define CURVE_N_48 {0xECEC196ACCC52973ULL, 0x581A0DB248B0A77AULL, 0xC7634D81F4372DDFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL}

static const uint64_t ecc_curve_p[NUM_ECC_DIGITS] =
    (uint64_t[]){ CONCAT_CURVE_P() };

#if ECC_CURVE == secp128r1
    static const uint64_t curve_p[NUM_ECC_DIGITS] = CURVE_P_16;
    static const uint64_t curve_b[NUM_ECC_DIGITS] = CURVE_B_16;
    static const uint64_t curve_n[NUM_ECC_DIGITS] = CURVE_N_16;
    static const struct { uint64_t x[NUM_ECC_DIGITS]; uint64_t y[NUM_ECC_DIGITS]; } curve_G = CURVE_G_16;
#elif ECC_CURVE == secp192r1
    static const uint64_t curve_p[NUM_ECC_DIGITS] = CURVE_P_24;
    static const uint64_t curve_b[NUM_ECC_DIGITS] = CURVE_B_24;
    static const uint64_t curve_n[NUM_ECC_DIGITS] = CURVE_N_24;
    static const struct { uint64_t x[NUM_ECC_DIGITS]; uint64_t y[NUM_ECC_DIGITS]; } curve_G = CURVE_G_24;
#elif ECC_CURVE == secp256r1
    static constexpr uint64_t curve_p[NUM_ECC_DIGITS] = CURVE_P_32;
    static constexpr uint64_t curve_b[NUM_ECC_DIGITS] = CURVE_B_32;
    static constexpr uint64_t curve_n[NUM_ECC_DIGITS] = CURVE_N_32;
    static const struct { uint64_t x[NUM_ECC_DIGITS]; uint64_t y[NUM_ECC_DIGITS]; } curve_G = CURVE_G_32;
#elif ECC_CURVE == secp384r1
    static const uint64_t curve_p[NUM_ECC_DIGITS] = CURVE_P_48;
    static const uint64_t curve_b[NUM_ECC_DIGITS] = CURVE_B_48;
    static const uint64_t curve_n[NUM_ECC_DIGITS] = CURVE_N_48;
    static const struct { uint64_t x[NUM_ECC_DIGITS]; uint64_t y[NUM_ECC_DIGITS]; } curve_G = CURVE_G_48;
#else
    #error "Unsupported ECC_CURVE configuration"
#endif

typedef struct {
    uint64_t x[NUM_ECC_DIGITS];
    uint64_t y[NUM_ECC_DIGITS];
} ecc_point_t;


static int ecc_get_random_bytes(uint8_t *out, const size_t len)
{
    if (!out || len == 0) return 0;
    int fd = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
    if (fd == -1) return 0;
    size_t left = len;
    uint8_t *p = out;
    while (left > 0) {
        ssize_t r = read(fd, p, left);
        if (r <= 0) {
            close(fd);
            memset(out, 0, len);
            return 0;
        }
        left -= static_cast<size_t>(r);
        p += r;
    }
    close(fd);
    return 1;
}

static void ecc_vli_clear(uint64_t *v)
{
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) v[i] = 0;
}

static int ecc_vli_is_zero(const uint64_t *v)
{
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) {
        if (v[i]) return 0;
    }
    return 1;
}

static void ecc_vli_set(uint64_t *dest, const uint64_t *src)
{
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) dest[i] = src[i];
}

static int ecc_vli_cmp(const uint64_t *a, const uint64_t *b)
{
    for (int i = NUM_ECC_DIGITS - 1; i >= 0; --i) {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
    }
    return 0;
}

static ecc_uint ecc_vli_num_digits(const uint64_t *v)
{
    for (int i = NUM_ECC_DIGITS - 1; i >= 0; --i) {
        if (v[i]) return static_cast<ecc_uint>(i + 1);
    }
    return 0;
}

static uint32_t ecc_vli_num_bits(const uint64_t *v)
{
    const ecc_uint nd = ecc_vli_num_digits(v);
    if (nd == 0) return 0;
    uint64_t top = v[nd - 1];
    uint32_t bits = 0;
    while (top) { ++bits; top >>= 1; }
    return (nd - 1) * 64 + bits;
}

static int ecc_vli_test_bit(const uint64_t *v, const uint32_t bit)
{
    if (constexpr uint32_t max_bits = NUM_ECC_DIGITS * 64U; bit >= max_bits) return 0;
    const uint32_t idx = bit / 64;
    const uint32_t off = bit % 64;
    return static_cast<int>(v[idx] >> off & 1ULL);
}

static uint64_t ecc_vli_lshift(uint64_t *result, const uint64_t *in, unsigned shift)
{
    uint64_t carry = 0;
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) {
        const uint64_t cur = in[i];
        result[i] = (cur << shift) | carry;
        carry = cur >> (64 - shift);
    }
    return carry;
}

static void ecc_vli_rshift1(uint64_t *v)
{
    uint64_t carry = 0;
    for (int i = NUM_ECC_DIGITS - 1; i >= 0; --i) {
        const uint64_t cur = v[i];
        v[i] = cur >> 1 | carry;
        carry = cur << 63;
    }
}

static uint64_t ecc_vli_add(uint64_t *result, const uint64_t *a, const uint64_t *b)
{
    uint64_t carry = 0;
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) {
        const uint64_t aa = a[i];
        const uint64_t bb = b[i];
        const uint64_t sum = aa + carry;
        const uint64_t carry1 = (sum < aa);
        const uint64_t res = sum + bb;
        const uint64_t carry2 = (res < sum);
        result[i] = res;
        carry = carry1 + carry2;

        if (carry > 1) carry = 1;
    }
    return carry;
}

static uint64_t ecc_vli_sub(uint64_t *result, const uint64_t *a, const uint64_t *b)
{
    uint64_t borrow = 0;
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) {
        const uint64_t aa = a[i];
        const uint64_t bb = b[i];
        const uint64_t sub = aa - bb - borrow;

        if (borrow) {
            borrow = aa <= bb ? 1 : 0;
        } else {
            borrow = aa < bb ? 1 : 0;
        }
        result[i] = sub;
    }
    return borrow;
}

#if ECC_SUPPORTS_INT128

static ecc_uint128_t ecc_mul64_64_to_128(const uint64_t a, const uint64_t b)
{
    return static_cast<ecc_uint128_t>(a) * static_cast<ecc_uint128_t>(b);
}

#else

static inline ecc_uint128_t ecc_mul64_64_to_128(uint64_t a, uint64_t b)
{
    uint64_t a0 = (uint32_t)a;
    uint64_t a1 = a >> 32;
    uint64_t b0 = (uint32_t)b;
    uint64_t b1 = b >> 32;

    uint64_t m0 = a0 * b0;
    uint64_t m1 = a0 * b1;
    uint64_t m2 = a1 * b0;
    uint64_t m3 = a1 * b1;

    uint64_t t = (m0 >> 32) + (m1 & 0xffffffffULL) + (m2 & 0xffffffffULL);
    uint64_t low = (m0 & 0xffffffffULL) | (t << 32);
    uint64_t high = m3 + (m1 >> 32) + (m2 >> 32) + (t >> 32);

    ecc_uint128_t r;
    r.low = low;
    r.high = high;
    return r;
}

#endif


static ecc_uint128_t ecc_add128(ecc_uint128_t a, ecc_uint128_t b)
{
#if ECC_SUPPORTS_INT128
    return a + b;
#else
    ecc_uint128_t res;
    res.low = a.low + b.low;
    res.high = a.high + b.high + (res.low < a.low);
    return res;
#endif
}

static uint64_t ecc_128_low64(ecc_uint128_t v)
{
#if ECC_SUPPORTS_INT128
    return static_cast<uint64_t>(v);
#else
    return v.low;
#endif
}
static uint64_t ecc_128_high64(ecc_uint128_t v)
{
#if ECC_SUPPORTS_INT128
    return static_cast<uint64_t>(v >> 64);
#else
    return v.high;
#endif
}

static void ecc_vli_mul(uint64_t *product, const uint64_t *a, const uint64_t *b)
{
    for (ecc_uint i = 0; i < 2 * NUM_ECC_DIGITS; ++i) product[i] = 0;

    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) {
        uint64_t carry_low = 0;
        uint64_t carry_high = 0;
        for (ecc_uint j = 0; j < NUM_ECC_DIGITS; ++j) {
            ecc_uint128_t mul = ecc_mul64_64_to_128(a[i], b[j]);
#if ECC_SUPPORTS_INT128
            const ecc_uint128_t accum = static_cast<ecc_uint128_t>(product[i + j]) + static_cast<ecc_uint128_t>(ecc_128_low64(mul)) + static_cast<ecc_uint128_t>(carry_low);
            product[i + j] = static_cast<uint64_t>(accum);
            carry_low = static_cast<uint64_t>(accum >> 64);
            carry_high += ecc_128_high64(mul);
#else
            uint64_t prev = product[i + j];
            uint64_t low_part = mul.low;
            uint64_t sum_low = prev + low_part + carry_low;
            product[i + j] = sum_low;
            uint64_t c1 = (sum_low < prev) ? 1 : 0;
            uint64_t c2 = (sum_low < low_part) ? 1 : 0;
            carry_low = (mul.high) + c1;

#endif
        }
#if ECC_SUPPORTS_INT128

        uint64_t k = i + NUM_ECC_DIGITS;
        while (carry_low || carry_high) {
            const ecc_uint128_t accum = static_cast<ecc_uint128_t>(product[k]) + static_cast<ecc_uint128_t>(carry_low);
            product[k] = static_cast<uint64_t>(accum);
            carry_low = static_cast<uint64_t>(accum >> 64);

            if (carry_high) {
                const uint64_t add = carry_high;
                const ecc_uint128_t accum2 = static_cast<ecc_uint128_t>(product[k]) + static_cast<ecc_uint128_t>(add);
                product[k] = static_cast<uint64_t>(accum2);
                carry_low += static_cast<uint64_t>(accum2 >> 64);
                carry_high = 0;
            }
            ++k;
        }
#else
#endif
    }
}

static void ecc_vli_square(uint64_t *product, const uint64_t *a)
{
    ecc_vli_mul(product, a, a);
}

static void ecc_vli_mmod_fast(uint64_t *result, const uint64_t *product);

static void ecc_vli_mod_mult_fast(uint64_t *result, const uint64_t *a, const uint64_t *b)
{
    uint64_t product[2 * NUM_ECC_DIGITS];
    for (ecc_uint i = 0; i < 2 * NUM_ECC_DIGITS; ++i) product[i] = 0;
    ecc_vli_mul(product, a, b);
    ecc_vli_mmod_fast(result, product);
}

static void ecc_vli_mod_square_fast(uint64_t *result, const uint64_t *a)
{
    uint64_t product[2 * NUM_ECC_DIGITS];
    for (ecc_uint i = 0; i < 2 * NUM_ECC_DIGITS; ++i) product[i] = 0;
    ecc_vli_square(product, a);
    ecc_vli_mmod_fast(result, product);
}

#if ECC_CURVE == secp128r1

static void ecc_vli_mmod_fast(uint64_t *result, uint64_t *product)
{
    uint64_t tmp[NUM_ECC_DIGITS];
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) result[i] = product[i];

    tmp[0] = product[2];
    tmp[1] = (product[3] & 0x1FFFFFFFFULL) | (product[2] << 33);
    ecc_vli_add(result, result, tmp);

    tmp[0] = (product[2] >> 31) | (product[3] << 33);
    tmp[1] = (product[3] >> 31) | ((product[2] & 0xFFFFFFFF80000000ULL) << 2);
    ecc_vli_add(result, result, tmp);

    tmp[0] = (product[2] >> 62) | (product[3] << 2);
    tmp[1] = (product[3] >> 62) | ((product[2] & 0xC000000000000000ULL) >> 29) | (product[3] << 35);
    ecc_vli_add(result, result, tmp);

    tmp[0] = (product[3] >> 29);
    tmp[1] = ((product[3] & 0xFFFFFFFFE0000000ULL) << 4);
    ecc_vli_add(result, result, tmp);

    tmp[0] = (product[3] >> 60);
    tmp[1] = (product[3] & 0xFFFFFFFE00000000ULL);
    ecc_vli_add(result, result, tmp);

    tmp[0] = 0;
    tmp[1] = ((product[3] & 0xF000000000000000ULL) >> 27);
    ecc_vli_add(result, result, tmp);

    while (ecc_vli_cmp(result, curve_p) >= 0) {
        ecc_vli_sub(result, result, curve_p);
    }
}

#elif ECC_CURVE == secp192r1

static void ecc_vli_mmod_fast(uint64_t *result, uint64_t *product)
{
    uint64_t tmp[NUM_ECC_DIGITS];
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) result[i] = product[i];

    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) tmp[i] = product[i + NUM_ECC_DIGITS];
    ecc_vli_add(result, result, tmp);

    tmp[0] = 0;
    tmp[1] = product[3];
    tmp[2] = product[4];
    ecc_vli_add(result, result, tmp);

    tmp[0] = product[5];
    tmp[1] = product[5];
    tmp[2] = 0;
    ecc_vli_add(result, result, tmp);

    while (ecc_vli_cmp(result, curve_p) >= 0) {
        ecc_vli_sub(result, result, curve_p);
    }
}

#elif ECC_CURVE == secp256r1

static void ecc_vli_mmod_fast(uint64_t *result, const uint64_t *product)
{
    uint64_t tmp[NUM_ECC_DIGITS];

    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) result[i] = product[i];

    tmp[0] = 0;
    tmp[1] = product[5] & 0xffffffff00000000ULL;
    tmp[2] = product[6];
    tmp[3] = product[7];
    ecc_vli_lshift(tmp, tmp, 1);
    ecc_vli_add(result, result, tmp);

    tmp[1] = product[6] << 32;
    tmp[2] = (product[6] >> 32) | (product[7] << 32);
    tmp[3] = product[7] >> 32;
    ecc_vli_lshift(tmp, tmp, 1);
    ecc_vli_add(result, result, tmp);

    tmp[0] = product[4];
    tmp[1] = product[5] & 0xffffffffULL;
    tmp[2] = 0;
    tmp[3] = product[7];
    ecc_vli_add(result, result, tmp);

    tmp[0] = (product[4] >> 32) | (product[5] << 32);
    tmp[1] = (product[5] >> 32) | (product[6] & 0xffffffff00000000ULL);
    tmp[2] = product[7];
    tmp[3] = (product[6] >> 32) | (product[4] << 32);
    ecc_vli_add(result, result, tmp);

    tmp[0] = (product[5] >> 32) | (product[6] << 32);
    tmp[1] = (product[6] >> 32);
    tmp[2] = 0;
    tmp[3] = (product[4] & 0xffffffffULL) | (product[5] << 32);
    ecc_vli_sub(result, result, tmp);

    tmp[0] = product[6];
    tmp[1] = product[7];
    tmp[2] = 0;
    tmp[3] = (product[4] >> 32) | (product[5] & 0xffffffff00000000ULL);
    ecc_vli_sub(result, result, tmp);

    tmp[0] = (product[6] >> 32) | (product[7] << 32);
    tmp[1] = (product[7] >> 32) | (product[4] << 32);
    tmp[2] = (product[4] >> 32) | (product[5] << 32);
    tmp[3] = (product[6] << 32);
    ecc_vli_sub(result, result, tmp);

    tmp[0] = product[7];
    tmp[1] = product[4] & 0xffffffff00000000ULL;
    tmp[2] = product[5];
    tmp[3] = product[6] & 0xffffffff00000000ULL;
    ecc_vli_sub(result, result, tmp);

    while (ecc_vli_cmp(result, curve_p) >= 0) {
        ecc_vli_sub(result, result, curve_p);
    }
}

#elif ECC_CURVE == secp384r1

static void ecc_omega_mult(uint64_t *result, const uint64_t *right)
{
    uint64_t tmp[NUM_ECC_DIGITS];
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) tmp[i] = right[i];
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) result[i] = right[i];

    ecc_vli_lshift(tmp, right, 32);

}

static void ecc_vli_mmod_fast(uint64_t *result, uint64_t *product)
{
    uint64_t tmp[2 * NUM_ECC_DIGITS];

    for (ecc_uint i = 0; i < 2 * NUM_ECC_DIGITS; ++i) tmp[i] = product[i];

    while (!ecc_vli_is_zero(tmp + NUM_ECC_DIGITS)) {
        uint64_t wtmp[2 * NUM_ECC_DIGITS];
        for (ecc_uint i = 0; i < 2 * NUM_ECC_DIGITS; ++i) wtmp[i] = 0;

        for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) wtmp[i] = tmp[i + NUM_ECC_DIGITS];

        uint64_t carry = ecc_vli_lshift(wtmp + 1, tmp + NUM_ECC_DIGITS, 32);

        for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) wtmp[i + 2] += tmp[i + NUM_ECC_DIGITS];

        uint64_t shifted[2 * NUM_ECC_DIGITS];
        for (ecc_uint i = 0; i < 2 * NUM_ECC_DIGITS; ++i) shifted[i] = 0;
        ecc_vli_lshift(shifted, tmp + NUM_ECC_DIGITS, 32);

        ecc_vli_sub(wtmp, wtmp, shifted);

        uint64_t carry_add = 0;
        for (ecc_uint i = 0; i < 2 * NUM_ECC_DIGITS; ++i) {
            uint64_t prev = tmp[i];
            uint64_t sum = prev + wtmp[i] + carry_add;
            carry_add = (sum < prev) ? 1 : 0;
            tmp[i] = sum;
        }

        for (ecc_uint i = NUM_ECC_DIGITS; i < 2 * NUM_ECC_DIGITS; ++i) tmp[i] = 0;
    }

    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) result[i] = tmp[i];
    while (ecc_vli_cmp(result, curve_p) >= 0) {
        ecc_vli_sub(result, result, curve_p);
    }
}

#endif


static void ecc_mod_add(uint64_t *result, const uint64_t *a, const uint64_t *b, const uint64_t *m)
{
    if (const uint64_t carry = ecc_vli_add(result, a, b); carry || ecc_vli_cmp(result, m) >= 0) {
        ecc_vli_sub(result, result, m);
    }
}

static void ecc_mod_sub(uint64_t *result, const uint64_t *a, const uint64_t *b, const uint64_t *m)
{
    if (uint64_t borrow = ecc_vli_sub(result, a, b)) {
        ecc_vli_add(result, result, m);
    }
}

static void ecc_mod_inv(uint64_t *result, const uint64_t *input, const uint64_t *m)
{
    if (ecc_vli_is_zero(input)) {
        ecc_vli_clear(result);
        return;
    }

    uint64_t a[NUM_ECC_DIGITS], b[NUM_ECC_DIGITS], u[NUM_ECC_DIGITS], v[NUM_ECC_DIGITS];
    ecc_vli_set(a, input);
    ecc_vli_set(b, m);
    ecc_vli_clear(u); u[0] = 1;
    ecc_vli_clear(v);

    while (true) {
        const int cmp = ecc_vli_cmp(a, b);
        if (cmp == 0) break;
        if ((a[0] & 1ULL) == 0) {
            ecc_vli_rshift1(a);
            if ((u[0] & 1ULL) != 0) {
                ecc_vli_add(u, u, m);
            }
            ecc_vli_rshift1(u);
        } else if ((b[0] & 1ULL) == 0) {
            ecc_vli_rshift1(b);
            if ((v[0] & 1ULL) != 0) {
                ecc_vli_add(v, v, m);
            }
            ecc_vli_rshift1(v);
        } else if (cmp > 0) {
            ecc_vli_sub(a, a, b);
            ecc_vli_rshift1(a);
            if (ecc_vli_cmp(u, v) < 0) {
                ecc_vli_add(u, u, m);
            }
            ecc_vli_sub(u, u, v);
            if ((u[0] & 1ULL) != 0) {
                ecc_vli_add(u, u, m);
            }
            ecc_vli_rshift1(u);
        } else {
            ecc_vli_sub(b, b, a);
            ecc_vli_rshift1(b);
            if (ecc_vli_cmp(v, u) < 0) {
                ecc_vli_add(v, v, m);
            }
            ecc_vli_sub(v, v, u);
            if ((v[0] & 1ULL) != 0) {
                ecc_vli_add(v, v, m);
            }
            ecc_vli_rshift1(v);
        }
    }

    ecc_vli_set(result, u);
}
    
static int ecc_point_is_zero(const ecc_point_t *P)
{
    return ecc_vli_is_zero(P->x) && ecc_vli_is_zero(P->y);
}

static void ecc_apply_z(uint64_t *X, uint64_t *Y, const uint64_t *Z)
{
    uint64_t t[NUM_ECC_DIGITS];
    ecc_vli_mod_square_fast(t, Z);     
    ecc_vli_mod_mult_fast(X, X, t);    
    ecc_vli_mod_mult_fast(t, t, Z);   
    ecc_vli_mod_mult_fast(Y, Y, t);    
}
    
static void ecc_point_double_jacobian(uint64_t *X1, uint64_t *Y1, uint64_t *Z1)
{
    if (ecc_vli_is_zero(Z1)) {
        return;
    }

    uint64_t t4[NUM_ECC_DIGITS];
    uint64_t t5[NUM_ECC_DIGITS];

    ecc_vli_mod_square_fast(t4, Y1);   
    ecc_vli_mod_mult_fast(t5, X1, t4); 
    ecc_vli_mod_square_fast(t4, t4);    
    ecc_vli_mod_mult_fast(Y1, Y1, Z1); 
    ecc_vli_mod_square_fast(Z1, Z1);   

    ecc_vli_mod_add(X1, X1, Z1, curve_p);
    ecc_vli_mod_add(Z1, Z1, Z1, curve_p);
    ecc_vli_mod_sub(Z1, X1, Z1, curve_p); 
    ecc_vli_mod_mult_fast(X1, X1, Z1);    

    ecc_vli_mod_add(Z1, X1, X1, curve_p);
    ecc_vli_mod_add(X1, X1, Z1, curve_p); 
    
    if (ecc_vli_test_bit(X1, 0)) {
        const uint64_t c = ecc_vli_add(X1, X1, curve_p);
        ecc_vli_rshift1(X1);
        X1[NUM_ECC_DIGITS - 1] |= (c << 63);
    } else {
        ecc_vli_rshift1(X1);
    }

    ecc_vli_mod_square_fast(Z1, X1);
    ecc_vli_mod_sub(Z1, Z1, t5, curve_p);
    ecc_vli_mod_sub(Z1, Z1, t5, curve_p);
    ecc_vli_mod_sub(t5, t5, Z1, curve_p);
    ecc_vli_mod_mult_fast(X1, X1, t5);
    ecc_vli_mod_sub(t4, X1, t4, curve_p);

    ecc_vli_set(X1, Z1);
    ecc_vli_set(Z1, Y1);
    ecc_vli_set(Y1, t4);
}

static void ecc_XYcZ_initial_double(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2, const uint64_t *initialZ)
{
    uint64_t z[NUM_ECC_DIGITS];
    ecc_vli_set(X2, X1);
    ecc_vli_set(Y2, Y1);

    ecc_vli_clear(z);
    z[0] = 1;
    if (initialZ) ecc_vli_set(z, initialZ);

    ecc_apply_z(X1, Y1, z);
    ecc_point_double_jacobian(X1, Y1, z);
    ecc_apply_z(X2, Y2, z);
}

static void ecc_XYcZ_add(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2)
{
    uint64_t t5[NUM_ECC_DIGITS];

    ecc_vli_mod_sub(t5, X2, X1, curve_p);   
    ecc_vli_mod_square_fast(t5, t5);        
    ecc_vli_mod_mult_fast(X1, X1, t5);      
    ecc_vli_mod_mult_fast(X2, X2, t5);     
    ecc_vli_mod_sub(Y2, Y2, Y1, curve_p);   
    ecc_vli_mod_square_fast(t5, Y2);        

    ecc_vli_mod_sub(t5, t5, X1, curve_p);   
    ecc_vli_mod_sub(t5, t5, X2, curve_p);  
    ecc_vli_mod_sub(X2, X2, X1, curve_p);   
    ecc_vli_mod_mult_fast(Y1, Y1, X2);      
    ecc_vli_mod_sub(X2, X1, t5, curve_p);  
    ecc_vli_mod_mult_fast(Y2, Y2, X2);     
    ecc_vli_mod_sub(Y2, Y2, Y1, curve_p);   

    ecc_vli_set(X2, t5);
}

static void ecc_XYcZ_addC(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2)
{
    uint64_t t5[NUM_ECC_DIGITS], t6[NUM_ECC_DIGITS], t7[NUM_ECC_DIGITS];

    ecc_vli_mod_sub(t5, X2, X1, curve_p);
    ecc_vli_mod_square_fast(t5, t5);
    ecc_vli_mod_mult_fast(X1, X1, t5);
    ecc_vli_mod_mult_fast(X2, X2, t5);
    ecc_vli_mod_add(t5, Y2, Y1, curve_p);
    ecc_vli_mod_sub(Y2, Y2, Y1, curve_p);

    ecc_vli_mod_sub(t6, X2, X1, curve_p);
    ecc_vli_mod_mult_fast(Y1, Y1, t6);
    ecc_vli_mod_add(t6, X1, X2, curve_p);
    ecc_vli_mod_square_fast(X2, Y2);
    ecc_vli_mod_sub(X2, X2, t6, curve_p);

    ecc_vli_mod_sub(t7, X1, X2, curve_p);
    ecc_vli_mod_mult_fast(Y2, Y2, t7);
    ecc_vli_mod_sub(Y2, Y2, Y1, curve_p);

    ecc_vli_mod_square_fast(t7, t5);
    ecc_vli_mod_sub(t7, t7, t6, curve_p);
    ecc_vli_mod_sub(t6, t7, X1, curve_p);
    ecc_vli_mod_mult_fast(t6, t6, t5);
    ecc_vli_mod_sub(Y1, t6, Y1, curve_p);

    ecc_vli_set(X1, t7);
}

static void ecc_point_mul(ecc_point_t *result, const ecc_point_t *point, const uint64_t *scalar, const uint64_t *initialZ)
{
    uint64_t Rx[2][NUM_ECC_DIGITS];
    uint64_t Ry[2][NUM_ECC_DIGITS];
    uint64_t z[NUM_ECC_DIGITS];

    ecc_vli_set(Rx[1], point->x);
    ecc_vli_set(Ry[1], point->y);
    ecc_XYcZ_initial_double(Rx[1], Ry[1], Rx[0], Ry[0], initialZ);

    const int nbits = static_cast<int>(ecc_vli_num_bits(scalar));
    if (nbits == 0) {
        ecc_vli_clear(result->x);
        ecc_vli_clear(result->y);
        return;
    }

    for (int i = nbits - 2; i > 0; --i) {
        const int nb = !ecc_vli_test_bit(scalar, static_cast<uint32_t>(i));
        ecc_XYcZ_addC(Rx[1 - nb], Ry[1 - nb], Rx[nb], Ry[nb]);
        ecc_XYcZ_add(Rx[nb], Ry[nb], Rx[1 - nb], Ry[1 - nb]);
    }

    const int nb = !ecc_vli_test_bit(scalar, 0);
    ecc_XYcZ_addC(Rx[1 - nb], Ry[1 - nb], Rx[nb], Ry[nb]);

    uint64_t ztmp[NUM_ECC_DIGITS];
    ecc_vli_mod_sub(ztmp, Rx[1], Rx[0], curve_p);
    ecc_vli_mod_mult_fast(ztmp, ztmp, Ry[1 - nb]);
    ecc_vli_mod_mult_fast(ztmp, ztmp, point->x);
    ecc_vli_mod_inv(ztmp, ztmp, curve_p);
    ecc_vli_mod_mult_fast(ztmp, ztmp, point->y);
    ecc_vli_mod_mult_fast(ztmp, ztmp, Rx[1 - nb]);

    ecc_XYcZ_add(Rx[nb], Ry[nb], Rx[1 - nb], Ry[1 - nb]);

    ecc_apply_z(Rx[0], Ry[0], ztmp);

    ecc_vli_set(result->x, Rx[0]);
    ecc_vli_set(result->y, Ry[0]);
}

static void ecc_bytes_to_native(uint64_t native[NUM_ECC_DIGITS], const uint8_t bytes[ECC_BYTES])
{
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) {
        const uint8_t *digit = bytes + 8 * (NUM_ECC_DIGITS - 1 - i);
        native[i] = static_cast<uint64_t>(digit[0]) << 56 | static_cast<uint64_t>(digit[1]) << 48 |
                    static_cast<uint64_t>(digit[2]) << 40 | static_cast<uint64_t>(digit[3]) << 32 |
                    static_cast<uint64_t>(digit[4]) << 24 | static_cast<uint64_t>(digit[5]) << 16 |
                    static_cast<uint64_t>(digit[6]) << 8  | static_cast<uint64_t>(digit[7]);
    }
}

static void ecc_native_to_bytes(uint8_t bytes[ECC_BYTES], const uint64_t native[NUM_ECC_DIGITS])
{
    for (ecc_uint i = 0; i < NUM_ECC_DIGITS; ++i) {
        uint8_t *digit = bytes + 8 * (NUM_ECC_DIGITS - 1 - i);
        digit[0] = static_cast<uint8_t>(native[i] >> 56);
        digit[1] = static_cast<uint8_t>(native[i] >> 48);
        digit[2] = static_cast<uint8_t>(native[i] >> 40);
        digit[3] = static_cast<uint8_t>(native[i] >> 32);
        digit[4] = static_cast<uint8_t>(native[i] >> 24);
        digit[5] = static_cast<uint8_t>(native[i] >> 16);
        digit[6] = static_cast<uint8_t>(native[i] >> 8);
        digit[7] = static_cast<uint8_t>(native[i]);
    }
}

static void ecc_mod_sqrt(uint64_t a[NUM_ECC_DIGITS])
{
    uint64_t p1[NUM_ECC_DIGITS];
    ecc_vli_set(p1, curve_p);
    uint64_t one[NUM_ECC_DIGITS]; ecc_vli_clear(one); one[0] = 1;
    ecc_vli_add(p1, p1, one);

    uint64_t result[NUM_ECC_DIGITS];
    ecc_vli_clear(result); result[0] = 1;

    const uint32_t bits = ecc_vli_num_bits(p1);
    for (int i = bits - 1; i > 1; --i) {
        ecc_vli_mod_square_fast(result, result);
        if (ecc_vli_test_bit(p1, static_cast<uint32_t>(i))) {
            ecc_vli_mod_mult_fast(result, result, a);
        }
    }
    ecc_vli_set(a, result);
}

static void ecc_point_decompress(ecc_point_t *point, const uint8_t compressed[ECC_BYTES + 1])
{
    uint64_t minus_a[NUM_ECC_DIGITS]; ecc_vli_clear(minus_a);
    minus_a[0] = 3;

    ecc_bytes_to_native(point->x, compressed + 1);


    ecc_vli_mod_square_fast(point->y, point->x);
    ecc_vli_mod_sub(point->y, point->y, minus_a, curve_p);
    ecc_vli_mod_mult_fast(point->y, point->y, point->x);
    ecc_vli_mod_add(point->y, point->y, curve_b, curve_p);

    ecc_mod_sqrt(point->y);

    if (const int parity = compressed[0] & 1; (point->y[0] & 1ULL) != static_cast<uint64_t>(parity)) {
        ecc_vli_sub(point->y, curve_p, point->y);
    }
}

static int ecc_make_keypair(uint8_t p_public[ECC_BYTES + 1], uint8_t p_private[ECC_BYTES])
{
    uint64_t priv[NUM_ECC_DIGITS];
    ecc_point_t pub;
    unsigned tries = 0;

    do {
        uint8_t rnd[ECC_BYTES];
        if (!ecc_get_random_bytes(rnd, ECC_BYTES) || (tries++ >= MAX_TRIES)) {
            memset(priv, 0, sizeof(priv));
            return 0;
        }
        ecc_bytes_to_native(priv, rnd);
        if (ecc_vli_is_zero(priv)) continue;

        if (ecc_vli_cmp(curve_n, priv) <= 0) {
            ecc_vli_sub(priv, priv, curve_n);
        }

        ecc_point_mul(&pub, &curve_G, priv, NULL);
    } while (ecc_point_is_zero(&pub));

    ecc_native_to_bytes(p_private, priv);
    ecc_native_to_bytes(p_public + 1, pub.x);
    p_public[0] = 2 + static_cast<uint8_t>(pub.y[0] & 1ULL);

    memset(priv, 0, sizeof(priv));
    return 1;
}

static int ecc_shared_secret(const uint8_t p_public[ECC_BYTES + 1], const uint8_t p_private[ECC_BYTES], uint8_t p_secret[ECC_BYTES])
{
    ecc_point_t pub;
    uint64_t priv[NUM_ECC_DIGITS];
    uint64_t rnd[NUM_ECC_DIGITS];

    if (!ecc_get_random_bytes(reinterpret_cast<uint8_t *>(rnd), ECC_BYTES)) {
        return 0;
    }

    ecc_point_decompress(&pub, p_public);
    ecc_bytes_to_native(priv, p_private);
    ecc_point_t product;
    ecc_point_mul(&product, &pub, priv, rnd);

    ecc_native_to_bytes(p_secret, product.x);
    return !ecc_point_is_zero(&product);
}

static int ecdsa_sign(const uint8_t p_private[ECC_BYTES], const uint8_t p_hash[ECC_BYTES], uint8_t p_signature[ECC_BYTES * 2])
{
    uint64_t k[NUM_ECC_DIGITS];
    uint64_t tmp[NUM_ECC_DIGITS];
    uint64_t s[NUM_ECC_DIGITS];
    ecc_point_t p;
    unsigned tries = 0;

    do {
        uint8_t rnd[ECC_BYTES];
        if (!ecc_get_random_bytes(rnd, ECC_BYTES) || (tries++ >= MAX_TRIES)) {
            return 0;
        }
        ecc_bytes_to_native(k, rnd);
        if (ecc_vli_is_zero(k)) continue;
        if (ecc_vli_cmp(curve_n, k) <= 0) ecc_vli_sub(k, k, curve_n);

        ecc_point_mul(&p, &curve_G, k, NULL);

        if (ecc_vli_cmp(curve_n, p.x) <= 0) ecc_vli_sub(p.x, p.x, curve_n);
    } while (ecc_vli_is_zero(p.x));

    ecc_native_to_bytes(p_signature, p.x);

    ecc_bytes_to_native(tmp, p_private);
    ecc_vli_mod_mult(s, p.x, tmp, curve_n);
    ecc_bytes_to_native(tmp, p_hash);
    ecc_vli_mod_add(s, tmp, s, curve_n);
    ecc_mod_inv(k, k, curve_n);
    ecc_vli_mod_mult(s, s, k, curve_n);

    ecc_native_to_bytes(p_signature + ECC_BYTES, s);
    return 1;
}

static int ecdsa_verify(const uint8_t p_public[ECC_BYTES + 1], const uint8_t p_hash[ECC_BYTES], const uint8_t p_signature[ECC_BYTES * 2])
{
    uint64_t u1[NUM_ECC_DIGITS], u2[NUM_ECC_DIGITS];
    uint64_t z[NUM_ECC_DIGITS];
    ecc_point_t pub, sum;
    uint64_t rx[NUM_ECC_DIGITS], ry[NUM_ECC_DIGITS];
    uint64_t tx[NUM_ECC_DIGITS], ty[NUM_ECC_DIGITS], tz[NUM_ECC_DIGITS];

    uint64_t r[NUM_ECC_DIGITS], s[NUM_ECC_DIGITS];

    ecc_point_decompress(&pub, p_public);
    ecc_bytes_to_native(r, p_signature);
    ecc_bytes_to_native(s, p_signature + ECC_BYTES);

    if (ecc_vli_is_zero(r) || ecc_vli_is_zero(s)) return 0;
    if (ecc_vli_cmp(curve_n, r) <= 0 || ecc_vli_cmp(curve_n, s) <= 0) return 0;

    ecc_mod_inv(z, s, curve_n);
    ecc_bytes_to_native(u1, p_hash);
    ecc_vli_mod_mult(u1, u1, z, curve_n);
    ecc_vli_mod_mult(u2, r, z, curve_n);

    ecc_vli_set(sum.x, pub.x);
    ecc_vli_set(sum.y, pub.y);
    ecc_vli_set(tx, curve_G.x);
    ecc_vli_set(ty, curve_G.y);
    ecc_vli_mod_sub(z, sum.x, tx, curve_p);
    ecc_XYcZ_add(tx, ty, sum.x, sum.y);
    ecc_mod_inv(z, z, curve_p);
    ecc_apply_z(sum.x, sum.y, z);

    ecc_point_t *points[4] = {nullptr, &curve_G, &pub, &sum };
    const uint32_t bits = ecc_vli_num_bits(u1) > ecc_vli_num_bits(u2) ? ecc_vli_num_bits(u1) : ecc_vli_num_bits(u2);

    const ecc_point_t *point = points[(!!ecc_vli_test_bit(u1, bits - 1)) | ((!!ecc_vli_test_bit(u2, bits - 1)) << 1)];
    ecc_vli_set(rx, point->x);
    ecc_vli_set(ry, point->y);
    ecc_vli_clear(tz); tz[0] = 1;

    for (int i = static_cast<int>(bits) - 2; i >= 0; --i)
    {
        ecc_point_double_jacobian(rx, ry, tz);
        const int index = !!ecc_vli_test_bit(u1, static_cast<uint32_t>(i)) | !!ecc_vli_test_bit(u2, static_cast<uint32_t>(i)) << 1;
        if (const ecc_point_t *pt = points[index])
        {
            ecc_vli_set(tx, pt->x);
            ecc_vli_set(ty, pt->y);
            ecc_apply_z(tx, ty, tz);
            ecc_vli_mod_sub(tz, rx, tx, curve_p);
            ecc_XYcZ_add(tx, ty, rx, ry);
            ecc_vli_mod_mult_fast(tz, tz, tz);
            ecc_vli_mod_mult_fast(tz, tz, tz);
        }
    }

    ecc_mod_inv(tz, tz, curve_p);
    ecc_apply_z(rx, ry, tz);

    if (ecc_vli_cmp(curve_n, rx) <= 0) ecc_vli_sub(rx, rx, curve_n);

    return ecc_vli_cmp(rx, r) == 0;
}

#define ecc_make_keypair ecc_make_keypair
#define ecc_shared_secret ecc_shared_secret
#define ecc_ecdsa_sign ecdsa_sign
#define ecc_ecdsa_verify ecdsa_verify

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ECC_MATH_H_ */
