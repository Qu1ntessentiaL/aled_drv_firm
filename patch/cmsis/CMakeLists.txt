cmake_minimum_required(VERSION 3.15)

add_definitions(-DSTM32F103xB)

add_library(cmsis STATIC
        Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
)

target_include_directories(cmsis PUBLIC
        Core/Include
        Device/ST/STM32F1xx/Include
)
