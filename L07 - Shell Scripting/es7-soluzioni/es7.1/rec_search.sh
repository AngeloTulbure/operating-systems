# Recursively search through a directory's subtree
# rec_search.sh minR C dir output

# NOTA: vedere soluzioni degli esempi 1 e 2 per le differenze
# tra le espressioni condizionali test, [ oppure [[.

cd "$3"
for file in * ; do
    # NB: se $3 non contiene alcun file, il metacarattere * non viene espanso, ma lascato così com'è. 
	# Quindi, in tal caso, questo ciclo viene eseguito una sola volta con $file=*. Ovviamente, non essendoci
	# in realtà alcun file con tale nome, se chiamassi immediatamente la stat, restituirebbe errore.
	# Per evitare ciò, controllo che $file sia davvero un file prima di eseguire la stat:
    
    echo executing on file=$file
    if [[ -f "$file" ]] ; then
        nocc=`grep -o $2 $file | wc -l`
    # l'opzione -o (--only-matching) filtra solo le stringe che fanno esattamente match con la stringa
    # passata come parametro. Mette ogni occorrenza su una linea diversa. Quindi mi basta contare le linee.
	    owner=`stat --format="%U" $file`
    #in alternativa
        #owner=`ls -l $file | awk '{print $4}'` 


        if [[ $owner == $USER ]] &&  [[ $nocc -gt $1 ]] ; then
            echo "`pwd`/$file" >> "$4" # scrivo in append dentro il file di output.
        fi
	
    elif [[ -d "$file" ]] ; then
        echo "Starting recursion on dir `pwd`/$file"
        "$0" "$1" "$2" "$file" "$4"
    fi
done

