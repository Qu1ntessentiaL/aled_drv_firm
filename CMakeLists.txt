cmake_minimum_required(VERSION 3.15)

set(CMAKE_VERBOSE_MAKEFILE OFF CACHE BOOL "" FORCE)

project(aled_drv_firm LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(toolchains/iar/stm32_toolchain_iar.cmake)

add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB)

add_subdirectory(Drivers/STM32CubeF1)

include_directories(
        Src
        Src/ARGB
        Src/ssd1306
        Src/button
        Src/ds18x20
        Src/at24cxx
        Src/RTC
)

file(GLOB_RECURSE SOURCES
        Src/main.c
        Src/stm32f1xx_it.c
        Src/stm32f1xx_hal_timebase_tim.c
        Src/ssd1306/ssd1306.c
        Src/ssd1306/fonts.c
        Src/ssd1306/images.c
        Src/system.c
        Src/ARGB/ARGB.c
        Src/button/button.c
        Src/ds18x20/ds18x20.c
        Src/ds18x20/OneWire.c
        Src/at24cxx/at24cxx.c
        Src/RTC/RTC.c
)

add_executable(${PROJECT_NAME}
        ${STARTUP_LOCATION}
        ${SOURCES}
        ${LINKER_SCRIPT}
)

# ------- Настройка сборки FreeRTOS -------

set(FREERTOS_KERNEL_PATH "Drivers/FreeRTOS-LTS/FreeRTOS/FreeRTOS-Kernel")

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config
        INTERFACE
        "Src"
)

if (DEFINED FREERTOS_SMP_EXAMPLE AND FREERTOS_SMP_EXAMPLE STREQUAL "1")
    message(STATUS "Build FreeRTOS SMP example")
    # Adding the following configurations to build SMP template port
    add_compile_options(-DconfigNUMBER_OF_CORES=2 -DconfigUSE_PASSIVE_IDLE_HOOK=0)
endif ()

set(FREERTOS_HEAP "1" CACHE STRING "" FORCE)

set(FREERTOS_PORT "IAR_ARM_CM3" CACHE STRING "" FORCE)

add_subdirectory(${FREERTOS_KERNEL_PATH} FreeRTOS-Kernel)

# -----------------------------------------

# stm32f1xx_hal_driver заменить на cmsis, если нет надобности в HAL
target_link_libraries(${PROJECT_NAME} PRIVATE stm32f1xx_hal_driver freertos_kernel freertos_config)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
        ${CMAKE_IAR_ELFTOOL} --silent --ihex $<TARGET_FILE:${PROJECT_NAME}> $<IF:$<BOOL:$<TARGET_PROPERTY:${PROJECT_NAME},OUTPUT_NAME>>,$<TARGET_PROPERTY:${PROJECT_NAME},OUTPUT_NAME>,$<TARGET_PROPERTY:${PROJECT_NAME},NAME>>.hex)
