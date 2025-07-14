if [ -z $1 ]; then
    echo "libtropic f439zi flash script"
    echo "usage: ./flash.sh PATH_TO_BINARY"
    exit 1
fi

openocd -f board/stm32f429discovery.cfg -c "program $1 verify reset exit"