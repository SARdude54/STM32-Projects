#include <stdint.h>

#include "app.h"
#include "bsp_button.h"
#include "bsp_led.h"
#include "uart.h"

#define USART2_CLOCK_HZ 4000000u
#define UART_BAUD_RATE  115200u

void app_init(void)
{
    bsp_led_init();
    bsp_button_init();

    uart2_init(USART2_CLOCK_HZ, UART_BAUD_RATE);

    uart2_write_string("Hello from STM32!\r\n");
}

void app_update(void){

    bsp_button_update();

    if(bsp_button_pressed() != 0u){
        bsp_led_toggle();
    }
}