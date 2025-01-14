#include "system.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/scb.h>

void vector_setup(void) {
  SCB_VTOR = BOOTLOADER_SIZE;
}


volatile uint64_t ticks = 0;

void sys_tick_handler(void) {
  ticks++;
}

static void rcc_setup(void) {
  rcc_clock_setup(&rcc_clock_config[RCC_CLOCK_CONFIG_HSI_PLL_64MHZ]);
}

static void systick_setup(void) {
  systick_set_frequency(SYSTICK_FREQ, CPU_FREQ);
  systick_counter_enable();
  systick_interrupt_enable();
}

uint64_t system_get_ticks(void) {
  return ticks;
}

void system_setup(void) {
    rcc_setup();
    systick_setup();
}

