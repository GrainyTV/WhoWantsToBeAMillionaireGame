#!/bin/sh

INPUT_FILE="supdata.log"
OUTPUT_FILE="valgrind.supp"

grep -Pzo '{\n([a-zA-Z0-9:<>*.\-_/ ]+\n)+}' "$INPUT_FILE" | awk 'BEGIN { RS="\0"; FS="\n"; }
{
    count[$0]++
}
END {
    for (key in count) {
        if (count[key] == 1) {
            print key
        }
    }
}' > "$OUTPUT_FILE"
