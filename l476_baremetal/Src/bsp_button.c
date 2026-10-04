#include "bsp_button.h"
#include "gpio.h"

#define BUTTON_PORT GPIOC
#define BUTTON_PIN 13u

static volatile uint32_t button_pressed_event;

static void bsp_button_exti_init(void){
    // enable syscfg clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // route PC13 to EXTI13
    SYSCFG->EXTICR[3] &= ~(0xFu << 4u);
    SYSCFG->EXTICR[3] |= (0x2u << 4u);

    // unmask EXTI13
    EXTI->IMR1 |= (1u << BUTTON_PIN);
    
    // disable rising edge for this line
    EXTI->RTSR1 &= ~(1u << BUTTON_PIN);

    // enable falling edge
    EXTI->FTSR1 |= (1u << BUTTON_PIN);
    
    // clear any state pending interrupt
    EXTI->PR1 = (1u << BUTTON_PIN);

    // enable the grouped NVIC interrupt
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 5u);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void EXTI15_10_IRQHandler(void){
    if((EXTI->PR1 & (1u << BUTTON_PIN)) != 0u){
        EXTI->PR1 = (1u << BUTTON_PIN);

        button_pressed_event = 1u;
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

    button_pressed_event = 0u;
    bsp_button_exti_init();
}

uint32_t bsp_button_read(void){
    return gpio_pin_read(BUTTON_PORT, BUTTON_PIN);
}

uint32_t bsp_button_pressed(void){
    if(button_pressed_event != 0u){
        button_pressed_event = 0u;
        return 1u;
    }
    return 0u;
}
