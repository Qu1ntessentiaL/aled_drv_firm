#include "main.h"

extern IWDG_HandleTypeDef hiwdg;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1, huart2;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;
extern ADC_HandleTypeDef hadc1;
extern RTC_HandleTypeDef hrtc;

DateTimeStruct sDateTime;

#define UART_BUFFER_SIZE 10
uint8_t uartBuffer[UART_BUFFER_SIZE];
uint8_t uartRxData;
uint16_t bufferIndex = 0;

#define UART_QUEUE_SIZE 10

// Очередь для хранения данных
QueueHandle_t uartQueue;

typedef enum {
    FLAG_NONE = 0x00,
    FLAG_DT_SET = 0x01,
    FLAG_YEAR_SET = 0x02,
    FLAG_MONTH_SET = 0x04,
    FLAG_DAY_SET = 0x08,
} PermissionFlags;

uint16_t permissions = FLAG_NONE;

void vMainTask(void *pvParameters);

void Buttons_task(void *pvParameters);

void vDynamicTask(void *pvParameters);

int main() {
    __disable_irq();
    HAL_Init();
    SystemClock_Config();
    UART1_Init();
    Button_PreInit();
    BUTTON_Init();
    MX_RTC_Init();
    TIM1_PWM_Init();
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __enable_irq();
    HAL_UART_Transmit_IT(&huart1, (uint8_t *) ("STM32F103CBT6 ready!\n\r"), strlen("STM32F103CBT6 ready!\n\r"));
    // Создание очереди
    uartQueue = xQueueCreate(UART_QUEUE_SIZE, sizeof(uint8_t));
    xTaskCreate(vMainTask, "Main task", 128, NULL, 3, NULL);
    xTaskCreate(Buttons_task, "Buttons task", 128, NULL, 3, NULL);
    vTaskStartScheduler();
}

void vMainTask(void *pvParameters) {
    TaskHandle_t xDynamicTaskHandle = NULL;
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1) {
        if (permissions & FLAG_DT_SET) {
            xTaskCreate(vDynamicTask,
                        "DynamicTask",
                        128,
                        NULL,
                        2,
                        &xDynamicTaskHandle);
            if (xDynamicTaskHandle != NULL) {
                permissions &= ~FLAG_DT_SET;
                GPIOB->BSRR |= GPIO_BSRR_BR10;
            } else {
                GPIOB->BSRR |= GPIO_BSRR_BR1;
            }
        }

        GPIOB->ODR ^= GPIO_ODR_ODR0;
        DateTime_Get(&sDateTime);
        vTaskDelayUntil(&xLastWakeTime, 1000);
    }
    //vTaskDelete(NULL);
}

void Buttons_task(void *pvParameters) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    char buff_tx[64] = {0,};
    Button_PreInit();
    while (1) {
        BUTTON_TimerProcess();
        BUTTON_Process();
        if (BUTTON_GetAction(REC) == BUTTON_SHORT_PRESS) {
            snprintf(buff_tx, 63, "Time %d:%d:%d\n\rDate %d-%d-%d\n\r", sDateTime.Hours, sDateTime.Minutes,
                     sDateTime.Seconds,
                     sDateTime.Date, sDateTime.Month, sDateTime.Year);
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *) buff_tx, strlen(buff_tx));
        }
        if (BUTTON_GetAction(PHOTO) == BUTTON_SHORT_PRESS) {
            const char buff_temp[] = "Запущен процесс установки времени!\n\r";
            HAL_UART_Transmit_IT(&huart1, (uint8_t *) buff_temp,
                                 strlen(buff_temp));
            permissions |= FLAG_DT_SET;
        }
        if (BUTTON_GetAction(UP) == BUTTON_SHORT_PRESS) {

        }
        if (BUTTON_GetAction(LEFT) == BUTTON_SHORT_PRESS) {

        }
        if (BUTTON_GetAction(MENU) == BUTTON_SHORT_PRESS) {
            uint32_t cnt_local = ((uint32_t) (RTC->CNTH << 16)) + (uint32_t) RTC->CNTL;
            char buffer[12];
            sprintf(buffer, "%u\n\r", cnt_local);
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *) buffer, strlen(buffer));
        }
        if (BUTTON_GetAction(RIGHT) == BUTTON_SHORT_PRESS) {
            DateTime_Set(2024, 8, 9, 10, 0, 0);
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *) "DateTime set!\n\r", sizeof("DateTime set!\n\r"));
        }
        if (BUTTON_GetAction(DOWN) == BUTTON_SHORT_PRESS) {

        }
        if (BUTTON_GetAction(SCENE) == BUTTON_SHORT_PRESS) {

        }
        if (BUTTON_GetAction(AWB) == BUTTON_SHORT_PRESS) {

        }
        BUTTON_ResetActions();
        vTaskDelayUntil(&xLastWakeTime, 1);
    }
    //vTaskDelete(NULL);
}

void vDynamicTask(void *pvParameters) {
    char buff_tx[] = "Введите год:\n\r";
    HAL_UART_Transmit_IT(&huart1, (uint8_t *) buff_tx, strlen(buff_tx));
    HAL_UART_Receive_IT();
    while (1) {}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // Отправка полученного байта в очередь
        xQueueSendFromISR(uartQueue, &uartRxData, NULL);

        // Начинаем приём следующего байта
        HAL_UART_Receive_IT(&huart1, &uartRxData, 1);
    }
}