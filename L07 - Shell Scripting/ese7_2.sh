#!/bin/bash
#date +%S
#controllo argomenti
if [ ! $# -gt 3 ]
then
    echo Devi passare almeno 4 argomenti
    exit -1
fi
if [ -z $1 ]
then
    echo The first argument is not a string
    exit 1
fi
if [ ! $2 -gt 0 ]
then
    echo The second argument is not >0
    exit 2
fi
if [ ! $3 -gt 0 ]
then
    echo The third argument is not >0
    exit 3
fi
f=$1
s=$2
m=$3
shift
shift
shift
#controllo path assoluti
for i in $*
do
    case $i in
        /*) ;;
        *) echo The argument is not an absolute path
        exit 4;;
    esac
done
log=`pwd`/cerca$f.log
echo > $log
ciclo=0

while [[ `expr $m - $ciclo` -gt 0 ]]
do
    ciclo=`expr $ciclo + 1`
    echo
    echo Sta per partire il ciclo $ciclo
    echo
    for i in $*
    do
        #programma
        if [[ "$0" = /* ]] ; then
        # se $0 è un path assoluto
        dir_name=`dirname "$0"`
        recursive_cmd="$dir_name/do_ese7_2.sh"
        elif [[ "$0" = */* ]] ; then
        # se c'è uno slash, ma non inizia con /
        # $0 è un path relativo
        dir_name=`dirname "$0"`
        recursive_cmd="`pwd`/$dir_name/do_ese7_2.sh"
        else
        # Non si tratta nè di un path relativo, nè di uno 
        # assoluto, il comando $0 sarà cercato in $PATH. 
        recursive_cmd=do_ese7_2.sh
        fi
        #Invoco il comando ricorsivo
        "$recursive_cmd" "$f" "$i" "$log"
    done
    sleep $s
done
