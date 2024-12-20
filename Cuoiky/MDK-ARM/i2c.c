#include "stm32f4xx.h"
#include "i2c.h"
#include "uart.h"

void I2C_Init(void)
{
	//Clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	
	//Analog mode
    GPIOB->MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1;
	
    //Open drain
	GPIOB->OTYPER |= GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9;
	
    //High speed output
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9;
	
    //Pull up resistor
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR9_0;
	
    //Alternative function for i2c
	GPIOB->AFR[1] |= (4 << (4*0)) | (4 << (4*1));
	
	//Reset I2C
	I2C1->CR1 |= (1<<15);
	I2C1->CR1 &= ~(1<<15);
	
	//I2C clock bus
    I2C1->CR2 |= (42<<0);
    I2C1->CCR = 210<<0;
    I2C1->TRISE = 43;
	
    //I2C enable
	I2C1->CR1 |= I2C_CR1_PE;
}

void I2C_SendData(uint8_t address, uint8_t data)
{
	//Acknowlegde enable
	I2C1->CR1 |= I2C_CR1_ACK;
	
	//Send start signal
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB)); // wait for start condition generation

	
	//Send slave address
    I2C1->DR = address;
    while(!(I2C1->SR1 & I2C_SR1_ADDR)); // wait for address matched
	//Dummy read the register to clear it before sending data
	(void)I2C1->SR1;
    (void)I2C1->SR2;
	
	//Send data
	while (!(I2C1->SR1 & I2C_SR1_TXE)); // wait for data register is empty
    I2C1->DR = data;
    while(!(I2C1->SR1 & I2C_SR1_BTF)); //wait for byte transfer
	
	//Send stop signal
    I2C1->CR1 |= I2C_CR1_STOP;
}