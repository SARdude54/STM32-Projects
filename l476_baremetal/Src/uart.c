#include "uart.h"
#include "gpio.h"
#include "stm32l476xx.h"

#define UART2_TX_PORT GPIOA
#define UART2_TX_PIN  2u
#define UART2_AF      7u

void uart2_init(uint32_t peripheral_clock_hz, uint32_t baud_rate)
{
    
    // 1. Configure PA2 as USART2_TX.
     
    gpio_pin_init(
        UART2_TX_PORT,
        UART2_TX_PIN,
        GPIO_MODE_ALTERNATE,
        GPIO_OUTPUT_PUSH_PULL,
        GPIO_SPEED_LOW,
        GPIO_PULL_NONE);

    gpio_pin_set_alternate_function(
        UART2_TX_PORT,
        UART2_TX_PIN,
        UART2_AF);

    
    // 2. Enable USART2 peripheral clock.
     
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
    (void)RCC->APB1ENR1;

    
    // 3. Disable USART while configuring it.
    
    USART2->CR1 &= ~USART_CR1_UE;

    
    // 4. 8 data bits.
    
    // M1:M0 = 00
     
    USART2->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0);

    
    // 5. No parity.
     
    USART2->CR1 &= ~USART_CR1_PCE;

    
     // 6. Oversampling by 16.
    
     // OVER8 = 0
     
    USART2->CR1 &= ~USART_CR1_OVER8;

    
     // 7. One stop bit.
     
     //STOP[1:0] = 00
     
    USART2->CR2 &= ~USART_CR2_STOP;

    
    // 8. Program baud rate.
     
    USART2->BRR = (peripheral_clock_hz + (baud_rate / 2u)) / baud_rate;

        
    // 9. Enable USART.
    
    USART2->CR1 |= USART_CR1_UE;

    // 10. Enable transmitter.
    
    USART2->CR1 |= USART_CR1_TE;


}


void uart2_write_byte(uint8_t byte)
{
    while ((USART2->ISR & USART_ISR_TXE) == 0u)
    {
    }

    USART2->TDR = byte;
}

void uart2_write_string(const char *string)
{
    while (*string != '\0')
    {
        uart2_write_byte((uint8_t)*string);
        string++;
    }
}
