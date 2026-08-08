#include <stdint.h>

#include "app.h"
#include "bsp_led.h"
#include "timebase.h"

#define LED_TOGGLE_PERIOD_MS 500u
#define LED_ON_PERIOD_MS 100u
#define LED_OFF_PERIOD_MS 900u

static uint32_t last_toggle_ms;
static led_state_t state;

void app_init(){
    last_toggle_ms = timebase_get_ms();
    bsp_led_init();

    led_state_t state;
    state = LED_STATE_OFF;
}

void app_update(){
    const uint32_t now_ms = timebase_get_ms();

    switch (state){
        case LED_STATE_OFF: {
            
            if((uint32_t)(now_ms - last_toggle_ms) >= LED_OFF_PERIOD_MS){ 
                state = LED_STATE_ON;
                last_toggle_ms += LED_OFF_PERIOD_MS;
                bsp_led_on();
            }
            break; 
        }

        case LED_STATE_ON: {
            if((uint32_t)(now_ms - last_toggle_ms) >= LED_ON_PERIOD_MS){
                state = LED_STATE_OFF;
                last_toggle_ms += LED_ON_PERIOD_MS;
                bsp_led_off();
            }
            break;
        }
    }
}


