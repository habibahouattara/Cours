package motion.model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

import javax.swing.SwingWorker;
import javax.swing.Timer;

import util.Contract;

public class WorkerAnimator extends AbstractAnimator implements Animator {
	
	// ATTRIBUTS
	
	private int speed;
	private boolean started;
	private boolean stopped;
	private boolean paused;
	private TickWorker worker;
	private volatile int delay;
	
	// CONSTRUCTEUR
	
	public WorkerAnimator(int max) {
		super(testPrecondMax(max));
		speed = max / 2;
		delay = sleepDuration();
	}
	
	// REQUETES
	
	@Override
	public int getSpeed() {
		return speed;
	}

	@Override
	public boolean hasStarted() {
		return started;
	}

	@Override
	public boolean hasStopped() {
		return hasStarted() && stopped;
	}

	@Override
	public boolean isPaused() {
		return isRunning() && paused;
	}

	@Override
	public boolean isResumed() {
		return isRunning() && !paused;
	}

	@Override
	public boolean isRunning() {
		return hasStarted() && !hasStopped();
	}
	
	// COMMANDES

	@Override
	public void pause() {
		Contract.checkCondition(isRunning());
		paused = true;
		fireStateChanged();
	}

	@Override
	public void resume() {
		Contract.checkCondition(isRunning());
		paused = false;
		this.notify();
		fireStateChanged();
	}

	@Override
	public void setSpeed(int d) {
		Contract.checkCondition(d >= 0 && d <= getMaxSpeed());
		speed = d;
		delay = sleepDuration();
	}

	@Override
	public void start() {
		started = true;
		paused = false;
		worker.execute();
		fireStateChanged();
	}

	@Override
	public void stop() {
		Contract.checkCondition(isRunning());
		stopped = true;
		worker.cancel(true);
		fireStateChanged();
	}
	
	// TYPE IMBRIQUE
	
	class TickWorker extends SwingWorker<Void, Runnable> {
		@Override
		protected Void doInBackground() throws Exception {
			while (!hasStopped()) {
				if (isPaused()) {
					synchronized (WorkerAnimator.this) {
						WorkerAnimator.this.wait();
					}
				} else {
					publish(new Runnable() {
						@Override
						public void run() {
							fireTickOccured();
						}
					});
					try {
						Thread.sleep(delay);
					} catch (InterruptedException ie) {
						// rien
					}
				}
			}
			return null;
		}
		
		@Override
		protected void process(List<Runnable> list) {
			for (Runnable r : list) {
				r.run();
			}
		}
	}
	
	// OUTILS
	
	private static int testPrecondMax(int max) {
		Contract.checkCondition(max > 0);
		return max;
	}
}
