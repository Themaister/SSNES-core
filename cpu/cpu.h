#ifndef __SSNES_CPU_H
#define __SSNES_CPU_H

#include <stdbool.h>
#include "system/util.h"

void ssnes_cpu_init(void);
void ssnes_cpu_deinit(void);

void ssnes_cpu_run_frame(void);
void ssnes_cpu_reset(void);

struct cpu_regs
{
   // GPP and index regs
   word_reg_t a, x, y;
   word_reg_t zero; // Just holds a zero value for STZ.

   // Stack pointer and PC
   word_reg_t sp;
   long_reg_t pc; // Includes the PBK

   // Processor flags
   struct
   {
      bool n, v, m, x, d, i, z, c;
   } p;
   bool e;
   bool wai_quit;
   bool wai;

   uint32_t db; // Data bank
   uint16_t dp; // Direct page
};

struct dma_channel
{
   unsigned trans_cnt;

   uint8_t ctrl;
   uint8_t dest;
   long_reg_t src;
   word_reg_t size;
};

struct hdma_channel
{
   word_reg_t table_addr;
   uint8_t line_counter;
   uint8_t indirect_bank;
};

struct cpu_status
{
   struct
   {
      bool reset;
      bool nmi;
      bool irq;
      bool irq_fired;
   } pending_irq;

   unsigned cycles_per_frame;
   unsigned cycles;
   unsigned smp_cycles;
   bool smp_state;

   struct
   {
      unsigned vcount;
      unsigned hcount;
      bool scanline_ready;
      bool frame_ready;
   } ppu;

   struct dma_channel dma_channels[8];
   uint8_t dma_enable;
   struct hdma_channel hdma_channels[8];
   uint8_t hdma_enable;
   uint8_t hdma_enable_frame;
   uint8_t hdma_do_transfer;
   bool hdma_run;

   struct
   {
      word_reg_t htime;
      word_reg_t vtime;
      unsigned vtrig;
      unsigned htrig;
      unsigned vhtrig;

      uint8_t nmi_flag;
      uint8_t irq_flag;
   } irq;

   struct
   {
      uint8_t nmitimen;

      uint8_t cgadd;
      bool cg_odd;
      uint8_t cgbuf;

      uint8_t vmain;
      word_reg_t vram_addr;
      word_reg_t vram_rd_buf;

      word_reg_t oam_addr;
      word_reg_t oam_addr_buf;
      bool oam_odd;
      uint8_t oam_buf;

      long_reg_t wram_addr;

      uint8_t apuio[4];

      bool memsel;

      uint16_t m7a, m7b, m7c, m7d;
      uint8_t m7prev;
      uint32_t m7mul;
   } regs;

   struct
   {
      word_reg_t data1;
      word_reg_t data2;
   } input[2];
};

struct cpu_alu_state
{
   uint8_t wrmpya, wrmpyb;
   word_reg_t wrdiv;
   uint8_t wrdivb;

   word_reg_t mul_rem_res; 
   word_reg_t div_quot;
   uint8_t div_rem;
};

struct cpu_state
{
   struct cpu_regs regs;
   struct cpu_status status;
   struct cpu_alu_state alu;
};

#endif
