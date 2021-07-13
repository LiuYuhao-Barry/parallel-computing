#!/bin/sh

declare n=1
declare maxN=12

mpicxx -Wall -std=c++11 trapezoidalIntegration.cpp

while [ $n -le $maxN ]
do
    echo "###################"
    echo "N = $n"
    mpirun -n $n a.out
    let "n++"
done

echo ""
echo "Program Complete"
