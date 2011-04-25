#include <stdbool.h>
#include "libsnes/libsnes.hpp"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

static void update(const uint16_t *frame, unsigned width, unsigned height)
{}
static void audio(uint16_t left, uint16_t right)
{}
static void input_poll(void)
{
}
static int16_t input_state(bool port, unsigned device, unsigned index, unsigned id)
{ return 0; }

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

   snes_init();

   snes_set_video_refresh(update);
   snes_set_audio_sample(audio);
   snes_set_input_poll(input_poll);
   snes_set_input_state(input_state);

   snes_load_cartridge_normal(NULL, buf, len);

   fclose(file);
   free(buf);

   snes_run();
   snes_run();
   snes_run();
   snes_run();
   snes_run();

   snes_term();
}
