#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"

rm -r diff/*
rm -r output/*

if [ ! -d diff ]; then
  mkdir -p diff;
fi

echo "*********************************"
echo "Running all test for assignment 3"
echo "*********************************\n"

for num in 1 2 3 4 5 6 7 8 9 10 11
do
    ./hsim -o output -l implicit -f first -i input/$num.in
done

echo "Implicit First"

for num in 1 2 3 4 5 6 7 8 9 10 11
do
    diff -q output/$num.implicit.first.out examples/$num.implicit.first.out >> diff/$num.implicit.first.out
    if [ $? -eq 1 ]
        then
            echo "$num: ${RED}FAIL${ENDCOLOR}"
        else
            echo "$num: ${GREEN}PASS${ENDCOLOR}"
    fi
done