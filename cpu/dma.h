#ifndef __CPU_DMA_H
#define __CPU_DMA_H

#include <stdint.h>
#include "state.h"
#include "macros.h"
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
   uint8_t data = cpu_readl(chan->src.l);

   //fprintf(stderr, "DMA: Write $%x -> $%x || Size: $%x\n", (unsigned)data, (unsigned)dest_addr, (unsigned)chan->size.w);

   ssnes_bus_write_2000(dest_addr, data);

   if (!(chan->ctrl & 0x08))
   {
      if (chan->ctrl & 0x10)
         chan->src.l++;
      else
         chan->src.l--;
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

#endif
