package phrase.model;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import javax.swing.AbstractListModel;

import util.Contract;

public class StdFilteringListModel extends AbstractListModel
		implements FilteringListModel {

	// ATTRIBUTS
	
	private Filter filter;
	private List<String> unfilteredList;
	private List<String> filteredList;
	
	// CONSTRUCTEUR
	
	public StdFilteringListModel() {
		unfilteredList = new ArrayList<String>();
		filteredList = new ArrayList<String>();
	}
	
	// REQUETES
	
	@Override
	public String getElementAt(int i) {
		Contract.checkCondition(i >= 0 && i < getSize());
		return filteredList.get(i);
	}

	@Override
	public Filter getFilter() {
		return filter;
	}

	@Override
	public int getSize() {
		return filteredList.size();
	}

	@Override
	public String getUnfilteredElementAt(int i) {
		Contract.checkCondition(i >= 0 && i < getSize());
		return unfilteredList.get(i);
	}

	@Override
	public int getUnfilteredSize() {
		return unfilteredList.size();
	}
	
	// COMMANDES
	
	@Override
	public void addElement(String element) {
		Contract.checkCondition(element != null);
		unfilteredList.add(element);
		if (filter == null || filter.accept(element)) {
			filteredList.add(element);
			fireIntervalAdded(element,
					filteredList.size() - 1,
					filteredList.size() - 1);
		}
	}

	@Override
	public void clearElements() {
		unfilteredList = new ArrayList<String>();
		filteredList.clear();
		fireContentsChanged(this, 0, 0);
	}

	@Override
	public void setElements(Collection<String> c) {
		Contract.checkCondition(c != null);
		unfilteredList = new ArrayList<String>(c);
		filteredList.clear();
		if (filter == null) {
			filteredList = new ArrayList<String>(c);
		} else {
			for (String s : unfilteredList) {
				if (filter.accept(s)) {
					filteredList.add(s);
				}
			}
		}
		fireContentsChanged(this, 0, Math.max(0,  filteredList.size() - 1));
	}

	@Override
	public void setFilter(Filter filter) {
		if (this.filter == filter) {
			return;
		}
		this.filter = filter;
		filteredList.clear();
		for (String s : unfilteredList) {
			if (filter == null || filter.accept(s)) {
				filteredList.add(s);
			}
		}
		if (filter != null) {
			PropertyChangeListener listener = new PropertyChangeListener() {
				@Override
				public void propertyChange(PropertyChangeEvent evt) {
					filteredList.clear();
					for (String s : unfilteredList) {
						if (StdFilteringListModel.this.filter.accept(s)) {
							filteredList.add(s);
						}
					}
					fireContentsChanged(this,
							0,
							Math.max(0,  filteredList.size() - 1));
				}
			};
			filter.addPropertyChangeListener(Filter.PROP_VALUE, listener);
		} else {
			filteredList = new ArrayList<String>(unfilteredList);
		}
		fireContentsChanged(this, 0, Math.max(0,  filteredList.size() - 1));
	}

}
