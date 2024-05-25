package phrase.view;

import java.awt.BorderLayout;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Collection;

import javax.swing.BorderFactory;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JComboBox;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import phrase.model.Filter;
import phrase.model.FilteringListModel;
import phrase.model.MarkableFilteringListModel;
import phrase.model.StdFilteringListModel;
import util.Contract;

public class FilteringPane extends JPanel {

    // ATTRIBUTS

    private final MarkableFilteringListModel listModel;
    private final JComboBox allFilters;
    private final JTextField filterValue;
    private final JList filteringList;

    // CONSTRUCTEURS

    public FilteringPane(Filter[] filters) {
        super(new BorderLayout());

        // MODELE
        listModel = new MarkableFilteringListModel();
        // VUE
        allFilters = new JComboBox(new DefaultComboBoxModel(filters));
        filterValue = new JTextField();
        filteringList = new JList(listModel);
        filteringList.setCellRenderer(new ItalicListCellRenderer());
        placeComponents();
        // CONTROLEUR
        connectControllers();
        
        setCurrentFilterFromSelectedItem();
    }

    // REQUETES

    public JComboBox getCombo() {
        return allFilters;
    }

    public JTextField getTextField() {
        return filterValue;
    }

    public JList getList() {
        return filteringList;
    }

    // COMMANDES

    /**
     * Ajoute un élément à la fin du modèle de getList().
     * @pre <pre>
     *     element != null </pre>
     * @post <pre>
     *     Let m ::= (FilteringListModel) getList().getModel()
     *     m.getUnfilteredSize() == old m.getUnfilteredSize() + 1
     *     m.getUnfilteredElementAt(m.getUnfilteredSize() - 1) == element </pre>
     */
    public void addElement(String element) {
        Contract.checkCondition(element != null);

        listModel.addElement(element);
    }
    
    /**
     * Supprime tous les éléments du modèle de getList().
     */
    public void clear() {
        listModel.clearElements();
    }

    /**
     * Remplace les éléments du modèle de getList() par ceux de c.
     * @pre <pre>
     *     c != null </pre>
     * @post <pre>
     *     Let m ::= (FilteringListModel) getList().getModel()
     *     m.getUnfilteredSize() == c.getSize()
     *     forall s in c :
     *         exists int i, 0 <= i < m.getUnfilteredSize() :
     *             m.getUnfilteredElementAt(i) == s </pre>
     */
    public void setElements(Collection<String> c) {
        Contract.checkCondition(c != null);

        listModel.setElements(c);
    }

    // OUTILS

    private void placeComponents() {
        JPanel p = new JPanel(new BorderLayout());
        { //--
            p.setBorder(BorderFactory.createTitledBorder("Recherche"));
            p.add(allFilters, BorderLayout.WEST);
            p.add(filterValue, BorderLayout.CENTER);
        } //--
        add(p, BorderLayout.NORTH);

        JScrollPane jsp = new JScrollPane(filteringList);
        { //--
            jsp.setBorder(BorderFactory.createTitledBorder("Résultat"));
        } //--
        add(jsp, BorderLayout.CENTER);
    }

    private void connectControllers() {
        /*****************/
        /** A COMPLETER **/
    	filterValue.getDocument().addDocumentListener(new DocumentListener() {
			@Override
			public void insertUpdate(DocumentEvent e) {
				if (listModel.getFilter() != null) {
					listModel.getFilter().setValue(filterValue.getText());
				}
			}

			@Override
			public void removeUpdate(DocumentEvent e) {
				if (listModel.getFilter() != null) {
					listModel.getFilter().setValue(filterValue.getText());
				}
			}

			@Override
			public void changedUpdate(DocumentEvent e) {
				// rien
			}
    	});
    	
    	allFilters.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				setCurrentFilterFromSelectedItem();
				if (listModel.getFilter() != null) {
					listModel.getFilter().setValue(filterValue.getText());
				}
			}
    	});
    	
    	filteringList.addMouseListener(new MouseAdapter() {
    		@Override
    		public void mouseClicked(MouseEvent e) {
    			if (e.getClickCount() == 2) {
    				listModel.toogleMarked(
    						(String) filteringList.getSelectedValue());
    			}
    		}
    	});
        /*****************/
    }

    private void setCurrentFilterFromSelectedItem() {
        /*****************/
        /** A COMPLETER **/
    	Filter f = (Filter) allFilters.getSelectedItem();
    	listModel.setFilter(f);
        /*****************/
    }
}
