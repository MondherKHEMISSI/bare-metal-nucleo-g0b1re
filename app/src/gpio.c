#include "gpio.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>


void gpio_setup(void) {

  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);
  gpio_set_af(LED_PORT, GPIO_AF2, LED_PIN);

}