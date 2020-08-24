#include <6502.h>
#include "mc6840.h"
#include "ff.h"

#define PRESCALER 57		//For STS-5/SBM-20/BOI-33 sensor tube 0.0057 * 10000 to avoid float

volatile uint8_t milliseconds = 0;
volatile uint32_t uptime_value = 0;
volatile uint8_t  geiger_ind = 0;
volatile uint16_t geiger_pulses[60];
volatile uint16_t tmp16;

uint16_t cpm;
uint32_t tmp;
static uint8_t i;

void mc6840_init (void) {
    MC6840_CON13 = TM_COUNTER_OUTPUT_DISABLE | TM_INTERUPT_ENABLE | TM_CONT_OP_MODE1 | TM_NORMAL_16BIT | TM_SYS_CLK | TMCR3_T3_CLK_NO_PRESCALER;	//CON3 first by default after reset. TIMER3 is used for systick. Output diable and sys clk.
    MC6840_CON2 = TM_COUNTER_OUTPUT_ENABLE | TM_INTERUPT_DISABLE | TM_CONT_OP_MODE1 | TM_NORMAL_16BIT | TM_SYS_CLK | TMCR2_WRITE_CR1;				//CON2 accessed directly. TIMER2 generates sound. Output enable and sys clk.
    MC6840_CON13 = TM_COUNTER_OUTPUT_DISABLE | TM_INTERUPT_DISABLE | TM_CONT_OP_MODE1 | TM_NORMAL_16BIT | TM_EXT_CLK | TMCR1_ALL_TIMERS_ALLOWED;	//CON1. TIMER1 counts Geiger pulses, so external source
    MC6840_TIMER1 = Swap2Bytes(0xFFFF);       //Remember about endianess - MC6800 family is big endian, 6502 is little endian. Remember that timer is decremented.
    MC6840_TIMER2 = Swap2Bytes(0xF82F);       //500 Hz signal on audio output
    MC6840_TIMER3 = Swap2Bytes(0xB1DF);       //20ms interrupt (0xFFFF - 10000) - it is decremented!
    
    for (i=0; i<60; i++) geiger_pulses[i] = 0x00;
}

DWORD get_fattime (void)
{
	return(0);
}

uint8_t millis(void) {
    SEI();
    tmp = milliseconds;
    CLI();
	return tmp;
}

uint32_t uptime (void) {
	SEI();
	tmp = uptime_value;
	CLI();
	return tmp;
}

void update_geiger_pulses (void) {
	SEI();
	asm("lda $6482");
	asm("sta _tmp16+1");
	asm("lda $6483");
	asm("sta _tmp16");
	MC6840_TIMER1 = 0xFFFF;
	CLI();
    //cpm = 0xFFFF - tmp16;
    geiger_pulses[geiger_ind] = 0xFFFF - tmp16;
    geiger_ind++;
    if (geiger_ind > 59) geiger_ind = 0;
}

uint16_t get_geiger_pulses (void) {
	cpm = 0;
	for (i=0; i<60; i++) cpm += geiger_pulses[i];
    return cpm;
}


uint32_t get_geiger_usv (void) {
	return (PRESCALER * cpm);
}



void set_sound_frequency (uint16_t freq) {
    if (freq < 24) return;          //Min required frequency. It will solve div/0 and word size issues.
    MC6840_TIMER2 = Swap2Bytes(0xFFFF-(1000000/freq));
}