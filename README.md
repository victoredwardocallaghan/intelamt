DMA Notes
=========

DMA Basics
----------

To start a DMA transfer you need to know three things:

 * Where the memory is located (what page)
 * The offset into the page
 * How much to transfer

DMA has two restrictions:

 * You cannot transfer more than 64K of data in one shot
 * You cannot cross a page boundary


Page Basics
-----------

Suppose some 1MB region of memory. Let it be divided into 16 pages of 64K a
piece in the follow way:

Page | Segment:Offset address
-----+-----------------------
 0   | 0000:0000 - 0000:FFFF
 1   | 1000:0000 - 1000:FFFF
 2   | 2000:0000 - 2000:FFFF
 3   | 3000:0000 - 3000:FFFF
 4   | 4000:0000 - 4000:FFFF
 5   | 5000:0000 - 5000:FFFF
 6   | 6000:0000 - 6000:FFFF
 7   | 7000:0000 - 7000:FFFF
 8   | 8000:0000 - 8000:FFFF
 9   | 9000:0000 - 9000:FFFF
 A   | A000:0000 - A000:FFFF
 B   | B000:0000 - B000:FFFF
 C   | C000:0000 - C000:FFFF
 D   | D000:0000 - D000:FFFF
 E   | E000:0000 - E000:FFFF
 F   | F000:0000 - F000:FFFF


DMA Transfer
------------

We can now encapsulate what we need for a DMA transfer in a struct as follows:

```
typedef struct
{
  uint8_t  page;
  uint32_t offset;
  uint32_t length;
} DMA_block;
```

We may find a pointers page and offset with the following function:

```
void fillDMAblock(DMA_block *blk, uint8_t *data)
{
  uint32_t tmp, segment, offset;
  uint64_t result;

  segment = FP_SEG(data);
  offset  = FP_OFF(data);
  blk->page = (segment & 0x0FFF) >> 12;
  tmp = (segment & 0x0FFF) << 4;
  result = offset + tmp;
  if (result > 0xFFFF)
    blk->page++;
  blk->offset = (uint32_t) result;
}
```

where FP_SEG and FP_OFF macros find the segment and the offset of the data
block in memory. Since we only need the page we can take the upper 4 bits of
the absolute 20 bit address of our memory location in the example above.
FP_OFF macro then finds the lower 12 bits.
