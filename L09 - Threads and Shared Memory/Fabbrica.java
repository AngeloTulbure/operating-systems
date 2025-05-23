package com.company;

public class Fabbrica {

    public static void main(String[] args) {
        Deposito deposito = new Deposito(15, 20);
        Nastro nastroCerchioni = new Nastro(deposito, 1, 10, Nastro.Tipo.CERCHIONI);
        Nastro nastroPneumatici = new Nastro(deposito, 1, 10, Nastro.Tipo.PNEUMATICI);
        Robot robot = new Robot(deposito, 10, 30);
        new Thread(nastroCerchioni).start();
        new Thread(nastroPneumatici).start();
        new Thread(robot).start();
    }
}
