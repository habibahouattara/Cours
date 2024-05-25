package gened.utils;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;

import javax.swing.JLabel;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;

/**
 * Il est nécessaire de dériver DefaultTreeCellRenderer, on ne peut pas se
 *  contenter d'en utiliser une instance.
 * En effet, plus loin dans le TP, nous allons utiliser un DefaultTreeCellEditor
 *  qui a besoin d'un DefaultTreeCellRenderer.
 * Donc si l'on procédait par délégation plutôt que par héritage, on ne pourrait
 *  pas utiliser GenCellRenderer...
 */
public class GenCellRenderer extends DefaultTreeCellRenderer {

    // CONSTANTES

    public static final int FONT_SIZE = 20;
    public static final String FONT_NAME = "Verdana";
    

    // CONSTRUCTEURS

    public GenCellRenderer() {
        setFont(new Font(FONT_NAME, Font.PLAIN, FONT_SIZE));
    }

    // REQUETES

    @Override
    public Component getTreeCellRendererComponent(
            JTree tree, Object value, boolean sel, boolean expanded,
            boolean leaf, int row, boolean hasFocus) {
        /*****************/
        /** A COMPLETER **/
    	DefaultMutableTreeNode node = (DefaultMutableTreeNode) value;
    	Person p = (Person) node.getUserObject();
    	setLeafIcon(p.getGender().getImage());
    	setOpenIcon(p.getGender().getImage());
    	setClosedIcon(p.getGender().getImage());
    	setIcon(p.getGender().getImage());
    	setBackgroundSelectionColor(p.getGender().getBackgroundSelectionColor());
    	setBorderSelectionColor(p.getGender().getBorderSelectionColor());
    	JLabel label = (JLabel) super.getTreeCellRendererComponent(
    			tree,
    			value,
    			sel,
    			expanded,
    			leaf,
    			row,
    			hasFocus);
    	return label;
        /*****************/
    }
    
    /**
     * Beug : sur Mac les icônes sont tronquées en hauteur.
     * Explication :
     * - Si l'on ne fixe pas la hauteur des lignes d'un JTree, c'est le
     *   renderer qui est utilisé pour calculer la hauteur de chaque ligne.
     * - Or DefaultTreeCellRenderer redéfinit ses méthodes invalidate,
     *   validate et revalidate avec un corps vide.
     * - Par conséquent, en tant que JLabel, le renderer ne peut pas
     *   recalculer correctement sa taille quand on change son icône.
     * Fix :
     * - Le renderer calcule sa hauteur préférée comme le max de la hauteur
     *   de son icône et de sa hauteur préférée actuelle.
     * Rq. :
     * - On pourrait aussi penser à fixer la hauteur des lignes du JTree,
     *   mais il faudrait connaître à l'avance la hauteur maximale des icônes
     *   utilisées.
     * - Par conséquent, cette solution serait moins générale que celle
     *   adoptée ici.
     */
    @Override
    public Dimension getPreferredSize() {
        Dimension d = super.getPreferredSize();
        if (d != null) {
            int h = getIcon().getIconHeight();
            if (d.height < h) {
                d.height = h;
            }
        }
        return d;
    }
}
