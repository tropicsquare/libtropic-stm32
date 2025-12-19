#!/usr/bin/env bash

BINARY_PATH="$1"
STLINK_SERIAL_NUMBER="$2"

if [ -z "$BINARY_PATH" ]; then
    echo "libtropic f439zi flash script"
    echo "usage: ./flash.sh PATH_TO_BINARY [STLINK_SERIAL_NUMBER]"
    exit 1
fi

if [ -z "$STLINK_SERIAL_NUMBER" ]; then
    echo "No STLink serial number provided, OpenOCD will autodiscover STLink programming interface."
    openocd -f board/stm32f429discovery.cfg -c "program $1 verify reset exit"
else
    echo "Using STLink serial number: $STLINK_SERIAL_NUMBER"
    openocd -f board/stm32f429discovery.cfg -c "adapter serial $STLINK_SERIAL_NUMBER" -c "program $1 verify reset exit"
fi