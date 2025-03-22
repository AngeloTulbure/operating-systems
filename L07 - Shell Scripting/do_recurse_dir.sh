#!/bin/bash

# NOTA SULLE ESPRESSIONI CONDIZIONALI IN BASH
# In bash per valutare espressioni condizionali (eg come condizioni degli if)
# si possono usare più di una variante.
# 1) Comando test: es. if test $# -ne 2; then 
#    La shell esegue il comando test che ritorna 0 (valore true in sh)
#    se la condizione passata come argomenti a test ($# -ne 2) risulta 
#    soddisfatta. Il valore di ritorno di test è sostituito all'espressione
#    condizionale.
# 2) Comando [: es. if [ $# -ne 2 ] ; then 
#    ESATTAMENTE equivalente all'uso di test. Può essere in tutto e 
#    per tutto considerato uno shortcut stilistico.
# 3) Comando [[: es. if [[ $# != 2 ]] ; then
#    Utilizzando la doppia quadra, non si utilizza un comando esterno
#    a bash per valutare la condizione, ma questa viene valutata direttamente
#    dalla shell. Uno dei vantaggi di questa versione (oltre alle 
#    prestazioni) è che si può far uso dell'espansione dei metacaratteri
#    all'interno della condizione stessa.
#	 All'interno delle [[ si possono inoltre utilizzare gli operatori logici && e || e
#    gli operatori > e < per il confronto tra stringhe!
#    Quindi si possono semplificare così le espressioni seguenti:
#    if test $a -eq $b -a $a -ne $c ; then    diventa    if [[ $a -eq $b && $a -ne $c ]] ; then 
#    if test $a -eq $b -o $a -ne $c ; then    diventa    if [[ $a -eq $b || $a -ne $c ]] ; then  
#    NB: in entrambi i casi GLI SPAZI TRA VARABILI, PARENTESI E OPERATORI 
#    SONO SIGNIFICATIVI PER LA SHELL!!!


if ! [[ -e "$1" ]] ; then
    echo -e "Il file $1 non esiste\n" >&2
    # le forme >&2 oppure 1>&2 sono equivalenti e indicano alla shell di redirigere quanto stampato su standard output sullo standard error.
    #  > serve per redirigere
    #  1 indica lo stdout (e può essere sottinteso)
    #  & indica che quel che segue deve essere considerato come un file descriptor (non come il semplice nome di un file)
    #  2 è il file descriptor dello sterr
    exit 1
fi 

if ! test -d "$1"; then
    echo "`pwd`/$1"
else
    cd "$1"
    for file in * ; do
       "$0" "$file"
    done 
fi
