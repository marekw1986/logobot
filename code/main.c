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
//		0x6400 - 
//		0x6480 - 
//		0x6500 - 
//		0x6580 - 
//		0x6600 - 
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
//#include "mos6551.h"
//#include "mc6840.h"
#include "delay.h"
#include "io.h"

static char buffer[64];
static uint16_t number = 0;

char* __fastcall__ utoa (unsigned val, char* buf, int radix);
char* __fastcall__ ultoa (unsigned long val, char* buf, int radix);
size_t __fastcall__ strlen (const char* s);


int main (void) {
	port_write(0x80);

    //mc6840_init();
    //mos6551_init();
	hd44780_init();
	
	CLI();

	hd44780_gotoxy(0, 0);
	hd44780_puts("6502 is still alive!");
	hd44780_gotoxy(1, 0);
	hd44780_puts("Device designed by");
	hd44780_gotoxy(2, 0);
	hd44780_puts("Marek Wiecek SQ9RZI");
	
	while(1) {
		hd44780_gotoxy(3, 0);
		hd44780_puts("                    ");		
		utoa(number,buffer, 10);
		hd44780_gotoxy(3, 0);
		hd44780_puts(buffer);
		number++;
		
		if (!(BTNS & BTN0)) { number += 50; }
		
		port_tgl(0x85);						//Toggle both LEDs and watchgod line
		delay_ms(250);			
		//mos6551_handle_rx();
	}
	
	return 0;
}
