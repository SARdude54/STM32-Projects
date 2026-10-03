#include "bsp_button.h"
#include "bsp_led.h"
#include "gpio.h"

#define BUTTON_PORT GPIOC
#define BUTTON_PIN 13u

static void bsp_button_exti_init(void){
    // enable syscfg clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // route PC13 to EXTI13
    SYSCFG->EXTICR[3] &= ~(0xFu << 4u);
    SYSCFG->EXTICR[3] |= (0x2u << 4u);

    // unmask EXTI13
    EXTI->IMR1 |= (1u << 13);
    
    // disable rising edge for this line
    EXTI->RTSR1 &= ~(1u << 13);

    // enable falling edge
    EXTI->FTSR1 |= (1u << 13);
    
    // clear any state pending interrupt
    EXTI->PR1 = (1u << 13);

    // enable the grouped NVIC interrupt
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 5u);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void EXTI15_10_IRQHandler(void){
    if((EXTI->PR1 & (1u << 13)) != 0u){
        EXTI->PR1 = (1u<< 13);

        bsp_led_toggle();
    }
}

void bsp_button_init(void){
    gpio_pin_init(
        BUTTON_PORT,
        BUTTON_PIN,
        GPIO_MODE_INPUT,
        GPIO_OUTPUT_PUSH_PULL,
        GPIO_SPEED_LOW,
        GPIO_PULL_NONE);

    bsp_button_exti_init();
}

uint32_t bsp_button_read(void){
    return gpio_pin_read(BUTTON_PORT, BUTTON_PIN);
}
