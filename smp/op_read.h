#ifndef __SMP_OP_READ_H
#define __SMP_OP_READ_H

static inline uint8_t smp_op_read_const(void)
{
   return smp_read_pc();
}

static inline uint8_t smp_op_read_a(void)
{
   return SMP.ya.b.l;
}

static inline uint8_t smp_op_read_x(void)
{
   return SMP.x;
}

static inline uint8_t smp_op_read_y(void)
{
   return SMP.ya.b.h;
} 

static inline uint16_t smp_op_read_ya(void)
{
   return SMP.ya.w;
}

static inline uint8_t smp_op_read_dp(void)
{
   return smp_read_dp(smp_read_pc());
}

static inline uint16_t smp_op_readw_dp(void)
{
   return smp_readw_dp(smp_read_pc());
}

static inline uint8_t smp_op_read_dpx(void)
{
   return smp_read_dp(SMP.x);
}

static inline uint8_t smp_op_read_dpy(void)
{
   return smp_read_dp(SMP.ya.b.h);
}

static inline uint8_t smp_op_read_stack(void)
{
   return smp_pop_stack();
}

static inline uint8_t smp_op_read_addr(void)
{
   uint16_t addr = smp_readw_pc();
   return smp_read_addr(addr);
}

static inline uint8_t smp_op_read_addrx(void)
{
   uint16_t addr = smp_readw_pc();
   return smp_read_addr(addr + SMP.x);
}

static inline uint8_t smp_op_read_idpx(void)
{
   uint8_t dp = smp_read_pc() + SMP.x;
   uint16_t addr = smp_readw_dp(dp);
   return smp_read_addr(addr);
}

static inline uint8_t smp_op_read_idpy(void)
{
   uint8_t dp = smp_read_pc();
   uint16_t addr = smp_readw_dp(dp) + SMP.ya.b.h;
   return smp_read_addr(addr);
}







#endif
