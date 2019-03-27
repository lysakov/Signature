#include "modular_arithmetic.hpp"

enum {
    MOD = 256,
    BYTE_SIZE = 8
};

typedef Block::Block<1024> uint1024_t;

struct Division_struct
{

    Division_struct (const uint1024_t&, const uint1024_t&);
    uint1024_t quoitient;
    uint1024_t remainder;

};

uint1024_t operator / (const uint1024_t&, const uint1024_t&);
uint1024_t operator % (const uint1024_t&, const uint1024_t&);
static Division_struct division (const uint1024_t&, const uint1024_t &);
static uint1024_t extend (const uint512_t &);

Division_struct::Division_struct (const uint1024_t &quoitient, const uint1024_t &remainder) : quoitient(quoitient), remainder(remainder)
{}

uint1024_t operator / (const uint1024_t &dividend, const uint1024_t &divider)
{
    return division(dividend, divider).quoitient;
}

uint1024_t operator % (const uint1024_t &dividend, const uint1024_t &divider)
{
    return division(dividend, divider).remainder;
}

uint512_t operator / (const uint512_t &dividend, const uint512_t &divider)
{
    return division(extend(dividend), extend(divider)).quoitient.split<512>()[0];
}

uint512_t operator % (const uint512_t &dividend, const uint512_t &divider)
{
    return division(extend(dividend), extend(divider)).remainder.split<512>()[0];
}

static uint1024_t extend (const uint512_t &num)
{
    uint8_t buf[2 * UINT512_SIZE] = {0};
    auto ptr = num.get_bitset();
    for (size_t i = 0; i < UINT512_SIZE; ++i) {
        buf[i] = ptr[i];
    }
    return uint1024_t(buf, 2 * UINT512_SIZE);
}

uint512_t extend (const uint256_t &num)
{
    uint8_t buf[UINT512_SIZE] = {0};
    auto ptr = num.get_bitset();
    for (size_t i = 0; i < UINT256_SIZE; ++i) {
        buf[i] = ptr[i];
    }
    return uint512_t(buf, UINT512_SIZE);
}

template <size_t S>
static size_t set_size (const Block::Block<S> &num)
{
    for (size_t i = 0; i < (S / BYTE_SIZE); ++i) {
        if (num.get_byte(S / BYTE_SIZE - i - 1) != 0) {
            return S / BYTE_SIZE - i;
        }
    }
    return 0;
}
static uint1024_t shift_l (const uint1024_t &num, size_t shift_p)
{
    const uint8_t *bitset = num.get_bitset();
    uint8_t new_bitset[2 * UINT512_SIZE] = {0};
    for (size_t i = 0; i < 2 * UINT512_SIZE - shift_p; ++i) {
        new_bitset[i + shift_p] = bitset[i];
    }
    return uint1024_t(new_bitset, 2 * UINT512_SIZE);
}

static uint1024_t extract (const uint1024_t &num, size_t first, size_t last)
{
    uint1024_t result;
    for (size_t i = first; i <= last; ++i) {
        result.set_byte(num.get_byte(i), i);
    }
    return result;
}

static void change (uint1024_t &num1, const uint1024_t &num2, size_t first, size_t last)
{
    for (size_t i = first; i <= last; ++i) {
        num1.set_byte(num2.get_byte(i), i);
    }
}

static uint1024_t multiplication (const uint1024_t &factor1, uint8_t factor2)
{
    uint8_t flag = 0;
    uint8_t result[2 * UINT512_SIZE] = {0};
    for (size_t i = 0; i < 2 * UINT512_SIZE; ++i) {
        result[i] = (uint8_t)((factor1.get_byte(i) * factor2 + flag) % MOD);
        flag = (uint8_t)((factor1.get_byte(i) * factor2 + flag) / MOD);
    }
    return uint1024_t(result, 2 * UINT512_SIZE);
}

static uint1024_t division (const uint1024_t &dividend, uint8_t divider)
{
    uint8_t result[2 * UINT512_SIZE] = {0};
    uint8_t remainder = 0;
    for (size_t i = 0; i < 2 * UINT512_SIZE; ++i) {
        result[2 * UINT512_SIZE - i - 1] = (uint8_t)((dividend.get_byte(2 * UINT512_SIZE - i - 1) + (uint32_t)remainder * MOD) / divider);
        remainder = (uint8_t)((dividend.get_byte(2 * UINT512_SIZE - i - 1) + (uint32_t)remainder * MOD) % divider);
    }
    return uint1024_t(result, 2 * UINT512_SIZE);
}  

template <size_t S>
static Block::Block<S> substraction (const Block::Block<S> &ext_min, const Block::Block<S> &ext_sub)
{
    int flag = 0;
    uint8_t result[S / BYTE_SIZE] = {0};
    for (size_t i = 0; i < S / BYTE_SIZE; ++i) {
        result[i] = (uint8_t)(ext_min.get_byte(i) - ext_sub.get_byte(i) - flag);
        if (ext_min.get_byte(i) < ext_sub.get_byte(i) + flag) {
            flag = 1;
        } else {
            flag = 0;
        }
    }
    return Block::Block<S>(result, S / BYTE_SIZE);
}

template <size_t S>
static Block::Block<S> addition (const Block::Block<S> &addend1, const Block::Block<S> &addend2)
{
    uint32_t flag = 0;
    uint8_t result[S / BYTE_SIZE] = {0};
    for (size_t i = 0; i < S / BYTE_SIZE; ++i) {
        result[i] = (uint8_t)(((uint32_t)addend1.get_byte(i) + (uint32_t)addend2.get_byte(i) + flag) % MOD);
        flag = ((uint32_t)addend1.get_byte(i) + (uint32_t)addend2.get_byte(i) + flag) / MOD;
    }
    return Block::Block<S>(result, S / BYTE_SIZE);
}

uint1024_t multiplication (const uint512_t &factor1, const uint512_t &factor2)
{
    uint1024_t result = 0;
    size_t j = 0;
    size_t m = set_size(factor1);
    size_t n = set_size(factor2);
    while (j < n) {
        if (factor2.get_byte(j) == 0) {
            result.set_byte(0, j + m);
        } else {
            uint32_t i = 0;
            uint32_t k = 0;
            while (i < m) {
                uint32_t t = factor1.get_byte(i) * factor2.get_byte(j) + result.get_byte(i + j) + k;
                result.set_byte((uint8_t)(t % MOD), i + j);
                k = t / MOD;
                ++i;
            }
            result.set_byte((uint8_t)k, j + m);
        }
        ++j;
    }
    return result;
}

static Division_struct division (const uint1024_t &dividend, const uint1024_t &divider)
{
    uint1024_t dividend_ext = dividend;
    uint1024_t quoitient;
    size_t dividend_size = set_size(dividend);
    size_t divider_size = set_size(divider);
    int j = dividend_size - divider_size;
    uint8_t norm = MOD / (divider.get_byte(divider_size - 1) + 1);
    dividend_ext = multiplication(dividend, norm);
    const uint1024_t divider_ext = multiplication(divider, norm);
    while (j >= 0) {
        uint32_t tmp = (uint32_t)dividend_ext.get_byte(j + divider_size) * MOD + (uint32_t)dividend_ext.get_byte(j + divider_size - 1);
        uint32_t q = tmp / divider_ext.get_byte(divider_size - 1);
        uint32_t r = tmp % divider_ext.get_byte(divider_size - 1);
        do {
            if (q == MOD || q * divider_ext.get_byte(divider_size - 2) > MOD * r + dividend_ext.get_byte(j + divider_size - 2)) {
                --q;
                r += divider_ext.get_byte(divider_size - 1);
            } else {
                break;
            }
        } while (r < MOD);
        auto tmp1 = extract(dividend_ext, j, j + divider_size);
        auto tmp_divider = shift_l(divider_ext, j);
        auto mult = multiplication(tmp_divider, (uint8_t)q);
        while (tmp1 < mult) {
            --q;
            mult = multiplication(tmp_divider, (uint8_t)q);
        }
        tmp1 = substraction(tmp1, mult);
        change(dividend_ext, tmp1, j, j + divider_size);
        quoitient.set_byte((uint8_t)q, j);
        --j;
    }
    return Division_struct(quoitient, division(dividend_ext, norm)); 
}

uint512_t inverse (const uint512_t &num, const uint512_t &modulus)
{
    uint1024_t r0 = extend(modulus);
    uint1024_t r1 = division(extend(num), r0).remainder;
    uint512_t p[3];
    uint512_t q[3];
    p[0] = 1;
    auto ds = division(r0, r1);
    p[1] = ds.quoitient.split<512>()[0];
    q[1] = p[1];
    r0 = r1;
    r1 = ds.remainder;
    int i = 1;
    size_t cnt = 1;
    while (r1 != (uint1024_t)(0)) {
        i = (i + 1) % 3;
        ++cnt;
        auto div_st = division(r0, r1);
        q[i] = div_st.quoitient.split<512>()[0];
        auto tmp = multiplication(q[i], p[(3 + i - 1) % 3]);
        p[i] = addition(tmp.split<512>()[0], p[(3 + i - 2) % 3]);
        r0 = r1;
        r1 = div_st.remainder;
    }
    if (cnt % 2) {
        return p[(3 + i - 1) % 3].split<512>()[0];
    } else {
        auto result = substraction(modulus, p[(3 + i - 1) % 3]);
        return result.split<512>()[0];
    }   
}

uint512_t opposite (const uint512_t &num, const uint512_t &modulus)
{
    return substraction(modulus, num);
}

uint512_t multiplication (const uint512_t &factor1, const uint512_t &factor2, const uint512_t &modulus)
{
    return (multiplication(factor1, factor2) % extend(modulus)).split<512>()[0];
}

uint512_t addition (const uint512_t &addend1, const uint512_t &addend2, const uint512_t &modulus)
{
    return (addition(extend(addend1), extend(addend2)) % extend(modulus)).split<512>()[0];
}

uint512_t substraction (const uint512_t &minued, const uint512_t &subtrahend, const uint512_t &modulus)
{
    return addition(minued, opposite(subtrahend, modulus), modulus);
}
