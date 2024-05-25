package gened.v5;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.event.TreeModelEvent;
import javax.swing.event.TreeModelListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.MutableTreeNode;
import javax.swing.tree.TreePath;

import gened.utils.DefaultTreeCellEditor2;
import gened.utils.GenCellEditor;
import gened.utils.GenCellRenderer;
import gened.utils.Person;

public class Genealogy {
	
	// ATTRIBUTS
	
	private JFrame frame;
	private GenTree tree;
	
	// CONSTRUCTEUR
	
	public Genealogy() {
		// VUE
		frame = createFrame();
		tree = new GenTree();
		tree.setModel(new DefaultTreeModel(null));
		placeComponents();
		// CONTROLEUR
		connectControllers();
	}
	
	// COMMANDES
	
	public void display() {
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
	}
	
	// OUTILS
	
	private JFrame createFrame() {
		JFrame jf = new JFrame();
		jf.setPreferredSize(new Dimension(980, 300));
		return jf;
	}
	
	private void placeComponents() {
		JScrollPane jsp = new JScrollPane();
		jsp.setViewportView(tree);
		frame.add(jsp, BorderLayout.CENTER);
	}
	
	private void connectControllers() {
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		tree.addTreeSelectionListener(new TreeSelectionListener() {
			@Override
			public void valueChanged(TreeSelectionEvent e) {
				frame.setTitle(changeTitle());
			}
		});
		
		tree.getModel().addTreeModelListener(new TreeModelListener() {
			@Override
			public void treeNodesChanged(TreeModelEvent e) {
				frame.setTitle(changeTitle());
			}

			@Override
			public void treeNodesInserted(TreeModelEvent e) {
				// rien
			}

			@Override
			public void treeNodesRemoved(TreeModelEvent e) {
				// rien
			}

			@Override
			public void treeStructureChanged(TreeModelEvent e) {
				// rien
			}
		});
	}
	
	private String changeTitle() {
		TreePath path = tree.getSelectionPath();
		if (path != null) {
			DefaultMutableTreeNode dmtn =
					(DefaultMutableTreeNode) path.getLastPathComponent();
			Person p = (Person) dmtn.getUserObject();
			StringBuffer sb = new StringBuffer();
			DefaultMutableTreeNode parent =
					(DefaultMutableTreeNode) dmtn.getParent();
			while (parent != null) {
				Person lastPerson = p;
				p = (Person) parent.getUserObject();
				sb.append(", " + lastPerson.getGender().getDesc()
						+ " de " + p.getName());
				parent = (DefaultMutableTreeNode) parent.getParent();
			}
			return sb.toString();
		}
		return "";
	}
	
	// POINT D'ENTREE
	
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				new Genealogy().display();
			}
		});
	}
}
