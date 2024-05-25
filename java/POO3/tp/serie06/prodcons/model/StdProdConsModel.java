package prodcons.model;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;

import javax.swing.event.SwingPropertyChangeSupport;

import prodcons.model.actor.StdConsumer;
import prodcons.model.actor.StdProducer;
import prodcons.util.Formatter;
import prodcons.util.event.SentenceEvent;
import prodcons.util.event.SentenceListener;
import util.Contract;

public class StdProdConsModel implements ProdConsModel {

    // ATTRIBUTS STATIQUES

    private static final int MAX_VALUE = 100;

    // ATTRIBUTS

    private final Actor[] actors;
    private final Box box;
    private final int prodNumber;
    private final int consNumber;
    private final PropertyChangeSupport support;

    private volatile boolean running;
    private volatile boolean frozen;

    // CONSTRUCTEURS

    public StdProdConsModel(int prod, int cons, int iter) {
        Contract.checkCondition(prod > 0 && cons > 0 && iter > 0);

        box = new UnsafeBox();
        prodNumber = prod;
        consNumber = cons;
        actors = new Actor[prodNumber + consNumber];
        SentenceListener sentence = new SentenceListener() {
			@Override
			public void sentenceSaid(SentenceEvent e) {
				String s = e.getSentence();
				support.firePropertyChange(PROP_SENTENCE, null, s);
			}
        };
        FrozenDetector fd = new FrozenDetector();
        for (int i = 0; i < prodNumber; i++) {
            actors[i] = new StdProducer(iter, MAX_VALUE, box);
            actors[i].addSentenceListener(sentence);
            actors[i].addPropertyChangeListener(Actor.PROP_ACTIVE, fd);
        }
        for (int i = prodNumber; i < prodNumber + consNumber; i++) {
            actors[i] = new StdConsumer(iter, box);
            actors[i].addSentenceListener(sentence);
            actors[i].addPropertyChangeListener(Actor.PROP_ACTIVE, fd);
        }
        support = new SwingPropertyChangeSupport(this, true);
    }

    // REQUETES

    @Override
    public Box box() {
        return box;
    }

    @Override
    public Actor consumer(int i) {
        Contract.checkCondition(0 <= i && i < consNumber);

        return actors[prodNumber + i];
    }

    @Override
    public int consumersNb() {
        return consNumber;
    }

    @Override
    public boolean isFrozen() {
        /*****************/
        /** A COMPLETER **/
    	return frozen;
        /*****************/
    }

    @Override
    public boolean isRunning() {
        return running;
    }

    @Override
    public Actor producer(int i) {
        Contract.checkCondition(0 <= i && i < prodNumber);

        return actors[i];
    }

    @Override
    public int producersNb() {
        return prodNumber;
    }
    
    // COMMANDES

    @Override
    public void addPropertyChangeListener(String pName,
                PropertyChangeListener lnr) {
        Contract.checkCondition(pName != null && lnr != null);

        support.addPropertyChangeListener(pName, lnr);
    }

    @Override
    public void start() {
        /*****************/
        /** A COMPLETER **/
    	box.clear();
    	Formatter.resetTime();
    	setRunning(true);
    	setFrozen(false);
    	for (Actor actor : actors) {
    		actor.start();
    	}
        /*****************/
    }

    @Override
    public void stop() {
        /*****************/
        /** A COMPLETER **/
    	Thread t = new Thread(new EraserTask());
    	t.start();
    	setRunning(false);
        /*****************/
    }

    // OUTILS
    
    private void setRunning(boolean b) {
        boolean oldRunning = isRunning();
        running = b;
        support.firePropertyChange(PROP_RUNNING, oldRunning, b);
    }
    
    private void setFrozen(boolean b) {
    	boolean oldFrozen = isFrozen();
    	frozen = b;
    	support.firePropertyChange(PROP_FROZEN, oldFrozen, b);
    }
    
    // TYPES IMBRIQUES
    
    private class FrozenDetector implements PropertyChangeListener {
    	
		@Override
		public void propertyChange(PropertyChangeEvent evt) {
			setFrozen(computeFrozenActor());
			if (frozen) {
				Thread t = new Thread(new EraserTask());
				t.start();
				setRunning(false);
			}
		}
		
		private boolean allProducerDead() {
			for (int i = 0; i < prodNumber; i++) {
				if (actors[i].isAlive()) {
					return false;
				}
			}
			return true;
		}
		
		private boolean allConsumerDead() {
			for (int i = prodNumber; i < prodNumber + consNumber; i++) {
				if (actors[i].isAlive()) {
					return false;
				}
			}
			return true;
		}
		
		private boolean allProducerAlive() {
			for (int i = 0; i < prodNumber; i++) {
				if (actors[i].isAlive() && actors[i].isActive()) {
					return false;
				}
			}
			return true;
		}
		
		private boolean allConsumerAlive() {
			for (int i = prodNumber; i < prodNumber + consNumber; i++) {
				if (actors[i].isAlive() && actors[i].isActive()) {
					return false;
				}
			}
			return true;
		}
		
		private boolean computeFrozenActor() {
			return allProducerDead() && allConsumerAlive()
					|| allConsumerDead() && allProducerAlive();
		}
    }
    
    private class EraserTask implements Runnable {
		@Override
		public void run() {
			for (Actor actor : actors) {
				if (actor.isAlive()) {
					actor.interruptAndWaitForTermination();
				}
			}
		}
    }
}
