package com.company;

import java.util.Date;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Barbecue {
    private static final int GRILL_MAX = 10;

    public enum TipoPreparazione {PSG, PN}
    private final Lock lock = new ReentrantLock();
    private boolean daPulire = false;
    private final Condition noGlutineQueue = lock.newCondition();
    private final Condition normalQueue = lock.newCondition();
    private final Condition inservienteQueue = lock.newCondition();
    private int noGlutineCounter = 0;
    private int noGlutineWaitCounter = 0;
    private int normalCounter = 0;

    public void pulizia() throws InterruptedException {
        lock.lock();
        try {
            while (noGlutineCounter > 0 || normalCounter > 0){
                inservienteQueue.await();
            }
            daPulire = false;
            System.out.println("Pulizia");
            noGlutineQueue.signalAll(); //piccola perdita di performance, codice piÃ¹ pulito
            normalQueue.signalAll();
        } finally {
            lock.unlock();
        }
    }

    public void inizioCottura(TipoPreparazione tipo) throws InterruptedException {
        lock.lock();
        try {
            if (tipo == TipoPreparazione.PN) {
                while (normalCounter >= GRILL_MAX || noGlutineWaitCounter > 0 || noGlutineCounter > 0){
                    normalQueue.await();
                }
                System.out.println("InizioCotturaPN");
                daPulire = true;
                normalCounter++;
            } else {
                while (noGlutineCounter >= GRILL_MAX || daPulire || normalCounter > 0){
                    noGlutineWaitCounter++;
                    noGlutineQueue.await();
                    noGlutineWaitCounter--;
                }
                System.out.println("InizioCotturaPSG");
                noGlutineCounter++;
            }
        } finally {
            lock.unlock();
        }
    }

    public void fineCottura(TipoPreparazione tipo) {
        lock.lock();
        try {
            Date date = new Date();
            if (tipo == TipoPreparazione.PN){
                if (normalCounter == 0)
                    return;
                System.out.printf("PN %d piatti %s\n", normalCounter, date.toString());
                normalCounter = 0;
                inservienteQueue.signal();
            } else {
                if (noGlutineCounter == 0)
                    return;
                System.out.printf("PSG %d piatti %s\n", noGlutineCounter, date.toString());
                noGlutineCounter = 0;
            }
            if (noGlutineWaitCounter > 0) {
                noGlutineQueue.signalAll(); //piccola perdita di performance, codice piÃ¹ pulito
            } else {
                normalQueue.signalAll();
            }
        } finally {
            lock.unlock();
        }
    }


}
