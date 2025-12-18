if [ -z $1 ]; then
    echo "Libtropic NUCLEO L432KC flash script"
    echo "usage: ./flash.sh PATH_TO_BINARY"
    exit 1
fi

openocd -f target/stm32l4x.cfg -c "program $1 verify reset exit"