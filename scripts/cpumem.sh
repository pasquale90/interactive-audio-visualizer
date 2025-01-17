#/usr/bin/env bash

mbool=false

clear

while true
do

    pid=$(pgrep -x interactive-aud)

    if [ -n "${pid}" ];then
        clear
        break;
    else
        if ! $mbool ; then
            printf "waiting for IAV process to start"
            mbool=true
        else
            echo -ne "."
        fi
    fi

    sleep 2
done

printf '\nIAV resources usage:\n%%cpu |  %%mem  | process | CPU temperature\n'

while kill -0 $pid 2> /dev/null;
do
    resources=$(top -b -n 2 -d 0.2 -p $pid | tail -1 | awk '{print $9 "  |  " $10 "   | " $12 }')
    temperature=`sensors | awk '/temp1/{print $2}'`
    echo -ne "$resources  |    $temperature\033[0K\r"
    sleep 0.1
done
clear
exit
