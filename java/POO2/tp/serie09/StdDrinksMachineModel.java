package serie09;

import java.util.Observable;

import serie06.BacktrackAlgo;
import serie06.ChangeAlgorithm;
import serie06.CoinTypes;
import serie06.DrinkTypes;
import serie06.DrinksMachineModel;
import serie06.MoneyAmount;
import serie06.StdMoneyAmount;
import serie06.StdStock;
import serie06.Stock;
import util.Contract;

public class StdDrinksMachineModel extends Observable
		implements DrinksMachineModel {
	
	// ATTRIBUTS
	
	private final Stock<DrinkTypes> drinksStock;
	private final MoneyAmount cashBox;
	private final MoneyAmount creditBox;
	private final MoneyAmount changeBox;
	private DrinkTypes lastDrink;
	
	// CONSTRUCTEUR
	
	public StdDrinksMachineModel() {
		drinksStock = new StdStock<DrinkTypes>(DrinkTypes.class);
		for (DrinkTypes d : DrinkTypes.values()) {
			drinksStock.addElement(d, MAX_DRINK);
		}
		cashBox = new StdMoneyAmount();
		creditBox = new StdMoneyAmount();
		changeBox = new StdMoneyAmount();
		lastDrink = null;
	}
	
	// REQUETES
	
	@Override
	public int getDrinkNb(DrinkTypes d) {
		Contract.checkCondition(d != null);
		return drinksStock.getNumber(d);
	}

	@Override
	public DrinkTypes getLastDrink() {
		return lastDrink;
	}

	@Override
	public int getCreditAmount() {
		return creditBox.getTotalValue();
	}

	@Override
	public int getCreditNb(CoinTypes c) {
		Contract.checkCondition(c != null);
		return creditBox.getNumber(c);
	}

	@Override
	public int getCashAmount() {
		return cashBox.getTotalValue();
	}

	@Override
	public int getCashNb(CoinTypes c) {
		Contract.checkCondition(c != null);
		return cashBox.getNumber(c);
	}

	@Override
	public int getChangeAmount() {
		return changeBox.getTotalValue();
	}

	@Override
	public int getChangeNb(CoinTypes c) {
		Contract.checkCondition(c != null);
		return changeBox.getNumber(c);
	}

	@Override
	public boolean canGetChange() {
		ChangeAlgorithm algo = new BacktrackAlgo(cashBox);
		algo.computeChange(creditBox.getTotalValue());
		return algo.solutionFound();
	}
	
	// COMMANDES
	
	@Override
	public void selectDrink(DrinkTypes d) {
		Contract.checkCondition(d != null);
		Contract.checkCondition(getDrinkNb(d) >= 1);
		Contract.checkCondition(getCreditAmount() >= d.getPrice());
		Contract.checkCondition(getLastDrink() == null);
		ChangeAlgorithm algo = new BacktrackAlgo(cashBox);
		int value = creditBox.getTotalValue();
		algo.computeChange(value);
		cashBox.addAmount(creditBox);
		drinksStock.removeElement(d);
		boolean changeIsOk = algo.solutionFound();
		if (value > d.getPrice() && changeIsOk) {
			algo.computeChange(value - d.getPrice());
			MoneyAmount amount = algo.getChange();
			changeBox.addAmount(amount);
			cashBox.removeAmount(amount);
		}
		creditBox.reset();
		lastDrink = d;
		setChanged();
		notifyObservers();
	}

	@Override
	public void fillStock(DrinkTypes d, int q) {
		Contract.checkCondition(d != null);
		Contract.checkCondition(q > 0);
		Contract.checkCondition(getDrinkNb(d) + q <= MAX_DRINK);
		drinksStock.addElement(d, q);
		setChanged();
		notifyObservers();
	}

	@Override
	public void fillCash(CoinTypes c, int q) {
		Contract.checkCondition(c != null);
		Contract.checkCondition(q > 0);
		Contract.checkCondition(getCashNb(c) + getCreditNb(c) + q <= MAX_COIN);
		cashBox.addElement(c, q);
		setChanged();
		notifyObservers();
	}

	@Override
	public void insertCoin(CoinTypes c) {
		Contract.checkCondition(c != null);
		int max = getCashNb(c) + getCreditNb(c);
		if (max == MAX_COIN) {
			changeBox.addElement(c);
		}
		if (max < MAX_COIN) {
			creditBox.addElement(c);
		}
		setChanged();
		notifyObservers();
	}

	@Override
	public void cancelCredit() {
		changeBox.addAmount(creditBox);
		creditBox.reset();
		setChanged();
		notifyObservers();
	}

	@Override
	public void takeDrink() {
		lastDrink = null;
		setChanged();
		notifyObservers();
	}

	@Override
	public void takeChange() {
		changeBox.reset();
		setChanged();
		notifyObservers();
	}

	@Override
	public void reset() {
		cashBox.reset();
		creditBox.reset();
		changeBox.reset();
		lastDrink = null;
		drinksStock.reset();
		setChanged();
		notifyObservers();
	}

}
