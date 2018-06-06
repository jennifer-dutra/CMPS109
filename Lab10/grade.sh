#!/bin/bash

#######################################################################
#
# Copyright (C) 2018 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

total=0;

check()
{
    TAG=$1
    NAME=$2
    MARKS=$3

    printf "%10s:" $NAME

    marks=0;
    grep $TAG check.out > /dev/null
    if [ $? -eq 0 ]
    then
        set `grep $TAG check.out`
        pct=$2
        if [[ $pct != 0* ]]
        then
            marks=`echo "scale=2; ($pct)*$MARKS" | bc -l`
        fi
    fi
    total=`echo "scale=2; $total + $marks" | bc -l`
    printf "%6.1f/%2s\n" $marks $MARKS
}

grade()
{
    NAME=$1
    MARKS=$2
    AVAILABLE=$3
    printf "%10s:%6.1f/%2s\n" $NAME $MARKS $AVAILABLE
}

printf "%s\n\n" "CMPS109 Spring 2018 Lab 10"

check 'RpncAWK: '  'AWK'      35
check 'RpncJS: '   'Node.js'  35  

printf "\n%10s: %5.1f/70\n\n" "Total" $total 

