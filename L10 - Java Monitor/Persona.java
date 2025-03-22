package com.company;

public class Persona implements Runnable{
    private Ufficio ufficio;
    private Ufficio.TipoRichiesta tipo;

    public Persona(Ufficio ufficio, Ufficio.TipoRichiesta tipo) {
        this.ufficio = ufficio;
        this.tipo = tipo;
    }

    @Override
    public void run() {
        try {
            while (true) {
                Thread.sleep(10000);
                System.out.println("richiedo " + tipo);
                ufficio.richiediServizio(tipo);
                Thread.sleep(5000);
                ufficio.rilasciaSportello(tipo);
                System.out.println("finito " + tipo);
            }
        } catch (Exception ignored){}
    }
}
