#include <stdint.h>
#include <string.h>
#include "cli.h"
#include "uart.h"

#define CLI_BUFFER_SIZE 32u

static char cli_buffer[CLI_BUFFER_SIZE];
static uint32_t cli_index;

static void cli_reset_buffer(void){
    cli_index = 0u;
    cli_buffer[0] = '\0';
}


void cli_init(void){
    cli_reset_buffer();

    uart2_write_string("\r\nSTM32> ");
}

void cli_update(void){
    uint8_t byte;

    if(uart2_read_byte(&byte) == 0u){
        return;
    }

    

    if (byte == '\r' || byte == '\n'){

        if(cli_index == 0) return;

        cli_buffer[cli_index] = '\0';

        uart2_write_string("\r\nCommand received: ");
        uart2_write_string(cli_buffer);
        uart2_write_string("\r\n");

        cli_reset_buffer();
        
        uart2_write_string("STM32> ");


    } else {
        if(strlen(cli_buffer) < (CLI_BUFFER_SIZE - 1)){
            cli_buffer[cli_index] = (char)byte;
            cli_index++;
            cli_buffer[cli_index] = '\0';
            uart2_write_byte(byte);
        }
    }

    

}
