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

// Some branchless ops which should come in handy :)

// Branchless ternary operation. cond ? a : b
// Relies on arithmetic right shift semantics.
static inline uint32_t isel_if(int32_t cond, uint32_t a, uint32_t b)
{
   uint32_t mask = (cond | (-cond)) >> 31;
   return (a & mask) | (b & ~mask);
}

#define iup_if(var, cond, a) { var = isel_if(cond, a, var); }
#define iup_eq(var, check, eq, a) { var = isel_eq(check, eq, a, var); }
#define iup_gez(var, check, val, a) { var = isel_gez(check, val, a, var); }
#define iup_lez(var, check, val, a) { var = isel_lez(check, val, a, var); }
#define iup_gte(var, check, val, a) { var = isel_gte(check, val, a, var); }
#define iup_lt(var, check, val, a) { var = isel_lt(check, val, a, var); }

// (var == eq) ? a : b
static inline uint32_t isel_eq(uint32_t var, uint32_t eq, uint32_t a, uint32_t b)
{
   return isel_if(var - eq, b, a);
}

// Branchless greater or equal 0. (cond >= 0) ? a : b
static inline uint32_t isel_gez(int32_t cond, uint32_t a, uint32_t b)
{
   uint32_t mask = cond >> 31;
   return (b & mask) | (a & ~mask);
}

// (cond < 0) ? a : b
static inline uint32_t isel_lez(int32_t cond, uint32_t a, uint32_t b)
{
   return isel_gez(cond, b, a);
}

static inline uint32_t isel_gte(int32_t var1, int32_t var2, uint32_t a, uint32_t b)
{
   int32_t mask = (var2 - var1) >> 31;
   return (a & mask) | (b & ~mask);
}

static inline uint32_t isel_lt(int32_t var1, int32_t var2, uint32_t a, uint32_t b)
{
   int32_t mask = (var1 - var2) >> 31;
   return (a & mask) | (b & ~mask);
}


#endif
