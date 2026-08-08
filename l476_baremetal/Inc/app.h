#ifndef APP_H
#define APP_H

// data types

typedef enum {
    LED_STATE_ON,
    LED_STATE_OFF
} led_state_t;

// function signatures
void app_init(void);
void app_update(void);

#endif
