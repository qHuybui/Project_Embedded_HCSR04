#include "stm32f4xx.h"

extern volatile uint8_t state;

#define SHUTDOWN 0
#define ACTIVE 1


void Interrupt_PB_Init(void);