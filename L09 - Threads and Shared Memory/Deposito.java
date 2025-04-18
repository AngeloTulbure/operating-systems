package com.company;
import java.util.concurrent.Semaphore;

public class Deposito {
    final int MAX_PNEUMATICI;
    final int MAX_CERCHIONI;
    Semaphore semCerch;
    Semaphore semPneu;
    Semaphore semRobot;
    Semaphore mutex;
    int buffCerchioni = 0;
    int buffPneumatici = 0;


    public Deposito(int max_pneumatici, int max_cerchioni) {
        MAX_PNEUMATICI = max_pneumatici;
        MAX_CERCHIONI = max_cerchioni;
        semCerch = new Semaphore(MAX_CERCHIONI);
        semPneu = new Semaphore(MAX_PNEUMATICI);
        semRobot = new Semaphore(0);
        mutex = new Semaphore(1);
    }

    private void addRobot(){
        try {
            mutex.acquire();
            if (buffCerchioni % 4 == 0 && buffCerchioni > 0 && buffPneumatici % 4 == 0 && buffPneumatici > 0) {
                buffCerchioni -= 4;
                buffPneumatici -= 4;
                semRobot.release();
            }
            mutex.release();
        } catch(InterruptedException ignored){}
    }

    public void preleva() {
        try {
            semRobot.acquire();
            semPneu.release(4);
            semCerch.release(4);
        } catch (InterruptedException ignored) {}
    }

    public void aggiungiP() {
        try {
            semPneu.acquire();
            buffPneumatici++;
            addRobot();
        } catch (InterruptedException ignored) {};
    }

    public void aggiungiC() {
        try {
            semCerch.acquire();
            buffCerchioni++;
            addRobot();
        } catch (InterruptedException ignored) {};
    }
}
