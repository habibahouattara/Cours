package serie08;

import java.util.Observable;

import util.Contract;

public class StdSpeedometerModel extends Observable
		implements SpeedometerModel {
	
	// ATTRIBUTS
	
	private double maxSpeed;
	private SpeedUnit speedUnit;
	private boolean isOn;
	private double step;
	private double speed;
	
	// CONSTRUCTEUR
	
	public StdSpeedometerModel(double step, double max) {
		Contract.checkCondition(1 <= step && step <= max);
		maxSpeed = max;
		speed = 0;
		this.step = step;
		speedUnit = SpeedUnit.KMH;
		isOn = false;
	}
	
	// REQUETES

	@Override
	public double getMaxSpeed() {
		return maxSpeed;
	}

	@Override
	public double getSpeed() {
		return speed;
	}

	@Override
	public double getStep() {
		return step;
	}

	@Override
	public SpeedUnit getUnit() {
		return speedUnit;
	}

	@Override
	public boolean isOn() {
		return isOn;
	}
	
	// COMMANDES
	
	@Override
	public void setUnit(SpeedUnit unit) {
		Contract.checkCondition(unit != null);
		double oldUnitPerKm = getUnit().getUnitPerKm();
		speedUnit = unit;
		double newUnitPerKm = getUnit().getUnitPerKm();
		speed = speed / oldUnitPerKm * newUnitPerKm;
		step = step / oldUnitPerKm * newUnitPerKm;
		maxSpeed = maxSpeed / oldUnitPerKm * newUnitPerKm;
		setChanged();
		notifyObservers();
	}

	@Override
	public void slowDown() {
		Contract.checkCondition(isOn());
		speed = Math.max(0,  speed - step);
		setChanged();
		notifyObservers();
	}

	@Override
	public void speedUp() {
		Contract.checkCondition(isOn());
		speed = Math.min(maxSpeed, speed + step);
		setChanged();
		notifyObservers();
	}

	@Override
	public void turnOff() {
		Contract.checkCondition(isOn());
		isOn = false;
		speed = 0;
		setChanged();
		notifyObservers();
	}

	@Override
	public void turnOn() {
		Contract.checkCondition(!isOn());
		isOn = true;
		setChanged();
		notifyObservers();
	}

}
