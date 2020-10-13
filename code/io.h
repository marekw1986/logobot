#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

#define PORT (*(volatile uint8_t*)0x7C00)
#define BTNS (*(volatile uint8_t*)0x6300)

#define PA_8255 (*(volatile uint8_t*)0x6400)
#define PB_8255	(*(volatile uint8_t*)0x6401)
#define PC_8255	(*(volatile uint8_t*)0x6402)
#define CONF_8255 (*(volatile uint8_t*)0x6403)

#define BTN_UP	0x01
#define BTN_DN	0x02
#define BTN0	0x04
#define BTN1	0x08
#define BTN2	0x10
#define BTN3	0x20
#define BTN4	0x40
#define BTN5	0x80

#define BACKLIGHT_PIN 0x10

#define SHORT_WAIT 3    // 3 * 20ms = 60 ms.

typedef struct key {
    uint8_t last_state;
	uint8_t timer;
} key_t;

void __fastcall__ port_write (uint8_t data);
void __fastcall__ port_set (uint8_t data);
void __fastcall__ port_clr (uint8_t data);
void __fastcall__ port_tgl (uint8_t data);
void __fastcall__ feed_hungry_watchdog (void);

#endif
