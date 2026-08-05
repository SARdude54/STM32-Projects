#ifndef TIMEBASE_H
#define TIMEBASE_H

#include <stdint.h>

void timebase_init(uint32_t system_clock_hz);
void delay_ms(uint32_t milliseconds);
uint32_t timebase_get_ms(void);


#endif