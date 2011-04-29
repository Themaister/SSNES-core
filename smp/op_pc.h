#ifndef __SMP_OP_PC_H
#define __SMP_OP_PC_H

static inline void smp_op_jmp(void)
{
   SMP.pc = smp_readw_pc();
}

static inline void smp_op_jmpix(void)
{
   SMP.pc = smp_readw_addr(smp_readw_pc() + SMP.x);
}

#define SMP_OP_BBC_DECL(bit) smp_op_bbc_##bit
#define SMP_OP_BBC(bit) \
   static inline void SMP_OP_BBC_DECL(bit) (void) \
   { \
      uint8_t data = smp_op_read_dp(); \
      uint8_t target = smp_read_pc(); \
      SMP.pc += (int16_t)isel_if(data & (1 << bit), 0, target); \
   }

SMP_OP_BBC(0)
SMP_OP_BBC(1)
SMP_OP_BBC(2)
SMP_OP_BBC(3)
SMP_OP_BBC(4)
SMP_OP_BBC(5)
SMP_OP_BBC(6)
SMP_OP_BBC(7)

#define SMP_OP_BBS_DECL(bit) smp_op_bbs_##bit
#define SMP_OP_BBS(bit) \
   static inline void SMP_OP_BBS_DECL(bit) (void) \
   { \
      uint8_t data = smp_op_read_dp(); \
      uint8_t target = smp_read_pc(); \
      SMP.pc += (int16_t)isel_if(data & (1 << bit), target, 0); \
   }

SMP_OP_BBS(0)
SMP_OP_BBS(1)
SMP_OP_BBS(2)
SMP_OP_BBS(3)
SMP_OP_BBS(4)
SMP_OP_BBS(5)
SMP_OP_BBS(6)
SMP_OP_BBS(7)

#define SMP_OP_BRANCH_FLAG_DECL(flag) smp_op_branch_flag_##flag
#define SMP_OP_BRANCH_FLAG(flag) \
   static inline void SMP_OP_BRANCH_FLAG_DECL(flag) (void) \
   { \
      uint8_t target = smp_read_pc(); \
      SMP.pc += (int16_t)isel_if(SMP.p.flag, target, 0); \
   }

#define SMP_OP_BRANCH_N_FLAG_DECL(flag) smp_op_branch_n_flag_##flag
#define SMP_OP_BRANCH_N_FLAG(flag) \
   static inline void SMP_OP_BRANCH_N_FLAG_DECL(flag) (void) \
   { \
      uint8_t target = smp_read_pc(); \
      SMP.pc += (int16_t)isel_if(SMP.p.flag, 0, target); \
   }

SMP_OP_BRANCH_FLAG(v)
SMP_OP_BRANCH_FLAG(c)
SMP_OP_BRANCH_FLAG(n)
SMP_OP_BRANCH_FLAG(z)
SMP_OP_BRANCH_N_FLAG(v)
SMP_OP_BRANCH_N_FLAG(c)
SMP_OP_BRANCH_N_FLAG(n)
SMP_OP_BRANCH_N_FLAG(z)

static inline void smp_do_call(uint16_t addr)
{
   smp_push_stack(SMP.pc >> 8);
   smp_push_stack(SMP.pc & 0xff);
   SMP.pc = addr;
}

static inline void smp_op_call(void)
{
   uint16_t target = smp_readw_pc();
   smp_do_call(target);
}

static inline void smp_op_pcall(void)
{
   uint16_t target = 0xff00 | smp_read_pc();
   smp_do_call(target);
}

#define SMP_OP_TCALL_DECL(index) smp_op_tcall_##index
#define SMP_OP_TCALL(index) \
   static inline void SMP_OP_TCALL_DECL(index) (void) \
   { \
      uint16_t addr = 0xffde - (index << 1); \
      smp_do_call(addr); \
   }

SMP_OP_TCALL(0)
SMP_OP_TCALL(1)
SMP_OP_TCALL(2)
SMP_OP_TCALL(3)
SMP_OP_TCALL(4)
SMP_OP_TCALL(5)
SMP_OP_TCALL(6)
SMP_OP_TCALL(7)
SMP_OP_TCALL(8)
SMP_OP_TCALL(9)
SMP_OP_TCALL(10)
SMP_OP_TCALL(11)
SMP_OP_TCALL(12)
SMP_OP_TCALL(13)
SMP_OP_TCALL(14)
SMP_OP_TCALL(15)

static inline void smp_op_ret(void)
{
   uint16_t target = smp_pop_stack();
   target |= (uint16_t)smp_pop_stack() << 8;
   SMP.pc = target;
}

// Pop flags first or return addr? :V
static inline void smp_op_ret1(void)
{
   uint16_t target = smp_pop_stack();
   target |= (uint16_t)smp_pop_stack() << 8;
   SMP.pc = target;
   smp_set_p(smp_pop_stack());
}


#endif
