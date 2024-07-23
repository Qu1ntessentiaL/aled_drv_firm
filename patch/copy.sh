#!/bin/bash

# Получение пути к директории скрипта
SCRIPT_DIR=$(dirname "$0")

# Определение исходных файлов относительно местоположения скрипта
FILES_TO_COPY=(
    "$SCRIPT_DIR/CMakeLists.txt"
    "$SCRIPT_DIR/cmsis/CMakeLists.txt"
    "$SCRIPT_DIR/hal_driver/CMakeLists.txt"
)

# Определение целевых директорий выше по уровню
TARGET_DIRS=(
    "$SCRIPT_DIR/../Drivers/STM32CubeF1"
    "$SCRIPT_DIR/../Drivers/STM32CubeF1/Drivers/CMSIS"
    "$SCRIPT_DIR/../Drivers/STM32CubeF1/Drivers/STM32F1xx_HAL_Driver"
)

# Проверка совпадения количества файлов и директорий
if [ ${#FILES_TO_COPY[@]} -ne ${#TARGET_DIRS[@]} ]; then
    echo "The number of files and target directories must match."
    exit 1
fi

# Копирование каждого файла в соответствующую директорию
for i in "${!FILES_TO_COPY[@]}"; do
    FILE=${FILES_TO_COPY[$i]}
    TARGET_DIR=${TARGET_DIRS[$i]}

    # Создание целевой директории, если она не существует
    mkdir -p "$TARGET_DIR"

    # Копирование файла
    if [ -f "$FILE" ]; then
        cp "$FILE" "$TARGET_DIR/"
        if [ $? -eq 0 ]; then
            echo "Copied $(basename $FILE) to $TARGET_DIR successfully."
        else
            echo "Failed to copy $(basename $FILE) to $TARGET_DIR."
        fi
    else
        echo "$FILE does not exist."
    fi
done