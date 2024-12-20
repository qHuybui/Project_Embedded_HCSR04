#include "stm32f4xx.h"

extern volatile uint8_t detect;

#define OBS_DETECT 1
#define OBS_NOT_DETECT 0

void HCSR04_Init(void);
float HCSR04_Read(void);
