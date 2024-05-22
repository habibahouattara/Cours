package serie06;

import java.util.EnumMap;
import java.util.Map;

import util.Contract;

public class StdStock<E extends Enum<E>> implements Stock<E> {
	
	// ATTRIBUTS
	
	private final Map<E, Integer> stock;
	
	// CONSTRUCTEUR
	
	public StdStock(Class<E> eType) {
		stock = new EnumMap<E, Integer>(eType);
	}
	
	// REQUETES
	
	@Override
	public int getNumber(E e) {
		Contract.checkCondition(e != null);
		return stock.get(e) != null ? stock.get(e) : 0;
	}

	@Override
	public int getTotalNumber() {
		Integer sum = 0;
		for (E e : stock.keySet()) {
			sum += getNumber(e);
		}
		return sum;
	}
	
	// COMMANDES
	
	@Override
	public void addElement(E e) {
		Contract.checkCondition(e != null);
		stock.put(e, getNumber(e) + 1);
	}

	@Override
	public void addElement(E e, int qty) {
		Contract.checkCondition(e != null);
		Contract.checkCondition(qty > 0);
		stock.put(e, getNumber(e) + qty);
	}

	@Override
	public void removeElement(E e) {
		Contract.checkCondition(e != null);
		Contract.checkCondition(getNumber(e) >= 1);
		stock.put(e, getNumber(e) - 1);
	}

	@Override
	public void removeElement(E e, int qty) {
		Contract.checkCondition(e != null);
		Contract.checkCondition(qty > 0);
		Contract.checkCondition(getNumber(e) >= qty);
		stock.put(e, getNumber(e) - qty);
	}

	@Override
	public void reset() {
		stock.clear();
	}

}
