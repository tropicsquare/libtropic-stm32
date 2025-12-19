#!/usr/bin/env bash

BINARY_PATH="$1"
STLINK_SERIAL_NUMBER="$2"

if [ -z "$BINARY_PATH" ]; then
    echo "libtropic f439zi script for flashing and running tests"
    echo "usage: ./run_test.sh PATH_TO_BINARY [STLINK_SERIAL_NUMBER]"
    exit 1
fi

if [ -z "$STLINK_SERIAL_NUMBER" ]; then
    echo "No STLink serial number provided, trying to autodiscover STLink UART..."
    DEV=$(find /dev/serial/by-id/ -type l -iname "usb-STMicroelectronics_STM32_STLink_*" | head -n1)

    if [ $? -ne 0 ]; then
        echo "No STLinks discovered. Please provide path manually."
        exit 1
    fi

    echo "Using autodiscovered STLink UART: $DEV"
else
    echo "Using STLink serial number: $STLINK_SERIAL_NUMBER"
    echo "Looking for corresponding UART device..."

    DEV=$(find /dev/serial/by-id/ -type l -iname "*${STLINK_SERIAL_NUMBER}*" | head -n1)

    if ! ls "$DEV"; then
        echo "Cannot open UART of ST-Link with serial number $STLINK_SERIAL_NUMBER, terminating."
        exit 1
    fi

    echo "Using UART device: $DEV"
fi

BAUD="115200"
SENTINEL_OK="TEST FINISHED"
SENTINEL_FAIL_1="ASSERT FAIL"
SENTINEL_FAIL_2="WARNING"
SENTINEL_FAIL_3="ERROR"

set -euo pipefail

# Configure serial port
stty -F "$DEV" "$BAUD" \
  cs8 -cstopb -parenb \
  -ixon -ixoff -crtscts \
  -icanon -echo -echoe -echok -echoctl -echoke \
  -icrnl -inlcr -igncr -opost min 1 time 0

serial_reader() {
    GOT_ERROR=0
    exec 3<"$DEV"
    while IFS= read -t 60 -r -u 3 line; do
        printf 'STM32: %s\n' "$line"

        if [[ "$line" == *"$SENTINEL_FAIL_1"* ]] \
        || [[ "$line" == *"$SENTINEL_FAIL_2"* ]] \
        || [[ "$line" == *"$SENTINEL_FAIL_3"* ]]; then
            GOT_ERROR=1
        elif [[ "$line" == *"$SENTINEL_OK"* ]]; then
            return $GOT_ERROR
        fi
    done
    return 2 # Timeout or serial read error
}

# Start serial reading in background
serial_reader &
READER_PID=$!

# Ensure the background serial reader is killed on script termination
cleanup() {
    if [ -n "${READER_PID:-}" ]; then
        if kill -0 "$READER_PID" 2>/dev/null; then
            kill "$READER_PID" 2>/dev/null || true
            wait "$READER_PID" 2>/dev/null || true
        fi
    fi
}

# On Ctrl+C or TERM, kill the reader and exit with 130. Always run cleanup on EXIT.
trap 'cleanup; exit 130' INT TERM
trap 'cleanup' EXIT

# ---- Flash the device ----
if [ -z "$STLINK_SERIAL_NUMBER" ]; then
    echo "OpenOCD will autodiscover STLink programming interface."
    openocd -f board/stm32f429discovery.cfg -c "program $BINARY_PATH verify reset exit"
else
    OPENOCD_SERIAL_NUMBER_ARG=
    echo "OpenOCD will use STLink serial number $STLINK_SERIAL_NUMBER for programming."
    openocd -f board/stm32f429discovery.cfg -c "adapter serial $STLINK_SERIAL_NUMBER" -c "program $BINARY_PATH verify reset exit"
fi

# ---- Wait for serial reader to finish ----
set +e
wait $READER_PID
EXIT_CODE=$?
set -e

if [ $EXIT_CODE -eq 0 ]; then
    echo "Test finished successfully."
elif [ $EXIT_CODE -eq 1 ]; then
    echo "Test failed."
else
    echo "Serial read error or timeout."
fi

exit $EXIT_CODE
