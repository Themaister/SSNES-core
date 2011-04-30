#ifndef __SMP_OP_WRITE_H
#define __SMP_OP_WRITE_H

static inline void smp_op_write_a(uint8_t rd)
{
   SMP.ya.b.l = rd;
}

static inline void smp_op_write_x(uint8_t rd)
{
   SMP.x = rd;
}

static inline void smp_op_write_y(uint8_t rd)
{
   SMP.ya.b.h = rd;
}

static inline void smp_op_write_ya(uint16_t rd)
{
   SMP.ya.w = rd;
}

static inline void smp_op_write_sp(uint8_t rd)
{
   SMP.sp = rd;
}

static inline void smp_op_write_dp(uint8_t rd)
{
   smp_write_dp(smp_read_pc(), rd);
}

static inline void smp_op_write_dpx(uint8_t rd)
{
   smp_write_dp(SMP.x, rd);
}

static inline void smp_op_write_dpix(uint8_t rd)
{
   smp_write_dp(smp_read_pc() + SMP.x, rd);
}

static inline void smp_op_write_dpiy(uint8_t rd)
{
   smp_write_dp(smp_read_pc() + SMP.ya.b.h, rd);
}

static inline void smp_op_write_dpx_inc(uint8_t rd)
{
   smp_write_dp(SMP.x++, rd);
}

static inline void smp_op_write_dpy(uint8_t rd)
{
   smp_write_dp(SMP.ya.b.h, rd);
}

static inline void smp_op_write_addr(uint8_t rd)
{
   smp_write_addr(smp_readw_pc(), rd);
}

static inline void smp_op_write_addrx(uint8_t rd)
{
   smp_write_addr(smp_readw_pc() + SMP.x, rd);
}

static inline void smp_op_write_addry(uint8_t rd)
{
   smp_write_addr(smp_readw_pc() + SMP.ya.b.h, rd);
}

static inline void smp_op_write_idpx(uint8_t rd)
{
   uint16_t a = smp_readw_dp(smp_read_pc() + SMP.x);
   smp_write_addr(a, rd);
}

static inline void smp_op_write_idpy(uint8_t rd)
{
   uint16_t a = smp_readw_dp(smp_read_pc());
   smp_write_addr(a + SMP.ya.b.h, rd);
}



#define SMP_OP_CLEAR_DECL(bit) smp_op_clear_##bit
#define SMP_OP_CLEAR(bit) \
   static inline void SMP_OP_CLEAR_DECL(bit) (void) \
   { \
      uint8_t mask = 1 << bit; \
      smp_write_dp_bit(smp_read_pc(), 0, mask); \
   }

SMP_OP_CLEAR(0)
SMP_OP_CLEAR(1)
SMP_OP_CLEAR(2)
SMP_OP_CLEAR(3)
SMP_OP_CLEAR(4)
SMP_OP_CLEAR(5)
SMP_OP_CLEAR(6)
SMP_OP_CLEAR(7)

#define SMP_OP_SET_DECL(bit) smp_op_set_##bit
#define SMP_OP_SET(bit) \
   static inline void SMP_OP_SET_DECL(bit) (void) \
   { \
      uint8_t mask = 1 << bit; \
      smp_write_dp_bit(smp_read_pc(), 0xff, mask); \
   }

SMP_OP_SET(0)
SMP_OP_SET(1)
SMP_OP_SET(2)
SMP_OP_SET(3)
SMP_OP_SET(4)
SMP_OP_SET(5)
SMP_OP_SET(6)
SMP_OP_SET(7)

static inline void smp_op_write_bit(uint16_t addr, uint8_t bit, uint8_t data)
{
   smp_write_addr_bit(addr, data, 1 << bit);
}


#endif
