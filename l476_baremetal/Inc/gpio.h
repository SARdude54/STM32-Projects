#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32l476xx.h"


typedef enum {
    GPIO_MODE_INPUT = 0u,
    GPIO_MODE_OUTPUT = 1u,
    GPIO_MODE_ALTERNATE = 2u,
    GPIO_MODE_ANALOG = 3u
} gpio_mode_t;

typedef enum {
    GPIO_OUTPUT_PUSH_PULL = 0u,
    GPIO_OUTPUT_OPEN_DRAIN = 1u
} gpio_output_type_t;

typedef enum {
    GPIO_SPEED_LOW = 0u,
    GPIO_SPEED_MEDIUM = 1u,
    GPIO_SPEED_HIGH = 2u,
    GPIO_SPEED_VERY_HIGH = 3u,
} gpio_speed_t;

typedef enum {
    GPIO_PULL_NONE = 0u,
    GPIO_PULL_UP = 1u,
    GPIO_PULL_DOWN = 2u
} gpio_pull_t;

void gpio_pin_init(GPIO_TypeDef *port, uint32_t pin, gpio_mode_t mode, gpio_output_type_t output_type, gpio_speed_t speed, gpio_pull_t pull);
void gpio_pin_write(GPIO_TypeDef *port, uint32_t pin, uint32_t state);
uint32_t gpio_pin_read(GPIO_TypeDef *port, uint32_t pin);
void gpio_pin_toggle(GPIO_TypeDef *port, uint32_t pin);

#endif