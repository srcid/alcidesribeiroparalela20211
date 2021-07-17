#!/usr/bin/env sh

for NUM_THREADS in 2, 4, 6, 8
do
    env OMP_NUM_THREADS=$NUM_THREADS time -f "%E" -a -o results.txt ./a.out 400 40000
done