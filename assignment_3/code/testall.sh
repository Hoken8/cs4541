for num in 1 2 3 4 5 6 7 8 9 10 11
do
    ./hsim -o output -l implicit -f first -i input/$num.in
done

for num in 1 2 3 4 5 6 7 8 9 10 11
do
    diff output/$num.implicit.first.out examples/$num.implicit.first.out > diff/$num.implicit.first.out
done