#include "bsp_button.h"
#include "gpio.h"
#include "timebase.h"

#define BUTTON_PORT GPIOC
#define BUTTON_PIN 13u
#define BUTTON_DEBOUNCE_MS 20u

typedef enum
{
    BUTTON_DEBOUNCE_IDLE,
    BUTTON_DEBOUNCE_WAIT
} button_debounce_state_t;

static volatile uint32_t button_irq_event;
static uint32_t button_pressed_event;

static button_debounce_state_t debounce_state;
static uint32_t debounce_start_ms;

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
        button_irq_event = 1u;
    
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

    
    button_irq_event = 0u;
    button_pressed_event = 0u;
    debounce_state = BUTTON_DEBOUNCE_IDLE;
    debounce_start_ms = 0u;

    bsp_button_exti_init();
}

void bsp_button_update(void){

    const uint32_t now_ms = timebase_get_ms();

    switch (debounce_state)
    {
    case BUTTON_DEBOUNCE_IDLE:
        
        if(button_irq_event != 0u){
        
            button_irq_event = 0u;
            debounce_start_ms = now_ms;
            debounce_state = BUTTON_DEBOUNCE_WAIT;
        }    

        break;

    case BUTTON_DEBOUNCE_WAIT:
        
        if((uint32_t)(now_ms - debounce_start_ms) >= BUTTON_DEBOUNCE_MS){
            if(bsp_button_read() == 0u){
                button_pressed_event = 1u;
            }
            button_irq_event = 0u;
            debounce_state = BUTTON_DEBOUNCE_IDLE;
        }

        break;
    
    default: 

        button_irq_event = 0u;
        button_pressed_event = 0u;
        debounce_start_ms = now_ms;
        debounce_state = BUTTON_DEBOUNCE_IDLE;    

        break;
    }
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
