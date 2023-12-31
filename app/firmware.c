
#include "stm32g0b1xx.h"

#include "system.h"
#include "timer.h"
#include "gpio.h"

static void initClocks(void) {
  RCC->IOPENR  |= RCC_IOPENR_GPIOAEN;
  RCC->APBENR1 |= RCC_APBENR1_USART2EN;
}

static void configPinMode(void) {
  GPIOA->MODER &= ~((3u << 4) | (3u << 6)); // RESET PINs (PA2, PA3)
  GPIOA->MODER |= ((2u << 4) | (2u << 6)); // SET PINs AF (PA2, PA3)
}

static void setAF(void) {
  GPIOA->AFR[0] &= ~((15u << 8) | (15u << 12)); // CLEAR (PA2, PA3) AF
  GPIOA->AFR[0] |= ((1u << 8) | (1u << 12)); // SET (PA2, PA3) AF1
}

static void configUart2Pins(void) {
  configPinMode();
  setAF();
}

static void confiUart2(void) {
  // CLEAR USART CR1
  USART2->CR1 &= ~(USART_CR1_M1
                  |(0x03 << 26)
                  |USART_CR1_OVER8
                  |USART_CR1_CMIE
                  |USART_CR1_MME
                  |USART_CR1_M0
                  |USART_CR1_PCE
                  |(0x1F << 3)
                  |USART_CR1_TE
                  |USART_CR1_RE
                  |USART_CR1_UE);

  // CLEAR USART CR2
  USART2->CR2 &= ~(USART_CR2_RTOEN
                  |USART_CR2_ABREN
                  |USART_CR2_MSBFIRST
                  |(0x03 << 16)
                  |USART_CR2_SWAP
                  |USART_CR2_LINEN
                  |(0x03 << 12)
                  |USART_CR2_CLKEN
                  |USART_CR2_LBDIE);

// CLEAR USART CR3
  USART2->CR3 &= ~(USART_CR3_TCBGTIE
                  |USART_CR3_DEM
                  |(0x7F << 3)
                  |USART_CR3_IREN
                  |USART_CR3_EIE);

// SET BITS
USART2->CR3 |=  (USART_CR3_OVRDIS
                |USART_CR3_ONEBIT);


USART2->BRR = 556; // baud rate 115_200 at input frequency of 8MHZ

// ENABLE UART
USART2->CR1 |= (USART_CR1_TE     // TRANSMITTER
               |USART_CR1_RE    // RECEIVER
               |USART_CR1_UE); // UART2

}

static uint8_t receiveUart(void) {
  uint8_t rxData = 0;

  while (!(USART2->ISR & (1 << 5)));
  rxData = USART2->RDR;
  
  return rxData;
}

static void transmitUart(uint8_t data) {
  while (!(USART2->ISR & (1 << 7)));

  USART2->TDR = data;
}

/*static void transmitStrUart(char* str) // POINTER TO A STRING TO BE TRANSMITTED
{
    // WHILE NOT END OF STRING
    while(*str)
    {
        // TRANSMIT CHARACTER THEN INCREMENT POINTER TO NEXT CHARACTER
        transmitUart(*str++);
    }
}*/
static void initUart(void) {
  initClocks();
  configUart2Pins();
  confiUart2();
}

int main(void) {
  initUart();
  gpio_setup();
  vector_setup();
  system_setup();
  timer_setup();
  

  uint64_t start_time = system_get_ticks();
  float duty_cycle = 0.0f;

  timer_pwm_set_duty_cycle(duty_cycle);

  while (1) {
    if (system_get_ticks() - start_time >= 50) {
      duty_cycle += 1.0f;
      if (duty_cycle > 100.0f) {
        duty_cycle = 0.0f;
      }
      timer_pwm_set_duty_cycle(duty_cycle);
      
      start_time = system_get_ticks();
    }

    /*char* data = "Hello world!!";
    transmitStrUart(data);
    for (uint32_t i = 0; i < 1000000; i++) {
      __asm__("nop");
    }*/
    

    if (receiveUart()) {
      transmitUart(receiveUart() + 1);
    }
    

  }

  return 0;
}
