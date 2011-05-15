#ifndef __DSP_H
#define __DSP_H

#include <stdint.h>

struct dsp_state
{};

#ifdef __cplusplus
extern "C" {
#endif

void ssnes_dsp_init(void);
void ssnes_dsp_deinit(void);

void ssnes_dsp_run(unsigned cycles);

uint8_t ssnes_dsp_read(uint8_t addr);
void ssnes_dsp_write(uint8_t addr, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif
