#!/bin/bash

[[ -z "$1" ]] && phrase="asdf" || phrase="$1"
data=data.txt
nthreads=$(seq 1 64)

rm $data
for n in $nthreads; do
    make clean > /dev/null
    make FLAGS="-DNUM_THREADS=$n" > /dev/null
    echo -n "threads=$n time="
    t=$( (time ./a.out wordy.txt $phrase) 2>&1 | grep real | awk '{print $2}')
    echo $t
    echo -n "$n," >> $data
    echo $t | sed -r "s/0m([0-9]\.[0-9]+)s/\1/" >> $data
done
