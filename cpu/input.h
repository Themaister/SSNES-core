#include "libsnes/interface.h"
#include <string.h>

static inline void input_autopoll(void)
{
   memset(STATUS.input, 0, sizeof(STATUS.input));

   for (unsigned p = 0; p < 2; p++)
   {
      for (unsigned i = 0; i < 8; i++)
         STATUS.input[p].data1.b.h |= (ssnes_input_cb(p, SNES_DEVICE_JOYPAD, 0, i) ? 1 : 0) << (7 - i);

      for (unsigned i = 0; i < 4; i++)
         STATUS.input[p].data1.b.l |= (ssnes_input_cb(p, SNES_DEVICE_JOYPAD, 0, i + 8) ? 1 : 0) << (7 - i);
   }
}
