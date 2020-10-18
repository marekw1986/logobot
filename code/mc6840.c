#include <6502.h>
#include <stdlib.h>
#include <string.h>
#include "mc6840.h"

volatile uint8_t milliseconds = 0;

char* __fastcall__ utoa (unsigned val, char* buf, int radix);
char* __fastcall__ strcat (char* s1, const char* s2);


void mc6840_init (void) {
    MC6840_CON13 = TM_COUNTER_OUTPUT_ENABLE | TM_INTERUPT_DISABLE | TM_CONT_OP_MODE1 | TM_NORMAL_16BIT | TM_SYS_CLK | TMCR3_T3_CLK_NO_PRESCALER;	//CON3 first by default after reset. TIMER3 is used for STEP1. Output diable and sys clk.
    MC6840_CON2 = TM_COUNTER_OUTPUT_ENABLE | TM_INTERUPT_DISABLE | TM_CONT_OP_MODE1 | TM_NORMAL_16BIT | TM_SYS_CLK | TMCR2_WRITE_CR1;				//CON2 accessed directly. TIMER2 is udes for STEP2. Output disable and sys clk.
    MC6840_CON13 = TM_COUNTER_OUTPUT_DISABLE | TM_INTERUPT_ENABLE | TM_CONT_OP_MODE1 | TM_NORMAL_16BIT | TM_SYS_CLK | TMCR1_ALL_TIMERS_ALLOWED;	//CON1. TIMER1 is used as SysTick. Output disable and sys clk.
    MC6840_TIMER1 = Swap2Bytes(0xB1DF);       //Remember about endianess - MC6800 family is big endian, 6502 is little endian. Remember that timer is decremented. 20ms interrupt (0xFFFF - 10000) - it is decremented!
    MC6840_TIMER2 = Swap2Bytes(0xFFFF);       //STEP2
    MC6840_TIMER3 = Swap2Bytes(0xFFFF);       //STEP1
}


uint8_t millis(void) {
	uint8_t tmp;
    SEI();
    tmp = milliseconds;
    CLI();
	return tmp;
}
