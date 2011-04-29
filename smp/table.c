#include "table.h"

#include "op.h"

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


smp_op_t ssnes_smp_optable[256];
uint8_t ssnes_smp_cycle_table[256];
