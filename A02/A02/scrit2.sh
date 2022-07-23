#!/bin/bash
for number in {10000..20000..2500}
do

    ./test $number 100000 >> "tnodes$number.txt" 

done
exit 0