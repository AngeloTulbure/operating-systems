package barbecue;
import java.util.Random;


public class GestioneBBQ {

	
	public static void main(String[] args) {
		Monitor m = new Monitor(6);
		Random r = new Random(System.currentTimeMillis());
		Preparazione[] preparazioni = new Preparazione[10];
		for(int i=0; i<10; i++)
			preparazioni[i] = new Preparazione(m, r);
		Inserviente inserviente = new Inserviente(m, r);
		for(int i=0; i<10; i++)
			preparazioni[i].start();
		inserviente.start();
	}

}
