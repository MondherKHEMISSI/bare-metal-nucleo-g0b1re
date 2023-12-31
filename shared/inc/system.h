#ifndef INC_SYSTEM_H
#define INC_SYSTEM_H

#include "common-defines.h"

#define BOOTLOADER_SIZE    (0x8000U)
#define CPU_FREQ           (64000000)
#define SYSTICK_FREQ       (1000)

void vector_setup(void);
void system_setup(void);
uint64_t system_get_ticks(void);

#endif // INC_SYSTEM_H