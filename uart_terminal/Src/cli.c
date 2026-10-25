#include <stdint.h>
#include <string.h>
#include "uart.h"
#include <bsp_led.h>
#include "cli.h"


#define CLI_BUFFER_SIZE 32u

static char cli_buffer[CLI_BUFFER_SIZE];
static uint32_t cli_index;

typedef enum{

    CLI_STATE_RECEIVING,
    CLI_STATE_OVERFLOW

} cli_state_t;

static cli_state_t cli_state;

static void cli_reset_buffer(void){
    cli_index = 0u;
    cli_buffer[0] = '\0';
}

static void cli_process_command(const char *command){
    if(strcmp(command, "help") == 0){
        uart2_write_string("Available commands:\r\n");
        uart2_write_string("  help\r\n");
        uart2_write_string("  led on\r\n");
        uart2_write_string("  led off\r\n");
        uart2_write_string("  led toggle\r\n");

        
    } else if(strcmp(command, "led on") == 0){
        bsp_led_on();
        uart2_write_string("LED turned ON\r\n");

    } else if(strcmp(command, "led off") == 0){
        bsp_led_off();
        uart2_write_string("LED turned OFF\r\n");

    } else if(strcmp(command, "led toggle") == 0){
        bsp_led_toggle();
        uart2_write_string("LED toggled\r\n");

    } else {
        uart2_write_string("Unknown command \r\n");
    }
}


void cli_init(void){
    cli_reset_buffer();

    cli_state = CLI_STATE_RECEIVING;

    uart2_write_string("STM32> ");
}

void cli_update(void){
    uint8_t byte;

    if(uart2_read_byte(&byte) == 0u) return;

    switch (cli_state){
        case CLI_STATE_RECEIVING: {
            
            if (byte == '\r' || byte == '\n'){

                if(cli_index == 0) break;

                cli_buffer[cli_index] = '\0';

                uart2_write_string("\r\n");

                cli_process_command(cli_buffer);

                cli_reset_buffer();
                
                uart2_write_string("STM32> ");


            } else if(byte == '\b' || byte == 0x7Fu) {
                if(cli_index > 0u){
                    cli_index--;
                    cli_buffer[cli_index] = '\0';
                    uart2_write_string("\b \b"); 
                }
            } else if (byte >= 0x20u && byte <= 0x7Eu) {

                if(cli_index < (CLI_BUFFER_SIZE - 1)){
                    cli_buffer[cli_index] = (char)byte;
                    cli_index++;
                    cli_buffer[cli_index] = '\0';
                    uart2_write_byte(byte);
                } else {
                    cli_state = CLI_STATE_OVERFLOW;
                }

            } 
            
            break;
        }
        
        case CLI_STATE_OVERFLOW: {

            if (byte == '\r' || byte == '\n'){

                uart2_write_string("\r\nERROR: Command length exceeded");

                cli_reset_buffer();

                cli_state = CLI_STATE_RECEIVING;
            
                uart2_write_string("STM32> ");
            }


            break;
        }
    
        default: {

            cli_reset_buffer();

            cli_state = CLI_STATE_RECEIVING;

            break;
        }
    }

    

}
