#!/bin/bash
if test $# -ne 1 ; then
    echo "Usage $0 dir"
fi
if ! test -d "$1" ; then
    echo "$1 is not a valid directory"
fi

cd "$1"
for i in * ; do
    if test -d "$i" ; then
        echo "${i}: `ls -1 $i | wc -l` elementi" >> summary.out
		#in questo caso voglio che il carattere ":" segua subito il valore della variabile $i, ma se scrivessi $i: la shell cercherebbe di sostituire ciò con una variabile di nome "i:" . Quindi pongo i tra graffe: ${i} 
    elif test -f "$i" ; then
        echo "${i}: `head -c 10 $i`" >> summary.out
    fi
done

