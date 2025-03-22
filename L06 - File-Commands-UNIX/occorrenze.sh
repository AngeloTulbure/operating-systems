#! /bin/bash

[ $# -gt 3 ] || [ $# -lt 2 ] || [ ! -d $1 ] && echo "Wrong arguments" && exit 1
oldpath=`pwd` && cd $1
for i in *; do
	[ -d $i ] && continue
	num=`grep -c $2 $i`;
	[ $# -eq 3 ] &&  echo "$i has $num occurrences" >> $3 || echo "$i has $num occurrences"
done
cd $oldpath
