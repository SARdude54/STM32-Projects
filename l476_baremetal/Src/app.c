#include <stdint.h>

#include "app.h"
#include "bsp_button.h"
#include "bsp_led.h"

void app_init(void)
{
    bsp_led_init();
    bsp_button_init();
}

void app_update(void){
    if(bsp_button_pressed() != 0u){
        bsp_led_toggle();
    }
}