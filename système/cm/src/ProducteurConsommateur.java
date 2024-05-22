/**
 * Un exemple de producteur/consommateur en Java.
 * Tout le code est placé dans un seul fichier pour
 * faciliter sa présentation lors du cours.
 */
public class ProducteurConsommateur {
    static final int N = 10;
    static Producteur p = new Producteur();
    static Consommateur c = new Consommateur();
    static TamponSynchronise tampon = new TamponSynchronise();

    public static void main(String args[]) {
        p.start();
        c.start();
    }
    
    static class Producteur extends Thread {
        public void run() {
            int i = 0;
            while (true) {
                System.out.println("On produit " + i);
                tampon.insert(i);
                ++i;
            }
        }
    }
    
    static class Consommateur extends Thread {
        public void run() {
            while (true) {
                int i = tampon.remove();
                System.out.println("On consomme " + i);
            }
        }
    }
    
    /**
     * Notre moniteur.
     */
    static class TamponSynchronise {
        private int buffer[] = new int[N];
        private int count = 0;
        private int lo = 0;
        private int hi = 0;
        
        public synchronized void insert(int i) {
            if (count == N) {
                try {
                    wait();
                } catch (InterruptedException exc) {
                    System.err.println("wait interrompu");
                    System.exit(1);
                }
            }
            buffer[hi] = i;
            hi = (hi + 1) % N;
            ++count;
            if (count == 1) {
                notify();
            }
        }
        
        public synchronized int remove() {
            int i;
            if (count == 0) {
                try {
                    wait();
                } catch (InterruptedException exc) {
                    System.err.println("wait interrompu");
                    System.exit(1);
                }
            }
            i = buffer[lo];
            lo = (lo + 1) % N;
            --count;
            if (count == N - 1) {
                notify();
            }
            return i;
        }
    }
}
