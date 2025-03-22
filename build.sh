#!/bin/bash
if [ "$1" == "debug" ]; then
    qmake CONFIG+=debug
else
    qmake CONFIG+=release
fi
make
