#include <stdint.h>

#include "app.h"
#include "bsp_button.h"
#include "bsp_led.h"

void app_init(void)
{
    bsp_led_init();
    bsp_button_init();
}

void app_update(void)
{
    const uint32_t button_state = bsp_button_read();

    if (button_state != 0u)
    {
        bsp_led_on();
    }
    else
    {
        bsp_led_off();
    }
}