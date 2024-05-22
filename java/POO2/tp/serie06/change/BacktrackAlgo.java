package serie06;

import util.Contract;

public class BacktrackAlgo extends AbstractAlgo {
	
	// CONSTRUCTEUR
	
	public BacktrackAlgo(MoneyAmount cash) {
		super(cash);
	}

	@Override
	public void computeChange(int amount) {
		Contract.checkCondition(amount >= 0);
		MoneyAmount change = null;
		int i = CoinTypes.values().length - 1;
		change = backtrack(amount, i, new StdMoneyAmount());
		if (change != null) {
			setFound(true);
			setChange(change);
		} else {
			setFound(false);
		}
	}
	
	// OUTILS
	
	private MoneyAmount backtrack(int amount, int i, MoneyAmount change) {
		assert amount >= 0 : "la somme doit être >= 0";
		assert i >= 0 : "hors borne";
		if (amount == 0) {
			return new StdMoneyAmount();
		}
		if (i == 0) {
			return null;
		}
		CoinTypes[] coins = CoinTypes.values();
		CoinTypes c = coins[i];
		int coinNb = getCash().getNumber(c);
		int coinValue = c.getFaceValue();
		for (int q = Math.min(coinNb, amount / coinValue); q >= 0; q--) {
			if (backtrack(amount - q * coinValue, i - 1, change) != null) {
				if (q > 0) {
					change.addElement(c, q);
				}
				return change;
			}
		}
		return null;
	}

}
