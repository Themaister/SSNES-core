#ifndef __CPU_MEMORY_H
#define __CPU_MEMORY_H

#include "cpu.h"
#include "state.h"
#include "macros.h"
#include "util.h"
#include "memmap.h"

// Somewhat memory mapped ...
static inline uint8_t cpu_readl(uint32_t addr)
{
   return ssnes_memmap_read_table[addr >> 13](addr);
}

static inline void cpu_writel(uint32_t addr, uint8_t data)
{
   ssnes_memmap_write_table[addr >> 13](addr, data);
}

static inline uint8_t cpu_read(uint16_t addr)
{
   return cpu_readl(REGS.db | addr);
}

static inline void cpu_write(uint16_t addr, uint8_t data)
{
   cpu_writel(REGS.db | addr, data);
}

static inline uint16_t cpu_readw(uint16_t addr)
{
   uint16_t res = 0;
   res |= cpu_read(addr);
   res |= ((uint16_t)cpu_read(addr + 1)) << 8;
   return res;
}

static inline void cpu_writew(uint16_t addr, uint16_t data)
{
   cpu_write(addr, data & 0xFF);
   cpu_write(addr + 1, data >> 8);
}

static inline uint16_t cpu_readlw(uint32_t addr)
{
   uint16_t res = 0;
   res |= cpu_readl(addr);
   res |= ((uint16_t)cpu_readl(addr + 1)) << 8;
   return res;
}

static inline void cpu_writelw(uint32_t addr, uint16_t data)
{
   cpu_writel(addr, data & 0xFF);
   cpu_writel(addr + 1, data >> 8);
}

static inline uint8_t cpu_read_pc(void)
{
   return cpu_readl(REGS.pc.w.l++);
}

static inline uint16_t cpu_readw_pc(void)
{
   uint16_t res = 0;
   res |= cpu_readl(REGS.pc.w.l++);
   res |= ((uint16_t)cpu_readl(REGS.pc.w.l++)) << 8;
   return res;
}

static inline uint32_t cpu_readl_pc(void)
{
   uint32_t res = 0;
   res |= cpu_readl(REGS.pc.w.l++);
   res |= ((uint16_t)cpu_readl(REGS.pc.w.l++)) << 8;
   res |= ((uint32_t)cpu_readl(REGS.pc.w.l++)) << 16;
   return res;
}

static inline uint8_t cpu_stack_pull(void)
{
   return cpu_readl(++REGS.sp.w);
}

static inline void cpu_stack_push(uint8_t data)
{
   cpu_writel(REGS.sp.w--, data);
}

// Direct page.
static inline uint8_t cpu_read_dp(uint8_t addr)
{
   return cpu_readl(REGS.dp | addr);
}

static inline uint16_t cpu_readw_dp(uint8_t addr)
{
   uint16_t res = 0;
   res |= cpu_read_dp(addr);
   res |= ((uint16_t)cpu_read_dp(addr + 1)) << 8;
   return res;
}

static inline uint32_t cpu_readl_dp(uint8_t addr)
{
   uint32_t res = 0;
   res |= cpu_read_dp(addr);
   res |= ((uint16_t)cpu_read_dp(addr + 1)) << 8;
   res |= ((uint32_t)cpu_read_dp(addr + 2)) << 16;
   return res;
}

static inline void cpu_write_dp(uint16_t addr, uint8_t data)
{
   cpu_writel(REGS.dp + addr, data);
}

static inline void cpu_writew_dp(uint16_t addr, uint16_t data)
{
   cpu_write_dp(addr, data & 0xFF);
   cpu_write_dp(addr + 1, data >> 8);
}

// Stack-relative
static inline uint8_t cpu_read_sp(uint16_t addr)
{
   return cpu_readl(REGS.sp.w + addr);
}

static inline uint16_t cpu_readw_sp(uint16_t addr)
{
   uint16_t res = 0;
   res |= cpu_read_sp(addr);
   res |= ((uint16_t)cpu_read_sp(addr)) << 8;
   return res;
}

static inline void cpu_write_sp(uint8_t addr, uint8_t data)
{
   cpu_writel(REGS.sp.w + addr, data);
}

static inline void cpu_writew_sp(uint8_t addr, uint16_t data)
{
   cpu_writel(REGS.sp.w + addr, data & 0xFF);
   cpu_writel(REGS.sp.w + addr + 1, data >> 8);
}

#endif
