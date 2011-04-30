#ifndef __SMP_MEMORY_H
#define __SMP_MEMORY_H

#include "memmap.h"

// Need MMAPing for registers and stuff.
static inline uint8_t smp_read(uint16_t addr)
{
   return ssnes_smp_memmap_read[addr >> 3](addr);
}

// SMP can modify indiviual bits. Some registers have sideeffects on writes, mask defines which bits to write.
static inline void smp_write(uint16_t addr, uint8_t data, uint8_t mask)
{
   ssnes_smp_memmap_write[addr >> 3](addr, data, mask);
}


static inline uint8_t smp_read_pc(void)
{
   return MEM.apuram[SMP.pc++];
}

static inline uint16_t smp_readw_pc(void)
{
   uint16_t ret = smp_read_pc();
   ret |= (uint16_t)smp_read_pc() << 8;
   return ret;
}

static inline uint8_t smp_read_addr(uint16_t addr)
{
   return smp_read(addr);
}

static inline uint16_t smp_readw_addr(uint16_t addr)
{
   uint16_t res = smp_read(addr);
   res |= (uint16_t)smp_read(addr + 1) << 8;
   return res;
}

static inline uint8_t smp_read_dp(uint8_t dp)
{
   return smp_read(((uint16_t)SMP.p.p << 8) | dp);
}

static inline uint16_t smp_readw_dp(uint8_t dp)
{
   uint16_t res = smp_read_dp(dp);
   res |= (uint16_t)smp_read_dp(dp + 1) << 8;
   return res;
}

static inline void smp_write_addr(uint16_t addr, uint8_t data)
{
   smp_write(addr, data, 0xff);
}

static inline void smp_write_addr_bit(uint16_t addr, uint8_t data, uint8_t mask)
{
   smp_write(addr, data, mask);
}

static inline void smp_write_dp(uint8_t dp, uint8_t data)
{
   smp_write(((uint16_t)SMP.p.p << 8) | dp, data, 0xff);
}

static inline void smp_writew_dp(uint8_t dp, uint16_t data)
{
   smp_write_dp(dp, data & 0xff);
   smp_write_dp(dp + 1, data >> 8);
}

static inline void smp_write_dp_bit(uint8_t dp, uint8_t data, uint8_t mask)
{
   smp_write(((uint16_t)SMP.p.p << 8) | dp, data, mask);
}

static inline void smp_push_stack(uint8_t data)
{
   smp_write(0x100 | SMP.sp--, data, 0xff);
}

static inline uint8_t smp_pop_stack(void)
{
   return smp_read(0x100 | ++SMP.sp);
}


#endif
