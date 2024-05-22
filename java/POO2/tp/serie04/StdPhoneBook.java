package serie04;

import java.util.ArrayList;
import java.util.List;
import java.util.NavigableMap;
import java.util.NavigableSet;
import java.util.TreeMap;

import util.Contract;

public class StdPhoneBook<C extends Contact & Comparable<C>, N>
		implements PhoneBook<C, N> {
	
	// ATTRIBUTS
	
	private final NavigableMap<C, List<N>> data;
	
	// CONSTRUCTEUR
	
	public StdPhoneBook() {
		data = new TreeMap<C, List<N>>();
	}
	
	// REQUETES

	@Override
	public NavigableSet<C> contacts() {
		return data.navigableKeySet();
	}

	@Override
	public boolean contains(C p) {
		Contract.checkCondition(p != null);
		return contacts().contains(p);
	}

	@Override
	public boolean isEmpty() {
		return contacts().size() == 0;
	}

	@Override
	public List<N> phoneNumbers(C p) {
		Contract.checkCondition(p != null);
		return data.get(p);
	}
	
	// COMMANDES

	@Override
	public void addEntry(C p, N n) {
		Contract.checkCondition((p != null) && !contains(p));
		Contract.checkCondition(n != null);
		List<N> list = new ArrayList<N>();
		list.add(n);
		data.put(p, list);
	}

	@Override
	public void addEntry(C p, List<N> nums) {
		Contract.checkCondition((p != null) && !contains(p));
		Contract.checkCondition((nums != null) && (nums.size() > 0));
		for (int i = 0; i < nums.size(); i++) {
			Contract.checkCondition(nums.get(i) != null);
		}
		List<N> list = new ArrayList<N>();
		for (int i = 0; i < nums.size(); i++) {
			N n = nums.get(i);
			if (!list.contains(n)) {
				list.add(n);
			}
		}
		data.put(p, list);
	}

	@Override
	public void addPhoneNumber(C p, N n) {
		Contract.checkCondition(p != null);
		Contract.checkCondition(n != null && !n.equals(""));
		if (contains(p)) {
			if (!phoneNumbers(p).contains(n)) {
				phoneNumbers(p).add(n);
			}
		} else {
			List<N> list = new ArrayList<N>();
			list.add(n);
			data.put(p, list);
		}
	}

	@Override
	public void clear() {
		data.clear();
	}

	@Override
	public void deletePhoneNumber(C p, N n) {
		Contract.checkCondition((p != null) && contains(p));
		Contract.checkCondition(n != null);
		if (phoneNumbers(p).contains(n)) {
			if (phoneNumbers(p).size() == 1) {
				removeEntry(p);
			} else {
				phoneNumbers(p).remove(n);
			}
		}
	}

	@Override
	public void removeEntry(C p) {
		Contract.checkCondition(p != null);
		data.remove(p);
	}

}
