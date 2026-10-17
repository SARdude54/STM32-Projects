#ifndef UART_H
#define UART_H

#include <stdint.h>

/**
 * head is where the producer will write the next byte
 * tail is wehre the consumer will read the next byte
 * 
 * producer is USART2_IRQHandler()
 * consumer is app_update()
 * 
 * ring buffer is when the head will wrap around instead of going past array
 * ring buffer is empty when uart2_rx == uart2_rx_tail
 */

void uart2_init(uint32_t peripheral_clock_hz, uint32_t baud_rate);
void uart2_write_byte(uint8_t byte);
void uart2_write_string(const char *string);

uint32_t uart2_read_byte(uint8_t *byte);

#endif