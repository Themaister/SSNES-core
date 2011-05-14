#include "libsnes/interface.h"
#include <string.h>

static inline void input_autopoll(bool autopoll)
{
   (void)autopoll;
   memset(STATUS.input, 0, sizeof(STATUS.input));

   for (unsigned p = 0; p < 2; p++)
   {
      for (unsigned i = 0; i < 8; i++)
         STATUS.input[p].input_data.b.h |= (ssnes_input_cb(p, SNES_DEVICE_JOYPAD, 0, i) ? 1 : 0) << (7 - i);

      for (unsigned i = 0; i < 4; i++)
         STATUS.input[p].input_data.b.l |= (ssnes_input_cb(p, SNES_DEVICE_JOYPAD, 0, i + 8) ? 1 : 0) << (7 - i);

   }

   if (autopoll)
   {
      STATUS.input[0].data1 = STATUS.input[0].input_data; 
      STATUS.input[1].data1 = STATUS.input[1].input_data; 
   }
}
