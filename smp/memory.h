#ifndef __SMP_MEMORY_H
#define __SMP_MEMORY_H

// Need MMAPing for registers and stuff.
static inline uint8_t smp_read(uint16_t addr)
{
   return MEM.apuram[addr];
}

static inline void smp_write(uint16_t addr, uint8_t data)
{
   MEM.apuram[addr] = data;
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
   smp_write(addr, data);
}

static inline void smp_write_dp(uint8_t dp, uint8_t data)
{
   smp_write(((uint16_t)SMP.p.p << 8) | dp, data);
}

static inline void smp_writew_dp(uint8_t dp, uint16_t data)
{
   smp_write_dp(dp, data & 0xff);
   smp_write_dp(dp + 1, data >> 8);
}

static inline void smp_push_stack(uint8_t data)
{
   MEM.apuram[0x100 | SMP.sp--] = data;
}

static inline uint8_t smp_pop_stack(void)
{
   return MEM.apuram[0x100 | ++SMP.sp];
}


#endif
