# Проект для работы с STM32F103CBT6 (Bluepill) в среде разработки CLION
В проект включены как подмодули репозитории:
1) FreeRTOS
2) STM32CubeF1

В папке patch находятся скрипты copy.sh и clear.sh, которые копируют и удаляют необходимые файлы CMakeLists.txt
в нужные папки подмодуля STM32CubeF1;
В папке toolchains/iar находятся файлы, необходимые для компилятора IAR Embedded Workbench, в дальнейшем возможно
добавление поддержки компилятора Keil и arm-none-eabi-gcc;
Файл stm32f103xb.cfg необходим для OpenOCD, чтобы прошивать и отлаживать IC в CLion
Исходный пользовательский код помещен в папку Src
Проект является тестовым и требует доработок
