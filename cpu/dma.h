#ifndef __CPU_DMA_H
#define __CPU_DMA_H

#include <stdint.h>
#include "system/state.h"
#include "system/macros.h"
#include "bus.h"
#include "memory.h"

// TODO: Optimize to branchless? :D This should be quite slow :v
static inline void run_dma_channel(unsigned channel)
{
   struct dma_channel *chan = &STATUS.dma_channels[channel];
   static const uint8_t transfer_addr[8][4] = {
      {0, 0, 0, 0},
      {0, 1, 0, 1},
      {0, 0, 0, 0},
      {0, 0, 1, 1},
      {0, 1, 2, 3},
      {0, 1, 0, 1},
      {0, 0, 0, 0},
      {0, 0, 1, 1}
   };

   uint16_t dest_addr = 0x2100 | (chan->dest + transfer_addr[chan->ctrl & 7][chan->trans_cnt++ & 3]);
   //dprintf(stderr, "DMA read: $%x\n", (unsigned)chan->src.l);
   uint8_t data = cpu_readl(chan->src.l);

   //dprintf(stderr, "DMA: Write $%x -> $%x || Size: $%x\n", (unsigned)data, (unsigned)dest_addr, (unsigned)chan->size.w);

   ssnes_bus_write_2000(dest_addr, data);
   //cpu_writel(dest_addr, data);

   if (!(chan->ctrl & 0x08))
   {
      if (chan->ctrl & 0x10)
         chan->src.l--;
      else
         chan->src.l++;
   }

   chan->src.l &= 0xffffff;

   // Check if channel is done.
   if (--chan->size.w == 0)
      STATUS.dma_enable &= ~(1 << channel);
}

static inline void cpu_run_dma(void)
{
   // Horrible stuff ;)
   if (STATUS.dma_enable & 0x1)
      run_dma_channel(0);
   else if (STATUS.dma_enable & 0x2)
      run_dma_channel(1);
   else if (STATUS.dma_enable & 0x4)
      run_dma_channel(2);
   else if (STATUS.dma_enable & 0x8)
      run_dma_channel(3);
   else if (STATUS.dma_enable & 0x10)
      run_dma_channel(4);
   else if (STATUS.dma_enable & 0x20)
      run_dma_channel(5);
   else if (STATUS.dma_enable & 0x40)
      run_dma_channel(6);
   else if (STATUS.dma_enable & 0x80)
      run_dma_channel(7);
}

static inline void hdma_do_transfer(unsigned i)
{
   bool indirect = STATUS.dma_channels[i].ctrl & 0x40;
   uint32_t base_addr = indirect ? (((uint32_t)STATUS.hdma_channels[i].indirect_bank << 16) | STATUS.dma_channels[i].size.w) : STATUS.hdma_channels[i].table_addr.w;

   uint16_t addr = 0x2100 | (uint16_t)STATUS.dma_channels[i].dest;

   unsigned mode = STATUS.dma_channels[i].ctrl & 7;

   switch (mode)
   {
      case 0: // p
         ssnes_bus_write_2000(addr + 0, cpu_readl(base_addr + 0));
         break;

      case 1: // p, p+1
         ssnes_bus_write_2000(addr + 0, cpu_readl(base_addr + 0));
         ssnes_bus_write_2000(addr + 1, cpu_readl(base_addr + 1));
         break;

      case 2: // p, p
      case 6:
         ssnes_bus_write_2000(addr + 0, cpu_readl(base_addr + 0));
         ssnes_bus_write_2000(addr + 0, cpu_readl(base_addr + 1));
         break;

      case 3: // p, p, p+1, p+1
      case 7:
         ssnes_bus_write_2000(addr + 0, cpu_readl(base_addr + 0));
         ssnes_bus_write_2000(addr + 0, cpu_readl(base_addr + 1));
         ssnes_bus_write_2000(addr + 1, cpu_readl(base_addr + 2));
         ssnes_bus_write_2000(addr + 1, cpu_readl(base_addr + 3));
         break;

      case 4: // p, p+1, p+2, p+3
         ssnes_bus_write_2000(addr + 0, cpu_readl(base_addr + 0));
         ssnes_bus_write_2000(addr + 1, cpu_readl(base_addr + 1));
         ssnes_bus_write_2000(addr + 2, cpu_readl(base_addr + 2));
         ssnes_bus_write_2000(addr + 3, cpu_readl(base_addr + 3));
         break;

      case 5: // p, p+1, p, p+1
         ssnes_bus_write_2000(addr + 0, cpu_readl(base_addr + 0));
         ssnes_bus_write_2000(addr + 1, cpu_readl(base_addr + 1));
         ssnes_bus_write_2000(addr + 0, cpu_readl(base_addr + 2));
         ssnes_bus_write_2000(addr + 1, cpu_readl(base_addr + 3));
         break;
   }

   static const unsigned table_inc[8] = { 1, 2, 2, 4, 4, 4, 2, 4 };
   if (indirect)
      STATUS.dma_channels[i].size.w += table_inc[mode];
   else
      STATUS.hdma_channels[i].table_addr.w += table_inc[mode];
}

static inline void hdma_run_channel(unsigned i)
{
   if (STATUS.hdma_do_transfer & (1 << i))
      hdma_do_transfer(i);

   uint8_t line_cnt = STATUS.hdma_channels[i].line_counter;
   uint8_t cnt = line_cnt & 0x7f;
   cnt = (cnt - 1) & 0x7f;
   uint8_t rep = line_cnt & 0x80;

   if (cnt == 0) // Counter zeroed, load new value.
   {
      long_reg_t addr;
      addr.w.h = STATUS.dma_channels[i].src.b.hl;
      addr.w.l = STATUS.hdma_channels[i].table_addr.w++;
      STATUS.hdma_channels[i].line_counter = cpu_readl(addr.l);

      if (STATUS.hdma_channels[i].line_counter)
      {
         STATUS.hdma_do_transfer |= (1 << i);

         // If indirect, reload indirect table.
         if (STATUS.dma_channels[i].ctrl & 0x40)
         {
            long_reg_t addr;
            addr.w.h = STATUS.hdma_channels[i].indirect_bank;
            addr.w.l = STATUS.hdma_channels[i].table_addr.w;
            STATUS.hdma_channels[i].table_addr.w += 2;
            STATUS.dma_channels[i].size.w = cpu_readlw(addr.l);
         }
      }
      else
         STATUS.hdma_enable_frame &= ~(1 << i);
   }
   else
   {
      if (!rep)
         STATUS.hdma_do_transfer &= ~(1 << i);

      STATUS.hdma_channels[i].line_counter = rep | cnt;
   }
}

// Sets up HDMA registers.
static inline void hdma_frame_init(void)
{
   STATUS.hdma_enable_frame = STATUS.hdma_enable;
   STATUS.hdma_do_transfer = 0;

   for (unsigned i = 0; i < 8; i++)
   {
      if (~STATUS.hdma_enable & (1 << i))
         continue;

      // Load table address.
      STATUS.hdma_channels[i].table_addr.w = STATUS.dma_channels[i].src.w.l;

      long_reg_t addr;
      addr.w.h = STATUS.dma_channels[i].src.b.hl;
      addr.w.l = STATUS.hdma_channels[i].table_addr.w++;

      // Load line counter.
      STATUS.hdma_channels[i].line_counter = cpu_readl(addr.l);

      // Nothing here, terminate the channel.
      if (STATUS.hdma_channels[i].line_counter == 0)
         STATUS.hdma_enable_frame &= ~(1 << i);
      else
      {
         // Indirect HDMA, load data.
         if (STATUS.dma_channels[i].ctrl & 0x40)
         {
            STATUS.hdma_channels[i].table_addr.w += 2;
            addr.w.l++;
            STATUS.dma_channels[i].size.w = cpu_readlw(addr.l);
         }

         STATUS.hdma_do_transfer |= 1 << i;
      }
   }
}

static inline void hdma_run(void)
{
   if (!STATUS.hdma_enable_frame)
      return;

   for (unsigned i = 0; i < 8; i++)
   {
      if (~STATUS.hdma_enable_frame & (1 << i))
         continue;

      hdma_run_channel(i);
   }
}

#endif
