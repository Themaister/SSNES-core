#ifndef __CPU_OP_ALU_H
#define __CPU_OP_ALU_H

#include <stdint.h>

// Straight ALU ops. Performed on A register.
static inline void cpu_op_adc_b(uint8_t rd)
{
   uint16_t result;

   if (!REGS.p.d) 
   {
      result = (uint16_t)REGS.a.b.l + (uint16_t)rd + (uint16_t)REGS.p.c;
   } 
   else 
   {
      result = (REGS.a.b.l & 0x0f) + (rd & 0x0f) + ((uint16_t)REGS.p.c << 0);
      if (result > 0x09) 
         result += 0x06;
      REGS.p.c = result > 0x0f;
      result = (REGS.a.b.l & 0xf0) + (rd & 0xf0) + ((uint16_t)REGS.p.c << 4) + (result & 0x0f);
   }

   REGS.p.v = ~(REGS.a.b.l ^ rd) & (REGS.a.b.l ^ result) & 0x80;
   if (REGS.p.d && result > 0x9f) 
      result += 0x60;

   REGS.p.c = result > 0xff;
   REGS.p.n = result & 0x80;
   REGS.p.z = (result & 0xff) == 0;

   REGS.a.b.l = result & 0xff;
}

static inline void cpu_op_adc_w(uint16_t rd)
{
   uint32_t result;

   if (!REGS.p.d) 
   {
      result = (uint32_t)REGS.a.w + (uint32_t)rd + (uint32_t)REGS.p.c;
   } 
   else 
   {
      result = (REGS.a.w & 0x000f) + (rd & 0x000f) + ((uint32_t)REGS.p.c <<  0);
      if (result > 0x0009) 
         result += 0x0006;
      REGS.p.c = result > 0x000f;
      result = (REGS.a.w & 0x00f0) + (rd & 0x00f0) + ((uint32_t)REGS.p.c <<  4) + (result & 0x000f);
      if (result > 0x009f) 
         result += 0x0060;
      REGS.p.c = result > 0x00ff;
      result = (REGS.a.w & 0x0f00) + (rd & 0x0f00) + ((uint32_t)REGS.p.c <<  8) + (result & 0x00ff);
      if (result > 0x09ff) 
         result += 0x0600;
      REGS.p.c = result > 0x0fff;
      result = (REGS.a.w & 0xf000) + (rd & 0xf000) + ((uint32_t)REGS.p.c << 12) + (result & 0x0fff);
   }

   REGS.p.v = ~(REGS.a.w ^ rd) & (REGS.a.w ^ result) & 0x8000;
   if (REGS.p.d && result > 0x9fff) 
      result += 0x6000;
   REGS.p.c = result > 0xffff;
   REGS.p.n = result & 0x8000;
   REGS.p.z = (result & 0xffff) == 0;

   REGS.a.w = result & 0xffff;
}

static inline void cpu_op_and_b(uint8_t rd)
{
   REGS.a.b.l &= rd;
   REGS.p.n = REGS.a.b.l & 0x80;
   REGS.p.z = REGS.a.b.l == 0;
}

static inline void cpu_op_and_w(uint16_t rd)
{
   REGS.a.w &= rd;
   REGS.p.n = REGS.a.w & 0x8000;
   REGS.p.z = REGS.a.w == 0;
}

static inline void cpu_op_bit_b(uint8_t rd) 
{
   REGS.p.n = rd & 0x80;
   REGS.p.v = rd & 0x40;
   REGS.p.z = (rd & REGS.a.b.l) == 0;
}

static inline void cpu_op_bit_w(uint16_t rd)
{
   REGS.p.n = rd & 0x8000;
   REGS.p.v = rd & 0x4000;
   REGS.p.z = (rd & REGS.a.w) == 0;
}

static inline void cpu_op_bit_imm_b(uint8_t rd)
{
   REGS.p.z = (rd & REGS.a.b.l) == 0;
}

static inline void cpu_op_bit_imm_w(uint16_t rd)
{
   REGS.p.z = (rd & REGS.a.w) == 0;
}

static inline void cpu_op_cmp_b(uint8_t rd)
{
   int r = (int)REGS.a.b.l - (int)rd;
   REGS.p.n = r & 0x80;
   REGS.p.z = (uint8_t)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_cmp_w(uint16_t rd)
{
   int r = (int)REGS.a.w - (int)rd;
   REGS.p.n = r & 0x8000;
   REGS.p.z = (uint16_t)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_cpx_b(uint8_t rd)
{
   int r = (int)REGS.x.b.l - (int)rd;
   REGS.p.n = r & 0x80;
   REGS.p.z = (uint8_t)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_cpx_w(uint16_t rd)
{
   int r = (int)REGS.x.w - (int)rd;
   REGS.p.n = r & 0x8000;
   REGS.p.z = (uint16_t)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_cpy_b(uint8_t rd)
{
   int r = (int)REGS.y.b.l - (int)rd;
   REGS.p.n = r & 0x80;
   REGS.p.z = (uint8_t)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_cpy_w(uint16_t rd)
{
   int r = (int)REGS.y.w - (int)rd;
   REGS.p.n = r & 0x8000;
   REGS.p.z = (uint16_t)r == 0;
   REGS.p.c = r >= 0;
}

static inline void cpu_op_eor_b(uint8_t rd)
{
   REGS.a.b.l ^= rd;
   REGS.p.n = REGS.a.b.l & 0x80;
   REGS.p.z = REGS.a.b.l == 0;
}

static inline void cpu_op_eor_w(uint16_t rd)
{
   REGS.a.w ^= rd;
   REGS.p.n = REGS.a.w & 0x8000;
   REGS.p.z = REGS.a.w == 0;
}

static inline void cpu_op_lda_b(uint8_t rd)
{
   REGS.a.b.l = rd;
   REGS.p.n = REGS.a.b.l & 0x80;
   REGS.p.z = REGS.a.b.l == 0;
}

static inline void cpu_op_lda_w(uint16_t rd)
{
   REGS.a.w = rd;
   REGS.p.n = REGS.a.w & 0x8000;
   REGS.p.z = REGS.a.w == 0;
}

static inline void cpu_op_ldx_b(uint8_t rd)
{
   REGS.x.b.l = rd;
   REGS.p.n = REGS.x.b.l & 0x80;
   REGS.p.z = REGS.x.b.l == 0;
}

static inline void cpu_op_ldx_w(uint16_t rd)
{
   REGS.x.w = rd;
   REGS.p.n = REGS.x.w & 0x8000;
   REGS.p.z = REGS.x.w == 0;
}

static inline void cpu_op_ldy_b(uint8_t rd)
{
   REGS.y.b.l = rd;
   REGS.p.n = REGS.y.b.l & 0x80;
   REGS.p.z = REGS.y.b.l == 0;
}

static inline void cpu_op_ldy_w(uint16_t rd)
{
   REGS.y.w = rd;
   REGS.p.n = REGS.y.w & 0x8000;
   REGS.p.z = REGS.y.w == 0;
}

static inline void cpu_op_ora_b(uint8_t rd)
{
   REGS.a.b.l |= rd;
   REGS.p.n = REGS.a.b.l & 0x80;
   REGS.p.z = REGS.a.b.l == 0;
}

static inline void cpu_op_ora_w(uint16_t rd)
{
   REGS.a.w |= rd;
   REGS.p.n = REGS.a.w & 0x8000;
   REGS.p.z = REGS.a.w == 0;
}

static inline void cpu_op_sbc_b(uint8_t rd)
{
   uint16_t result;
   rd ^= 0xff;

   if (!REGS.p.d) 
   {
      result = (uint16_t)REGS.a.b.l + (uint16_t)rd + (uint16_t)REGS.p.c;
   } 
   else 
   {
      result = (REGS.a.b.l & 0x0f) + (rd & 0x0f) + ((uint16_t)REGS.p.c << 0);
      if (result <= 0x0f) 
         result -= 0x06;
      REGS.p.c = result > 0x0f;
      result = (REGS.a.b.l & 0xf0) + (rd & 0xf0) + ((uint16_t)REGS.p.c << 4) + (result & 0x0f);
   }

   REGS.p.v = ~(REGS.a.b.l ^ rd) & (REGS.a.b.l ^ result) & 0x80;
   if (REGS.p.d && result <= 0xff) 
      result -= 0x60;
   REGS.p.c = result > 0xff;
   REGS.p.n = result & 0x80;
   REGS.p.z = (result & 0xff) == 0;

   REGS.a.b.l = result & 0xff;
}

static inline void cpu_op_sbc_w(uint16_t rd)
{
   uint32_t result;
   rd ^= 0xffff;

   if (!REGS.p.d) 
   {
      result = (uint32_t)REGS.a.w + (uint32_t)rd + (uint32_t)REGS.p.c;
   } 
   else 
   {
      result = (REGS.a.w & 0x000f) + (rd & 0x000f) + ((uint32_t)REGS.p.c <<  0);
      if (result <= 0x000f) 
         result -= 0x0006;
      REGS.p.c = result > 0x000f;

      result = (REGS.a.w & 0x00f0) + (rd & 0x00f0) + ((uint32_t)REGS.p.c <<  4) + (result & 0x000f);
      if (result <= 0x00ff) 
         result -= 0x0060;
      REGS.p.c = result > 0x00ff;

      result = (REGS.a.w & 0x0f00) + (rd & 0x0f00) + ((uint32_t)REGS.p.c <<  8) + (result & 0x00ff);
      if (result <= 0x0fff) 
         result -= 0x0600;
      REGS.p.c = result > 0x0fff;

      result = (REGS.a.w & 0xf000) + (rd & 0xf000) + ((uint32_t)REGS.p.c << 12) + (result & 0x0fff);
   }

   REGS.p.v = ~(REGS.a.w ^ rd) & (REGS.a.w ^ result) & 0x8000;
   if (REGS.p.d && result <= 0xffff) 
      result -= 0x6000;
   REGS.p.c = result > 0xffff;
   REGS.p.n = result & 0x8000;
   REGS.p.z = (result & 0xffff) == 0;

   REGS.a.w = result & 0xffff;
}

// Modifying ALU ops. Data is read, modified and written back.
static inline uint8_t cpu_op_inc_b(uint8_t rd)
{
   rd++;
   REGS.p.n = rd & 0x80;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint16_t cpu_op_inc_w(uint16_t rd)
{
   rd++;
   REGS.p.n = rd & 0x8000;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint8_t cpu_op_dec_b(uint8_t rd)
{
   rd--;
   REGS.p.n = rd & 0x80;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint16_t cpu_op_dec_w(uint16_t rd)
{
   rd--;
   REGS.p.n = rd & 0x8000;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint8_t cpu_op_asl_b(uint8_t rd)
{
   REGS.p.c = rd & 0x80;
   rd <<= 1;
   REGS.p.n = rd & 0x80;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint16_t cpu_op_asl_w(uint16_t rd)
{
   REGS.p.c = rd & 0x8000;
   rd <<= 1;
   REGS.p.n = rd & 0x8000;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint8_t cpu_op_lsr_b(uint8_t rd)
{
   REGS.p.c = rd & 1;
   rd >>= 1;
   REGS.p.n = rd & 0x80;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint16_t cpu_op_lsr_w(uint16_t rd)
{
   REGS.p.c = rd & 1;
   rd >>= 1;
   REGS.p.n = rd & 0x8000;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint8_t cpu_op_rol_b(uint8_t rd)
{
   unsigned carry = (unsigned)REGS.p.c;
   REGS.p.c = rd & 0x80;
   rd = (rd << 1) | carry;
   REGS.p.n = rd & 0x80;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint16_t cpu_op_rol_w(uint16_t rd)
{
   unsigned carry = (unsigned)REGS.p.c;
   REGS.p.c = rd & 0x8000;
   rd = (rd << 1) | carry;
   REGS.p.n = rd & 0x8000;
   REGS.p.z = rd == 0;

   return rd;
}

static inline uint8_t cpu_op_ror_b(uint8_t rd)
{
   unsigned carry = (unsigned)REGS.p.c << 7;
   REGS.p.c = rd & 1;
   rd = carry | (rd >> 1);
   REGS.p.n = rd & 0x80;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint16_t cpu_op_ror_w(uint16_t rd)
{
   unsigned carry = (unsigned)REGS.p.c << 15;
   REGS.p.c = rd & 1;
   rd = carry | (rd >> 1);
   REGS.p.n = rd & 0x8000;
   REGS.p.z = rd == 0;
   return rd;
}

static inline uint8_t cpu_op_trb_b(uint8_t rd)
{
   REGS.p.z = (rd & REGS.a.b.l) == 0;
   rd &= ~REGS.a.b.l;
   return rd;
}

static inline uint16_t cpu_op_trb_w(uint16_t rd)
{
   REGS.p.z = (rd & REGS.a.w) == 0;
   rd &= ~REGS.a.w;
   return rd;
}

static inline uint8_t cpu_op_tsb_b(uint8_t rd)
{
   REGS.p.z = (rd & REGS.a.b.l) == 0;
   rd |= REGS.a.b.l;
   return rd;
}

static inline uint16_t cpu_op_tsb_w(uint16_t rd)
{
   REGS.p.z = (rd & REGS.a.w) == 0;
   rd |= REGS.a.w;
   return rd;
}


#endif
