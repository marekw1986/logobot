#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "parser.h"
#include "mc6840.h"
#include "mos6551.h"

char* __fastcall__ strtok (char* s1, const char* s2);
int __fastcall__ strcasecmp (const char* s1, const char* s2);
char* __fastcall__ strchr (const char* s, int c);
int __fastcall__ atoi (const char* s);
char* __fastcall__ ultoa (unsigned long val, char* buf, int radix);

static uint8_t test_service(char * params);

#define CMD_CNT 	1

const TCMD komendy[CMD_CNT] = {
		{"test", 		test_service}
};


void parse_cmd(char * pBuf) {
	char* tok;
	uint8_t i;
	
	if ( strchr(pBuf, ' ') ) {			// Polecenia z paarametrami
		tok = strtok(pBuf, " \0");
		for(i=0;i<CMD_CNT;i++) {
			if ( 0 == strcasecmp(tok, komendy[i].komenda) ) {
				tok = strtok(NULL, " ");
				if (0 == komendy[i].cmd_service(tok)) { mos6551_puts("Invalid or missing parameter\r\n"); }
				else { mos6551_puts("OK\r\n"); }
				return;
			}
		}
		mos6551_puts("Unknown command\r\n");
	}
	else {							// Polecenia bez parametrów
		for(i=0;i<CMD_CNT;i++) {
			if ( 0 == strcasecmp(pBuf, komendy[i].komenda) ) {
				if (0 == komendy[i].cmd_service(NULL)) { mos6551_puts("Invalid or missing parameter\r\n"); }
				else { mos6551_puts("OK\r\n"); }
				return;
			}
		}
		mos6551_puts("Unknown command\r\n");
	}
}


static uint8_t test_service(char * params) {
	char* tok;
	int8_t supbuf[4];
	
	mos6551_puts("Test\r\n");
	
	return 1;	
}
