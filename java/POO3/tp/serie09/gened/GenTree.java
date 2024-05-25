package gened.v5;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.EnumMap;
import java.util.Map;

import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JTree;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.DefaultTreeSelectionModel;
import javax.swing.tree.MutableTreeNode;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import gened.utils.DefaultTreeCellEditor2;
import gened.utils.GenCellEditor;
import gened.utils.GenCellRenderer;
import gened.utils.Person;

public class GenTree extends JTree {

    // ATTRIBUTS

    // tous les éléments du menu surgissant (qui correspondent aux
    // constantes de Item)
    private final Map<Item, JMenuItem> menuItems;
    private final JPopupMenu menu;

    // CONSTRUCTEURS

    public GenTree() {
        super((TreeNode) null);
        // VUE
        /*****************/
        /** A COMPLETER **/
        menuItems = buildMenuItemsMap();
        menu = buildPopupMenu();
        this.setCellRenderer(new GenCellRenderer());
        this.setEditable(true);
        this.setCellEditor(
        		new DefaultTreeCellEditor2(
        				this, new GenCellRenderer(), new GenCellEditor()));
        DefaultTreeSelectionModel selection = new DefaultTreeSelectionModel();
        selection.setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        this.setSelectionModel(selection);
        this.setComponentPopupMenu(menu);
        /*****************/
        // CONTROLEUR
        connectControllers();
        updateMenuItems();
    }

    // REQUETES

    @Override
    public DefaultTreeModel getModel() {
        return (DefaultTreeModel) super.getModel();
    }

    // OUTILS

    private JPopupMenu buildPopupMenu() {
        /*****************/
        /** A COMPLETER **/
    	JPopupMenu jpm = new JPopupMenu();
    	for (Item item : Item.STRUCT) {
    		if (item == null) {
    			jpm.addSeparator();
    		} else {
    			jpm.add(menuItems.get(item));
    		}
    	}
    	return jpm;
        /*****************/
    }
    
    private Map<Item, JMenuItem> buildMenuItemsMap() {
        /*****************/
        /** A COMPLETER **/
    	Map<Item, JMenuItem> map = new EnumMap<Item, JMenuItem>(Item.class);
    	for (Item item : Item.values()) {
    		map.put(item, new JMenuItem(item.toString()));
    	}
    	return map;
        /*****************/
    }

    private void connectControllers() {
        // écoute les changements de sélection sur l'arbre,
        // pour mettre à jour l'état d'activabilité des éléments de menu
        /*****************/
        /** A COMPLETER **/
    	this.addTreeSelectionListener(new TreeSelectionListener() {
			@Override
			public void valueChanged(TreeSelectionEvent e) {
				updateMenuItems();
			}
    	});
        /*****************/
        
        // écoute les éléments de menu,
        // pour appliquer leur comportement sur l'arbre
        /*****************/
        /** A COMPLETER **/
    	ActionListener listeners = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				JMenuItem jmi = (JMenuItem) e.getSource();
				for (Item item : Item.values()) {
					if (jmi.getText().equals(item.toString())) {
						item.applyOn(GenTree.this);
						break;
					}
				}
			}
    	};
    	for (JMenuItem jmi : menuItems.values()) {
    		jmi.addActionListener(listeners);
    	}
        /*****************/
    }

    /**
     * Activation ou désactivation de chaque JMenuItem selon l'état de l'arbre.
     */
    private void updateMenuItems() {
        for (Map.Entry<Item, JMenuItem> entry : menuItems.entrySet()) {
            Item i = entry.getKey();
            JMenuItem jmi = entry.getValue();
            jmi.setEnabled(i.getEnabledValue(this));
        }
    }
    
    // TYPES IMBRIQUES

    private enum Item {
        CREATE_ROOT() {
            /**
             * Retourne true ssi t n'a pas de racine.
             */
            @Override boolean getEnabledValue(GenTree t) {
                /*****************/
                /** A COMPLETER **/
            	return t.getModel().getRoot() == null;
                /*****************/
            }
            /**
             * @pre t n'a pas de racine
             * @post t possède une nouvelle racine (de genre aléatoire),
             *       et cette racine est maintenant sélectionnée
             */
            @Override void applyOn(GenTree t) {
                /*****************/
                /** A COMPLETER **/
            	assert getEnabledValue(t);
            	t.getModel().setRoot(new DefaultMutableTreeNode(new Person()));
            	t.setSelectionPath(new TreePath(t.getModel().getRoot()));
                /*****************/
            }
        },
        CREATE_BROTHER_BEFORE() {
            /**
             * Retourne true ssi un nœud de t est sélectionné, et ce n'est pas
             *  la racine.
             */
            @Override boolean getEnabledValue(GenTree t) {
                /*****************/
                /** A COMPLETER **/
            	DefaultTreeModel dtm = t.getModel();
            	TreePath path = t.getSelectionPath();
            	if (path == null) {
            		return false;
            	}
            	if (dtm.getRoot() != path.getLastPathComponent()
            			&& path.getLastPathComponent() != null) {
            		return true;
            	}
            	return false;
                /*****************/
            }
            /**
             * @pre un nœud de t est sélectionné, et ce n'est pas la racine
             * @post t possède un nouveau nœud (de genre aléatoire) juste avant
             *       le nœud précédemment sélectionné, et ce nouveau nœud est
             *       maintenant sélectionné
             */
            @Override void applyOn(GenTree t) {
                /*****************/
                /** A COMPLETER **/
            	assert getEnabledValue(t);
            	DefaultTreeModel dtm = t.getModel();
            	DefaultMutableTreeNode selected =
            			(DefaultMutableTreeNode) t.getSelectionPath().getLastPathComponent();
            	DefaultMutableTreeNode node =
            			new DefaultMutableTreeNode(new Person());
            	DefaultMutableTreeNode parent =
            			(DefaultMutableTreeNode) selected.getParent();
            	dtm.insertNodeInto(node, parent, parent.getIndex(selected));
            	t.setSelectionPath(new TreePath(node.getPath()));
                /*****************/
            }
        },
        CREATE_BROTHER_AFTER() {
            /**
             * Retourne true ssi un nœud de t est sélectionné, et ce n'est pas
             *  la racine.
             */
            @Override boolean getEnabledValue(GenTree t) {
                /*****************/
                /** A COMPLETER **/
            	DefaultTreeModel dtm = t.getModel();
            	TreePath path = t.getSelectionPath();
            	if (path == null) {
            		return false;
            	}
            	return (dtm.getRoot() != path.getLastPathComponent()
            			&& path.getLastPathComponent() != null);
                /*****************/
            }
            /**
             * @pre un nœud de t est sélectionné, et ce n'est pas la racine
             * @post t possède un nouveau nœud (de genre aléatoire) juste après
             *       le nœud précédemment sélectionné, et ce nouveau nœud est
             *       maintenant sélectionné
             */
            @Override void applyOn(GenTree t) {
                /*****************/
                /** A COMPLETER **/
            	assert getEnabledValue(t);
            	DefaultTreeModel dtm = t.getModel();
            	DefaultMutableTreeNode selected =
            			(DefaultMutableTreeNode) t.getSelectionPath().getLastPathComponent();
            	DefaultMutableTreeNode node =
            			new DefaultMutableTreeNode(new Person());
            	DefaultMutableTreeNode parent =
            			(DefaultMutableTreeNode) selected.getParent();
            	dtm.insertNodeInto(node, parent, parent.getIndex(selected)+1);
            	t.setSelectionPath(new TreePath(node.getPath()));
                /*****************/
            }
        },
        CREATE_FIRST_SON() {
            /**
             * Retourne true ssi un nœud de t est sélectionné, et qu'il n'a pas
             *  de fils.
             */
            @Override boolean getEnabledValue(GenTree t) {
                /*****************/
                /** A COMPLETER **/
            	TreePath path = t.getSelectionPath();
            	if (path == null) {
            		return false;
            	}
            	DefaultMutableTreeNode node =
            			(DefaultMutableTreeNode) path.getLastPathComponent();
            	return node != null && node.getChildCount() == 0;
                /*****************/
            }
            /**
             * @pre un nœud de t est sélectionné, et il n'a pas de fils
             * @post t possède un nouveau nœud (de genre aléatoire) juste
             *       au-dessous du nœud précédemment sélectionné, et ce nouveau
             *       nœud est maintenant sélectionné
             */
            @Override void applyOn(GenTree t) {
                /*****************/
                /** A COMPLETER **/
            	assert getEnabledValue(t);
            	DefaultMutableTreeNode selected =
            			(DefaultMutableTreeNode) t.getSelectionPath().getLastPathComponent();
            	DefaultMutableTreeNode node =
            			new DefaultMutableTreeNode(new Person());
            	t.getModel().insertNodeInto(node, selected, 0);
            	t.setSelectionPath(new TreePath(node.getPath()));
                /*****************/
            }
        },
        DELETE_SELECTION() {
            /**
             * Retourne true ssi un nœud de t est sélectionné.
             */
            @Override boolean getEnabledValue(GenTree t) {
                /*****************/
                /** A COMPLETER **/
            	TreePath path = t.getSelectionPath();
            	if (path == null) {
            		return false;
            	}
            	return path.getLastPathComponent() != null;
                /*****************/
            }
            /**
             * @pre un nœud de t est sélectionné
             * @post le nœud qui était sélectionné dans t a été retiré,
             *       et plus aucun nœud de t n'est sélectionné
             */
            @Override void applyOn(GenTree t) {
                /*****************/
                /** A COMPLETER **/
            	assert getEnabledValue(t);
            	DefaultMutableTreeNode selected =
            			(DefaultMutableTreeNode) t.getSelectionPath().getLastPathComponent();
            	if (selected == t.getModel().getRoot()) {
            		t.getModel().setRoot(null);
            		return;
            	}
            	t.getModel().removeNodeFromParent(selected);
            	t.setSelectionPath(null);
                /*****************/
            }
        };

        static final Item[] STRUCT = new Item[] {
                CREATE_ROOT, null,
                CREATE_FIRST_SON, null,
                CREATE_BROTHER_BEFORE, CREATE_BROTHER_AFTER, null,
                DELETE_SELECTION
        };
        
        @Override
        public String toString() {
            return name().toLowerCase().replace('_', ' ');
        }
        
        /**
         * Indique si le JMenuItem correspondant à this doit être activable
         *  ou non, en fonction de l'état de t.
         */
        abstract boolean getEnabledValue(GenTree t);

        /**
         * Applique sur t le comportement du JMenuItem correspondant à this.
         */
        abstract void applyOn(GenTree t);
    }
}
