#!/bin/bash

# esame dir start N



# Controllo argomenti
if [[ $# -ne 3 ]]; then
    echo "Errore: numero di argomenti non corretto" 
    echo -e "Usage:\n\t$0 dir start N" 
    exit 1
fi

if ! [[ -d "$1" ]]; then
    echo "Errore parametro 1: $1 non esiste o non e' una cartella" 
    exit 1
fi

case $1 in
    /*)
    ;;
    *)
        echo "Errore parametro 1: $1 deve essere un path assoluto"
        exit 2
    ;;
esac

if [[ $3 = *[!0-9]* ]] ; then
    echo "Errore parametro 3: $3 non è un intero positivo" 1>&2
    exit 1
fi



outfile="`pwd`/esito.out"
> "$outfile"

# In questo caso, se avessi utilizzato test oppure [, 
# che sono comandi esterni, non avrei avuto l'espansione dell'*,
# in quanto l'espansione dei metacaratteri avviene dopo l'esecuzione
# di comandi in bash.
if [[ "$0" = /* ]] ; then
    #Iniziando con /, si tratta di un path assoluto
    #(eg /home/andrea/recurse_dir.sh)

    #Estrazione di parti di path: man dirname oppure man basename
    dir_name=`dirname "$0"`
    recursive_command="$dir_name/do_recurse_dir.sh"
elif [[ "$0" = */* ]] ; then
    # C'è uno slash nel comando, ma non inizia con /. Path relativo
    dir_name=`dirname "$0"`
    recursive_command="`pwd`/$dir_name/do_recurse_dir.sh"
else 
    # Non si tratta ne di un path relativo, ne di uno assoluto.
    # E' un path "secco": il comando sarà dunque cercato
    # nelle cartelle indicate dalla variabile d'ambiente $PATH.
    recursive_command=do_recurse_dir.sh
fi

#Invoco il comando ricorsivo
"$recursive_command" "$1" "$2" "$3" "$outfile"

