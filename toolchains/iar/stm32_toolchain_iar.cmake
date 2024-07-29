# Установка компиляторов
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(STARTUP_FILE_NAME startup_stm32f103xb)
set(STARTUP_LOCATION "${CMAKE_SOURCE_DIR}/toolchains/iar/${STARTUP_FILE_NAME}.s")
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/toolchains/iar/stm32f103xb_flash.icf")

set(EW_ROOT_DIR "C:/Program Files/IAR Systems/Embedded Workbench 9.1/arm")
set(CMAKE_ASM_COMPILER "${EW_ROOT_DIR}/bin/iasmarm.exe")
set(CMAKE_С_COMPILER "${EW_ROOT_DIR}/bin/iссarm.exe")

set(COMMON_COMPILER_FLAGS "--cpu=Cortex-M3 --fpu=None --dlib_config normal --no_cse --no_unroll --no_code_motion --no_tbaa --no_clustering --no_scheduling --endian=little -e")
set(CMAKE_C_FLAGS "${COMMON_COMPILER_FLAGS} ")
set(CMAKE_CXX_FLAGS "${COMMON_COMPILER_FLAGS} --c++ --no_exceptions --no_rtti")
set(CMAKE_ASM_FLAGS "--cpu=Cortex-M3 --fpu=None -s+ -r -t8")

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(COMMON_LINKER_FLAGS "--semihosting --config ${LINKER_SCRIPT}")
set(CMAKE_C_LINK_FLAGS "${COMMON_LINKER_FLAGS}")
set(CMAKE_CXX_LINK_FLAGS "${COMMON_LINKER_FLAGS}")