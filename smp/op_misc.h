#ifndef __SMP_OP_MISC
#define __SMP_OP_MISC

static inline void smp_op_nop(void)
{}

static inline void smp_op_clrc(void)
{
   SMP.p.c = false;
}

static inline void smp_op_clrp(void)
{
   SMP.p.p = false;
}

static inline void smp_op_clrv(void)
{
   SMP.p.v = false;
   SMP.p.h = false;
}

static inline void smp_op_setc(void)
{
   SMP.p.c = true;
}

static inline void smp_op_setp(void)
{
   SMP.p.p = true;
}

// Wtf is this? Stole code from bSNES, hurr durr.
static inline void smp_op_daa(void)
{
   if (SMP.p.c || SMP.ya.b.l > 0x99)
   {
      SMP.ya.b.l += 0x60;
      SMP.p.c = true;
   }

   if (SMP.p.h || (SMP.ya.b.l & 15) > 0x09)
      SMP.ya.b.l += 0x06;

   SMP.p.n = SMP.ya.b.l & 0x80;
   SMP.p.z = (SMP.ya.b.l == 0);
}

static inline void smp_op_das(void)
{
   if (!SMP.p.c || SMP.ya.b.l > 0x99)
   {
      SMP.ya.b.l -= 0x60;
      SMP.p.c = true;
   }

   if (!SMP.p.h || (SMP.ya.b.l & 15) > 0x09)
      SMP.ya.b.l -= 0x06;

   SMP.p.n = SMP.ya.b.l & 0x80;
   SMP.p.z = (SMP.ya.b.l == 0);
}

static inline void smp_op_brk(void)
{
   uint16_t addr = smp_read(0xffde);
   addr |= (uint16_t)smp_read(0xffdf) << 8;
   smp_push_stack(SMP.pc >> 8);
   smp_push_stack(SMP.pc & 0xff);
   smp_push_stack(smp_get_p());
   SMP.pc = addr;
   SMP.p.b = true;
   SMP.p.i = false;
}

static inline void smp_op_di(void)
{
   SMP.p.i = false;
}

static inline void smp_op_ei(void)
{
   SMP.p.i = true;
}

static inline void smp_op_notc(void)
{
   SMP.p.c ^= true;
}

static inline void smp_op_stp(void)
{
   SMP.pc--;
}

// Nybble swap, interesting ... :v
static inline void smp_op_xcn(void)
{
   uint8_t res = (SMP.ya.b.l << 4) | (SMP.ya.b.l >> 4);
   SMP.p.z = (res == 0);
   SMP.p.n = res & 0x80;
   SMP.ya.b.l = res;
}

static inline void smp_op_pop_a(void)
{
   SMP.ya.b.l = smp_pop_stack();
}

static inline void smp_op_pop_x(void)
{
   SMP.x = smp_pop_stack();
}

static inline void smp_op_pop_y(void)
{
   SMP.ya.b.h = smp_pop_stack();
}

static inline void smp_op_pop_p(void)
{
   smp_set_p(smp_pop_stack());
}

static inline void smp_op_push_a(void)
{
   smp_push_stack(SMP.ya.b.l);
}

static inline void smp_op_push_x(void)
{
   smp_push_stack(SMP.x);
}

static inline void smp_op_push_y(void)
{
   smp_push_stack(SMP.ya.b.h);
}

static inline void smp_op_push_p(void)
{
   smp_push_stack(smp_get_p());
}

#endif
