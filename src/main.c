
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
  while (1) {
    //printk("Tick\n");
    rt_thread_mdelay(500);
  }

}