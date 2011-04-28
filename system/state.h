#ifndef __STATE_H
#define __STATE_H

#include "cpu/cpu.h"
#include "smp/smp.h"
#include "dsp/dsp.h"
#include "ppu/ppu.h"

#include <stdint.h>

struct mem_state
{
   uint8_t wram[128 * 1024];
   union
   {
      uint8_t b[64 * 1024];
      uint16_t w[32 * 1024];
   } vram;

   union
   {
      uint8_t b[512 + 32];
      uint16_t w[256 + 16];
      uint32_t l[128 + 8];
   } oam;

   union
   {
      uint8_t b[512];
      uint16_t w[256];
   } cgram;

   uint8_t apuram[64 * 1024];

   struct
   {
      uint8_t *sram;
      uint8_t *rom;
   } cart;
};

struct ssnes_state
{
   struct mem_state mem;
   struct cpu_state cpu;
   struct smp_state smp;
   struct dsp_state dsp;
   struct ppu_state ppu;
};

extern struct ssnes_state ssnes_state;

#endif
