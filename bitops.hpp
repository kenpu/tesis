#ifndef BITOPS_HPP
#define BITOPS_HPP

#include <stdint.h>
#include "stats.hpp"

const uint8_t index64[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

/**
 * bitScanForward
 * @author Kim Walisch (2012)
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
inline uint8_t bitScanForward(uint64_t bb) {
   const uint64_t debruijn64 = 0x03f79d71b4cb0a89UL;
   return index64[((bb ^ (bb-1)) * debruijn64) >> 58];
}

inline uint8_t testBit(uint64_t x, uint8_t pos){
    testbitOps++;
    return (x >> pos) & 1;
}

inline uint8_t popCount64(uint64_t i)
{
  popcountOps++;
  i = i - ((i >> 1) & 0x5555555555555555);
  i = (i & 0x3333333333333333) + ((i >> 2) & 0x3333333333333333);
  i = (i + (i >> 4)) & 0xF0F0F0F0F0F0F0F;
  return (i * 0x101010101010101) >> 56;
}

inline uint8_t popCountUpToBit(uint64_t x, uint8_t pos){ // pos not included
    return popCount64(x << (64 - pos));
}

#endif // BITOPS_HPP
