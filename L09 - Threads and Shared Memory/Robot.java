package com.company;

public class Robot extends Thread{
    final Deposito deposito;
    final int periodo;
    final int maxCicli;

    public Robot(Deposito deposito, int periodo, int maxCicli) {
      this.deposito = deposito;
      this.periodo = periodo;
      this.maxCicli = maxCicli;
    }

    @Override
    public void run() {
        int counter = 0;
          for (int i = 0; i < maxCicli; i++) {
            try {
              sleep(periodo * 1000);
              deposito.preleva();
              System.out.printf("Macchina costruita, num: %d\n", counter);
            } catch (InterruptedException ignored) {}
          }
        }
}
