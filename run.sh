#!/bin/sh

cd src/
make > /dev/null
cd ..

echo "data\t\t\tn\tsol\t\ttime" > exact.out
echo "data\t\t\tn\ttab\tsol\t\titer\ttime" > tabu.out
for d in data/*.dat; do
    echo $d
    ./src/exact $d >> exact.out
    ./src/tabu $d >> tabu.out
done
