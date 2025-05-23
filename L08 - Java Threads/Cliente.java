package com.company;

import java.util.Random;

public class Cliente extends Thread {
    private Supermercato supermercato;

    public Cliente(Supermercato s){
        this.supermercato = s;
    }

    private void sleep_ignore(int seconds){
        try {
            sleep(seconds * 1000);
        } catch (InterruptedException ignored) {}
    }

    public void run(){
        Random rand = new Random(System.currentTimeMillis());
        while (!supermercato.entra())
            sleep_ignore(rand.nextInt(5));
        sleep_ignore(rand.nextInt(10));
        supermercato.esci(rand.nextInt(100));
    }
}
