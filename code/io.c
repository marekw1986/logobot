#include "io.h"

static uint8_t port_cache = 0;


void __fastcall__ port_write (uint8_t data) {
    port_cache = data;
    PORT = port_cache;
}


void __fastcall__ port_set (uint8_t data) {
    port_cache |= data;
    PORT = port_cache;
}


void __fastcall__ port_clr (uint8_t data) {
    port_cache &= ~data;
    PORT = port_cache;
}


void __fastcall__ port_tgl (uint8_t data) {
    port_cache ^= data;
    PORT = port_cache;
}


void __fastcall__ feed_hungry_watchdog (void) {
	port_clr(0x01);
	asm("nop");
	asm("nop");
	asm("nop");
	port_set(0x01);
}
