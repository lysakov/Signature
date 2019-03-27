#ifndef MODULAR_ARITHMETIC
#define MODULAR_ARITHMETIC

#include "block.hpp"

enum {
    UINT512_SIZE = 64,
    UINT256_SIZE = 32
};

typedef Block::Block<512> uint512_t;
typedef Block::Block<256> uint256_t;

uint512_t extend (const uint256_t&);
uint512_t operator / (const uint512_t&, const uint512_t&);
uint512_t operator % (const uint512_t&, const uint512_t&);
uint512_t inverse (const uint512_t&, const uint512_t&);
uint512_t opposite (const uint512_t&, const uint512_t&);
uint512_t multiplication (const uint512_t&, const uint512_t&, const uint512_t&);
uint512_t addition (const uint512_t&, const uint512_t&, const uint512_t&);
uint512_t substraction (const uint512_t&, const uint512_t&, const uint512_t&);

#endif
