#!/bin/bash

# NOTA: vedere soluzioni degli esempi 1 e 2 per le differenze
# tra le espressioni condizionali test, [ oppure [[.

#Controllo argomenti
if [[ $# -lt 3 ]]; then
    echo "Errore: numero di argomenti non corretto" 1>&2
    echo -e "Usage:\n\t$0 minR C dir1 ... dirN" 1>&2
    # le forme >&2 oppure 1>&2 sono equivalenti e indicano alla shell di redirigere quanto stampato su standard output sullo standard error.
    #  > serve per redirigere
    #  1 indica lo stdout (e può essere sottinteso)
    #  & indica che quel che segue deve essere considerato come un file descriptor (non come il semplice nome di un file)
    #  2 è il file descriptor dello sterr
    exit 1
fi

if [[ $1 = *[!0-9]* ]] ; then
    echo "$1 non è un intero positivo" 1>&2
    exit 1
fi


minR=$1
C=$2
output=~/summary.out   #equivale a:  output=$HOME/summary.out

shift 2 #scarto i primi due parametri in ingresso

for dir in "$@"; do
    if ! [[ -d "$dir" ]]; then
        echo "Errore: $dir non esiste o non è una cartella" 1>&2
        exit 1
    fi
done

# se uso la forma <for dir in $*; do> e ho un file/direttorio con spazi "pippo pluto", esso genera due esecuzioni del ciclo. 
# La prima con $dir=pippo e la seconda con $dir=pluto. Questo ovviamente non è corretto.
# la forma <for dir in "$@"; do> mi permette di ciclare correttamente anche in caso di file/direttori con spazi.
# In questo caso se un file/direttorio si chiama "pippo pluto", viene fatta una sola esecuzione del ciclo con $dir="pippo pluto"


case "$0" in
    # La directory inizia per / Path assoluto.
    /*) 
    dir_name=`dirname $0`
    recursive_command="$dir_name"/rec_search.sh
    ;;
    */*)
    # La directory non inizia per slash, ma ha uno slash al suo interno.
    # Path relativo.
    dir_name=`dirname $0`
    recursive_command="`pwd`/$dir_name/rec_search.sh"
    ;;
    *)
    #Path né assoluto nP relativo, il comando deve essere nel $PATH
    #comando nel path
    recursive_command=rec_search.sh
    ;;
esac


#creo il file di output oppure, se già esiste, ne cancello il contenuto
> "$output"

for dir in "$@"; do
    echo "Launching recursion for $dir"
    "$recursive_command" "$minR" "$C" "$dir" "$output"
done




