package barbecue;
import java.util.concurrent.locks.*;
import java.util.Date;


public class Monitor {
	
	private final int PSG=0, PN=1;
	
	private Lock mutex = new ReentrantLock();
	private Condition[] coda;
	private Condition attesaInserviente;
	private int[] sospesiInCoda;
	private int[] inCottura;
	private int[] piatti_completati;
	private int capienzaMassima;
	private int griglia_corrente=-1; // griglia correntemente in uso
	private boolean clean, cleanerIn, inservienteInAttesa;
	Date d=new Date();
	
	public Monitor(int capienzaMassima){
		
		this.capienzaMassima = capienzaMassima;
		
		sospesiInCoda = new int[2];
		inCottura = new int[2];
		piatti_completati = new int[2];
		coda = new Condition[2];
		for(int i=0; i<2; i++){
			sospesiInCoda[i] = 0;
			inCottura[i] = 0;
			piatti_completati[i]=0;
			coda[i] = mutex.newCondition();
		}
		
		attesaInserviente = mutex.newCondition();
		
		clean = true;
		cleanerIn = false;
		inservienteInAttesa = false;
	}
	
	
	//metodi entry
	public void acquireBBQ(int tipo) throws InterruptedException{
		boolean inizio=false;
		mutex.lock();
		
		try{
		if(tipo==PSG){
			while(inCottura[PSG]==capienzaMassima || inCottura[PN]>0 || !clean){
				sospesiInCoda[PSG]++;
				coda[PSG].await();
				sospesiInCoda[PSG]--;
			}
		}else{
			while(cleanerIn || inCottura[PSG]>0 || sospesiInCoda[PSG]>0 || inCottura[PN]==capienzaMassima){
				sospesiInCoda[PN]++;
				coda[PN].await();
				sospesiInCoda[PN]--;
			}
		}
		if (griglia_corrente==-1)
			inizio=true; //Ã¨ la prima fase di cottura - serve per decidere se stampare o no
		if (griglia_corrente!=tipo) //cambio griglia
		{	griglia_corrente=tipo;
			if (tipo==PN && !inizio)
			{	System.out.println("PSG â€“ "+ piatti_completati[PSG] + "  piatti - " +  d.toString());
				piatti_completati[tipo]=0;
			}
			else if (tipo==PSG && !inizio)
			{	System.out.println("PN â€“ "+ piatti_completati[PN] + "  piatti - " +  d.toString());
			piatti_completati[tipo]=0;
		}
		}
		inCottura[tipo]++;
		if(tipo==PN)
			clean = false;
			
		
		}finally{
			mutex.unlock();
		}
	}
	
	public void releaseBBQ(int tipo){
		mutex.lock();
		
			inCottura[tipo]--;
			piatti_completati[tipo]++;
			if(inCottura[tipo]==0){
				if(tipo==PN && inservienteInAttesa)
							attesaInserviente.signal();
				if(tipo==PSG && sospesiInCoda[PN]>0)
					coda[PN].signalAll();
			}
			else{
				if(tipo==PSG && sospesiInCoda[PSG]>0)
					coda[PSG].signal();
				if(tipo==PN && sospesiInCoda[PSG]==0 && sospesiInCoda[PN]>0)
					coda[PN].signal();
			}
			
		mutex.unlock();
	}
	
	public void acquireToClean() throws InterruptedException{
		mutex.lock();
		try{
			//Non serve controllare che non ci siano PSG perchÃ© in tal caso clean=true
			while(clean || inCottura[PN]>0){
				inservienteInAttesa = true;
				//System.out.println("L'inserviente attende di pulire");
				attesaInserviente.await();
				inservienteInAttesa = false;
			}
			
			//System.out.println("L'inserviente inizia a pulire");
			cleanerIn = true;
			
		}finally{
			mutex.unlock();
		}
	}
	
	public void releaseAfterClean(){
		mutex.lock();
			clean = true;
			cleanerIn = false;
			//System.out.println("L'inserviente smette di pulire");
			if(sospesiInCoda[PSG]>0)
				coda[PSG].signalAll();
		mutex.unlock();
	}
	
}
