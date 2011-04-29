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

#define DECL_ALU_MOVE_OP(op) \
   SMP_OP_ALU_DPX_DPY(op) \
   SMP_OP_ALU_DP_DP(op) \
   SMP_OP_ALU_DP_IMM(op) \
   SMP_OP_ALU_IMM(op) \
   SMP_OP_ALU_DP(op) \
   SMP_OP_ALU_DPIX(op) \
   SMP_OP_ALU_IDPX(op) \
   SMP_OP_ALU_IDPY(op) \
   SMP_OP_ALU_ADDR(op) \
   SMP_OP_ALU_ADDRX(op) \
   SMP_OP_ALU_ADDRY(op) \

#define DECL_MAIN_ALU \
   DECL_ALU_MOVE_OP(adc) \
   DECL_ALU_MOVE_OP(and) \
   DECL_ALU_MOVE_OP(cmp) \
   DECL_ALU_MOVE_OP(eor) \
   DECL_ALU_MOVE_OP(or) \
   DECL_ALU_MOVE_OP(sbc)


#define SMP_OP_ALU_DPX_DPY_DECL(op) smp_op_alu_dpx_dpy_##op
#define SMP_OP_ALU_DPX_DPY(op) \
   static inline void SMP_OP_ALU_DPX_DPY_DECL(op) (void) \
   { \
      uint8_t src2 = smp_op_read_dpy(); \
      uint8_t src1 = smp_op_read_dpx(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_dpx(res); \
   }

#define SMP_OP_ALU_DP_DP_DECL(op) smp_op_alu_dp_dp_##op
#define SMP_OP_ALU_DP_DP(op) \
   static inline void SMP_OP_ALU_DP_DP_DECL(op) (void) \
   { \
      uint8_t src2 = smp_op_read_dp(); \
      uint8_t src1 = smp_op_read_dp(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_dp(MEM.apuram[SMP.pc - 2], res); \
   }

#define SMP_OP_ALU_DP_IMM_DECL(op) smp_op_alu_dp_imm_##op
#define SMP_OP_ALU_DP_IMM(op) \
   static inline void SMP_OP_ALU_DP_IMM_DECL(op) (void) \
   { \
      uint8_t src2 = smp_op_read_const(); \
      uint8_t src1 = smp_op_read_dp(); \
      uint8_t res = smp_op_##op (src1, src2); \
      smp_op_write_dp(MEM.apuram[SMP.pc - 2], res); \
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
SMP_OP_MOVE_Y(dpix)
SMP_OP_MOVE_Y(addr)

static inline void smp_op_move_dpx_inc_a(void)
{
   smp_op_write_dpx_inc(SMP.ya.b.l);
}

static inline void smp_op_move_dpx_a(void)
{
   smp_op_write_dpx(SMP.ya.b.l);
}

static inline void smp_op_move_idpx_a(void)
{
   uint8_t dp = smp_read_pc();
   smp_op_write_idpx(dp, SMP.ya.b.l);
}

static inline void smp_op_move_idpy_a(void)
{
   uint8_t dp = smp_read_pc();
   smp_op_write_idpy(dp, SMP.ya.b.l);
}

static inline void smp_op_move_dp_dp(void)
{
   uint8_t dp_in = smp_read_pc();
   uint8_t dp_out = smp_read_pc();
   smp_write_dp(dp_out, smp_read_dp(dp_in));
}

#endif
