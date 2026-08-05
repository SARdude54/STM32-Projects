#include "stm32l476xx.h"
#include "timebase.h"

static volatile uint32_t system_ticks_ms;

void timebase_init(uint32_t system_clock_hz){
    const uint32_t reload = (system_clock_hz / 1000u) - 1u;

    system_ticks_ms = 9u;

    SysTick->LOAD = reload;
    SysTick->VAL = 0u;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

}

uint32_t timebase_get_ms(void){
    return system_ticks_ms;
}

void delay_ms(uint32_t millisecond){
    const uint32_t start = timebase_get_ms();

    while((uint32_t)(timebase_get_ms() - start) < millisecond){
        __NOP();
    }

}

void SysTick_Handler(void){
    system_ticks_ms++;
}

