package serie10.model;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.util.Observable;
import java.util.Observer;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.MutableComboBoxModel;
import javax.swing.SwingUtilities;

public class Splitter {
	
	// ATTRIBUTS
	
	private JFrame frame;
	private JTextField jtf_fileName;
	private JTextField jtf_fragSize;
	private JTextArea jta_descr;
	private JButton browseButton;
	private JButton splitButton;
	private JComboBox fragNbBox;
	private SplitManager model;
	
	// CONSTRUCTEUR
	
	public Splitter() {
		createModel();
		createView();
		placeComponents();
		createController();
	}
	
	// COMMANDES
	
	public void display() {
		refresh(null);
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
	}
	
	// OUTILS
	
	private void createModel() {
		model = new StdSplitManager();
	}
	
	private void createView() {
		frame = new JFrame("Fragmenteur de fichiers");
		
		jtf_fileName = new JTextField();
		jtf_fileName.setColumns(20);
		
		jtf_fragSize = new JTextField();
		jtf_fragSize.setColumns(6);
		
		browseButton = new JButton("Parcourir...");
		splitButton = new JButton("Fragmenter!");
		
		fragNbBox = new JComboBox();
		jta_descr = new JTextArea();
	}
	
	private void placeComponents() {
		JPanel p = new JPanel();
		{
			p.add(new JLabel("Fichier à fragmenter:"));
			p.add(splitButton);
			p.add(browseButton);
		}
		frame.add(p, BorderLayout.NORTH);
		
		p = new JPanel(new GridLayout(2, 1));
		{
			JPanel q = new JPanel(new BorderLayout());
			{
				JPanel r = new JPanel(new GridLayout(2, 2));
				{
					JPanel s = new JPanel(new FlowLayout(FlowLayout.RIGHT));
					{
						s.add(new JLabel("Nb. fragments:"));
					}
					r.add(s);
					
					s = new JPanel(new FlowLayout(FlowLayout.LEFT));
					{
						s.add(fragNbBox);
					}
					r.add(s);
					
					s = new JPanel(new FlowLayout(FlowLayout.RIGHT));
					{
						s.add(new JLabel("Taille des fragments*:;"));
					}
					r.add(s);
					
					s = new JPanel(new FlowLayout(FlowLayout.LEFT));
					{
						s.add(jtf_fragSize);
						s.add(new JLabel("octets"));
					}
					r.add(s);
				}
				q.add(r);
			}
			p.add(q);
			
			q = new JPanel();
			{
				q.add(splitButton);
			}
			p.add(q);
		}
		frame.add(p, BorderLayout.WEST);
		frame.add(new JScrollPane(jta_descr), BorderLayout.CENTER);
		frame.add(new JLabel("(*) Il s'agit de la taille de chaque fragment"
				+ " à un octet près, sauf peut-être pour le dernier fragment"),
				BorderLayout.SOUTH);
	}
	
	private void createController() {
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		((Observable) model).addObserver(new Observer() {
			@Override
			public void update(Observable o, Object arg) {
				refresh(arg);
			}
		});
		
		jtf_fileName.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				model.changeFor(new File(jtf_fileName.getText()));
			}
		});
		
		browseButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				JFileChooser jfc = new JFileChooser();
				if (jfc.showOpenDialog(frame) == JFileChooser.APPROVE_OPTION) {
					model.changeFor(jfc.getSelectedFile());
				}
			}
		});
		
		fragNbBox.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				model.setSplitsNumber(fragNbBox.getSelectedIndex()+1);
			}
		});
		
		jtf_fragSize.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					long size = Long.valueOf(jtf_fragSize.getText());
					if (size < SplitManager.MIN_FRAGMENT_SIZE) {
						throw new NumberFormatException();
					}
					model.setSplitsSizes(size);
				} catch (NumberFormatException ex) {
					JOptionPane.showMessageDialog(
							null,
							"Impossible de fragmenter",
							"Erreur",
							JOptionPane.ERROR_MESSAGE
					);
				}
			}
		});
		
		splitButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					model.split();
					JOptionPane.showMessageDialog(
							null,
							"Fragmentation réussi",
							"Information",
							JOptionPane.INFORMATION_MESSAGE
					);
				} catch (IOException ex) {
					JOptionPane.showMessageDialog(
							null,
							"Fragmentation échec",
							"Erreur",
							JOptionPane.ERROR_MESSAGE
					);
				}
			}
		});
	}
	
	private void refresh(Object arg) {
		if (arg == ChangeConfig.FILE_CHANGE || arg == null) {
			fragNbBox.setEnabled(model.canSplit());
			jtf_fragSize.setEnabled(model.canSplit());
			splitButton.setEnabled(model.canSplit());
			jta_descr.setEnabled(model.canSplit());
			
			MutableComboBoxModel combo = new DefaultComboBoxModel();
			for (int i = 1; i <= model.getMaxFragmentNb(); i++) {
				combo.addElement(i);
			}
			fragNbBox.setModel(combo);
			
			if (!model.canSplit()) {
				jtf_fragSize.setText("");
				jta_descr.setText("Taille total du fichier :");
				jta_descr.append("non defini");
				jta_descr.append("\n\n");
				jta_descr.append("Description des fragments du fichier :\n");
				jta_descr.append(" non defini");
				jta_descr.append("\n\n");
				jta_descr.append("Taille moyenne d'un fragment : ");
				jta_descr.append("non defini");
				jta_descr.append(".");
			}
		}
		
		if (arg == ChangeConfig.FILE_CHANGE) {
			jtf_fileName.setText(model.getFile().getAbsolutePath());
			if (!model.canSplit()) {
				jtf_fileName.setText("C'est pas le nom d'un fichier valide");
				JOptionPane.showMessageDialog(
						null,
						"Pas un nom de fichier valide",
						"Erreur",
						JOptionPane.ERROR_MESSAGE
				);
			}
		}
		
		if (arg == ChangeConfig.CONFIG_CHANGE || model.canSplit()) {
			jtf_fragSize.setText(String.valueOf(model.getSplitsNames()[0]));
			jta_descr.setText("Taille totale du fichier : ");
			jta_descr.append(String.valueOf(model.getFile().length()));
			jta_descr.append(" octets");
			jta_descr.append("Description des fragments du fichier : \n");
			for (int i = 0; i < model.getSplitsSizes().length; i++) {
				jta_descr.append(" " + model.getSplitsNames()[i]
						+ " : " + model.getSplitsSizes()[i] + " octes\n");
			}
			jta_descr.append("\n");
			jta_descr.append("Taille moyenne d'un fragment : ");
			jta_descr.append(String.valueOf(
					model.getFile().length() / model.getSplitsSizes().length));
			jta_descr.append(" octets.");
			
			ActionListener[] listeners = fragNbBox.getActionListeners();
			for (ActionListener listener : listeners) {
				fragNbBox.removeActionListener(listener);
			}
			fragNbBox.setSelectedIndex(model.getSplitsSizes().length-1);
			for (ActionListener listener : listeners) {
				fragNbBox.addActionListener(listener);
			}
		}
	}
	
	// TYPE IMBRIQUE
	
	public enum ChangeConfig {
		CONFIG_CHANGE(),
		FILE_CHANGE();
		
		ChangeConfig() {
			// rien
		}
	}
	
	// POINT D'ENTREE
	
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				new Splitter().display();
			}
		});
	}
}
