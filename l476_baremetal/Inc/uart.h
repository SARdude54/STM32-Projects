#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart2_init(uint32_t peripheral_clock_hz, uint32_t baud_rate);
void uart2_write_byte(uint8_t byte);
void uart2_write_string(const char *string);

uint8_t uart2_read_byte(void);
uint32_t uart2_read_byte_nonblocking(uint8_t *byte);

#endif