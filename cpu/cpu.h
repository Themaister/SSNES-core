#ifndef __CPU_H
#define __CPU_H

#include <stdbool.h>
#include <stdint.h>

void cpu_init(void);
void cpu_entry(void);

struct cpu_regs
{
   word_reg_t a, x, y;

   struct
   {
      bool n, v, m, x, d, i, z, c;
   } p;
   bool e;

   bool irq;
   bool wai;
   uint8_t mdr;
};

struct cpu_status
{
   bool irq_pending;
   uint16_t irq_vector;

   // ?!
   unsigned clock_count;
   unsigned line_clocks;

   // NMI
   bool nmi_valid;
   bool nmi_line;
   bool nmi_transition;
   bool nmi_pending;
   bool nmi_hold;

   // IRQ
   bool irq_valid;
   bool irq_line;
   bool irq_transition;
   bool irq_pending;
   bool irq_hold;

   bool reset_pending;

   // DMA/HDMA
   bool dma_active;
   unsigned dma_counter;
   unsigned dma_clocks;
   bool dma_pending;
   bool hdma_pending;
   bool hdma_mode;

   // Auto-joypad polling
   bool auto_joypad_active;

   // MMIO
   // $2140-$217f
   uint8_t port[4];

   // $2181-$2183
   uint32_t wram_addr;

   // $4200
   bool nmi_enabled;
   bool hirq_enabled, virq_enabled;
   bool auto_joypad_poll;

   // $4201
   uint8_t pio;

   // $4202-$4203
   uint8_t wrmpya, wrmpyb;

   // $4204-$4206
   uint8_t wrdiva, wrdivb;

   // $4207-$420a
   uint16_t hirq_pos;
   uint16_t virq_pos;

   unsigned rom_speed;

   // $4214-$4217
   uint16_t rddiv;
   uint16_t rdmpy;

   // $4218-$421f
   uint16_t joy1, joy2, joy3, joy4;
};

struct cpu_state
{
   struct cpu_regs regs;
   struct cpu_status status;
   struct
   {
      unsigned mpyctr;
      unsigned divctr;
      unsigned shift;
   } alu;
};

#endif
