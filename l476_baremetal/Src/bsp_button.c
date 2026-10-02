#include "bsp_button.h"
#include "gpio.h"

#define BUTTON_PORT GPIOC
#define BUTTON_PIN 13u

void bsp_button_init(void){
    gpio_pin_init(
        BUTTON_PORT,
        BUTTON_PIN,
        GPIO_MODE_INPUT,
        GPIO_OUTPUT_PUSH_PULL,
        GPIO_SPEED_LOW,
        GPIO_PULL_NONE);
}

uint32_t bsp_button_read(void){
    return gpio_pin_read(BUTTON_PORT, BUTTON_PIN);
}
