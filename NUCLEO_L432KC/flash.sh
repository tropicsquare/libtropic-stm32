if [ -z $1 ]; then
    echo "libtropic l432kc flash script"
    echo "usage: ./flash.sh PATH_TO_BINARY"
    exit 1
fi

openocd -f board/st_nucleo_l4.cfg -c "program $1 verify reset exit"