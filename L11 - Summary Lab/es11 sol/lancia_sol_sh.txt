#!/bin/bash
#Usage: ./lancia.sh fileout timeout fase

#Controllo il numero dei parametri
if [ $# -ne 3 ]; then
	echo "Usage: ./lancia.sh fileout timeout fase"
	exit
fi

fileout=$1
>$fileout  #se il file esiste giÃ , cancello qualunque cosa vi sia stata scritta in precedenza

#Controllo che il file sia scrivibile
if [[ ! -w "$fileout" ]]; then
	echo "$fileout is not writable"
	exit
fi

timeout=$2

#Controllo che il timeout sia un numero intero
if [[ "$timeout" == *[!0-9]* ]]; then
	echo "$timeout is not a number"
	exit
fi

fase=$3


if [[ $fase != PSG ]] && [[ $fase != PN ]]; then
	echo "$fase not correct"
	exit
fi

#Genero il figlio tramite la & in modo da permettere al padre di proseguire
java -jar barbecue.jar > "$fileout" &


#Interrompo l'esecuzione del padre per $timeout secondi
sleep $timeout

#Mi salvo il pid del figlio facendo il parsing del comando ps
pid_figlio=`ps | grep "java"  | cut -d ' ' -f1`
echo il figlio ha pid=$pid_figlio

#Uccido il processo figlio
kill -9 $pid_figlio

#Stampo le linee corrispondenti
if [[ $fase == PSG ]] ; then
	cat $fileout | grep '^PSG'
else
	cat $fileout | grep '^PN'
fi

exit

