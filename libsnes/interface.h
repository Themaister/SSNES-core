#ifndef __INTERFACE_H
#define __INTERFACE_H

#include <stdbool.h>
#include "libsnes.hpp"

extern snes_video_refresh_t ssnes_video_cb;
extern snes_audio_sample_t ssnes_audio_cb;
extern snes_input_poll_t ssnes_poll_cb;
extern snes_input_state_t ssnes_input_cb;

#endif
