#!/bin/sh

if [ "$#" -ne 2 ] || { [ "$1" != "compile" ] && [ "$1" != "run" ] && [ "$1" != "build" ]; }; then
    echo "Must use arguments 'compile', 'run' or 'build' followed by 'FILENAME'"
    exit 1
fi

FILENAME=$2
if [ "${FILENAME##*.}" = "c" ]; then
    BASENAME="${FILENAME%.c}"
else
    BASENAME="$FILENAME"
fi

if [ ! -e "${BASENAME}.c" ]; then
    echo "Unable to locate '${BASENAME}.c'"
    exit 1
fi

if [ "$1" = "compile" ]; then
    gcc "${BASENAME}.c" -Wall -Wextra -pedantic -std=c99 -Wvla -Wfloat-equal -g3 -fsanitize=address -fsanitize=undefined -o "$BASENAME"
    if [ $? -eq 0 ]; then
        echo "'$2' was compiled successfully"
    else
        echo "Compilation Failed"
    fi
fi

if [ "$1" = "run" ]; then
    if [ -e "${BASENAME}" ]; then
        ./"$BASENAME"
        exit 0
    else
        echo "Unable to locate '$BASENAME'"
        exit 1
    fi
fi

if [ "$1" = "build" ]; then
    gcc "${BASENAME}.c" -Wall -Wextra -pedantic -std=c99 -Wvla -Wfloat-equal -g3 -fsanitize=address -fsanitize=undefined -o "$BASENAME" && ./"$BASENAME"
    if [ $? -ne 0 ]; then
        echo "Compilation Failed"
        exit 1
    fi
fi
exit 0
