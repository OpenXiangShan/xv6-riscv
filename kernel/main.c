#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");
    Log();
    kinit();         // physical page allocator
    Log();
    kvminit();       // create kernel page table
    Log();
    kvminithart();   // turn on paging
    Log();
    procinit();      // process table
    Log();
    trapinit();      // trap vectors
    Log();
    trapinithart();  // install kernel trap vector
    Log();
    plicinit();      // set up interrupt controller
    Log();
    plicinithart();  // ask PLIC for device interrupts
    Log();
    binit();         // buffer cache
    Log();
    iinit();         // inode cache
    Log();
    fileinit();      // file table
    Log();
#if defined(__NEMU__) || defined(__NOOP__)
    ramdisk_init();
#else
    virtio_disk_init(); // emulated hard disk
#endif
    Log();
    userinit();      // first user process
    Log();
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
