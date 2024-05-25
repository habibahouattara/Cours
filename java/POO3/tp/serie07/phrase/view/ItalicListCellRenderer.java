package phrase.view;

import java.awt.Component;
import java.awt.Font;

import javax.swing.DefaultListCellRenderer;
import javax.swing.JList;
import javax.swing.ListCellRenderer;

import phrase.model.MarkableFilteringListModel;

public class ItalicListCellRenderer implements ListCellRenderer {
	
	// ATTRIBUTS
	
	private DefaultListCellRenderer delegate;
	
	// CONSTRUCTEUR
	
	public ItalicListCellRenderer() {
		delegate = new DefaultListCellRenderer();
	}
	
	// REQUETE
	
	public Component getListCellRendererComponent(JList list,
			Object value,
			int index,
			boolean isSelected,
			boolean cellHasFocus) {
		Component result = delegate.getListCellRendererComponent(list,
				value,
				index,
				isSelected,
				cellHasFocus);
		MarkableFilteringListModel listModel =
				(MarkableFilteringListModel) list.getModel();
		if (listModel.isMarked((String) value)) {
			result.setFont(result.getFont().deriveFont(Font.ITALIC));
		} else {
			result.setFont(result.getFont().deriveFont(Font.BOLD));
		}
		return result;
	}
}
