#!/bin/bash

# Controllo argomenti
if [ $# -ne 1 ] ; then
    echo -e "Sintassi: $0 dir" >&2
    exit 1
fi

#Avrei potuto utilizzare allo stesso modo test oppure [ indifferentemente
if ! [[ -d "$1" ]] ; then
    echo -e "Il file $1 non è una directory" >&2
    exit 1
fi 



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
"$recursive_command" "$1" 



# in alternativa, dopo il controllo dei parametri avrei potuto fare semplicemente:

# oldpath=$PATH
# PATH=$PATH:`pwd`
# do_recurse_dir.sh "$1"
# PATH=$oldpath

# questa soluzione (che è accettata ai fini dell'esame) ha un unico problema:
# che succede se recurse_dir.sh e do_recurse_dir.sh stanno in: /home/dloreti/recurse_dir.sh e io mi trovo in /home (current directory) ?
# chiamerò lo script cosi: ./dloreti/recurse_dir.sh
# ma `pwd` vale /home, quindi cosa aggiungo al PATH ?    do_recurse_dir.sh verrà trovato??