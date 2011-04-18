#ifndef __CPU_OP_ALU_H
#define __CPU_OP_ALU_H

static inline void cpu_op_adc_b(void)
{
   int result;

   if (!REGS.p.d) 
   {
      result = REGS.a.l + REGS.rd.l + REGS.p.c;
   } 
   else 
   {
      result = (REGS.a.l & 0x0f) + (REGS.rd.l & 0x0f) + (REGS.p.c << 0);
      if (result > 0x09) 
         result += 0x06;
      REGS.p.c = result > 0x0f;
      result = (REGS.a.l & 0xf0) + (REGS.rd.l & 0xf0) + (REGS.p.c << 4) + (result & 0x0f);
   }

   REGS.p.v = ~(REGS.a.l ^ REGS.rd.l) & (REGS.a.l ^ result) & 0x80;
   if (REGS.p.d && result > 0x9f) 
      result += 0x60;
   REGS.p.c = result > 0xff;
   REGS.p.n = result & 0x80;
   REGS.p.z = (uint8_t)result == 0;

   REGS.a.l = result;
}

static inline void cpu_op_adc_w(void)
{
   int result;

   if (!REGS.p.d) 
   {
      result = REGS.a.w + REGS.rd.w + REGS.p.c;
   } 
   else 
   {
      result = (REGS.a.w & 0x000f) + (REGS.rd.w & 0x000f) + (REGS.p.c <<  0);
      if (result > 0x0009) 
         result += 0x0006;
      REGS.p.c = result > 0x000f;
      result = (REGS.a.w & 0x00f0) + (REGS.rd.w & 0x00f0) + (REGS.p.c <<  4) + (result & 0x000f);
      if (result > 0x009f) 
         result += 0x0060;
      REGS.p.c = result > 0x00ff;
      result = (REGS.a.w & 0x0f00) + (REGS.rd.w & 0x0f00) + (REGS.p.c <<  8) + (result & 0x00ff);
      if (result > 0x09ff) 
         result += 0x0600;
      REGS.p.c = result > 0x0fff;
      result = (REGS.a.w & 0xf000) + (REGS.rd.w & 0xf000) + (REGS.p.c << 12) + (result & 0x0fff);
   }

   REGS.p.v = ~(REGS.a.w ^ REGS.rd.w) & (REGS.a.w ^ result) & 0x8000;
   if (REGS.p.d && result > 0x9fff) 
      result += 0x6000;
   REGS.p.c = result > 0xffff;
   REGS.p.n = result & 0x8000;
   REGS.p.z = (uint16_t)result == 0;

   REGS.a.w = result;
}

static inline void cpu_op_and_b(void)
{
   REGS.a.l &= REGS.rd.l;
   REGS.p.n = REGS.a.l & 0x80;
   REGS.p.z = REGS.a.l == 0;
}

static inline void cpu_op_and_w(void)
{
   REGS.a.w &= REGS.rd.w;
   REGS.p.n = REGS.a.w & 0x8000;
   REGS.p.z = REGS.a.w == 0;
}

static inline void cpu_op_bit_b(void) 
{
   REGS.p.n = REGS.rd.l & 0x80;
   REGS.p.v = REGS.rd.l & 0x40;
   REGS.p.z = (REGS.rd.l & REGS.a.l) == 0;
}

static inline void cpu_op_bit_w(void)
{
   REGS.p.n = REGS.rd.w & 0x8000;
   REGS.p.v = REGS.rd.w & 0x4000;
   REGS.p.z = (REGS.rd.w & REGS.a.w) == 0;
}

static inline void cpu_op_cmp_b(void)
{
   int r = REGS.a.l - REGS.rd.l;
   REGS.p.n = r & 0x80;
   REGS.p.z = (uint8)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_cmp_w(void)
{
   int r = REGS.a.w - REGS.rd.w;
   REGS.p.n = r & 0x8000;
   REGS.p.z = (uint16)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_cpx_b(void)
{
   int r = REGS.x.l - REGS.rd.l;
   REGS.p.n = r & 0x80;
   REGS.p.z = (uint8)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_cpx_w(void)
{
   int r = REGS.x.w - REGS.rd.w;
   REGS.p.n = r & 0x8000;
   REGS.p.z = (uint16)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_cpy_b(void)
{
   int r = REGS.y.l - REGS.rd.l;
   REGS.p.n = r & 0x80;
   REGS.p.z = (uint8)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_cpy_w(void)
{
   int r = REGS.y.w - REGS.rd.w;
   REGS.p.n = r & 0x8000;
   REGS.p.z = (uint16)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_eor_b(void)
{
   REGS.a.l ^= REGS.rd.l;
   REGS.p.n = REGS.a.l & 0x80;
   REGS.p.z = REGS.a.l == 0;
}

static inline void cpu_op_eor_w(void)
{
   REGS.a.w ^= REGS.rd.w;
   REGS.p.n = REGS.a.w & 0x8000;
   REGS.p.z = REGS.a.w == 0;
}

static inline void cpu_op_lda_b(void)
{
   REGS.a.l = REGS.rd.l;
   REGS.p.n = REGS.a.l & 0x80;
   REGS.p.z = REGS.a.l == 0;
}

static inline void cpu_op_lda_w(void)
{
   REGS.a.w = REGS.rd.w;
   REGS.p.n = REGS.a.w & 0x8000;
   REGS.p.z = REGS.a.w == 0;
}

static inline void cpu_op_ldx_b(void)
{
   REGS.x.l = REGS.rd.l;
   REGS.p.n = REGS.x.l & 0x80;
   REGS.p.z = REGS.x.l == 0;
}

static inline void cpu_op_ldx_w(void)
{
   REGS.x.w = REGS.rd.w;
   REGS.p.n = REGS.x.w & 0x8000;
   REGS.p.z = REGS.x.w == 0;
}

static inline void cpu_op_ldy_b(void)
{
   REGS.y.l = REGS.rd.l;
   REGS.p.n = REGS.y.l & 0x80;
   REGS.p.z = REGS.y.l == 0;
}

static inline void cpu_op_ldy_w(void)
{
   REGS.y.w = REGS.rd.w;
   REGS.p.n = REGS.y.w & 0x8000;
   REGS.p.z = REGS.y.w == 0;
}

static inline void cpu_op_ora_b(void)
{
   REGS.a.l |= REGS.rd.l;
   REGS.p.n = REGS.a.l & 0x80;
   REGS.p.z = REGS.a.l == 0;
}

static inline void cpu_op_ora_w(void)
{
   REGS.a.w |= REGS.rd.w;
   REGS.p.n = REGS.a.w & 0x8000;
   REGS.p.z = REGS.a.w == 0;
}

static inline void cpu_op_sbc_b(void)
{
   int result;
   REGS.rd.l ^= 0xff;

   if (!REGS.p.d) 
   {
      result = REGS.a.l + REGS.rd.l + REGS.p.c;
   } 
   else 
   {
      result = (REGS.a.l & 0x0f) + (REGS.rd.l & 0x0f) + (REGS.p.c << 0);
      if (result <= 0x0f) 
         result -= 0x06;
      REGS.p.c = result > 0x0f;
      result = (REGS.a.l & 0xf0) + (REGS.rd.l & 0xf0) + (REGS.p.c << 4) + (result & 0x0f);
   }

   REGS.p.v = ~(REGS.a.l ^ REGS.rd.l) & (REGS.a.l ^ result) & 0x80;
   if (REGS.p.d && result <= 0xff) 
      result -= 0x60;
   REGS.p.c = result > 0xff;
   REGS.p.n = result & 0x80;
   REGS.p.z = (uint8_t)result == 0;

   REGS.a.l = result;
}

static inline void cpu_op_sbc_w(void)
{
   int result;
   REGS.rd.w ^= 0xffff;

   if (!REGS.p.d) 
   {
      result = REGS.a.w + REGS.rd.w + REGS.p.c;
   } 
   else 
   {
      result = (REGS.a.w & 0x000f) + (REGS.rd.w & 0x000f) + (REGS.p.c <<  0);
      if (result <= 0x000f) 
         result -= 0x0006;
      REGS.p.c = result > 0x000f;

      result = (REGS.a.w & 0x00f0) + (REGS.rd.w & 0x00f0) + (REGS.p.c <<  4) + (result & 0x000f);
      if (result <= 0x00ff) 
         result -= 0x0060;
      REGS.p.c = result > 0x00ff;

      result = (REGS.a.w & 0x0f00) + (REGS.rd.w & 0x0f00) + (REGS.p.c <<  8) + (result & 0x00ff);
      if (result <= 0x0fff) 
         result -= 0x0600;
      REGS.p.c = result > 0x0fff;

      result = (REGS.a.w & 0xf000) + (REGS.rd.w & 0xf000) + (REGS.p.c << 12) + (result & 0x0fff);
   }

   REGS.p.v = ~(REGS.a.w ^ REGS.rd.w) & (REGS.a.w ^ result) & 0x8000;
   if (REGS.p.d && result <= 0xffff) 
      result -= 0x6000;
   REGS.p.c = result > 0xffff;
   REGS.p.n = result & 0x8000;
   REGS.p.z = (uint16_t)result == 0;

   REGS.a.w = result;
}

static inline void cpu_op_inc_b(void)
{
   REGS.rd.l++;
   REGS.p.n = REGS.rd.l & 0x80;
   REGS.p.z = REGS.rd.l == 0;
}

static inline void cpu_op_inc_w(void)
{
   REGS.rd.w++;
   REGS.p.n = REGS.rd.w & 0x8000;
   REGS.p.z = REGS.rd.w == 0;
}

static inline void cpu_op_dec_b(void)
{
   REGS.rd.l--;
   REGS.p.n = REGS.rd.l & 0x80;
   REGS.p.z = REGS.rd.l == 0;
}

static inline void cpu_op_dec_w(void)
{
   REGS.rd.w--;
   REGS.p.n = REGS.rd.w & 0x8000;
   REGS.p.z = REGS.rd.w == 0;
}

static inline void cpu_op_asl_b(void)
{
   REGS.p.c = REGS.rd.l & 0x80;
   REGS.rd.l <<= 1;
   REGS.p.n = REGS.rd.l & 0x80;
   REGS.p.z = REGS.rd.l == 0;
}

static inline void cpu_op_asl_w(void)
{
   REGS.p.c = REGS.rd.w & 0x8000;
   REGS.rd.w <<= 1;
   REGS.p.n = REGS.rd.w & 0x8000;
   REGS.p.z = REGS.rd.w == 0;
}

static inline void cpu_op_lsr_b(void)
{
   REGS.p.c = REGS.rd.l & 1;
   REGS.rd.l >>= 1;
   REGS.p.n = REGS.rd.l & 0x80;
   REGS.p.z = REGS.rd.l == 0;
}

static inline void cpu_op_lsr_w(void)
{
   REGS.p.c = REGS.rd.w & 1;
   REGS.rd.w >>= 1;
   REGS.p.n = REGS.rd.w & 0x8000;
   REGS.p.z = REGS.rd.w == 0;
}

static inline void cpu_op_rol_b(void)
{
   unsigned carry = (unsigned)REGS.p.c;
   REGS.p.c = REGS.rd.l & 0x80;
   REGS.rd.l = (REGS.rd.l << 1) | carry;
   REGS.p.n = REGS.rd.l & 0x80;
   REGS.p.z = REGS.rd.l == 0;
}

static inline void cpu_op_rol_w(void)
{
   unsigned carry = (unsigned)REGS.p.c;
   REGS.p.c = REGS.rd.w & 0x8000;
   REGS.rd.w = (REGS.rd.w << 1) | carry;
   REGS.p.n = REGS.rd.w & 0x8000;
   REGS.p.z = REGS.rd.w == 0;
}

static inline void cpu_op_ror_b(void)
{
   unsigned carry = (unsigned)REGS.p.c << 7;
   REGS.p.c = REGS.rd.l & 1;
   REGS.rd.l = carry | (REGS.rd.l >> 1);
   REGS.p.n = REGS.rd.l & 0x80;
   REGS.p.z = REGS.rd.l == 0;
}

static inline void cpu_op_ror_w(void)
{
   unsigned carry = (unsigned)REGS.p.c << 15;
   REGS.p.c = REGS.rd.w & 1;
   REGS.rd.w = carry | (REGS.rd.w >> 1);
   REGS.p.n = REGS.rd.w & 0x8000;
   REGS.p.z = REGS.rd.w == 0;
}

static inline void cpu_op_trb_b(void)
{
   REGS.p.z = (REGS.rd.l & REGS.a.l) == 0;
   REGS.rd.l &= ~REGS.a.l;
}

static inline void cpu_op_trb_w(void)
{
   REGS.p.z = (REGS.rd.w & REGS.a.w) == 0;
   REGS.rd.w &= ~REGS.a.w;
}

static inline void cpu_op_tsb_b(void)
{
   REGS.p.z = (REGS.rd.l & REGS.a.l) == 0;
   REGS.rd.l |= REGS.a.l;
}

static inline void cpu_op_tsb_w(void)
{
   REGS.p.z = (REGS.rd.w & REGS.a.w) == 0;
   REGS.rd.w |= REGS.a.w;
}


#endif
