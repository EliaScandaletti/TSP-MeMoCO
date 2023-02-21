#!/bin/sh

cd src/
make > /dev/null
cd ..

echo "data\t\t\tn\tsol\t\ttime" > exact.out
for d in data/*.dat; do
    echo $d
    ./src/exact $d >> exact.out
done
