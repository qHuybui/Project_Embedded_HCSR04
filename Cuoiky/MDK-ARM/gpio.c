#include "stm32f4xx.h"

// PB0: TRIG (Output)
void GPIOB0_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;   // B?t clock GPIOB
    GPIOB->MODER &= ~GPIO_MODER_MODER0;   // X�a bit mode hi?n t?i
    GPIOB->MODER |= GPIO_MODER_MODER0_0;  // C?u h�nh PB0 l�m Output
}

void GPIOB0_On(void) {
    GPIOB->ODR |= GPIO_ODR_ODR_0;  // �ua PB0 l�n m?c cao
}

void GPIOB0_Off(void) {
    GPIOB->ODR &= ~GPIO_ODR_ODR_0; // �ua PB0 xu?ng m?c th?p
}

// PC2: ECHO (Input)
void GPIOC2_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;  // B?t clock GPIOC
    GPIOC->MODER &= ~GPIO_MODER_MODER2;   // C?u h�nh PC2 l�m Input
}

uint8_t GPIOC2_Read(void) {
    return (GPIOC->IDR & GPIO_IDR_IDR_2) >> 2;  // �?c tr?ng th�i PC2
}
