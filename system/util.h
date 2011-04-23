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

typedef union
{
   uint32_t l;
   struct
   {
#ifdef BIG_ENDIAN
      uint8_t hh;
      uint8_t hl;
      uint8_t lh;
      uint8_t ll;
#else
      uint8_t ll;
      uint8_t lh;
      uint8_t hl;
      uint8_t hh;
#endif
   } b;
   struct
   {
#ifdef BIG_ENDIAN
      uint16_t h;
      uint16_t l;
#else
      uint16_t l;
      uint16_t h;
#endif
   } w;
} long_reg_t;

// Branchless ternary operation. cond ? a : b
// Relies on arithmetic right shift semantics.
static inline unsigned isel_if(int32_t cond, unsigned a, unsigned b)
{
   unsigned mask = (cond | (-cond)) >> 31;
   return (a & mask) | (b & ~mask);
}

// Branchless greater or equal 0. (cond >= 0) ? a : b
static inline unsigned isel_gez(int32_t cond, unsigned a, unsigned b)
{
   unsigned mask = cond >> 31;
   return (b & mask) | (a & ~mask);
}


#endif
