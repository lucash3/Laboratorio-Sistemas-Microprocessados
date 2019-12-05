#include <math.h>

#ifndef CLOCK_INCLUDED
#define GPIO_INCLUDED

#define FREQUENCE 0
#define PERIOD 1
#define MY_ACLK 32768
#define MY_SMCLK 1000000
#define MY_MCLK 16000000

int get_N(float freq_or_period, _Bool type, double frequence_clock) {
    if (type == PERIOD) {
        return (int)(ceil(freq_or_period * frequence_clock) - 1);
    } else {
        return (int)(ceil(frequence_clock / freq_or_period) - 1);
    }
}
#endif
