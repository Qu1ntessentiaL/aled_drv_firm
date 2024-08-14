#include "main.h"

extern IWDG_HandleTypeDef hiwdg;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1, huart2;
extern TIM_HandleTypeDef htim1, htim2, htim3, htim4;
extern ADC_HandleTypeDef hadc1;
extern RTC_HandleTypeDef hrtc;

DateTimeStruct sDateTime, sDateTime_tmp;

#define UART_BUFFER_SIZE 10
uint8_t uartBuffer[UART_BUFFER_SIZE];
uint8_t uartRxData;
uint16_t bufferIndex = 0;

#define UART_QUEUE_SIZE 10

// Очередь для хранения данных
QueueHandle_t uartQueue;

typedef enum {
    FLAG_NONE = 0x00,
    FLAG_YEAR_SET = 0x01,
    FLAG_MONTH_SET = 0x02,
    FLAG_DAY_SET = 0x04,
    FLAG_HOUR_SET = 0x08,
    FLAG_MINUTE_SET = 0x10,
    FLAG_SECOND_SET = 0x20,
    FLAG_DT_CMPLT = 0x40,
} PermissionFlags;

uint16_t permissions = FLAG_NONE;

void vMainTask(void *pvParameters);

void Buttons_task(void *pvParameters);

void UART_Task(void *pvParameters);

void handleError(void);

int main() {
    __disable_irq();
    HAL_Init();
    SystemClock_Config();
    UART1_Init();
    Button_PreInit();
    BUTTON_Init();
    I2C1_Init();
    if (AT24xx_Init() == true)
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *) ("AT24C32 ready!\n\r"), strlen("AT24C32 ready!\n\r"));
    else
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *) ("Error!\n\r"), strlen("Error!\n\r"));
    MX_RTC_Init();
    TIM1_PWM_Init();
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __enable_irq();
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *) ("STM32F103CBT6 ready!\n\r"), strlen("STM32F103CBT6 ready!\n\r"));
    // Создание очереди
    uartQueue = xQueueCreate(UART_QUEUE_SIZE, sizeof(uint8_t));

    if (uartQueue == NULL) {
        handleError();
        while (1);
    }

    xTaskCreate(vMainTask, "Main task", 128, NULL, 3, NULL);
    xTaskCreate(Buttons_task, "Buttons task", 128, NULL, 3, NULL);
    xTaskCreate(UART_Task, "UART task", 128, NULL, 3, NULL);
    vTaskStartScheduler();
}

void vMainTask(void *pvParameters) {
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1) {
        if (permissions & FLAG_YEAR_SET) {
            HAL_UART_Receive_IT(&huart1, &uartRxData, 1);
        } else if (permissions & FLAG_DT_CMPLT) {
            DateTime_Set(&sDateTime);
            permissions &= ~FLAG_DT_CMPLT;
            permissions |= FLAG_NONE;
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
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *) buff_temp,
                                  strlen(buff_temp));
            permissions |= FLAG_YEAR_SET;
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
            sDateTime_tmp.Year = 2024;
            sDateTime_tmp.Month = 8;
            sDateTime_tmp.Date = 13;
            sDateTime_tmp.Hours = 15;
            sDateTime_tmp.Minutes = 52;
            sDateTime_tmp.Seconds = 0;
            DateTime_Set(&sDateTime_tmp);
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // Отправка полученного байта в очередь
        xQueueSendFromISR(uartQueue, &uartRxData, NULL);

        // Начинаем приём следующего байта
        HAL_UART_Receive_IT(&huart1, &uartRxData, 1);
    }
}