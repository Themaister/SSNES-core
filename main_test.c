#include "cpu.h"
#include "system.h"
#include "cart.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
   assert(argc == 2);

   FILE *file = fopen(argv[1], "rb");
   assert(file);

   fseek(file, 0, SEEK_END);
   long len = ftell(file);
   rewind(file);

   void *buf = malloc(len);
   assert(buf);

   fread(buf, 1, len, file);

   ssnes_init();
   ssnes_cartridge_load(buf, len);

   fclose(file);
   free(buf);

   ssnes_cpu_run_frame();
   ssnes_cpu_run_frame();
   ssnes_cpu_run_frame();
   ssnes_cpu_run_frame();
   ssnes_cpu_run_frame();

   ssnes_cartridge_unload();
   ssnes_deinit();
}
