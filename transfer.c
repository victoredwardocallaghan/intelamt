#include <stdint.h>
#include <string.h>

#define NUMBER_OF_DMA_ENGINES 3

struct dmadesc_t {
  uint32_t src_lo;
  uint32_t src_hi;
  uint32_t dst_lo;
  uint32_t dst_hi;
  uint32_t count;
  uint32_t res1;
  uint32_t res2;
  uint32_t res3;
} dmadesc[NUMBER_OF_DMA_ENGINES];


// SR instruction: Store to Auxiliary Register
void sr(uint32_t addr, uint32_t value)
{
  asm("sr r1, [r0]");
}

void dma_amt2host(uint32_t idx, /* the id of DMA engine */
                  uint32_t amt_source_addr,
                  uint32_t host_dest_addr,
                  uint32_t transfer_length)

{
  uint32_t srbase = 0x5010 + 4 * idx;
  memset(&dmadesc[idx], 0, sizeof(dmadesc[idx]));
  dmadesc[idx].src_lo = amt_source_addr;
  dmadesc[idx].dst_lo = host_dest_addr;
  dmadesc[idx].count = transfer_length;
  sr(srbase + 1, (uint32_t)&dmadesc[idx]);
  sr(srbase + 2, 0);
  sr(srbase + 3, 0);
  sr(srbase + 0, 0x189);
}
