#! /bin/sh

minr=$1
c=$2
dir1n="$(echo $@ | cut -d' ' -f3-)"
startdir="$(pwd)"

for i in $dir1n; do #dirs with spaces will fail, should shange $IFS
	test ! -d $i && exit
	cd $i
	dirs="$(ls -d */)"
	$0 $minr $c $dirs
	find . -maxdepth 1 -type f | while read f; do
		grep -c $c $f | xargs test $minr -lt && stat -c '%U' $f | xargs test $USER = && echo $f >> $HOME/file_out.txt
	done
	cd $startdir
done
