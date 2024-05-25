package phrase.model;

import java.util.HashSet;
import java.util.Set;

import util.Contract;

public class MarkableFilteringListModel extends StdFilteringListModel
		implements FilteringListModel {
	
	// ATTRIBUTS
	
	private Set<String> markedElements;
	
	// CONSTRUCTEUR
	
	public MarkableFilteringListModel() {
		super();
		markedElements = new HashSet<String>();
	}
	
	// REQUETES
	
	public boolean isMarked(String s) {
		Contract.checkCondition(s != null);
		return markedElements.contains(s);
	}
	
	// COMMANDES
	
	public void toogleMarked(String s) {
		Contract.checkCondition(s != null);
		if (isMarked(s)) {
			markedElements.remove(s);
		} else {
			markedElements.add(s);
		}
		fireContentsChanged(this, 0, 0);
	}
}
