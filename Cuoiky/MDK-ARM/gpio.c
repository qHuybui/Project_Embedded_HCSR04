#include "stm32f4xx.h"

// PB0: TRIG (Output)
void GPIOB0_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;   // B?t clock GPIOB
    GPIOB->MODER &= ~GPIO_MODER_MODER0;   // Xóa bit mode hi?n t?i
    GPIOB->MODER |= GPIO_MODER_MODER0_0;  // C?u hình PB0 làm Output
}

void GPIOB0_On(void) {
    GPIOB->ODR |= GPIO_ODR_ODR_0;  // Ðua PB0 lên m?c cao
}

void GPIOB0_Off(void) {
    GPIOB->ODR &= ~GPIO_ODR_ODR_0; // Ðua PB0 xu?ng m?c th?p
}

// PC2: ECHO (Input)
void GPIOC2_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;  // B?t clock GPIOC
    GPIOC->MODER &= ~GPIO_MODER_MODER2;   // C?u hình PC2 làm Input
}

uint8_t GPIOC2_Read(void) {
    return (GPIOC->IDR & GPIO_IDR_IDR_2) >> 2;  // Ð?c tr?ng thái PC2
}
