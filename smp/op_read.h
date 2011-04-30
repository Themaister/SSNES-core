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

static inline uint8_t smp_op_read_sp(void)
{
   return SMP.sp;
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

static inline uint8_t smp_op_read_dpx_inc(void)
{
   return smp_read_dp(SMP.x++);
}

static inline uint8_t smp_op_read_dpy(void)
{
   return smp_read_dp(SMP.ya.b.h);
}

static inline uint8_t smp_op_read_dpix(void)
{
   return smp_read_dp(smp_read_pc() + SMP.x);
}

static inline uint8_t smp_op_read_dpiy(void)
{
   return smp_read_dp(smp_read_pc() + SMP.ya.b.h);
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

static inline uint8_t smp_op_read_addry(void)
{
   uint16_t addr = smp_readw_pc();
   return smp_read_addr(addr + SMP.ya.b.h);
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

static inline uint8_t smp_op_read_bit(void)
{
   uint16_t addr = smp_readw_pc();
   uint16_t bit = addr >> 13;
   addr &= 0x1fff;
   return (smp_read_addr(addr) >> bit) & 1;
}

#define SMP_OP_ALU_BIT_DECL(op) smp_op_alu_bit_##op
#define SMP_OP_ALU_BIT(op) \
   static inline void SMP_OP_ALU_BIT_DECL(op) (void) \
   { \
      smp_op_##op (smp_op_read_bit()); \
   }

SMP_OP_ALU_BIT(and1)
SMP_OP_ALU_BIT(andn1)
SMP_OP_ALU_BIT(or1)
SMP_OP_ALU_BIT(orn1)
SMP_OP_ALU_BIT(eor1)


#define DECL_ALU_MOVE_OP(op) \
   SMP_OP_ALU_DPX_DPY(op) \
   SMP_OP_ALU_DP_DP(op) \
   SMP_OP_ALU_DP_IMM(op) \
   SMP_OP_ALU_IMM(op) \
   SMP_OP_ALU_DP(op) \
   SMP_OP_ALU_DPX(op) \
   SMP_OP_ALU_DPIX(op) \
   SMP_OP_ALU_IDPX(op) \
   SMP_OP_ALU_IDPY(op) \
   SMP_OP_ALU_ADDR(op) \
   SMP_OP_ALU_ADDRX(op) \
   SMP_OP_ALU_ADDRY(op) \

#define DECL_MAIN_ALU \
   DECL_ALU_MOVE_OP(adc) \
   DECL_ALU_MOVE_OP(and) \
   DECL_ALU_MOVE_OP(eor) \
   DECL_ALU_MOVE_OP(or) \
   DECL_ALU_MOVE_OP(sbc)

#define SMP_OP_ALU_CMP_DECL(read1, read2) smp_op_alu_cmp_##read1##_##read2
#define SMP_OP_ALU_CMP(read1, read2) \
   static inline void SMP_OP_ALU_CMP_DECL(read1, read2) (void) \
   { \
      uint8_t src1 = smp_op_read_##read1 (); \
      uint8_t src2 = smp_op_read_##read2 (); \
      smp_op_cmp(src1, src2); \
   }

SMP_OP_ALU_CMP(dpx, dpy)
SMP_OP_ALU_CMP(dp, dp)
SMP_OP_ALU_CMP(dp, const)
SMP_OP_ALU_CMP(a, const)
SMP_OP_ALU_CMP(a, dp)
SMP_OP_ALU_CMP(a, dpx)
SMP_OP_ALU_CMP(a, dpix)
SMP_OP_ALU_CMP(a, idpx)
SMP_OP_ALU_CMP(a, idpy)
SMP_OP_ALU_CMP(a, addr)
SMP_OP_ALU_CMP(a, addrx)
SMP_OP_ALU_CMP(a, addry)

#define SMP_OP_ALU_DPX_DPY_DECL(op) smp_op_alu_dpx_dpy_##op
#define SMP_OP_ALU_DPX_DPY(op) \
   static inline void SMP_OP_ALU_DPX_DPY_DECL(op) (void) \
   { \
      uint8_t dpy = smp_read_pc(); \
      uint8_t dpx = smp_read_pc(); \
      uint8_t src2 = smp_read_dp(dpy); \
      uint8_t src1 = smp_read_dp(dpx); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_write_dp(dpx, res); \
   }

#define SMP_OP_ALU_DP_DP_DECL(op) smp_op_alu_dp_dp_##op
#define SMP_OP_ALU_DP_DP(op) \
   static inline void SMP_OP_ALU_DP_DP_DECL(op) (void) \
   { \
      uint8_t dp_src = smp_read_pc(); \
      uint8_t dp_dst = smp_read_pc(); \
      uint8_t src2 = smp_read_dp(dp_src); \
      uint8_t src1 = smp_read_dp(dp_dst); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_write_dp(dp_dst, res); \
   }

#define SMP_OP_ALU_DP_IMM_DECL(op) smp_op_alu_dp_imm_##op
#define SMP_OP_ALU_DP_IMM(op) \
   static inline void SMP_OP_ALU_DP_IMM_DECL(op) (void) \
   { \
      uint8_t src2 = smp_op_read_const(); \
      uint8_t dp = smp_read_pc(); \
      uint8_t src1 = smp_read_dp(dp); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_write_dp(dp, res); \
   }

#define SMP_OP_ALU_IMM_DECL(op) smp_op_alu_imm_##op
#define SMP_OP_ALU_IMM(op) \
   static inline void SMP_OP_ALU_IMM_DECL(op) (void) \
   { \
      uint8_t src1 = smp_op_read_a(); \
      uint8_t src2 = smp_op_read_const(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_a(res); \
   }

#define SMP_OP_ALU_DP_DECL(op) smp_op_alu_dp_##op
#define SMP_OP_ALU_DP(op) \
   static inline void SMP_OP_ALU_DP_DECL(op) (void) \
   { \
      uint8_t src1 = smp_op_read_a(); \
      uint8_t src2 = smp_op_read_dp(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_a(res); \
   }

#define SMP_OP_ALU_DPX_DECL(op) smp_op_alu_dpx_##op
#define SMP_OP_ALU_DPX(op) \
   static inline void SMP_OP_ALU_DPX_DECL(op) (void) \
   { \
      uint8_t src1 = smp_op_read_a(); \
      uint8_t src2 = smp_op_read_dpx(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_a(res); \
   }

#define SMP_OP_ALU_DPIX_DECL(op) smp_op_alu_dpix_##op
#define SMP_OP_ALU_DPIX(op) \
   static inline void SMP_OP_ALU_DPIX_DECL(op) (void) \
   { \
      uint8_t src1 = smp_op_read_a(); \
      uint8_t src2 = smp_op_read_dpix(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_a(res); \
   }

#define SMP_OP_ALU_IDPX_DECL(op) smp_op_alu_idpx_##op
#define SMP_OP_ALU_IDPX(op) \
   static inline void SMP_OP_ALU_IDPX_DECL(op) (void) \
   { \
      uint8_t src1 = smp_op_read_a(); \
      uint8_t src2 = smp_op_read_idpx(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_a(res); \
   }

#define SMP_OP_ALU_IDPY_DECL(op) smp_op_alu_idpy_##op
#define SMP_OP_ALU_IDPY(op) \
   static inline void SMP_OP_ALU_IDPY_DECL(op) (void) \
   { \
      uint8_t src1 = smp_op_read_a(); \
      uint8_t src2 = smp_op_read_idpy(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_a(res); \
   }

#define SMP_OP_ALU_ADDR_DECL(op) smp_op_alu_addr_##op
#define SMP_OP_ALU_ADDR(op) \
   static inline void SMP_OP_ALU_ADDR_DECL(op) (void) \
   { \
      uint8_t src1 = smp_op_read_a(); \
      uint8_t src2 = smp_op_read_addr(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_a(res); \
   }

#define SMP_OP_ALU_ADDRX_DECL(op) smp_op_alu_addrx_##op
#define SMP_OP_ALU_ADDRX(op) \
   static inline void SMP_OP_ALU_ADDRX_DECL(op) (void) \
   { \
      uint8_t src1 = smp_op_read_a(); \
      uint8_t src2 = smp_op_read_addrx(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_a(res); \
   }

#define SMP_OP_ALU_ADDRY_DECL(op) smp_op_alu_addry_##op
#define SMP_OP_ALU_ADDRY(op) \
   static inline void SMP_OP_ALU_ADDRY_DECL(op) (void) \
   { \
      uint8_t src1 = smp_op_read_a(); \
      uint8_t src2 = smp_op_read_addry(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_a(res); \
   }


DECL_MAIN_ALU

#define SMP_OP_ALUXY_CMP_DECL(reg, mode) smp_op_alu##reg##_##mode##_cmp
#define SMP_OP_ALUXY_CMP(reg, mode) \
   static inline void SMP_OP_ALUXY_CMP_DECL(reg, mode) (void) \
   { \
      uint8_t src1 = smp_op_read_##reg (); \
      uint8_t src2 = smp_op_read_##mode (); \
      smp_op_cmp(src1, src2); \
   }

SMP_OP_ALUXY_CMP(x, const)
SMP_OP_ALUXY_CMP(x, dp)
SMP_OP_ALUXY_CMP(x, addr)
SMP_OP_ALUXY_CMP(y, const)
SMP_OP_ALUXY_CMP(y, dp)
SMP_OP_ALUXY_CMP(y, addr)

#define SMP_OP_ALUW_DECL(op) smp_op_aluw##op
#define SMP_OP_ALUW(op) \
   static inline void SMP_OP_ALUW_DECL(op) (void) \
   { \
      SMP.ya.w = smp_op_##op (SMP.ya.w, smp_op_readw_dp()); \
   }

SMP_OP_ALUW(cmpw)
SMP_OP_ALUW(addw)
SMP_OP_ALUW(subw)


static inline void smp_op_move_sp_x(void)
{
   SMP.sp = SMP.x;
}

#define SMP_OP_MOVE_A_DECL(op) smp_op_move_a_##op
#define SMP_OP_MOVE_A(op) \
   static inline void SMP_OP_MOVE_A_DECL(op) (void) \
   { \
      uint8_t data = smp_op_read_##op (); \
      SMP.ya.b.l = data; \
      SMP.p.z = (data == 0); \
      SMP.p.n = data & 0x80; \
   }

SMP_OP_MOVE_A(const)
SMP_OP_MOVE_A(dpx)
SMP_OP_MOVE_A(dpx_inc)
SMP_OP_MOVE_A(idpx)
SMP_OP_MOVE_A(idpy)
SMP_OP_MOVE_A(x)
SMP_OP_MOVE_A(y)
SMP_OP_MOVE_A(dp)
SMP_OP_MOVE_A(dpix)
SMP_OP_MOVE_A(addr)
SMP_OP_MOVE_A(addrx)
SMP_OP_MOVE_A(addry)

#define SMP_OP_MOVE_X_DECL(op) smp_op_move_x_##op
#define SMP_OP_MOVE_X(op) \
   static inline void SMP_OP_MOVE_X_DECL(op) (void) \
   { \
      uint8_t data = smp_op_read_##op (); \
      SMP.x = data; \
      SMP.p.z = (data == 0); \
      SMP.p.n = data & 0x80; \
   }

SMP_OP_MOVE_X(const)
SMP_OP_MOVE_X(a)
SMP_OP_MOVE_X(sp)
SMP_OP_MOVE_X(dp)
SMP_OP_MOVE_X(dpy)
SMP_OP_MOVE_X(dpiy)
SMP_OP_MOVE_X(addr)

#define SMP_OP_MOVE_Y_DECL(op) smp_op_move_y_##op
#define SMP_OP_MOVE_Y(op) \
   static inline void SMP_OP_MOVE_Y_DECL(op) (void) \
   { \
      uint8_t data = smp_op_read_##op (); \
      SMP.ya.b.h = data; \
      SMP.p.z = (data == 0); \
      SMP.p.n = data & 0x80; \
   }

SMP_OP_MOVE_Y(const)
SMP_OP_MOVE_Y(a)
SMP_OP_MOVE_Y(dp)
SMP_OP_MOVE_Y(dpx)
SMP_OP_MOVE_Y(dpix)
SMP_OP_MOVE_Y(addr)

#define SMP_OP_MOVE_DECL(dst, src) smp_op_move_##dst##_##src
#define SMP_OP_MOVE(dst, src) \
   static inline void SMP_OP_MOVE_DECL(dst, src) (void) \
   { \
      smp_op_write_##dst (smp_op_read_##src ()); \
   }

SMP_OP_MOVE(addrx, a)
SMP_OP_MOVE(addry, a)
SMP_OP_MOVE(addrx, y)
SMP_OP_MOVE(addry, x)

SMP_OP_MOVE(dp, a)
SMP_OP_MOVE(dp, y)
SMP_OP_MOVE(dp, x)

SMP_OP_MOVE(dpix, a)
SMP_OP_MOVE(dpix, y)
SMP_OP_MOVE(dpiy, x)

SMP_OP_MOVE(addr, a)
SMP_OP_MOVE(addr, y)
SMP_OP_MOVE(addr, x)

SMP_OP_MOVE(dp, const)

SMP_OP_MOVE(dpx_inc, a)
SMP_OP_MOVE(dpx, a)
SMP_OP_MOVE(idpy, a)
SMP_OP_MOVE(idpx, a)

SMP_OP_MOVE(dp, dp)

static inline void smp_op_movw_ya_d(void)
{
   uint16_t wd = smp_op_readw_dp();
   SMP.p.z = (wd == 0);
   SMP.p.n = wd & 0x8000;
   SMP.ya.w = wd;
}

static inline void smp_op_movw_d_ya(void)
{
   uint8_t dp = smp_read_pc();
   smp_writew_dp(dp, SMP.ya.w);
}

static inline void smp_op_mov1_c_bit(void)
{
   SMP.p.c = smp_op_read_bit();
}

static inline void smp_op_mov1_bit_c(void)
{
   uint16_t addr = smp_readw_pc();
   uint8_t bit = addr >> 13;
   uint8_t mask = 1 << bit;
   smp_write_addr_bit(addr & 0x1fff, (uint8_t)SMP.p.c << bit, mask); 
}

#endif
