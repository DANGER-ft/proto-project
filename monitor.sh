#!/bin/bash
#Онли линукс, для загрузки. load - вызывает uploadfs и открывает терминал обратно
while true; do
    script -qfc "pio run --target monitor" /dev/null &
    PID=$!
    while read -r line; do
        [ "$line" = "load" ] && break
    done
    kill -INT -$PID 2>/dev/null || kill -INT $PID 2>/dev/null
    sleep 0.5
    kill -9 -$PID 2>/dev/null || kill -9 $PID 2>/dev/null
    wait $PID 2>/dev/null
    pio run --target uploadfs
done
