#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"

#define GPIO_BUTTON_NOT_PRESSED                              (GPIO_PIN_SET)

#define BUTTONS_LONG_PRESS_MS                                500
#define BUTTONS_VERY_LONG_PRESS_MS                           1500

typedef enum {
    REC,
    PHOTO,
    UP,
    LEFT,
    MENU,
    RIGHT,
    DOWN,
    SCENE,
    AWB,
    BUTTONS_NUM
} ButtonID;

#define DEBOUNCE_TIME_MS    100
#define GPIO_BUTTON_PRESSED (!GPIO_BUTTON_NOT_PRESSED)

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} McuPin;

typedef enum {
    BUTTON_STARTING = 0,
    BUTTON_NOT_PRESSED = 1,
    BUTTON_WAIT_DEBOUNCE = 2,
    BUTTON_PRESSED = 3,
} ButtonState;

typedef enum {
    BUTTON_NONE = 0,
    BUTTON_SHORT_PRESS = 1,
    BUTTON_LONG_PRESS = 2,
    BUTTON_VERY_LONG_PRESS = 3,
} ButtonAction;

void BUTTON_Process();

void BUTTON_TimerProcess();

ButtonAction BUTTON_GetAction(uint8_t index);

void BUTTON_ResetActions();

void BUTTON_Init();

void Button_PreInit();

#ifdef __cplusplus
}
#endif

#endif //BUTTON_H
