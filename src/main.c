#include "bonfire.h"
#include "uart.h"
#include "bonfire_gpio.h"
#include "mem_rw.h"
#include <stdio.h>
#include "rtthread.h"

#ifdef SIM
#pragma message "Compiling for Simulator"
#endif

uint64_t get_timer_value()
{
#if __riscv_xlen == 32
  while (1) {
    uint32_t hi = read_csr(mcycleh);
    uint32_t lo = read_csr(mcycle);
    if (hi == read_csr(mcycleh))
      return ((uint64_t)hi << 32) | lo;
  }
#else
  return read_csr(mcycle);
#endif
}

void delay_micros(uint64_t delay)
{
uint64_t s =  get_timer_value();
uint64_t ticks = SYSCLK / 1000000 * delay;

    while (get_timer_value()-s < ticks);
}


void printInfo()
{

  printf("UART Divisor: %lu\r\n",uart_getDivisor());

#ifndef BONFIRE_CORE
  printf("MIMPID: %08lx\r\n",read_csr(mimpid));
  printf("MISA: %08lx\r\n",read_csr(misa));
#endif

}


int main() {

int i=0;

  _write_word((void*)GPIO_BASE+GPIO_OUTPUT_EN,0xf);
  _write_word((void*)GPIO_BASE+GPIO_OUTPUT_VAL,0x9);
  //wait(3000000);

  uart_setBaudRate(PLATFORM_BAUDRATE);
  printInfo();

  while(1) {
     delay_micros(200000);
    _write_word((void*)GPIO_BASE+GPIO_OUTPUT_VAL, 1 << (i++ % 4 ) );   
    if ((i % 4) == 0) {
  #ifndef BONFIRE_CORE
      printf("Uptime: %d sec\r\n",(int)(get_timer_value()/SYSCLK));     
  #else
      printf("run number %d\r\n",i);
  #endif
  #ifdef SIM
    if (i>4) writechar(0x1a); // Terminate Simulation
  #endif
    }
  }


}