package serie06;

import util.Contract;

public class StdMoneyAmount extends StdStock<CoinTypes> implements MoneyAmount {
	
	// CONSTRUCTEUR
	
	public StdMoneyAmount() {
		super(CoinTypes.class);
	}
	
	// REQUETES
	
	@Override
	public int getValue(CoinTypes c) {
		Contract.checkCondition(c != null);
		return getNumber(c) * c.getFaceValue();
	}

	@Override
	public int getTotalValue() {
		int sum = 0;
		for (CoinTypes c : CoinTypes.values()) {
			sum += getValue(c);
		}
		return sum;
	}
	
	// COMMANDES
	
	@Override
	public void addAmount(MoneyAmount amount) {
		Contract.checkCondition(amount != null);
		for (CoinTypes c : CoinTypes.values()) {
			if (amount.getNumber(c) > 0) {
				addElement(c, amount.getNumber(c));
			}
		}
	}

	@Override
	public MoneyAmount computeChange(int s) {
		Contract.checkCondition(s > 0);
		MoneyAmount amount = new StdMoneyAmount();
		amount.addAmount(this);
		ChangeAlgorithm algo = new BacktrackAlgo(amount);
		algo.computeChange(s);
		return algo.solutionFound() ? algo.getChange() : null;
	}

	@Override
	public void removeAmount(MoneyAmount amount) {
		Contract.checkCondition(amount != null);
		for (CoinTypes c : CoinTypes.values()) {
			Contract.checkCondition(getNumber(c) >= amount.getNumber(c));
		}
		for (CoinTypes c : CoinTypes.values()) {
			if (amount.getNumber(c) > 0) {
				removeElement(c, amount.getNumber(c));
			}
		}
	}

}
