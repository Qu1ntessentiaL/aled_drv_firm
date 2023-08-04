cmake_minimum_required(VERSION 3.16)
project(aled_drv_firm LANGUAGES C CXX ASM)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_VERBOSE_MAKEFILE OFF CACHE BOOL "" FORCE)

set(STARTUP_FILE_NAME startup_stm32f103xb)
set(STARTUP_LOCATION "${CMAKE_SOURCE_DIR}/Startup/${STARTUP_FILE_NAME}.s")
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/stm32f103xb_flash.icf")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(EW_ROOT_DIR "C:/Program Files/IAR Systems/Embedded Workbench 9.1/arm")
set(CMAKE_ASM_COMPILER "${EW_ROOT_DIR}/bin/iasmarm.exe")

set(COMMON_COMPILER_FLAGS "--cpu=Cortex-M3 --fpu=None --dlib_config normal --no_cse --no_unroll --no_code_motion --no_tbaa --no_clustering --no_scheduling --endian=little -e")
set(CMAKE_C_FLAGS "${COMMON_COMPILER_FLAGS} ")
set(CMAKE_CXX_FLAGS "${COMMON_COMPILER_FLAGS} --c++ --no_exceptions --no_rtti")
set(CMAKE_ASM_FLAGS "--cpu=Cortex-M3 --fpu=None -s+ -r -t8")

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

include_directories(
        Drivers/CMSIS/Core/Include/
        Drivers/CMSIS/Device/ST/STM32F1xx/Include/
        Drivers/aarch32/
        Drivers/STM32F1xx_HAL_Driver/Inc/
        Src/
        Src/ARGB/
        Src/ssd1306/
        Src/button
)

add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB)

file(GLOB_RECURSE SOURCES
        Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
        Drivers/STM32F1xx_HAL_Driver/Src/*.c
        Src/main.c
        Src/stm32f1xx_it.c
        Src/ssd1306/ssd1306.c
        Src/ssd1306/fonts.c
        Src/ssd1306/images.c
        Src/system.c
        Src/ARGB/ARGB.c
        Src/button/button.c
        )

set(COMMON_LINKER_FLAGS "--semihosting --config ${LINKER_SCRIPT}")
set(CMAKE_C_LINK_FLAGS "${COMMON_LINKER_FLAGS}")
set(CMAKE_CXX_LINK_FLAGS "${COMMON_LINKER_FLAGS}")

add_executable(${PROJECT_NAME}
        ${STARTUP_LOCATION}
        ${SOURCES}
        ${LINKER_SCRIPT}
        )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
        ${CMAKE_IAR_ELFTOOL} --silent --ihex $<TARGET_FILE:${PROJECT_NAME}> $<IF:$<BOOL:$<TARGET_PROPERTY:${PROJECT_NAME},OUTPUT_NAME>>,$<TARGET_PROPERTY:${PROJECT_NAME},OUTPUT_NAME>,$<TARGET_PROPERTY:${PROJECT_NAME},NAME>>.hex)
