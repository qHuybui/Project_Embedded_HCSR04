#include "stm32f4xx.h"
#include<stdio.h>
#include "button.h"
#include "hcsr04.h"

void GPIO_Init(void) {
    //clock GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    //PB4 là chân output push-pull
    GPIOB->MODER &= ~(GPIO_MODER_MODE4);
    GPIOB->MODER |= (1 << GPIO_MODER_MODE4_Pos); // Output mode

    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT4); // Push-pull
    GPIOB->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED4_Pos); // High speed
	
	GPIOB->ODR |= GPIO_ODR_OD4;
	
	GPIOA->MODER &= ~(GPIO_MODER_MODER4);       // Clear mode
    GPIOA->MODER |= GPIO_MODER_MODER4_0;       // Output mode
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT4);       // Push-pull
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4;  // High speed
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4);
}

void Timer3_Off(void) {
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;
	GPIOB->ODR &= ~GPIO_ODR_OD4;
}

void Timer3_On(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
}


void Timer3_Initilize(uint16_t value) {
    // B?t clock Timer3
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;


    TIM3->CR1 = 0;
    TIM3->PSC = 15999; // Prescaler = 15999, t?n s? d?m = 16MHz / (PSC+1) = 1 kHz
	TIM3->CR1 &= ~TIM_CR1_CEN;
	TIM3->ARR = value;
	TIM3->EGR |= TIM_EGR_UG;
    // Kích ho?t ng?t khi b? d?m tràn
    TIM3->DIER |= TIM_DIER_UIE;
	TIM3->CR1 |= TIM_CR1_CEN;
    // Kích ho?t ng?t toàn c?c
    NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn, 6);
	
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;
		if (detect == OBS_DETECT) {
			GPIOB->ODR ^= GPIO_ODR_OD4;
		} else if (detect == OBS_NOT_DETECT || state == SHUTDOWN) {
			GPIOB->ODR &= ~GPIO_ODR_OD4;
		}
    }
}

void Timer4_Initilize(void) {
    // B?t clock Timer3
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // C?u hình ch? d? d?m Up-counter và không chia t?n s? clock
    TIM4->CR1 = 0;
    TIM4->PSC = 15999; // Prescaler = 15999, t?n s? d?m = 16MHz / (PSC+1) = 1 kHz
	TIM4->CR1 &= ~TIM_CR1_CEN;
    // Giá tr? Auto-Reload ban d?u tuong ?ng v?i 2Hz (chu k? 500ms)
	TIM4->ARR = 1000;
    // Kích ho?t ng?t khi b? d?m tràn
    TIM4->DIER |= TIM_DIER_UIE;
	TIM4->CR1 |= TIM_CR1_CEN;
    // Kích ho?t ng?t toàn c?c
    NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 7);
}

void TIM4_IRQHandler(void) {
    if (TIM4->SR & TIM_SR_UIF) {
        TIM4->SR &= ~TIM_SR_UIF;
		if (state == ACTIVE) {
			GPIOA->ODR ^= GPIO_ODR_OD4;
		} else {
			GPIOA->ODR &= ~GPIO_ODR_OD4;
		}
    }
}