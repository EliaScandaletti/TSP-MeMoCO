#!/bin/bash

cd src/
make > /dev/null
cd ..

echo -e "data\t\t\tn\tsol\t\ttime" > exact.out
echo -e "data\t\t\tn\ttab\tsol\t\titer\ttime" > tabu.out
for d in data/*.dat; do
    echo "./src/exact $d"
    ./src/exact $d >> exact.out
    for t in {0..20..2}; do
        echo "./src/tabu $d $t 100000"
        ./src/tabu $d $t 100000 >> tabu.out
    done
done
