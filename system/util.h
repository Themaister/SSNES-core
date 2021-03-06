#ifndef __UTIL_H
#define __UTIL_H

#include <stdint.h>

typedef union
{
   uint16_t w;
#ifdef SSNES_BIG_ENDIAN
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
#ifdef SSNES_BIG_ENDIAN
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
#ifdef SSNES_BIG_ENDIAN
      uint16_t h;
      uint16_t l;
#else
      uint16_t l;
      uint16_t h;
#endif
   } w;
} long_reg_t;

// Not totally safe :)
#ifdef SSNES_BIG_ENDIAN
// Ye, this is PPC only ... :D
#include <ppc_intrinsics.h>
static inline uint16_t read_le16_(const uint16_t *addr)
{
   return __lhbrx(addr, 0);
}

static inline uint32_t read_le32_(const uint32_t *addr)
{
   return __lwbrx(addr, 0);
}

static inline void write_le16_(uint16_t *addr, uint16_t data)
{
   __sthbrx(data, addr, 0);
}

static inline void write_le32_(uint32_t *addr, uint32_t data)
{
   __stwbrx(data, addr, 0);
}

#define READ_16LE(addr)        read_le16_(addr)
#define READ_32LE(addr)        read_le32_(addr)
#define WRITE_16LE(addr, in)   write_le16_(addr, in)
#define WRITE_32LE(addr, in)   write_le32_(addr, in)

#else

#define READ_16LE(addr)        (*(const uint16_t*)(addr))
#define READ_32LE(addr)        (*(const uint32_t*)(addr))
#define WRITE_16LE(addr, data) (*(uint16_t*)(addr) = data)
#define WRITE_32LE(addr, data) (*(uint32_t*)(addr) = data)
#endif

// Some branchless ops which should come in handy :)

// Branchless ternary operation. cond ? a : b
// Relies on arithmetic right shift semantics.
//

#ifdef ARCH_INORDER
static inline uint32_t isel_if(int32_t cond, uint32_t a, uint32_t b)
{
   uint32_t mask = (cond | (-cond)) >> 31;
   return (a & mask) | (b & ~mask);
}

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

#else // x86 seems to prefer branching or it optimizes better to cmove or something.

#define isel_if(cond, a, b) ((cond) ? (a) : (b))
#define isel_eq(cond, eq, a, b) ((cond) == (eq) ? (a) : (b))
#define isel_gez(cond, a, b) ((cond) >= 0 ? (a) : (b))
#define isel_lez(cond, a, b) ((cond) < 0 ? (a) : (b))
#define isel_gte(var1, var2, a, b) ((var1) >= (var2) ? (a) : (b))
#define isel_lt(var1, var2, a, b) ((var1) < (var2) ? (a) : (b))

#endif

#define iup_if(var, cond, a) { var = isel_if(cond, a, var); }
#define iup_eq(var, check, eq, a) { var = isel_eq(check, eq, a, var); }
#define iup_gez(var, check, val, a) { var = isel_gez(check, val, a, var); }
#define iup_lez(var, check, val, a) { var = isel_lez(check, val, a, var); }
#define iup_gte(var, check, val, a) { var = isel_gte(check, val, a, var); }
#define iup_lt(var, check, val, a) { var = isel_lt(check, val, a, var); }


#ifdef SSNES_DEBUG
#include <stdio.h>
#define dprintf(fmt, args...) fprintf(stderr, fmt, ##args)
#define dputc(c) fputc((c), stderr)
#else
#define dprintf(fmt, args...)
#define dputc(c)
#endif


#define ALIGNED __attribute__((aligned(16)))


#endif
