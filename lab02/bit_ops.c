#include <stdio.h>
#include "bit_ops.h"
#define MAX_BIT 31

// Return the nth bit of x.
// Assume 0 <= n <= 31
unsigned get_bit(unsigned x,
        unsigned n) {
    x = x>>n;
    x = x&1;
    return x;
}

// Set the nth bit of the value of x to v.
// Assume 0 <= n <= 31, and v is 0 or 1
void set_bit(unsigned * x,
             unsigned n,
             unsigned v) {

    if(v==get_bit(*x,n)) return;
    
    *x=*x^(1<<n);
    
}

// Flip the nth bit of the value of x.
// Assume 0 <= n <= 31
void flip_bit(unsigned * x,
              unsigned n) {
    *x=*x^(1<<n);
    
}

