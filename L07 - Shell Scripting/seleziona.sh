#! /bin/sh

test -z "$1" -o -z "$2" -o -z "$3" && exit

dir=$1
start=$2
n=$3
startdir="$(pwd)"

find ./$dir/ -type d | while read sub; do
	num=$(ls $sub ${start}* | wc -l)
	test $num -gt $n && echo "$startdir/$sub $num" > $startdir/esito.out
done
