
#include "rtthread.h"
#include "console.h"
#include "shell.h"




// void printInfo()
// {

//   printf("UART Divisor: %lu\r\n",uart_getDivisor());

// #ifndef BONFIRE_CORE
//   printf("MIMPID: %08lx\r\n",read_csr(mimpid));
//   printf("MISA: %08lx\r\n",read_csr(misa));
// #endif

// }



int main() {

  printk("Entry point %lx\n",finsh_system_init);
  //finsh_system_init();
  printk("main.c\n");
  void * buffer = malloc(256);
  printk("Buffer allocated at %lx\n",buffer);
  rt_thread_mdelay(2000);
  free(buffer);
  while (1) {
    //printk("Tick\n");
    rt_thread_mdelay(2000);
  }

}