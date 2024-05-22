package serie06;

import util.Contract;

public class GreedyAlgo extends AbstractAlgo {
	
	// CONSTRUCTEUR
	
	public GreedyAlgo(MoneyAmount cash) {
		super(cash);
	}
	
	// COMMANDE
	
	@Override
	public void computeChange(int amount) {
		Contract.checkCondition(amount >= 0);
		MoneyAmount cash = getCash();
		MoneyAmount change = new StdMoneyAmount();
		CoinTypes[] tab = CoinTypes.values();
		for (int i = tab.length - 1; i >= 0 && amount > 0; i++) {
			CoinTypes c = tab[i];
			int q = Math.min(cash.getNumber(c), amount / c.getFaceValue());
			amount -= q * c.getFaceValue();
			if (q > 0) {
				change.addElement(c, q);
			}
		}
		if (amount != 0) {
			setFound(false);
		}
		if (amount == 0) {
			setChange(change);
			setFound(true);
		}
 	}

}
