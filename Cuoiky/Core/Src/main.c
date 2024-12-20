#include "stm32f4xx.h"
#include "pwm.h"
#include "delay.h"
#include "i2c.h"
#include "hcsr04.h"
#include "lcd.h"
#include "led.h"
#include "button.h"
#include "uart.h"
#include <stdio.h>

volatile uint8_t detect = OBS_NOT_DETECT;
volatile uint8_t state = SHUTDOWN;


int main(void)
{	
	GPIO_Init();
	Timer3_Initilize(500);
	Timer4_Initilize();
    Timer2_Init();
    I2C_Init();
    LCD_Init();
	HCSR04_Init();
	Interrupt_PB_Init();
	float distance = 0;
    while(1)
    {
		if (state == ACTIVE) {
			for (int i =0; i <20; i++) {
				distance += HCSR04_Read();
				delay_us(4000);
			}
			distance /= 20;
			if (distance < 50) {
				detect = OBS_DETECT;
				if (distance < 50 && distance > 40) {
					Timer3_Initilize(500);
				} else if (distance <= 40 && distance > 30) {
					Timer3_Initilize(400);
				} else if (distance <= 30 && distance > 20) {
					Timer3_Initilize(300);
				} else if (distance <= 20 && distance > 10) {
					Timer3_Initilize(200);
				} else if (distance <= 10 && distance > 0) {
					Timer3_Initilize(100);
				}							
				LCD_PutString("SYS:1 DETECT:1  ",1);
				delay_us(1000);
				LCD_PutStringFloat("Distance: ", distance, 2);
			} else {
				detect = OBS_NOT_DETECT;
				LCD_PutString("SYS:1 DETECT:0  ", 1);
				delay_us(1000);
				LCD_PutString("                ", 2);
			}
		} else {
			LCD_PutString("SYS:0           ", 1);
			delay_us(1000);
			LCD_PutString("                ", 2);
			delay_us(1000);
		}
		
    }
}
