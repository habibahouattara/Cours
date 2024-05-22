package serie07;

import java.awt.BorderLayout;
import java.awt.ComponentOrientation;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Observable;
import java.util.Observer;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

public class Swelling {
	
	// ATTRIBUTS
	
	private JFrame frame;
	private JButton modifyButton;
	private JButton resetButton;
	private JTextField factorField;
	private SwellingModel model;
	
	// CONSTRUCTEUR
	
	public Swelling() {
		createModel();
		createView();
		placeComponents();
		createController();
	}
	
	// COMMANDES
	
	public void display() {
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
		
		model.setMin(frame.getPreferredSize());
		model.setMax(Toolkit.getDefaultToolkit().getScreenSize());
		model.setCurrent(frame.getPreferredSize());
	}
	
	// OUTILS
	
	private void createModel() {
		model = new StdSwellingModel();
	}
	
	private void createView() {
		frame = new JFrame("Baudruche");
		frame.setPreferredSize(new Dimension(500, 100));
		
		modifyButton = new JButton("Modifier");
		resetButton = new JButton("Réinitialiser");
		
		factorField = new JTextField();
		factorField.setComponentOrientation(ComponentOrientation.RIGHT_TO_LEFT);
		factorField.setPreferredSize(new Dimension(50, 20));
	}
	
	private void placeComponents() {
		JPanel p = new JPanel();
		{ //--
			p.add(new JLabel("Facteur: "));
			p.add(factorField);
			p.add(new JLabel("%"));
			p.add(modifyButton);
		} //--
		frame.add(p, BorderLayout.NORTH);
		
		p = new JPanel();
		{ //--
			p.add(resetButton);
		} //--
		frame.add(p, BorderLayout.SOUTH);
	}
	
	private void createController() {
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		((Observable) model).addObserver(new Observer() {
			@Override
			public void update(Observable o, Object arg) {
				Dimension d = model.current();
				frame.setSize(d);
			}
		});
		
		modifyButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					double value = Double.parseDouble(factorField.getText());
					if (!model.isValidScaleFactor(value)) {
						throw new NumberFormatException();
					} else {
						model.scaleCurrent(value);
					}
				} catch (NumberFormatException ex) {
					factorField.selectAll();
					factorField.requestFocusInWindow();
					JOptionPane.showMessageDialog(
							null,
							"Valeur inattendue:" + factorField.getText(),
							"Erreur",
							JOptionPane.ERROR_MESSAGE
					);
				}
				frame.setLocationRelativeTo(null);
			}
		});
		
		resetButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				model.setCurrent(model.min());
				frame.setLocationRelativeTo(null);
			}
		});
	}
	
	// POINT D'ENTREE
	
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				new Swelling().display();
			}
		});
	}
}
