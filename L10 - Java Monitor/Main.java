package com.company;

public class Main {

    public static void main(String[] args) {
        Ufficio ufficio = new Ufficio(5);
        for (int i = 0; i < 10; i++){
            new Thread(new Persona(ufficio, Ufficio.TipoRichiesta.CER)).start();
            try {
                Thread.sleep(573);
            } catch (InterruptedException ignored) {}
        }
        for (int i = 0; i < 5; i++){
            new Thread(new Persona(ufficio, Ufficio.TipoRichiesta.IDE)).start();
            try {
                Thread.sleep(573);
            } catch (InterruptedException ignored) {}
        }
        while (true){
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
