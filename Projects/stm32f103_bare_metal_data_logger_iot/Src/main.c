/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


#include <stdint.h>
#include <stdio.h>
#include "gb_gpio.h"
#include "gb_i2c.h"
#include "gb_rcc.h"
#include "GB_UART.h"
#include "spi.h"
#include "gb_t_delay.h"
#include "w25q128jv.h"
#include "stm32f1xx.h"
#include "gb_ssd1306.h"
#include "GB_millis_systick.h"
#include "GB_ESP8266_Basic.h"
#include "GB_ESP8266_client.h"
#include "gb_dht11.h"

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

const char gb_urlpath[100] ="/update?api_key=K86PTN9OPANWW8MF&field1";
const char gb_url[100] = "api.thingspeak.com";

char rx_buf[1025];
char tx_buf[10];

int main(void)
{

    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
    systick_millis(32000);
    system_clk();
	timer_initialise();

	ssd1306_init();
	ssd1306_clear(GB_SSD1306_COLOR_BLACK);
	ssd1306_update_data();


	GB_uart_init1();  //terminal
	GB_printString1("IOT DATA Logger");
	GB_uart_init2();  	//ESP8266
	__NVIC_EnableIRQ(USART2_IRQn);
	__NVIC_SetPriority (USART2_IRQn, 1);
	GB_esp8266_initialise_client();
	uint8_t gb_t = 90;

float temp =0;
	//spi_init();
	//char * tran_buff = " Temperature Reading on this day at time: 32*c\n";


//	erase_sector4KB(read_addr1);
//	delay_ms(100);
//	W25_Write_Data(read_addr1,tran_buff,47);
//	delay_ms(100);
	/* Loop forever */
	while(1)
	{

		GB_dht11();
		temp = GB_dht11_temp();
	   ssd1306_GotoXY(0, 0);
	   ssd1306_print_string("Temperature:  ", GB_SSD1306_COLOR_WHITE);
	   ssd1306_float(temp,GB_SSD1306_COLOR_WHITE);
	   //ssd1306_print_decimel(GB_dht11_temp(),GB_SSD1306_COLOR_WHITE);
	   ssd1306_print_string(" *C", GB_SSD1306_COLOR_WHITE);
	   ssd1306_update_data();


		GB_esp8266_connectTCPserver(gb_url,80);
	    GB_esp8266_cipsendfloat(temp,gb_urlpath);
	    GB_esp8266_tcpgetcommandfloat(gb_urlpath,temp);
		delay_ms(2000);
		//gb_t++;

		GB_printString1("\nTemperature : ");
		GB_float_value1(temp);
		GB_printString1("\n");
		delay_ms(10);

//		 erase_sector4KB(read_addr1);
//		 delay_ms(100);
//		 W25_Write_Data(read_addr1,tran_buff,135);
//		 delay_ms(100);
//		 W25_Read_Data(read_addr1,rx_buf,47);
//		 delay_ms(100);
//		 GB_printString1(rx_buf);
		// GB_printString1("\n");
	//	 delay_ms(100);


	}
}
