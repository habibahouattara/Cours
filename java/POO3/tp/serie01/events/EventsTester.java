package events;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowFocusListener;
import java.awt.event.WindowListener;
import java.awt.event.WindowStateListener;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;

public class EventsTester {
	
	// ATTRIBUTS
	
	private final JFrame mainFrame;
	private final JFrame testFrame;
	private final JButton btnNewTestFrame;
	private final JButton resetButton;
	private final Map<String, JTextArea> areaNameMap;
	private int n;
	
	// CONSTRUCTEUR
	
	public EventsTester() {
		// VUE
		mainFrame = createMainFrame();
		testFrame = createNewTestFrame();
		btnNewTestFrame = new JButton("Nouvelle fenêtre");
		resetButton = new JButton("RAZ compteur");
		areaNameMap = new LinkedHashMap<String, JTextArea>();
		n = 1;
		createEvent();
		stringAreaEvent();
		placeComponents();
		// CONTROLEUR
		connectControllers();
	}
	
	// COMMANDES
	
	public void displayMainFrame() {
		mainFrame.pack();
		mainFrame.setLocationRelativeTo(null);
		mainFrame.setVisible(true);
	}
	
	// OUTILS
	
	private JFrame createMainFrame() {
		JFrame jf = new JFrame("Test sur les évènements - Zone d'affichage");
		jf.setPreferredSize(new Dimension(800, 700));
		return jf;
	}
	
	private JFrame createNewTestFrame() {
		JFrame jf = new JFrame("Zone de test");
		jf.setPreferredSize(new Dimension(300, 100));
		jf.pack();
		jf.setLocationRelativeTo(mainFrame);
		jf.setVisible(true);
		jf.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		return jf;
	}
	
	private void placeComponents() {
		JPanel p = new JPanel();
		{ //--
			p.add(btnNewTestFrame);
			p.add(resetButton);
		} //--
		mainFrame.add(p, BorderLayout.NORTH);
		
		p = new JPanel(new GridLayout(3, 3));
		{ //--
			for (String s : areaNameMap.keySet()) {
				JScrollPane jsp = new JScrollPane(areaNameMap.get(s));
				jsp.setBorder(BorderFactory.createTitledBorder(s));
				p.add(jsp);
			}
		} //--
		mainFrame.add(p, BorderLayout.CENTER);
	}
	
	private void connectControllers() {
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		btnNewTestFrame.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (testFrame != null) {
					if (JOptionPane.showConfirmDialog(null,
							"Souhaitez-vous fermer la fenêtre de test?",
							"Confirmation",
							JOptionPane.YES_NO_OPTION)
							== JOptionPane.YES_OPTION) {
						for (String s : areaNameMap.keySet()) {
							areaNameMap.get(s).setText("");
						}
						n = 1;
						if (testFrame != null) {
							testFrame.dispose();
						}
						createNewTestFrame();
					}
				} else {
					createNewTestFrame();
				}
			}
		});
		
		resetButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				n = 1;
				for (String s : areaNameMap.keySet()) {
					areaNameMap.get(s).append("--- RAZ " + n + " ---\n");
				}
			}
		});
	}
	
	private void createEvent() {
		testFrame.addWindowListener(new WindowListener() {
			@Override
			public void windowOpened(WindowEvent e) {
				areaNameMap.get("WindowListener").append(n + " WINDOW_OPENED\n");
				++n;
				System.out.println(e.getSource().toString() + "\n");
			}

			@Override
			public void windowClosing(WindowEvent e) {
				areaNameMap.get("WindowListener").append(n + " WINDOW_CLOSING\n");
				++n;
				System.out.println(e.getSource().toString() + "\n");
			}

			@Override
			public void windowClosed(WindowEvent e) {
				areaNameMap.get("WindowListener").append(n + " WINDOW_CLOSED\n");
				++n;
				System.out.println(e.getSource().toString() + "\n");
			}

			@Override
			public void windowIconified(WindowEvent e) {
				areaNameMap.get("WindowListener").append(n + " WINDOW_ICONIFIED\n");
				++n;
				System.out.println(e.getSource().toString() + "\n");
			}

			@Override
			public void windowDeiconified(WindowEvent e) {
				areaNameMap.get("WindowListener").append(n + " WINDOW_DEICONIFIED\n");
				++n;
				System.out.println(e.getSource().toString() + "\n");
			}

			@Override
			public void windowActivated(WindowEvent e) {
				areaNameMap.get("WindowListener").append(n + " WINDOW_ACTIVATED\n");
				++n;
				System.out.println(e.getSource().toString() + "\n");
			}

			@Override
			public void windowDeactivated(WindowEvent e) {
				areaNameMap.get("WindowListener").append(n + " WINDOW_DEACTIVATED\n");
				++n;
				System.out.println(e.getSource().toString() + "\n");
			}
		});
		
		testFrame.addWindowFocusListener(new WindowFocusListener() {
			@Override
			public void windowGainedFocus(WindowEvent e) {
				areaNameMap.get("WindowFocusListener").append(n + " WINDOW_GAINED_FOCUS\n");
				++n;
			}

			@Override
			public void windowLostFocus(WindowEvent e) {
				areaNameMap.get("WindowFocusListener").append(n + " WINDOW_LOST_FOCUS\n");
				++n;
			}
		});
		
		testFrame.addWindowStateListener(new WindowStateListener() {
			@Override
			public void windowStateChanged(WindowEvent e) {
				areaNameMap.get("MouseListener").append(n + " MOUSE_CLICKED\n");
				++n;
			}
		});
		
		testFrame.addMouseListener(new MouseListener() {
			@Override
			public void mouseClicked(MouseEvent e) {
				areaNameMap.get("MouseListener").append(n + " MOUSE_CLICKED\n");
				++n;
			}

			@Override
			public void mousePressed(MouseEvent e) {
				areaNameMap.get("MouseListener").append(n + " MOUSE_PRESSED\n");
				++n;
			}

			@Override
			public void mouseReleased(MouseEvent e) {
				areaNameMap.get("MouseListener").append(n + " MOUSE_RELEASED\n");
				++n;
			}

			@Override
			public void mouseEntered(MouseEvent e) {
				areaNameMap.get("MouseListener").append(n + " MOUSE_ENTERED\n");
				++n;
			}

			@Override
			public void mouseExited(MouseEvent e) {
				areaNameMap.get("MouseListener").append(n + " MOUSE_EXIETD\n");
				++n;
			}
		});
		
		testFrame.addMouseMotionListener(new MouseMotionListener() {
			@Override
			public void mouseDragged(MouseEvent e) {
				areaNameMap.get("MouseWheelListener").append(n + " MOUSE_DRAGGED\n");
				++n;
			}

			@Override
			public void mouseMoved(MouseEvent e) {
				areaNameMap.get("MouseMotionListener").append(n + " MOUSE_MOVED\n");
				++n;
			}
		});
		
		testFrame.addMouseWheelListener(new MouseWheelListener() {
			@Override
			public void mouseWheelMoved(MouseWheelEvent e) {
				areaNameMap.get("MouseWheelListener").append(n + " MOUSEWHEEL_MOVED\n");
				++n;
				System.out.println(e.toString());
			}
		});
		
		testFrame.addKeyListener(new KeyListener() {
			@Override
			public void keyTyped(KeyEvent e) {
				areaNameMap.get("KeyListener").append(n + " KEY_TYPED\n");
				++n;
				System.out.println(e);
			}

			@Override
			public void keyPressed(KeyEvent e) {
				areaNameMap.get("KeyListener").append(n + " KEY_PRESSED\n");
				++n;
				System.out.println(e.getKeyChar());
			}

			@Override
			public void keyReleased(KeyEvent e) {
				areaNameMap.get("KeyListener").append(n + " KEY_RELEASED\n");
				++n;
				System.out.println(e.toString());
			}
		});
	}
	
	private void stringAreaEvent() {
		String mouseListener = "MouseListener";
		String mouseWheelListener = "MouseWheelListener";
		String mouseMotionListener = "MouseMotionListener";
		String windowListener = "WindowListener";
		String windowFocusListener = "WindowFocusListener";
		String windowStateListener = "WindowStateListener";
		String keyListener = "KeyListener";
		
		areaNameMap.put(keyListener, new JTextArea());
		areaNameMap.put(mouseListener, new JTextArea());
		areaNameMap.put(mouseWheelListener, new JTextArea());
		areaNameMap.put(mouseMotionListener, new JTextArea());
		areaNameMap.put(windowListener, new JTextArea());
		areaNameMap.put(windowFocusListener, new JTextArea());
		areaNameMap.put(windowStateListener, new JTextArea());
	}
	
	// POINT D'ENTREE
	
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				new EventsTester().displayMainFrame();
			}
		});
	}
}
