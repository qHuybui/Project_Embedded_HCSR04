#include "stm32f4xx.h"
#include "button.h"

void Interrupt_PB_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	
	GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1); 
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1); 
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR1_0);

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB | SYSCFG_EXTICR1_EXTI1_PB;
	
	EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR1;
	EXTI->RTSR |= EXTI_RTSR_TR0 | EXTI_RTSR_TR1;
	
	NVIC_EnableIRQ(EXTI0_IRQn); 
	NVIC_EnableIRQ(EXTI1_IRQn); 
	
	NVIC_SetPriority(EXTI0_IRQn, 2); 
	NVIC_SetPriority(EXTI1_IRQn, 1); 

}

void EXTI0_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PR0) {
		if (state == SHUTDOWN) {
			state = ACTIVE;
		} else {
			state = SHUTDOWN;
		}
        EXTI->PR |= EXTI_PR_PR0;
    }
}

void EXTI1_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR1) {
		NVIC_SystemReset();
        EXTI->PR |= EXTI_PR_PR1;
    }
    
}