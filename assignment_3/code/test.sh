#!/bin/sh

RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"


echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
echo "> Testing all input files for assignment 3 >"
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"

echo ">> Clearing old test results"

rm -r diff/*
rm -r output/*

if [ ! -d diff ]; then
  mkdir -p diff;
fi

if [ ! -d output ]; then
  mkdir -p output;
fi

echo ">> Running Implicit First Programs\n"
for num in 1 2 3 4 5 6 7 8 9 10 11
do
    ./hsim -o output -l implicit -f first -i input/$num.in
done

echo "Implicit First Test Results"

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

echo ">> Running Implicit Best Programs\n"
for num in 1 2 3 4 5 6 7 8 9 10 11
do
    ./hsim -o output -l implicit -f best -i input/$num.in
done

echo "Implicit Best Results"

for num in 1 2 3 4 5 6 7 8 9 10 11
do
    diff -q output/$num.implicit.best.out examples/$num.implicit.best.out >> diff/$num.implicit.best.out
    if [ $? -eq 1 ]
        then
            echo "$num: ${RED}FAIL${ENDCOLOR}"
        else
            echo "$num: ${GREEN}PASS${ENDCOLOR}"
    fi
done

echo ">> Running Explicit First Programs\n"
for num in 1 2 3 4 5 6 7 8 9 10 11
do
    ./hsim -o output -l explicit -f first -i input/$num.in
done

echo "Explicit First Results"

for num in 1 2 3 4 5 6 7 8 9 10 11
do
    diff -q output/$num.explicit.first.out examples/$num.explicit.first.out >> diff/$num.explicit.first.out
    if [ $? -eq 1 ]
        then
            echo "$num: ${RED}FAIL${ENDCOLOR}"
        else
            echo "$num: ${GREEN}PASS${ENDCOLOR}"
    fi
done

echo ">> Running Explicit Best Fit Programs\n"
for num in 1 2 3 4 5 6 7 8 9 10 11
do
    ./hsim -o output -l explicit -f best -i input/$num.in
done

echo "Explicit Best Fit Results"

for num in 1 2 3 4 5 6 7 8 9 10 11
do
    diff -q output/$num.explicit.best.out examples/$num.explicit.best.out >> diff/$num.explicit.best.out
    if [ $? -eq 1 ]
        then
            echo "$num: ${RED}FAIL${ENDCOLOR}"
        else
            echo "$num: ${GREEN}PASS${ENDCOLOR}"
    fi
done