#include "gpio.h"

static void gpio_enable_clock(GPIO_TypeDef *port)
{
    if (port == GPIOA)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    }
    else if (port == GPIOB)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    }
    else if (port == GPIOC)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    }
    else if (port == GPIOD)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    }
    else if (port == GPIOE)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
    }
    else if (port == GPIOF)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
    }
    else if (port == GPIOG)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN;
    }
    else if (port == GPIOH)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
    }
    else
    {
        return;
    }

    /*
     * Ensure the peripheral clock-enable write completes before
     * accessing the GPIO peripheral registers.
     */
    (void)RCC->AHB2ENR;
}
void gpio_pin_write(GPIO_TypeDef *port, uint32_t pin, uint32_t state){
    if(state != 0u){
        port->BSRR = (1u << pin);
    } else {
        port->BSRR = (1u << (pin + 16u));
    }
}

uint32_t gpio_pin_read(GPIO_TypeDef *port, uint32_t pin){
    return (port->IDR >> pin) & 1u;
}

void gpio_pin_toggle(GPIO_TypeDef *port, uint32_t pin)
{
    if (((port->ODR >> pin) & 1u) != 0u)
    {
        port->BSRR = (1u << (pin + 16u));
    }
    else
    {
        port->BSRR = (1u << pin);
    }
}

void gpio_pin_init(GPIO_TypeDef *port, uint32_t pin, gpio_mode_t mode, gpio_output_type_t output_type, gpio_speed_t speed, gpio_pull_t pull){
    const uint32_t shift = pin * 2u;
    
    // GPIOA is connectewd to the AHB2 peripheral bus
    // Enable its peripheral clock beffore accessing its registers
    gpio_enable_clock(port);

    /*
    GPIO mode register: two bits per pin
    00 = input
    01 = general-purpose output
    10 = alternative-function mode
    11 = analog mode
    
    PA5 occupies MODER bits [11:10]
    */

    port->MODER &= ~(3u << shift); // fuield mask
    port->MODER |= ((uint32_t)mode << shift); // new_value

    /*
    Output type:
    0 = push-pull
    1 = open-drain
    */
    port->OTYPER &= ~(1u << pin);
    port->OTYPER |= ((uint32_t)output_type << pin);

    /*
    Output speed:
    00 = low speed

    Low speed for user LED
    */  
    port->OSPEEDR &= ~(3u << shift);
    port->OSPEEDR |= ((uint32_t)speed << shift);

   /*
   Pull configuration
   00 = no pull-up or pull-down
   */
    port->PUPDR &= ~(3u << shift);
    port->PUPDR |= ((uint32_t)pull << shift);

}

void gpio_pin_set_alternate_function(GPIO_TypeDef *port, uint32_t pin, uint32_t alternate_function){
    const uint32_t afr_index = pin / 8u;
    const uint32_t afr_shift = (pin % 8u) * 4u;

    port->AFR[afr_index] &= ~(0xFu << afr_shift);
    port->AFR[afr_index] |= ((alternate_function & 0xFu) << afr_shift);

}
