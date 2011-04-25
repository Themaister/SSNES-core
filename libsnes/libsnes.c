#include <stdbool.h>
#include "libsnes.hpp"

#include "system/system.h"
#include "system/cart.h"
#include "cpu/cpu.h"

snes_video_refresh_t ssnes_video_cb;
snes_audio_sample_t ssnes_audio_cb;
snes_input_poll_t ssnes_poll_cb;
snes_input_state_t ssnes_input_cb;

unsigned snes_library_revision_major(void)
{
   return 1;
}

unsigned snes_library_revision_minor(void)
{
   return 3;
}

void snes_set_video_refresh(snes_video_refresh_t cb)
{
   ssnes_video_cb = cb;
}

void snes_set_audio_sample(snes_audio_sample_t cb)
{
   ssnes_audio_cb = cb;
}

void snes_set_input_poll(snes_input_poll_t cb)
{
   ssnes_poll_cb = cb;
}

void snes_set_input_state(snes_input_state_t cb)
{
   ssnes_input_cb = cb;
}

void snes_set_controller_port_device(bool port, unsigned device)
{
   (void)port;
   (void)device;
}

void snes_set_cartridge_basename(const char *basename)
{
   (void)basename;
}

void snes_init(void)
{
   ssnes_init();
}

void snes_term(void)
{
   ssnes_deinit();
}

void snes_power(void)
{
   void snes_reset();
}

void snes_reset(void)
{
   ssnes_cpu_reset();
}

void snes_run(void)
{
   ssnes_cpu_run_frame();
}

unsigned snes_serialize_size(void)
{
   return 0;
}

bool snes_serialize(uint8_t *data, unsigned size)
{
   (void)data;
   (void)size;
   return false;
}

bool snes_unserialize(const uint8_t *data, unsigned size)
{
   (void)data;
   (void)size;
   return false;
}

void snes_cheat_reset(void)
{}

void snes_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

bool snes_load_cartridge_normal(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size
)
{
   (void)rom_xml;
   ssnes_cartridge_load(rom_data, rom_size);
   return true;
}

bool snes_load_cartridge_bsx_slotted(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
)
{
   (void)rom_xml; (void)rom_data; (void)rom_size;
   (void)bsx_xml; (void)bsx_data; (void)bsx_size;
   return false;
}

bool snes_load_cartridge_bsx(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
)
{
   (void)rom_xml; (void)rom_data; (void)rom_size;
   (void)bsx_xml; (void)bsx_data; (void)bsx_size;
   return false;
}

bool snes_load_cartridge_sufami_turbo(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *sta_xml, const uint8_t *sta_data, unsigned sta_size,
  const char *stb_xml, const uint8_t *stb_data, unsigned stb_size
)
{
   (void)rom_xml; (void)rom_data; (void)rom_size;
   (void)sta_xml; (void)sta_data; (void)sta_size;
   (void)stb_xml; (void)stb_data; (void)stb_size;
   return false;
}

bool snes_load_cartridge_super_game_boy(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *dmg_xml, const uint8_t *dmg_data, unsigned dmg_size
)
{
   (void)rom_xml; (void)rom_data; (void)rom_size;
   (void)dmg_xml; (void)dmg_data; (void)dmg_size;
   return false;
}

void snes_unload_cartridge(void)
{
   ssnes_cartridge_unload();
}

bool snes_get_region(void)
{
   return SNES_REGION_NTSC;
}

uint8_t *snes_get_memory_data(unsigned id)
{
   (void)id;
   return NULL;
}

unsigned snes_get_memory_size(unsigned id)
{
   (void)id;
   return 0;
}
