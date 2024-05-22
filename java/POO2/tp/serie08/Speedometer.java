package serie08;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.Map;
import java.util.Observable;
import java.util.Observer;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.SwingUtilities;

import serie08.SpeedometerModel.SpeedUnit;

public class Speedometer {
	
	// ATTRIBUTS
	
	private JFrame frame;
	private Map<JRadioButton, SpeedUnit> speedUnitButtonsMap;
	private Map<SpeedUnit, JRadioButton> radioButtonsMap;
	private JButton slowDownButton;
	private JButton speedUpButton;
	private JButton powerButton;
	private GraphicSpeedometer gs;
	private SpeedometerModel model;
	
	// CONSTRUCTEUR
	
	public Speedometer() {
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
		model = new StdSpeedometerModel(5, 100);
	}
	
	private void createView() {
		frame = new JFrame("Speedometer");
		speedUnitButtonsMap = new HashMap<JRadioButton, SpeedUnit>();
		radioButtonsMap = new EnumMap<SpeedUnit, JRadioButton>(SpeedUnit.class);
		for (SpeedUnit u : SpeedUnit.values()) {
			JRadioButton jrb = new JRadioButton(u.toString());
			speedUnitButtonsMap.put(jrb, u);
			radioButtonsMap.put(u, jrb);
		}
		slowDownButton = new JButton("-");
		slowDownButton.setEnabled(false);
		speedUpButton = new JButton("+");
		speedUpButton.setEnabled(false);
		powerButton = new JButton("Turn ON");
		gs = new GraphicSpeedometer(model);
	}
	
	private void placeComponents() {
		JPanel p = new JPanel(new GridLayout(0, 1));
		{ //--
			JPanel q = new JPanel();
			{ //--
				JPanel r = new JPanel(new GridLayout(0, 1));
				{ //--
					r.setBorder(BorderFactory.createEtchedBorder());
					for (SpeedUnit u : SpeedUnit.values()) {
						r.add(radioButtonsMap.get(u));
					}
				} //--
				q.add(r);
			} //--
			p.add(q);
			
			q = new JPanel();
			{ //--
				JPanel r = new JPanel(new GridLayout(1, 0));
				{ //--
					r.add(slowDownButton);
					r.add(speedUpButton);
				} //--
				q.add(r);
			} //--
			p.add(q);
			
			q = new JPanel();
			{ //--
				q.add(powerButton);
			} //--
			p.add(q);
		} //--
		frame.add(p, BorderLayout.WEST);
		frame.add(gs, BorderLayout.CENTER);
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
				model.setUnit(speedUnitButtonsMap.get(e.getSource()));
			}
		};
		for (JRadioButton jrb : speedUnitButtonsMap.keySet()) {
			jrb.addActionListener(listener);
		}
		
		slowDownButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (model.isOn()) {
					model.slowDown();
				}
			}
		});
		
		speedUpButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (model.isOn()) {
					model.speedUp();
				}
			}
		});
		
		powerButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (model.isOn()) {
					model.turnOff();
				} else {
					model.turnOn();
				}
			}
		});
		ButtonGroup btnGroup = new ButtonGroup();
		for (JRadioButton jrb : speedUnitButtonsMap.keySet()) {
			btnGroup.add(jrb);
		}
	}
	
	private void refresh() {
		if (!model.isOn()) {
			powerButton.setText("Turn OFF");
			slowDownButton.setEnabled(false);
			speedUpButton.setEnabled(false);
		} else {
			powerButton.setText("Turn ON");
			if (model.getSpeed() == 0) {
				slowDownButton.setEnabled(false);
			} else {
				slowDownButton.setEnabled(true);
			}
			if (model.getSpeed() == model.getMaxSpeed()) {
				speedUpButton.setEnabled(false);
			} else {
				speedUpButton.setEnabled(true);
			}
		}
		radioButtonsMap.get(model.getUnit()).setSelected(true);
	}
	
	// POINT D'ENTREE
	
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				new Speedometer().display();
			}
		});
	}
}
