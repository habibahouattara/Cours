package layouts;

import java.awt.BorderLayout;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Map.Entry;

import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.GroupLayout.ParallelGroup;
import javax.swing.GroupLayout.SequentialGroup;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

public class GroupDistrib extends Distrib {
	
	// ATTRIBUTS STATIQUE
	
	private static final int BIG_SPACE = 12;
	private static final int SMALL_SPACE = 6;
    
    // ATTRIBUTS
    
    private Map<JButton, JLabel> cmds;
    private GroupLayout lmp;
    
    // CONSTRUCTEURS

    public GroupDistrib() {
        super("GroupDistrib de boissons");
    }

    // OUTILS

    @Override
    protected void createView(String title) {
        super.createView(title);
        cmds = buildCmds();
    }
    
    private Map<JButton, JLabel> buildCmds() {
        Map<JButton, JLabel> r = new LinkedHashMap<JButton, JLabel>();
        r.put(jb(B_ORANGE), jl(L_ORANGE));
        r.put(jb(B_CHOCO), jl(L_CHOCO));
        r.put(jb(B_COFFEE), jl(L_COFFEE));
        return r;
    }
    
    @Override
    protected void placeComponents() {
        /*****************/
        /** A COMPLETER **/
    	int pref = GroupLayout.PREFERRED_SIZE;
    	int def = GroupLayout.DEFAULT_SIZE;
    	int bSize = 0;
    	for (JButton jb : cmds.keySet()) {
    		if (bSize < jb.getPreferredSize().width) {
    			bSize = jb.getPreferredSize().width;
    		}
    	}
    	int size = Math.max(jb(B_INSERT).getPreferredSize().width,
    			jb(B_CANCEL).getPreferredSize().width);
    	JPanel p = new JPanel(null);
    	{
    		lmp = new GroupLayout(p);
    		p.setLayout(lmp);
    	}
    	
    	// horizontal
    	
    	ParallelGroup pGroup1 =
    			lmp.createParallelGroup(
    					Alignment.CENTER).addComponent(
    							jl(L_INFO)).addComponent(jl(L_CREDIT));
    	SequentialGroup seqGroup1 = lmp.createSequentialGroup().addGap(
    			BIG_SPACE);
    	ParallelGroup pGroup2 = lmp.createParallelGroup(Alignment.LEADING);
    	SequentialGroup seqGroup2 =
    			lmp.createSequentialGroup().addGroup(
    					lmp.createParallelGroup(
    							Alignment.LEADING).addComponent(
    									jb(B_ORANGE),
    									bSize,
    									bSize,
    									bSize).addComponent(
    											jb(B_CHOCO),
    											bSize,
    											bSize,
    											bSize).addComponent(
    													jb(B_COFFEE),
    													bSize,
    													bSize,
    													bSize)
    					).addGap(SMALL_SPACE).addGroup(
    							lmp.createParallelGroup(
    									Alignment.LEADING).addComponent(
    											jl(L_ORANGE)
    											).addComponent(
    													jl(L_CHOCO)
    													).addComponent(
    															jl(L_COFFEE)
    															)
    							).addGap(BIG_SPACE);
    	SequentialGroup seqGroup3 =
    			lmp.createSequentialGroup().addComponent(
    					jl(L_DRINK)
    					).addGap(
    							SMALL_SPACE).addComponent(
    									jtf(F_DRINK)
    									).addGap(BIG_SPACE);
    	pGroup2.addGroup(seqGroup2).addGroup(seqGroup3);
    	seqGroup1.addGroup(pGroup2);
    	
    	pGroup2 = lmp.createParallelGroup(Alignment.LEADING);
    	seqGroup2 =
    			lmp.createSequentialGroup().addGroup(
    					lmp.createParallelGroup(
    							Alignment.LEADING).addComponent(
    									jb(B_INSERT),
    									size,
    									size,
    									size).addComponent(
    											jb(B_CANCEL),
    											size,
    											size,
    											size)
    					).addGap(SMALL_SPACE).addComponent(
    							jtf(F_INSERT)).addGap(
    									SMALL_SPACE).addComponent(
    											jl(L_INSERT)).addGap(
    													BIG_SPACE);
    	pGroup2.addGroup(seqGroup2);
    	
    	seqGroup2 =
    			lmp.createSequentialGroup().addComponent(
    					jl(L_BACK_PRE)).addGap(
    							SMALL_SPACE).addComponent(
    									jtf(F_BACK)).addGap(
    											SMALL_SPACE).addComponent(
    													jl(L_BACK_POST)
    													).addGap(BIG_SPACE);
    	pGroup2.addGroup(seqGroup2);
    	seqGroup1.addGroup(pGroup2);
    	pGroup1.addGroup(seqGroup1).addComponent(jb(B_TAKE));
    	
    	lmp.setHorizontalGroup(pGroup1);
    	
    	// vertical
    	
    	seqGroup1 =
    			lmp.createSequentialGroup().addGap(
    					SMALL_SPACE).addComponent(
    							jl(L_INFO)).addGap(
    									SMALL_SPACE).addComponent(
    											jl(L_CREDIT)
    											).addGap(SMALL_SPACE);
    	pGroup1 = lmp.createParallelGroup(Alignment.LEADING);
    	seqGroup2 =
    			lmp.createSequentialGroup().addGroup(
    					lmp.createBaselineGroup(
    							false, true
    							).addComponent(
    									jb(B_ORANGE)
    									).addComponent(
    											jl(L_ORANGE)
    											)
    					).addGap(SMALL_SPACE).addGroup(
    							lmp.createBaselineGroup(
    									false, true).addComponent(
    											jb(B_CHOCO)).addComponent(
    													jl(L_CHOCO)
    													)).addGap(
    											SMALL_SPACE).addGroup(
    													lmp.createBaselineGroup(
    															false, true
    															).addComponent(
    																	jb(B_COFFEE)
    																	).addComponent(
    																			jl(L_COFFEE))
    													).addGap(SMALL_SPACE);
    	pGroup1.addGroup(seqGroup2);
    	
    	seqGroup2 =
    			lmp.createSequentialGroup().addGroup(
    					lmp.createBaselineGroup(
    							false, true
    							).addComponent(
    									jb(B_INSERT)
    									).addComponent(
    											jtf(F_INSERT)
    											).addComponent(
    													jl(L_INSERT))
    					).addGap(
    							SMALL_SPACE).addComponent(
    									jb(B_CANCEL),
    									pref,
    									def,
    									pref);
    	pGroup1.addGroup(seqGroup2);
    	seqGroup1.addGroup(pGroup1);
    	
    	pGroup1 = lmp.createParallelGroup(Alignment.LEADING);
    	pGroup2 =
    			lmp.createBaselineGroup(
    					false, true).addComponent(
    							jl(L_DRINK)).addComponent(jtf(F_DRINK));
    	pGroup1.addGroup(pGroup2);
    	
    	pGroup2 =
    			lmp.createBaselineGroup(
    					false, true).addComponent(
    							jl(L_BACK_PRE)).addComponent(
    									jtf(F_BACK)).addComponent(
    											jl(L_BACK_POST));
    	pGroup1.addGroup(pGroup2);
    	seqGroup1.addGroup(pGroup1).addGap(
    			SMALL_SPACE).addComponent(
    					jb(B_TAKE)).addGap(
    							BIG_SPACE);
    	
    	lmp.setVerticalGroup(seqGroup1);
    	frame().add(p);
        /*****************/
    }

    // POINT D'ENTREE

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new GroupDistrib().display();
            }
        });
    }
}
