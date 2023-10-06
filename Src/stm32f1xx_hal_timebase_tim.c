#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"

extern TIM_HandleTypeDef htim2;

void TIM2_IRQHandler(void);

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock, uwAPB1Prescaler = 0U;

    uint32_t uwPrescalerValue = 0U;
    uint32_t pFLatency;
    HAL_StatusTypeDef status = HAL_OK;

    __HAL_RCC_TIM2_CLK_ENABLE();

    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    uwAPB1Prescaler = clkconfig.APB1CLKDivider;

    if (uwAPB1Prescaler == RCC_HCLK_DIV1) {
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    } else {
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
    }

    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

    htim2.Instance = TIM2;
    htim2.Init.Period = (1000000U / 1000U) - 1U;
    htim2.Init.Prescaler = uwPrescalerValue;
    htim2.Init.ClockDivision = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    status = HAL_TIM_Base_Init(&htim2);
    if (status == HAL_OK) {
        status = HAL_TIM_Base_Start_IT(&htim2);
        if (status == HAL_OK) {
            HAL_NVIC_EnableIRQ(TIM2_IRQn);
            if (TickPriority < (1UL << __NVIC_PRIO_BITS)) {
                HAL_NVIC_SetPriority(TIM2_IRQn, TickPriority, 0U);
                uwTickPrio = TickPriority;
            } else {
                status = HAL_ERROR;
            }
        }
    }

    return status;
}

void HAL_SuspendTick(void) {
    __HAL_TIM_DISABLE_IT(&htim2, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void) {
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
}

