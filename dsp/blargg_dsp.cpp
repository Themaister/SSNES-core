#include "dsp.h"
#include "SPC_DSP.h"
#include "system/state.h"
#include "system/macros.h"
#include "libsnes/interface.h"
#include <stdint.h>

#include <iostream>

static SPC_DSP dsp;
static SPC_DSP::sample_t dsp_buffer[1024];

void ssnes_dsp_init(void)
{
   dsp.init(MEM.apuram);
   dsp.reset();
}

void ssnes_dsp_deinit(void)
{}

void ssnes_dsp_run(unsigned cycles)
{
   dsp.set_output(dsp_buffer, 1024);
   dsp.run(cycles);
   unsigned samples = dsp.sample_count();
   for (unsigned i = 0; i < (samples >> 1); i++)
      ssnes_audio_cb(dsp_buffer[i << 1], dsp_buffer[(i << 1) + 1]);
}

uint8_t ssnes_dsp_read(uint8_t addr)
{
   return dsp.read(addr); 
}

void ssnes_dsp_write(uint8_t addr, uint8_t data)
{
   dsp.write(addr, data);
}
