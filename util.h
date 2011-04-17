#ifndef __UTIL_H
#define __UTIL_H

#include <stdint.h>

typedef union
{
   uint16_t w;
#ifdef BIG_ENDIAN
   struct
   {
      uint8_t h;
      uint8_t l;
   } b;
#else
   struct
   {
      uint8_t l;
      uint8_t h;
   } b;
#endif
} word_reg_t;

// Branchless ternary operation. cond ? a : b
static inline unsigned isel_if(int32_t cond, unsigned a, unsigned b)
{
   unsigned mask = cond | (-cond);
   return (a & mask) | (b & ~mask);
}


#endif
