#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"
#define MAX_BIT 31

void lfsr_calculate(uint16_t *reg) {
//TODO
/* reg store data, like:
 * 0100 0011 1101 1010
 * may need to use set_bit()
 */
    uint16_t a5=get_bit(*reg,5);
    uint16_t a3=get_bit(*reg,3);
    uint16_t a2=get_bit(*reg,2);
    uint16_t a0=get_bit(*reg,0);

    *reg=*reg>>1;
    
    uint16_t result=a5^(a3^(a2^a0));
    set_bit(reg,15,result);
}

void set_bit(uint16_t *x,
             uint16_t n,
             uint16_t v) {

    if(v==get_bit(*x,n)) return;
    
    *x=*x^(1<<n);
    
}

uint16_t get_bit(uint16_t x,
        uint16_t n) {
    x = x>>n;
    x = x&1;
    return x;
}

