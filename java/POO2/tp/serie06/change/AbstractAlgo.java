package serie06;

import util.Contract;

public abstract class AbstractAlgo implements ChangeAlgorithm {
	
	// ATTRIBUTS
	
	private final MoneyAmount cash;
	private MoneyAmount change;
	private boolean found;
	
	// CONSTRUCTEUR
	
	protected AbstractAlgo(MoneyAmount cash) {
	this.cash = copyMoneyAmount(cash);
	}
	
	// REQUETES
	
	public MoneyAmount getCash() {
		return copyMoneyAmount(cash);
	}
	
	public MoneyAmount getChange() {
		Contract.checkCondition(found);
		return copyMoneyAmount(change);
	}
	
	public boolean solutionFound() {
		return found;
	}
	
	// COMMANDES
	
	public abstract void computeChange(int amount);
	
	// OUTILS
	
	private static MoneyAmount copyMoneyAmount(MoneyAmount amount) {
		assert amount != null;
		MoneyAmount result = new StdMoneyAmount();
		result.addAmount(amount);
		return result;
	}
	
	protected void setChange(MoneyAmount amount) {
		assert amount != null;
		change = copyMoneyAmount(amount);
	}
	
	protected void setFound(boolean b) {
		found = b;
	}
}
