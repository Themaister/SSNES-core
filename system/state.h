#ifndef __STATE_H
#define __STATE_H

#include "cpu.h"
#include "smp.h"
#include "dsp.h"
#include "ppu.h"

#define CPU state.cpu
#define SMP state.smp
#define DSP state.dsp
#define PPU state.ppu

struct snes_state
{
   struct cpu_state cpu;
   struct smp_state smp;
   struct dsp_state dsp;
   struct ppu_state ppu;
};

extern snes_state state;

#endif
