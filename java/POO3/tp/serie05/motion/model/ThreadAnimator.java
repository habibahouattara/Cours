package motion.model;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Timer;

import util.Contract;

public class ThreadAnimator extends AbstractAnimator implements Animator {
	
	// ATTRIBUTS
	
	private int speed;
	private boolean started;
	private boolean stopped;
	private boolean paused;
	private Thread thread;
	private volatile int delay;
	
	// CONSTRUCTEUR
	
	public ThreadAnimator(int max) {
		super(testPrecondMax(max));
		speed = max / 2;
		delay = sleepDuration();
		thread = new Thread(new TickLoop());
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
		this.notifyAll();
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
		thread.start();
		fireStateChanged();
	}

	@Override
	public void stop() {
		Contract.checkCondition(isRunning());
		stopped = true;
		thread.interrupt();
		fireStateChanged();
	}
	
	// TYPE IMBRIQUE
	
	class TickLoop implements Runnable {
		@Override
		public void run() {
			while (!hasStopped()) {
				if (isPaused()) {
					synchronized (ThreadAnimator.this) {
						try {
							ThreadAnimator.this.wait();
						} catch (InterruptedException ie) {
							// rien
						}
					}
				} else {
					fireTickOccured();
					try {
						Thread.sleep(delay);
					} catch (InterruptedException ie) {
						// rien
					}
				}
			}
		}
	}
	
	// OUTILS
	
	private static int testPrecondMax(int max) {
		Contract.checkCondition(max > 0);
		return max;
	}
}
