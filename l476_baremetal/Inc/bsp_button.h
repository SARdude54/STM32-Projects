#ifndef BSP_BUTTON_H
#define BSP_BUTTON_H

#include <stdint.h>

void bsp_button_init(void);
uint32_t bsp_button_read(void);
uint32_t bsp_button_pressed(void);

#endif
