
#include <stdio.h>
#include <stdlib.h>
#define MAXN 30
#define MAXV 10

int M[MAXN][MAXV];
int V;
int N;

void figlio(int indice);
int cerca_indice(int *pid, int p);

void main(int argc, char *argv[]) {
int i, j, k, status, n_children;
int pid[MAXN], p; 


if (argc!=3)
{   printf("%s: sintassi sbagliata\n", argv[0]);
    exit(1);
}

N = atoi(argv[1]);
V = atoi(argv[2]);


for ( i=0; i<N; i++ ) {
    printf("immetti i %d voti dello studente %d:\n", V, i);
    for(j=0; j<V; j++)
       scanf("%d", &M[i][j]);
}

// stampa matrice:
printf("\n ----- DATI -----\n");
for ( i=0; i<N; i++ ) {
    printf("voti dello studente %d:",  i);
    for(j=0; j<V; j++)
    {   
        printf("\t%d", M[i][j]);
    }
    printf("\n");
}
printf("----------------\n\n");


for ( i=0; i<N; i++ ) { // creazione figli
    pid[i] = fork();
    if ( pid[i] == 0 ) { // Eseguito dai figli
        figlio(i);
    }
    else if ( pid[i] > 0 ) { // Eseguito dal padre
        printf("%d: ho creato il figlio %d (PID %d)\n", getpid(),i, pid[i]);
    }
    else {
        perror("Fork error:");
        exit(1);
    }
    

}

for ( i=0; i<N; i++ ) { // attesa figli
    p=wait(&status);
    k=cerca_indice(pid,p); // ricerca indice del figlio di pid p
    if ((char)status==0) 
        printf("\nIl voto dello studente n.%d (pid=%d) è %d\n",k,p, status>>8);
    else 
        printf("term. involontaria  per segnale %d\n",(char)status);
}



}

void figlio(int indice)
{   int voto, j;
    float media=0;

    for(j=0;j<V; j++)
        media+=M[indice][j];
    media=media/V; // valore reale
    voto=(int)media; // parte intera
    if (media>=(voto+0.5)) // arrotondamento verso l'alto
        voto++;
    //printf("studente %d - media: %f \t voto: %d\n", indice, media, voto);
    exit(voto);
}

int cerca_indice(int *pid, int p){
    int i;
    for(i=0;i<N; i++)
        if (pid[i]==p)
            return i;
}
