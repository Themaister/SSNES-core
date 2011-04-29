#ifndef __SMP_OP_RMW_H
#define __SMP_OP_RMW_H

#define SMP_OP_RMW_A_DECL(op) smp_op_rmw_a_##op
#define SMP_OP_RMW_A(op) \
   static inline void SMP_OP_RMW_A_DECL(op) (void) \
   { \
      SMP.ya.b.l = smp_op_##op (SMP.ya.b.l); \
   }

#define SMP_OP_RMW_X_DECL(op) smp_op_rmw_x_##op
#define SMP_OP_RMW_X(op) \
   static inline void SMP_OP_RMW_X_DECL(op) (void) \
   { \
      SMP.x = smp_op_##op (SMP.x); \
   }

#define SMP_OP_RMW_Y_DECL(op) smp_op_rmw_y_##op
#define SMP_OP_RMW_Y(op) \
   static inline void SMP_OP_RMW_Y_DECL(op) (void) \
   { \
      SMP.ya.b.h = smp_op_##op (SMP.ya.b.h); \
   }

#define SMP_OP_RMW_DP_DECL(op) smp_op_rmw_dp_##op
#define SMP_OP_RMW_DP(op) \
   static inline void SMP_OP_RMW_DP_DECL(op) (void) \
   { \
      uint8_t dp = smp_read_pc(); \
      smp_write_dp(dp, smp_op_##op (smp_read_dp(dp))); \
   }

#define SMP_OP_RMW_DPX_DECL(op) smp_op_rmw_dpx_##op
#define SMP_OP_RMW_DPX(op) \
   static inline void SMP_OP_RMW_DPX_DECL(op) (void) \
   { \
      uint8_t dp = smp_read_pc() + SMP.x; \
      smp_write_dp(dp, smp_op_##op (smp_read_dp(dp))); \
   }

#define SMP_OP_RMW_ADDR_DECL(op) smp_op_rmw_addr_##op
#define SMP_OP_RMW_ADDR(op) \
   static inline void SMP_OP_RMW_ADDR_DECL(op) (void) \
   { \
      uint16_t addr = smp_readw_pc(); \
      smp_write_addr(addr, smp_op_##op (smp_read_addr(addr))); \
   }

SMP_OP_RMW_A(dec);
SMP_OP_RMW_A(inc);
SMP_OP_RMW_A(lsr);
SMP_OP_RMW_A(asl);

SMP_OP_RMW_DP(dec);
SMP_OP_RMW_DP(inc);
SMP_OP_RMW_DP(lsr);
SMP_OP_RMW_DP(asl);

SMP_OP_RMW_DPX(dec);
SMP_OP_RMW_DPX(inc);
SMP_OP_RMW_DPX(lsr);
SMP_OP_RMW_DPX(asl);

SMP_OP_RMW_ADDR(dec);
SMP_OP_RMW_ADDR(inc);
SMP_OP_RMW_ADDR(lsr);
SMP_OP_RMW_ADDR(asl);

SMP_OP_RMW_X(dec);
SMP_OP_RMW_X(inc);
SMP_OP_RMW_Y(dec);
SMP_OP_RMW_Y(inc);

static inline void smp_op_rmw_dp_decw(void)
{
   uint8_t dp = smp_read_pc();
   smp_writew_dp(dp, smp_op_decw(smp_readw_dp(dp)));
}

static inline void smp_op_rmw_dp_incw(void)
{
   uint8_t dp = smp_read_pc();
   smp_writew_dp(dp, smp_op_incw(smp_readw_dp(dp)));
}

#endif
