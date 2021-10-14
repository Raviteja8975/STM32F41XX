/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <gb_gpio.h>
#include <gb_i2c.h>
#include <gb_rcc.h>
#include <GB_UART.h>
#include <gb_t_delay.h>
#include <gb_ina219.h>
#include "stm32f1xx.h"
/*
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
*/

#define DEMCR        			*((volatile uint32_t*) 0xE000EDFCU )

/* ITM register addresses */
#define ITM_STIMULUS_PORT0   	*((volatile uint32_t*) 0xE0000000 )
#define ITM_TRACE_EN          	*((volatile uint32_t*) 0xE0000E00 )


int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		//__io_putchar(*ptr++);
		 ITM_SendChar(*ptr++);
	}
	return len;
}


int main(void)
{
	system_clk();
	timer_initialise();
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

	ina219_init();
	GB_uart_init1();
	GB_printString1("Bus Voltage: ");

    /* Loop forever */
	while(1)
	{
		GB_printString1("Bus Voltage: ");
		//GB_decimel2(GPIOA->IDR);
		GB_printString1("Bus Voltage: ");
		ina219_busvoltage();
		GB_printString1("V\n");

		GB_printString1("Shunt Voltage: ");
		ina219_shuntvoltage();
		GB_printString1("mV\n");

		GB_printString1("Shunt Current: ");
		ina219_shuntcurrent();
		GB_printString1("mA\n");

		GB_printString1("Load Power : ");
		ina219_Loadpower();
		GB_printString1("mW\n");


		GB_printString1("\n");
		GB_printString1("\n");
		GB_printString1("\n");
		delay_ms(2000);
	}

}
