package serie07;

import java.awt.Dimension;
import java.util.Observable;

import util.Contract;

public class StdSwellingModel extends Observable implements SwellingModel {
	
	// ATTRIBUTS
	
	private Dimension min;
	private Dimension max;
	private Dimension cur;
	
	// CONSTRUCTEUR
	
	public StdSwellingModel() {
		min = new Dimension(0, 0);
		max = new Dimension(0, 0);
		cur = new Dimension(0, 0);
	}
	
	// REQUETES
	
	@Override
	public Dimension current() {
		return new Dimension(cur.width, cur.height);
	}

	@Override
	public boolean isSurroundedBy(Dimension d) {
		Contract.checkCondition(d != null);
		if (cur.height <= d.height && cur.width <= d.width) {
			return true;
		}
		return false;
	}

	@Override
	public boolean isSurrounding(Dimension d) {
		Contract.checkCondition(d != null);
		if (cur.height >= d.height && cur.width >= d.width) {
			return true;
		}
		return false;
	}

	@Override
	public boolean isValidScaleFactor(double f) {
		if (f < MIN_FACTOR) {
			return false;
		}
		int w = (int) (current().width * (1 + (float) (f / 100)));
		int h = (int) (current().height * (1 + (float) (f / 100)));
		Dimension d = new Dimension(w, h);
		if (f >= 0) {
			return isSurroundedBy(d) && (d.height <= max.height
					&& d.width <= max.width);
		} else {
			return (min.height <= d.height && min.width <= d.width
					&& isSurrounding(d));
		}
	}

	@Override
	public Dimension max() {
		return new Dimension(max.width, max.height);
	}

	@Override
	public Dimension min() {
		return new Dimension(min.width, min.height);
	}
	
	// COMMANDES
	
	@Override
	public void scaleCurrent(double f) {
		Contract.checkCondition(isValidScaleFactor(f));
		int w = (int) (cur.width * (1 + f / 100));
		int h = (int) (cur.height * (1 + f / 100));
		cur = new Dimension(w, h);
		setChanged();
		notifyObservers();
	}

	@Override
	public void setCurrent(Dimension d) {
		Contract.checkCondition(d != null);
		Contract.checkCondition(min().height <= d.height
				&& d.height <= max().height
				&& min().width <= d.width
				&& d.width <= max().width);
		cur = d;
		setChanged();
		notifyObservers();
	}

	@Override
	public void setMax(Dimension d) {
		Contract.checkCondition(d != null);
		Contract.checkCondition(0 <= d.height && 0 <= d.width);
		if (min.height > d.height && min.width > d.width) {
			min = d;
		}
		if (cur.height > d.height && cur.width > d.width) {
			cur = d;
		}
		max = d;
		setChanged();
		notifyObservers();
	}

	@Override
	public void setMin(Dimension d) {
		Contract.checkCondition(d != null);
		Contract.checkCondition(0 <= d.height && 0 <= d.width);
		if (min.height < d.height && min.width < d.width) {
			cur = d;
		}
		if (max.height < d.height && max.width < d.width) {
			max = d;
		}
		min = d;
		setChanged();
		notifyObservers();
	}

}
