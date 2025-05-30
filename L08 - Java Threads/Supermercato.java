package com.company;

public class Supermercato {
    private final int POSTI_DISPONIBILI = 5;
    private int postiOccupati = 0;
    private int cassa = 0;
    private int totClienti = 0;

    public synchronized void stampa(){
        System.out.printf("Cassa: %d, Clienti serviti: %d", cassa, totClienti);
    }
    public synchronized boolean entra(){
        if (postiOccupati < POSTI_DISPONIBILI){
            postiOccupati++;
            totClienti++;
            return true;
        } else {
            return false;
        }
    }

    public synchronized void esci(int totSpesa){
        postiOccupati--;
        cassa += totSpesa;
    }
}
