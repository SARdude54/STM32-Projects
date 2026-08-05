#include "bsp_led.h"
#include "gpio.h"


#define LED_PORT GPIOA
#define LED_PIN 5u


void bsp_led_init(void){
    gpio_pin_init(
        LED_PORT,
        LED_PIN,
        GPIO_MODE_OUTPUT,
        GPIO_OUTPUT_PUSH_PULL,
        GPIO_SPEED_LOW,
        GPIO_PULL_NONE);
}

void bsp_led_on(void){
    gpio_pin_write(LED_PORT, LED_PIN, 1u);
}

void bsp_led_off(void){
    gpio_pin_write(LED_PORT, LED_PIN, 0u);
}

void bsp_led_toggle(void){
   gpio_pin_toggle(LED_PORT, LED_PIN);
}


