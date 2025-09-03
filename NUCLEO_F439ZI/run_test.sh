#!/usr/bin/env bash

if [ -z $1 ]; then
    echo "libtropic f439zi script for flashing and running tests"
    echo "usage: ./run_test.sh PATH_TO_BINARY [PATH_TO_STLINK_UART]"
    exit 1
fi

if [ -z $2 ]; then
    echo "No UART selected, trying to autodiscover STLink UART..."
    STLINKS=$(ls /dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_*)

    if [ $? -ne 0 ]; then
        echo "No STLinks discovered. Please provide path manually."
        exit 1
    fi

    DEV=$(echo $STLINKS | head -n 1)
    echo "Using autodiscovered STLink UART: $DEV"
else

    if ! ls "$2"; then
        echo "Cannot open UART at $2, terminating."
        exit 1
    fi

    DEV="$2"
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
    FAILED=0
    exec 3<"$DEV"
    while IFS= read -r -u 3 line; do
        line="${line%$'\r'}"  # strip trailing CR
        printf '%s\n' "$line"

        if [[ "$line" == *"$SENTINEL_FAIL_1"* ]] \
        || [[ "$line" == *"$SENTINEL_FAIL_2"* ]] \
        || [[ "$line" == *"$SENTINEL_FAIL_3"* ]]; then
            FAILED=1
        elif [[ "$line" == *"$SENTINEL_OK"* ]]; then
            return $FAILED
        fi
    done
    return $FAILED
}

# Start serial reading in background
serial_reader &
READER_PID=$!

# ---- Flash the device ----
openocd -f board/stm32f429discovery.cfg -c "program $1 verify reset exit"

# ---- Wait for serial reader to finish ----
wait $READER_PID
EXIT_CODE=$?

echo "Serial reader exited with code $EXIT_CODE."
exit $EXIT_CODE
