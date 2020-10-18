//0x4000 - EEPROM
//0x6000 - CS0		LCD + BUTTONS
//		0x6000 - NOT USED
//		0x6080 - NOT USED
//		0x6100 - NOT USED
//		0x6180 - NOT USED
//		0x6200 - NOT USED
//		0x6280 - NOT USED
//		0x6300 - BUTTONS
//		0x6380 - HD44780 LCD

//0x6400 - CS1
//		0x6400 - 8255
//		0x6480 - TIMER
//		0x6500 - UART
//		0x6580 - CF
//		0x6600 - NOT USED
//		0x6680 - NOT USED
//		0x6700 - NOT USED
//		0x6780 - NOT USED

//0x6800 - CS2
//0x6C00 - CS3
//0x7000 - CS4
//0x7400 - CS5
//0x7800 - CS6
//0x7C00 - CS7

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <6502.h>
#include "config.h"
#include "hd44780.h"
#include "mos6551.h"
#include "mc6840.h"
#include "delay.h"
#include "io.h"

static char buffer[64];
static uint16_t number = 0;
static uint8_t timer = 0;
static uint8_t uart_timer = 0;

char* __fastcall__ utoa (unsigned val, char* buf, int radix);
char* __fastcall__ ultoa (unsigned long val, char* buf, int radix);
size_t __fastcall__ strlen (const char* s);


int main (void) {
	port_write(0x81);
	
	CONF_8255 = 0x80;			//Mode 0, PA, PB and PC is n output
	PA_8255 = 0x81; 			//(GATE3=1, GATE1=0, PA5=0. PA4=0, DIR1=0, DIR2=0, PA1=0, GATE2=1)
	PB_8255 = 0x00;				//All PB outputs low
	PC_8255 = 0x00;	 			//All PC outputs low

	mc6840_init();
	mos6551_init();
	hd44780_init();
	
	CLI();

	hd44780_gotoxy(0, 0);
	hd44780_puts("6502 is still alive!");
	hd44780_gotoxy(1, 0);
	hd44780_puts("Device designed by");
	hd44780_gotoxy(2, 0);
	hd44780_puts("Marek Wiecek SQ9RZI");
	
	while(1) {
		if ( (uint8_t)(millis() - timer) > 12 ) {			//12x20ms
			timer = millis();
			port_tgl(0x84);						//Toggle both LEDs
			feed_hungry_watchdog();				//Reset watchdog
		
			hd44780_gotoxy(3, 0);
			hd44780_puts("                    ");		
			utoa(number,buffer, 10);
			hd44780_gotoxy(3, 0);
			hd44780_puts(buffer);
			number++;
			if (!(BTNS & BTN0)) { number += 50; }
		}			
		
		if ( (uint8_t)(millis() - uart_timer) > 40 ) {
			uart_timer = millis();
			mos6551_puts("Test\r\n");
		}	
			
		mos6551_handle_rx();
	}
	
	return 0;
}
