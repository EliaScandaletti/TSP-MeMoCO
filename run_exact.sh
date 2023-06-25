#!/bin/bash

make -C src/ exact > /dev/null

res_dir="results/exact"
mkdir -p $res_dir

N=$(nproc)
i=0

for d in data/*.dat; do
   ((i=i%N)); ((i++==0)) && wait
    to=${d/data/$res_dir}
    to=${to/dat/res}
    (
    ./src/exact $d 1800 >> $to;
    echo "./src/exact $d"
    )&
done
wait