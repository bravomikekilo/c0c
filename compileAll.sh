#! /usr/bin/env bash

for TestFile in $2/*.c0
do
    echo "compile ${TestFile}"
    $1 $TestFile >$3/${TestFile##*/}.asm
done