package layouts;

import static java.awt.Component.CENTER_ALIGNMENT;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.util.LinkedHashMap;
import java.util.Map;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

public class BoxDistrib extends Distrib {
    
    // CONSTANTES DE CLASSE

    private static final int EXTRA_SPACE = 38;
    private static final int BIG_SPACE = 12;
    private static final int SMALL_SPACE = 6;
    
    // ATTRIBUTS
    
    // Association entre les boutons de boisson et les labels qui les suivent
    private Map<JButton, JLabel> cmds;

    // CONSTRUCTEURS

    public BoxDistrib() {
        super("BoxDistrib de boisson");
    }

    // OUTILS

    @Override
    protected void createView(String title) {
        super.createView(title);
        cmds = buildCmds();
        config();
    }
    
    /**
     * Construction de la map qui associe un label à chaque bouton de boisson.
     */
    private Map<JButton, JLabel> buildCmds() {
        Map<JButton, JLabel> r = new LinkedHashMap<JButton, JLabel>();
        r.put(jb(B_ORANGE), jl(L_ORANGE));
        r.put(jb(B_CHOCO), jl(L_CHOCO));
        r.put(jb(B_COFFEE), jl(L_COFFEE));
        return r;
    }
    
    /**
     * Adapte la taille de certains composants :
     * - les boutons de boisson ont tous la largeur du plus large d'entre eux
     * - les boutons d'action (insert et cancel) ont la même largeur
     * 
     * - la largeur max des champs de texte reste infinie horizontalement,
     *   mais leur hauteur max est ramenée à leur hauteur préférée
     * 
     * - le bouton take sera horizontalement centré dans sa box verticale
     * - le label d'information sur le rendu de monnaie tout au nord
     *   sera horizontalement centré dans sa box verticale
     * - le label d'information sur le crédit actuel juste au dessous
     *   sera horizontalement centré dans sa box verticale.
     */
    private void config() {
        linkButtonSizes(jb(B_ORANGE), jb(B_CHOCO), jb(B_COFFEE));
        linkButtonSizes(jb(B_INSERT), jb(B_CANCEL));
        
        for (JTextField tf : jtfs()) {
            setHorizontallyExpandableOnly(tf);
        }
        
        setHorizontallyCentered(jb(B_TAKE), jl(L_INFO), jl(L_CREDIT));
    }

    /**
     * Tous les boutons passés en paramètre adoptent la largeur du plus large
     *  d'entre eux.
     */
    private static void linkButtonSizes(JButton... buttons) {
        /*****************/
        /** A COMPLETER **/
    	int max = 0;
    	for (JButton btn : buttons) {
    		max = Math.max(max,  btn.getPreferredSize().width);
    	}
    	for (JButton btn : buttons) {
    		Dimension d = btn.getPreferredSize();
    		d.setSize(max, d.getHeight());
    		btn.setMaximumSize(d);
    	}
        /*****************/
    }
    
    /**
     * La largeur maximale de tf reste la même (l'infini), mais sa hauteur
     *  maximale est ramenée à sa hauteur préférée.
     */
    private static void setHorizontallyExpandableOnly(JTextField tf) {
        /*****************/
        /** A COMPLETER **/
    	Dimension d = tf.getMaximumSize();
    	d.setSize(d.getWidth(), tf.getPreferredSize().getHeight());
    	tf.setMaximumSize(d);
        /*****************/
    }
    
    /**
     * Tous les composants passés en paramètre sont configurés pour se centrer
     *  horizontalement dans une boite verticale.
     */
    private static void setHorizontallyCentered(JComponent... comps) {
        /*****************/
        /** A COMPLETER **/
    	for (JComponent jc : comps) {
    		jc.setAlignmentX(CENTER_ALIGNMENT);
    	}
        /*****************/
    }

    @Override
    protected void placeComponents() {
        /*****************/
        /** A COMPLETER **/
    	Box box1 = Box.createVerticalBox();
    	{
    		box1.add(Box.createRigidArea(new Dimension(0,BIG_SPACE)));
    		box1.add(jl(L_INFO));
    		box1.add(Box.createRigidArea(new Dimension(0,BIG_SPACE)));
    		box1.add(jl(L_CREDIT));
    		box1.add(Box.createRigidArea(new Dimension(0,BIG_SPACE)));
    		
    		Box box2 = Box.createHorizontalBox();
    		{
    			box2.add(Box.createRigidArea(new Dimension(BIG_SPACE,0)));
    			
    			Box box3 = Box.createVerticalBox();
    			{
    				Box box4 = null;
    				for (JButton jb : cmds.keySet()) {
    					box4 = Box.createHorizontalBox();
    					{
    						box4.setAlignmentX(Box.LEFT_ALIGNMENT);;
    						box4.add(jb);
    						box4.add(Box.createRigidArea(
    								new Dimension(SMALL_SPACE,0)));
    						box4.add(cmds.get(jb));
    					}
    					box3.add(box4);
    					box3.add(Box.createRigidArea(
    							new Dimension(0,SMALL_SPACE)));
    				}
    				box4 = Box.createHorizontalBox();
    				{
    					box4.setAlignmentX(Box.LEFT_ALIGNMENT);;
    					box4.add(jl(L_DRINK));
    					box4.add(Box.createRigidArea(
    							new Dimension(SMALL_SPACE,0)));
    					box4.add(jtf(F_DRINK));
    				}
    				box3.add(box4);
    			}
    			box2.add(box3);
    			box2.add(Box.createRigidArea(new Dimension(BIG_SPACE,0)));
    			
    			box3 = Box.createVerticalBox();
    			{
    				Box box4 = Box.createHorizontalBox();
    				{
    					box4.setAlignmentX(Box.LEFT_ALIGNMENT);;
    					box4.add(jb(B_INSERT));
    					box4.add(Box.createRigidArea(
    							new Dimension(SMALL_SPACE,0)));
    					box4.add(jtf(F_INSERT));
    					box4.add(Box.createRigidArea(
    							new Dimension(0,SMALL_SPACE)));
    					box4.add(jl(L_INSERT));
    				}
    				box3.add(box4);
    				box3.add(Box.createRigidArea(
    						new Dimension(0,SMALL_SPACE)));
    				
    				box4 = Box.createHorizontalBox();
    				{
    					box4.setAlignmentX(Box.LEFT_ALIGNMENT);;
    					box4.add(jb(B_CANCEL));
    				}
    				box3.add(box4);
    				box3.add(Box.createRigidArea(
    						new Dimension(0,EXTRA_SPACE)));
    				
    				box4 = Box.createHorizontalBox();
    				{
    					box4.setAlignmentX(Box.LEFT_ALIGNMENT);;
    					box4.add(jl(L_BACK_PRE));
    					box4.add(Box.createRigidArea(
    							new Dimension(SMALL_SPACE,0)));
    					box4.add(jtf(F_BACK));
    					box4.add(Box.createRigidArea(
    							new Dimension(SMALL_SPACE,0)));
    					box4.add(jl(L_BACK_POST));
    				}
    				box3.add(box4);
    			}
    			box2.add(box3);
    			box2.add(Box.createRigidArea(new Dimension(BIG_SPACE,0)));
    		}
    		box1.add(box2);
    		box1.add(Box.createRigidArea(new Dimension(0,BIG_SPACE)));
    		box1.add(jb(B_TAKE));
    		box1.add(Box.createVerticalStrut(BIG_SPACE));
    		box1.add(Box.createVerticalGlue());
    	}
    	frame().add(box1);
        /*****************/
    }

    // POINT D'ENTREE

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new BoxDistrib().display();
            }
        });
    }
}
