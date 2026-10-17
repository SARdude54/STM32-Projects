#include "uart.h"
#include "gpio.h"
#include "stm32l476xx.h"

#define UART2_TX_PORT GPIOA
#define UART2_TX_PIN  2u

#define UART2_RX_PORT GPIOA
#define UART2_RX_PIN 3u

#define UART2_AF      7u

#define UART2_RX_BUFFER_SIZE 16u

static volatile uint8_t uart2_rx_buffer[UART2_RX_BUFFER_SIZE];
static volatile uint32_t uart2_rx_head;
static volatile uint32_t uart2_rx_tail;
static volatile uint32_t uart2_rx_overflow_count;

void uart2_init(uint32_t peripheral_clock_hz, uint32_t baud_rate){
    
    /* PA2 = USART2_TX */
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

    /* PA3 = USART2_RX */
    gpio_pin_init(
        UART2_RX_PORT,
        UART2_RX_PIN,
        GPIO_MODE_ALTERNATE,
        GPIO_OUTPUT_PUSH_PULL,
        GPIO_SPEED_LOW,
        GPIO_PULL_NONE);

    gpio_pin_set_alternate_function(
        UART2_RX_PORT,
        UART2_RX_PIN,
        UART2_AF);

    /* USART2 clock */
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
    (void)RCC->APB1ENR1;

    /* init software receieve state before interrupts are enabled*/
    uart2_rx_head = 0u;
    uart2_rx_tail = 0u;
    uart2_rx_overflow_count = 0u;

    /* Disable USART while configuring */
    USART2->CR1 &= ~USART_CR1_UE;

    /* 8 data bits, no parity, oversampling x16 */
    USART2->CR1 &= ~(USART_CR1_M1 |
                     USART_CR1_M0 |
                     USART_CR1_PCE |
                     USART_CR1_OVER8);

    /* 1 stop bit */
    USART2->CR2 &= ~USART_CR2_STOP;

    /* Baud rate */
    USART2->BRR = (peripheral_clock_hz + (baud_rate / 2u)) / baud_rate;

    /* Enable USART */
    USART2->CR1 |= USART_CR1_UE;

    /* Enable transmitter and receiver */
    USART2->CR1 |= USART_CR1_TE;
    USART2->CR1 |= USART_CR1_RE;

    // enable the recieve data interrupt
    USART2->CR1 |= USART_CR1_RXNEIE; // tells USART to generate an interrupt request when RXNE becomes set.

    // USART2 RXNEIE -> USART2 interrupt request -> NVIC USART_IRQn enabled -> CPU executes USART2_IRQHandler()
    NVIC_ClearPendingIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 5u);
    NVIC_EnableIRQ(USART2_IRQn);

}


void uart2_write_byte(uint8_t byte){
    while ((USART2->ISR & USART_ISR_TXE) == 0u)
    {
    }

    USART2->TDR = byte;
}

void uart2_write_string(const char *string){
    while (*string != '\0')
    {
        uart2_write_byte((uint8_t)*string);
        string++;
    }
}

uint32_t uart2_read_byte(uint8_t *byte)
{
    if (uart2_rx_head == uart2_rx_tail)
    {
        return 0u;
    }

    *byte = uart2_rx_buffer[uart2_rx_tail];

    uart2_rx_tail =
        (uart2_rx_tail + 1u) % UART2_RX_BUFFER_SIZE;

    return 1u;
}

void USART2_IRQHandler(void)
{
    if ((USART2->ISR & USART_ISR_RXNE) != 0u)
    {
        const uint8_t byte = (uint8_t)USART2->RDR;

        const uint32_t next_head =
            (uart2_rx_head + 1u) % UART2_RX_BUFFER_SIZE;

        if (next_head != uart2_rx_tail)
        {
            uart2_rx_buffer[uart2_rx_head] = byte;
            uart2_rx_head = next_head;
        }
        else
        {
            uart2_rx_overflow_count++;
        }
    }
}
