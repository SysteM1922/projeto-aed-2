#!/bin/bash
for number in {16302..20000}
do

    ./nm $number 100000 >> "ts2.txt" 

done
exit 0