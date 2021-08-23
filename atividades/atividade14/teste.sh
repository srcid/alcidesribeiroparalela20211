#!/usr/bin/env sh

echo "cluster,size,cpus,time" > res.txt
s=2048

for i in 2 4 8
do
    echo "1x8,$s,$i,$(mpirun -np $i ./a.out $s)" >> res.txt
done

sed 's/8$/4' "/home/hostfile" > "hostfile"

for i in 2 4 8
do
    echo "2x4,$s,$i,$(mpirun --hostfile hostfile -np $i ./a.out $s)" >> res.txt
done