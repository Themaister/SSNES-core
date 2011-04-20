#include "cpu.h"
#include <string.h>

void cpu_init(void)
{
   cpu_reset();
}

static void cpu_init_registers(void)
{
}

void cpu_reset(void)
{
   memset(&state, 0, sizeof(state)); 
   cpu_init_registers();
}

void cpu_run_frame(void)
{
   for (;;)
   {





   }
}
