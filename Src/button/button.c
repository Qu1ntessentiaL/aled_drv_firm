#include "button.h"

//REC = PA5
//PHOTO = PA4
//UP = PA0
//LEFT = PA3
//MENU = PA1
//RIGHT = PC13
//DOWN = PA2
//SCENE = PA6
//AWB = PA7
//D2 = PB1
//D3 = PB10
//D5 = PB0
//IR_IC = PB11

static McuPin buttons[BUTTONS_NUM] = {{GPIOA, GPIO_PIN_5},   //REC
                                      {GPIOA, GPIO_PIN_4},   //PHOTO
                                      {GPIOA, GPIO_PIN_0},    //UP
                                      {GPIOA, GPIO_PIN_3},    //LEFT
                                      {GPIOA, GPIO_PIN_1},    //MENU
                                      {GPIOC, GPIO_PIN_13},    //RIGHT
                                      {GPIOA, GPIO_PIN_2},    //DOWN
                                      {GPIOA, GPIO_PIN_6},    //SCENE
                                      {GPIOA, GPIO_PIN_7}};   //AWB

static uint16_t debounceCounter[BUTTONS_NUM];
static uint8_t waitButtonRelease[BUTTONS_NUM];
static uint16_t buttonPressCounter[BUTTONS_NUM];

static ButtonAction buttonActions[BUTTONS_NUM];
static ButtonState buttonState[BUTTONS_NUM];

void BUTTON_LowLevelManager() {
    uint8_t currentStates[BUTTONS_NUM];

    for (uint8_t i = 0; i < BUTTONS_NUM; i++) {
        currentStates[i] = HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin);

        switch (buttonState[i]) {
            case BUTTON_STARTING:
                if (currentStates[i] == GPIO_BUTTON_NOT_PRESSED) {
                    buttonState[i] = BUTTON_NOT_PRESSED;
                }
                break;

            case BUTTON_NOT_PRESSED:
                if (currentStates[i] == GPIO_BUTTON_PRESSED) {
                    buttonState[i] = BUTTON_WAIT_DEBOUNCE;
                    debounceCounter[i] = 0;
                }
                break;

            case BUTTON_WAIT_DEBOUNCE:
                if (debounceCounter[i] == DEBOUNCE_TIME_MS) {
                    if (currentStates[i] == GPIO_BUTTON_PRESSED) {
                        buttonState[i] = BUTTON_PRESSED;
                    } else {
                        buttonState[i] = BUTTON_NOT_PRESSED;
                    }
                }
                break;

            case BUTTON_PRESSED:
                if (currentStates[i] == GPIO_BUTTON_NOT_PRESSED) {
                    buttonState[i] = BUTTON_WAIT_DEBOUNCE;
                    debounceCounter[i] = 0;
                }
                break;

            default:
                break;
        }
    }
}

void BUTTON_HighLevelManager() {
    for (uint8_t i = 0; i < BUTTONS_NUM; i++) {
        if (buttonActions[i] == BUTTON_NONE) {
            if (waitButtonRelease[i] == 0) {
                if (buttonState[i] == BUTTON_PRESSED) {
                    waitButtonRelease[i] = 1;
                }
            } else {
                if (buttonState[i] == BUTTON_NOT_PRESSED) {
                    waitButtonRelease[i] = 0;

                    if (buttonPressCounter[i] >= BUTTONS_VERY_LONG_PRESS_MS) {
                        buttonActions[i] = BUTTON_VERY_LONG_PRESS;
                    } else {
                        if (buttonPressCounter[i] >= BUTTONS_LONG_PRESS_MS) {
                            buttonActions[i] = BUTTON_LONG_PRESS;
                        } else {
                            buttonActions[i] = BUTTON_SHORT_PRESS;
                        }
                    }
                }
            }
        }
    }
}

void BUTTON_Process() {
    BUTTON_LowLevelManager();
    BUTTON_HighLevelManager();
}

void BUTTON_TimerProcess() {
    for (uint8_t i = 0; i < BUTTONS_NUM; i++) {
        if (debounceCounter[i] < DEBOUNCE_TIME_MS) {
            debounceCounter[i]++;
        }

        if (waitButtonRelease[i] == 1) {
            buttonPressCounter[i]++;
        } else {
            buttonPressCounter[i] = 0;
        }
    }
}

ButtonAction BUTTON_GetAction(uint8_t index) {
    return buttonActions[index];
}

void BUTTON_ResetActions() {
    for (uint8_t i = 0; i < BUTTONS_NUM; i++) {
        buttonActions[i] = BUTTON_NONE;
    }
}

void BUTTON_Init() {
    for (uint8_t i = 0; i < BUTTONS_NUM; i++) {
        debounceCounter[i] = 0;
        waitButtonRelease[i] = 0;
        buttonPressCounter[i] = 0;
        buttonActions[i] = BUTTON_NONE;
        buttonState[i] = BUTTON_STARTING;
    }
}

void Button_PreInit() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                          GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIOB->BSRR |= GPIO_BSRR_BS0 | GPIO_BSRR_BS1 | GPIO_BSRR_BS10;

    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}