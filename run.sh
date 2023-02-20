#!/bin/sh

cd src/
make > /dev/null
cd ..

echo '' > exact.out
for d in data/*.dat; do
    ./src/exact $d >> exact.out
done
