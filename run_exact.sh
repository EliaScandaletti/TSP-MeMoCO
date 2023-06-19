#!/bin/bash

make -C src/ exact > /dev/null

res_dir="results/exact"
mkdir -p $res_dir

N=8
i=0
(
for d in data/*.dat; do
   ((i=i%N)); ((i++==0)) && wait
    echo "./src/exact $d"
    to=${d/data/$res_dir}
    to=${to/dat/res}
    ./src/exact $d >> $to &
done
)