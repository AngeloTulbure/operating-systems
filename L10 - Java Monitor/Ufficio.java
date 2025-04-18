package com.company;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Ufficio {
    private int numSportelli ;
    public enum TipoRichiesta {CER, IDE}
    private final int tipiLen = TipoRichiesta.values().length;
    private Lock lock = new ReentrantLock();
    private int sportelliOccupati = 0;
    private int[] counter = new int[tipiLen];
    private Condition[] queue = new Condition[tipiLen];

    public Ufficio(int numSportelli) {
        this.numSportelli = numSportelli;
        for (int i = 0; i < tipiLen; i++) {
            counter[i] = 0;
            queue[i] = lock.newCondition();
        }
    }

    public void richiediServizio(TipoRichiesta tipo) throws InterruptedException {
        lock.lock();
        try {
            int tipoNum = tipo.ordinal();
            int otherTipo = (tipoNum + 1) % tipiLen;
            while (sportelliOccupati >= numSportelli || counter[tipoNum] < counter[otherTipo]) {
                queue[tipoNum].await();
            }
            sportelliOccupati++;
        } finally {
            lock.unlock();
        }
    }

    public void rilasciaSportello(TipoRichiesta tipo){
        lock.lock();
        try {
            int tipoNum = tipo.ordinal();
            int otherTipo = (tipoNum + 1) % tipiLen;
            queue[tipoNum].signalAll();
            queue[otherTipo].signalAll();
            counter[tipoNum]++;
            sportelliOccupati--;
        } finally {
            lock.unlock();
        }
    }
}
