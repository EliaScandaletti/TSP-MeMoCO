#!/bin/bash

make -C src/ tabu > /dev/null

res_dir="results/tabu"
mkdir -p $res_dir

run() {
    d=$1
    n=$2
    to=${d/data/$res_dir}
    to=${to/dat/res}
    for t in $(seq $((n/10)) $((n/10)) $((n/2))); do
        for i in {20..250..15}; do
            for g in $(seq $((2*i)) 50 $((5*i))); do
                ((m=10*g))
                ./src/tabu $d $t $i $g $m >> $to
            done
        done
    done
}

N=8
i=0
(
for d in data/*.dat; do
   ((i=i%N)); ((i++==0)) && wait
    echo "./src/tabu $d"
    n=${d#*tsp}
    n=${n%_*}
    run $d $n &
done
)