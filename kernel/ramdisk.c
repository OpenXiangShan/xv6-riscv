//
// ramdisk that uses the disk image loaded by qemu -rdinit fs.img
//

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "buf.h"

extern char ramdisk_start;
extern char ramdisk_end;

#define RAMDISK (&ramdisk_start)
#define RAMDISK_SIZE (&ramdisk_end - RAMDISK)

void
ramdisk_init(void)
{
}

void
ramdisk_rw(struct buf *b, int write)
{
  if(!holdingsleep(&b->lock))
    panic("ramdiskrw: buf not locked");

  if(b->blockno >= FSSIZE)
    panic("ramdiskrw: blockno too big");

  uint64 diskaddr = b->blockno * BSIZE;
  char *addr = (char *)RAMDISK + diskaddr;
  if (diskaddr >= RAMDISK_SIZE) {
    panic("ramdisk: out of bound");
  }

  if(write){ memmove(addr, b->data, BSIZE); }
  else { memmove(b->data, addr, BSIZE); }
}
