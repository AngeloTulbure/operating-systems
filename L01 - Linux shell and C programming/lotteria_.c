#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#define MAX 20  // massimo numero di studenti
#define dim 12  // dimensione identificatore

void usage(char* prog_name)
{
    fprintf(stderr,"Usage:\n\t%s: stud1 stud2 .. studN W .\n", prog_name);
}
typedef struct {
    char id[dim+1]; // nome dello studente + term stringa
    unsigned int biglietto; // biglietto assegnato allo studente
} studente;

int verifica(int estratto, studente *studenti, int n)
{   int i;
    for (i=0; i<n; i++)
        if (studenti[i].biglietto==estratto)
            return 0;
    return 1;
}


int main(int argc, char *argv[]) {
    studente STUD[MAX];
    unsigned int N;
    int W; // numero biglietto vincente
	int i,j, k, fine;
    int unico=0;
    char campus[3];
    srand(time(NULL));
    
 	if ( argc <= 2 ) /*Almeno uno studente + il numero vincente*/{
		fprintf(stderr,"Numero di argomenti non valido\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	W=atoi(argv[argc-1]); // biglietto vincitore
    N=argc-2; // numero studenti partecipanti
    
    if ((W<=0)||(W>N))
    {   fprintf(stderr,"W deve essere positivo e minore o uguale a N! \n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
    }
	
	for ( i=0; i<N ; i++ ) 
    {   strcpy(STUD[i].id, argv[i+1]);
        // controlli sull'id:
        if (strlen(STUD[i].id)!= dim)
        {   fprintf(stderr,"dimensione id dello studente %d non corretta \n");
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    
    
    // verifica campus:
    strncpy(campus, STUD[i].id, 2);
    
    if (strcmp(campus, "BO")&& strcmp(campus,"RA") && strcmp(campus,"CE") && strcmp(campus,"FO"))
    {   fprintf(stderr,"studente %s: campus non corretto.\n",STUD[i].id);
            usage(argv[0]);
            exit(EXIT_FAILURE);
    }    
        
    
    for(j=2;j<dim;j++)
        if (isdigit(STUD[i].id[j])==0){
            fprintf(stderr,"studente %s: formato errato.\n",STUD[i].id);
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
 
    printf("lo studente n. %d ha id %s\n", i+1, STUD[i].id); 
    
    }

    // assegnazione biglietti;
    for (i=0; i<N; i++)
    {  fine=0;
       while(!fine) {
        k=rand()%N+1;
        unico=verifica(k, STUD, i); // verifica se il numero Ã¨ giÃ  uscito
        if (unico) 
        {   STUD[i].biglietto=k;
            printf("assegnato biglietto %d a studente %s\n", k, STUD[i].id);
            fine=1;
        }
       }
    }
    // individuazione vincitore;
    for (i=0; i<N; i++)
    {  
        if (STUD[i].biglietto ==W) 
        {   
            printf("\nLo studente %s ha vinto la lotteria\n\n", STUD[i].id);
            return EXIT_SUCCESS;
        }
    }
	
    
}

    
