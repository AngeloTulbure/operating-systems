package com.company;

public class Nastro extends Thread{
    final Deposito deposito;
    final int periodo;
    final int maxCicli;
    /* Implementazione orribile, per migliorare sarebbe utile un qualche tipo di puntatore a funzione */
    public enum Tipo {
        CERCHIONI, PNEUMATICI
    }
    Tipo tipo;

    public Nastro(Deposito deposito, int periodo, int maxCicli, Tipo tipo) {
        this.deposito = deposito;
        this.periodo = periodo;
        this.maxCicli = maxCicli;
        this.tipo = tipo;
    }

    @Override
    public void run() {
        for (int i = 0; i < maxCicli; i++) {
            try {
                sleep(periodo * 1000);
                if (tipo == Tipo.CERCHIONI) {
                    deposito.aggiungiC();
                    System.out.println("Aggiunto cerchione");
                } else {
                    deposito.aggiungiP();
                    System.out.println("Aggiunto pneumatico");
                }
            } catch (InterruptedException ignored) {}
        }
    }
}
