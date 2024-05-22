package serie09;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Observable;
import java.util.Observer;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

import serie06.CoinTypes;
import serie06.DrinkTypes;
import serie06.DrinksMachineModel;

public class DrinksMachine {
	
	// ATTRIBUTS
	
	private JFrame frame;
	private JLabel lb_changeInfo;
	private JLabel lb_creditInfo;
	private JButton insertButton;
	private JButton consumeButton;
	private JButton cancelButton;
	private JTextField jtf_insertCoin;
	private JTextField jtf_takeDrink;
	private JTextField jtf_takeChange;
	private Map<JButton, DrinkTypes> drinksButtonMap;
	private DrinksMachineModel model;
	
	// CONSTRUCTEUR

	public DrinksMachine() {
		createModel();
		createView();
		placeComponents();
		createController();
	}
	
	// COMMANDES
	
	public void display() {
		refresh();
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
	}
	
	// OUTILS
	
	private void createModel() {
		model = new StdDrinksMachineModel();
		for (CoinTypes c : CoinTypes.values()) {
			model.fillCash(c,
					(int) (Math.random() * DrinksMachineModel.MAX_COIN + 1));
		}
	}
	
	private void createView() {
		frame = new JFrame("Distributeur de boissons");
		
		lb_changeInfo = new JLabel();
		lb_creditInfo = new JLabel();
		
		drinksButtonMap = new LinkedHashMap<JButton, DrinkTypes>();
		for (DrinkTypes d : DrinkTypes.values()) {
			drinksButtonMap.put(new JButton(d.toString()), d);
		}
		
		insertButton = new JButton("Insérer");
		cancelButton = new JButton("Annuler");
		consumeButton = new JButton("Prenez la boisson et/ou la monnaie");
		
		jtf_insertCoin = new JTextField();
		jtf_insertCoin.setHorizontalAlignment(JTextField.RIGHT);
		jtf_insertCoin.setColumns(5);
		
		jtf_takeDrink = new JTextField();
		jtf_takeDrink.setHorizontalAlignment(JTextField.CENTER);
		jtf_takeDrink.setEditable(false);
		jtf_takeDrink.setColumns(10);
		
		jtf_takeChange = new JTextField();
		jtf_takeChange.setHorizontalAlignment(JTextField.RIGHT);
		jtf_takeChange.setEditable(false);
		jtf_takeChange.setColumns(5);
	}
	
	private void placeComponents() {
		JPanel p = new JPanel(new GridLayout(2, 1));
		{ //--
			JPanel q = new JPanel();
			{ //--
				q.add(lb_changeInfo);
			} //--
			p.add(q);
			
			q = new JPanel();
			{ //--
				q.add(lb_creditInfo);
			} //--
			p.add(q);
		} //--
		frame.add(p, BorderLayout.NORTH);
		
		p = new JPanel(new GridLayout(0, 2));
		{ //--
			for (JButton btn : drinksButtonMap.keySet()) {
				p.add(btn);
				p.add(new JLabel(" " + String.valueOf(
						drinksButtonMap.get(btn).getPrice() + " cents")));
			}
		} //--
		frame.add(p, BorderLayout.WEST);
		
		p = new JPanel();
		{ //--
			JPanel q = new JPanel(new GridLayout(2, 2));
			{ //--
				q.add(insertButton);
				JPanel r = new JPanel();
				{ //--
					r.add(jtf_insertCoin);
					r.add(new JLabel("cents"));
				} //--
				q.add(r);
				q.add(cancelButton);
			} //--
			p.add(q);
		} //--
		frame.add(p, BorderLayout.EAST);
		
		p = new JPanel(new GridLayout(2, 1));
		{ //--
			JPanel q = new JPanel(new GridLayout(1, 2));
			{ //--
				JPanel r = new JPanel();
				{ //--
					r.add(new JLabel("Boisson: "));
					r.add(jtf_takeDrink);
				} //--
				q.add(r);
				
				r = new JPanel();
				{ //--
					r.add(new JLabel("Monnaie: "));
					r.add(jtf_takeChange);
					r.add(new JLabel(" cents"));
				} //--
				q.add(r);
			} //--
			p.add(q);
			
			q = new JPanel();
			{ //--
				q.add(consumeButton);
			} //--
			p.add(q);
		} //--
		frame.add(p, BorderLayout.SOUTH);
	}
	
	private void createController() {
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		((Observable) model).addObserver(new Observer() {
			@Override
			public void update(Observable o, Object arg) {
				refresh();
			}
		});
		
		ActionListener listener = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				DrinkTypes d = drinksButtonMap.get(e.getSource());
				if (model.getCreditAmount() >= d.getPrice()) {
					if (model.getLastDrink() == null) {
						model.selectDrink(d);
					} else {
						JOptionPane.showMessageDialog(
								null,
								"Prenez votre boisson",
								"Erreur",
								JOptionPane.INFORMATION_MESSAGE
						);
					}
				} 
			}
		};
		for (JButton btn : drinksButtonMap.keySet()) {
			btn.addActionListener(listener);
		}
		
		insertButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					CoinTypes c = CoinTypes.getCoinType(
							Integer.parseInt(jtf_insertCoin.getText()));
					if (c == null) {
						throw new NumberFormatException();
					} else {
						jtf_insertCoin.setText("");
						model.insertCoin(c);
					}
				} catch (NumberFormatException ex) {
					jtf_insertCoin.selectAll();
					jtf_insertCoin.requestFocusInWindow();
					JOptionPane.showMessageDialog(
							null,
							"Valeur inattendue:" + jtf_insertCoin.getText(),
							"Erreur",
							JOptionPane.ERROR_MESSAGE
					);
				}
			}
		});
		
		cancelButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				model.cancelCredit();
			}
		});
		
		consumeButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				model.takeChange();
				model.takeDrink();
			}
		});
	}
	
	private void refresh() {
		if (model.canGetChange()) {
			lb_changeInfo.setText("Cet appareil rend la monnaie");
		} else {
			lb_changeInfo.setText("Cet appareil ne rend pas le monnaie");
		}
		lb_creditInfo.setText("Vous avez " + model.getCreditAmount() + " cents");
		
		DrinkTypes d = model.getLastDrink();
		if (d != null) {
			jtf_takeDrink.setText(model.getLastDrink().toString());
			jtf_takeChange.setText(String.valueOf(model.getChangeAmount()));
		} else {
			jtf_takeDrink.setText("");
			jtf_takeChange.setText("");
		}
		
		for (JButton btn : drinksButtonMap.keySet()) {
			if (model.getDrinkNb(drinksButtonMap.get(btn)) > 0) {
				btn.setEnabled(true);
			} else {
				btn.setEnabled(false);
			}
		}
	}
	
	// POINT D'ENTREE
	
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				new DrinksMachine().display();
			}
		});
	}
}
