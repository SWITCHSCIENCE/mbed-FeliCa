/*
 * RC-S620/S sample program for mbed
 *
 * Copyright 2011 SWITCHSCIENCE
 *
 */

#include <inttypes.h>
#include "mbed.h"
#include "RCS620S.h"

static char HEXSTRING[17] = "0123456789ABCDEF";
char *itoh(uint8_t val, char *ptr);

DigitalOut serialready(LED1);
DigitalOut felicarwready(LED2);
DigitalOut cardread(LED3);

Serial pc(USBTX, USBRX);
Serial serial(p9, p10);

int main() {
    int ret;
    int loop;
    char idbuffer[17];
    char *hexid;
    char pmmbuffer[17];
    char *hexpmm;

    serial.baud(115200);
    serialready = 1;
    pc.printf("Serial Ready.\n\r");

    ret = 0;
    RCS620S felica;   
    while (ret == 0) {
        wait(1);
        ret = felica.initDevice();
    }
    felicarwready = 1;
    pc.printf("FeliCa Reader/Writer Ready.\n\r");

    wait(2);

	while (1) {
    
        pc.printf("Please FeliCa Touch!  ");
        
        hexid = idbuffer + sizeof(idbuffer);        // hexid = buffer[ last of elements ];
        *--hexid = '\0';
        hexpmm = pmmbuffer + sizeof(pmmbuffer);
        *--hexpmm = '\0';
        
        while (felica.polling(0xffff) == 0);
        cardread = 1;
        felicarwready = 0;

        
        pc.printf("idm: ");
        for (loop = 7; loop >= 0; loop--){
            hexid = itoh(felica.idm[loop], hexid);
        }
        pc.printf("%s  /", hexid);
        
        pc.printf("pmm: ");
        for (loop = 7; loop >= 0; loop--){
            hexpmm = itoh(felica.pmm[loop], hexpmm);
        }
        pc.printf("%s\n\r", hexpmm);
        
        wait(1);
        cardread = 0;
        felicarwready = 1;
    }
}

char *itoh(uint8_t val, char *ptr) {
    *--ptr = HEXSTRING[(val % 16)];     // (val & 0x0f)
    val /= 16;                          // val = val >> 4;
    *--ptr = HEXSTRING[(val % 16)];     // (val & 0x0f)
    return(ptr);
}
