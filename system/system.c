#include "system.h"
#include "memory.h"

#include "cpu/cpu.h"
#include "ppu/ppu.h"
#include "dsp/dsp.h"
#include "smp/smp.h"

void ssnes_init(void)
{
   ssnes_pool_init(1 << 26);

   ssnes_cpu_init();
   ssnes_ppu_init();
   ssnes_smp_init();
   ssnes_dsp_init();
}

void ssnes_deinit(void)
{
   ssnes_cpu_deinit();
   ssnes_ppu_deinit();
   ssnes_smp_deinit();
   ssnes_dsp_deinit();

   ssnes_pool_free();
}
