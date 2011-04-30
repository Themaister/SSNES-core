#ifndef __SSNES_SMP_ALU_H
#define __SSNES_SMP_ALU_H

// Shamelessly pilfered from bSNES, huzzah! :D

static inline uint8_t smp_op_mov(uint8_t rd)
{
   SMP.ya.b.l = rd;
   SMP.p.z = (rd == 0);
   SMP.p.n = rd & 0x80;
   return rd;
}

static inline uint8_t smp_op_adc(uint8_t a, uint8_t b)
{
   int r = a + b + SMP.p.c;
   SMP.p.n = r & 0x80;
   SMP.p.h = (a ^ b ^ r) & 0x10; // What the fuck is this?!?!
   SMP.p.z = (r == 0);
   SMP.p.c = r > 0xff;
   return r;
}

// Ye, I have issues understanding this shit ;)
static inline uint16_t smp_op_addw(uint16_t a, uint16_t b)
{
   uint16_t r;
   SMP.p.c = false;
   r = smp_op_adc(a & 0xff, b & 0xff);
   r |= (uint16_t)smp_op_adc(a >> 8, b >> 8) << 8;
   SMP.p.z = (r == 0);
   return r;
}

static inline uint8_t smp_op_and(uint8_t a, uint8_t b)
{
   a &= b;
   SMP.p.n = a & 0x80;
   SMP.p.z = (a == 0);
   return a;
}

static inline uint8_t smp_op_cmp(uint8_t a, uint8_t b)
{
   int r = a - b;
   SMP.p.n = r & 0x80;
   SMP.p.z = (r == 0);
   SMP.p.c = r >= 0;
   return a;
}

static inline uint16_t smp_op_cmpw(uint16_t a, uint16_t b)
{
   int r = a - b;
   SMP.p.n = r & 0x8000;
   SMP.p.z = (r == 0);
   SMP.p.c = r >= 0;
   return a;
}

static inline uint8_t smp_op_eor(uint8_t a, uint8_t b)
{
   a ^= b;
   SMP.p.n = a & 0x80;
   SMP.p.z = (a == 0);
   return a;
}

static inline uint8_t smp_op_or(uint8_t a, uint8_t b)
{
   a |= b;
   SMP.p.n = a & 0x80;
   SMP.p.z = (a == 0);
   return a;
}

static inline uint8_t smp_op_sbc(uint8_t a, uint8_t b)
{
   int r = a - b - !SMP.p.c;
   SMP.p.n = r & 0x80;
   SMP.p.v = (a ^ b) & (a ^ r) & 0x80;
   SMP.p.h = !((a ^ b ^ r) & 0x10);
   SMP.p.z = (r == 0);
   SMP.p.c = r >= 0;
   return r;
}

static inline uint16_t smp_op_subw(uint16_t a, uint16_t b)
{
   uint16_t r;
   SMP.p.c = true;
   r = smp_op_sbc(a, b);
   r |= (uint16_t)smp_op_sbc(a >> 8, b >> 8) << 8;
   SMP.p.z = (r == 0);
   return r;
}

static inline uint8_t smp_op_inc(uint8_t a)
{
   a++;
   SMP.p.n = a & 0x80;
   SMP.p.z = (a == 0);
   return a;
}

static inline uint8_t smp_op_dec(uint8_t a)
{
   a--;
   SMP.p.n = a & 0x80;
   SMP.p.z = (a == 0);
   return a;
}

static inline uint16_t smp_op_incw(uint16_t a)
{
   a++;
   SMP.p.n = a & 0x8000;
   SMP.p.z = (a == 0);
   return a;
}

static inline uint16_t smp_op_decw(uint16_t a)
{
   a--;
   SMP.p.n = a & 0x8000;
   SMP.p.z = (a == 0);
   return a;
}

static inline uint8_t smp_op_asl(uint8_t a)
{
   SMP.p.c = a & 0x80;
   a <<= 1;
   SMP.p.n = a & 0x80;
   SMP.p.z = (a == 0);
   return a;
}

static inline uint8_t smp_op_lsr(uint8_t a)
{
   SMP.p.c = a & 0x01;
   a >>= 1;
   SMP.p.n = a & 0x80;
   SMP.p.z = (a == 0);
   return a;
}

static inline uint8_t smp_op_rol(uint8_t a)
{
   uint8_t carry = SMP.p.c;
   SMP.p.c = a & 0x80;
   a = (a << 1) | carry;
   SMP.p.n = a & 0x80;
   SMP.p.z = (a == 0);
   return a;
}

static inline uint8_t smp_op_ror(uint8_t a)
{
   uint8_t carry = (uint8_t)SMP.p.c << 7;
   SMP.p.c = a & 0x80;
   a = (a >> 1) | carry;
   SMP.p.n = a & 0x80;
   SMP.p.z = (a == 0);
   return a;
}

static inline void smp_op_mul(void)
{
   uint16_t res = (uint16_t)SMP.ya.b.l * (uint16_t)SMP.ya.b.h;
   SMP.ya.w = res;
   SMP.p.z = (res == 0);
   SMP.p.n = res & 0x8000;
}

static inline void smp_op_div(void)
{
   uint8_t div = SMP.ya.w / SMP.x;
   uint8_t rem = SMP.ya.w % SMP.x;
   SMP.ya.b.l = div;
   SMP.ya.b.h = rem;
   // TODO: Fix flags :D
}


static inline void smp_op_and1(uint8_t rd)
{
   SMP.p.c &= rd;
}

static inline void smp_op_andn1(uint8_t rd)
{
   SMP.p.c &= ~rd;
}

static inline void smp_op_or1(uint8_t rd)
{
   SMP.p.c |= rd;
}

static inline void smp_op_orn1(uint8_t rd)
{
   SMP.p.c |= ~rd;
}

static inline void smp_op_eor1(uint8_t rd)
{
   SMP.p.c ^= rd;
}

static inline void smp_op_mov1(uint8_t rd)
{
   SMP.p.c = rd;
}

#endif
