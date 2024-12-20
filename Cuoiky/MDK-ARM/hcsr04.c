#include "hcsr04.h"
#include "delay.h" // Thu vi?n delay ph?i du?c c?u h nh tru?c

void HCSR04_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    GPIOC->MODER |= GPIO_MODER_MODER0_0;
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;

    // Configure ECHO pin as input
    GPIOC->MODER &= ~GPIO_MODER_MODER1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR1;
}

float HCSR04_Read(void)
{
    uint32_t time = 0;

    GPIOC->BSRR = (1 << 0);
    delay_us(10);
    GPIOC->BSRR = (1 << 16);

    while (!(GPIOC->IDR & (1 << 1)))
        ;

    // Start Timer
    TIM2->PSC = 16 - 1;
    TIM2->ARR = 0xFFFF;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->SR &= ~TIM_SR_UIF;
    TIM2->CR1 |= TIM_CR1_CEN;

    while ((GPIOC->IDR & (1 << 1)))
    {
        if (TIM2->SR & TIM_SR_UIF)
        {
            TIM2->CR1 &= ~TIM_CR1_CEN;
            return 0;
        }
    }

    // Stop timer and read value
    time = TIM2->CNT;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    return (float)time / 58.0;
}