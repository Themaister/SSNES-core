#ifndef __STATE_H
#define __STATE_H

#include "cpu_state.h"
#include "smp.h"
#include "dsp.h"
#include "ppu.h"

#define CPU state.cpu
#define REGS state.cpu.regs
#define SMP state.smp
#define DSP state.dsp
#define PPU state.ppu
#define MEM state.mem

#include <stdint.h>

struct mem_state
{
   uint8_t wram[128 * 1024];
   uint8_t vram[64 * 1024];
   uint8_t oam[512 + 32];
   uint8_t cgram[512];

   uint8_t apuram[64 * 1024];
};

struct snes_state
{
   struct mem_state mem;
   struct cpu_state cpu;
   struct smp_state smp;
   struct dsp_state dsp;
   struct ppu_state ppu;
};

extern struct snes_state state;

#endif
