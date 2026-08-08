#include <stdint.h>

#include "app.h"
#include "bsp_led.h"
#include "timebase.h"

#define LED_TOGGLE_PERIOD_MS 500u

static uint32_t last_toggle_ms;

void app_init(){
    last_toggle_ms = timebase_get_ms();
    bsp_led_init();
}

void app_update(){
    const uint32_t now_ms = timebase_get_ms();

    if((uint32_t)(now_ms - last_toggle_ms) >= LED_TOGGLE_PERIOD_MS){
        last_toggle_ms += LED_TOGGLE_PERIOD_MS;
        bsp_led_toggle();
    }
}
