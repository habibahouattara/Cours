package serie08;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.util.Observable;
import java.util.Observer;

import javax.swing.JComponent;

public class GraphicSpeedometer extends JComponent {
	
	// CONSTANTES
	
	// marge horizontale interne de part et d'autre du composant
	private static final int MARGIN = 40;
	// épaisseur de la ligne horizontale graduée
	private static final int THICK = 3;
	// demi-hauteur d'une graduation
	private static final int MARK = 5;
	// largeur de la base du triangle pour la tête de la flèche
	private static final int ARROW_BASE = 20;
	// épaisseur du corps de la flèche
	private static final int ARROW_THICK = 4;
	// hauteur du corps de la flèche
	private static final int ARROW_HEIGHT = 20;
	// hauteur préférée d'un GraphicSpeedometer
	private static final int PREFERRED_HEIGHT = 3 * (
			3 * MARK + ARROW_BASE / 2 + ARROW_HEIGHT);
	// facteur d'échelle pour l'espacement entre deux graduations
	private static final double ASPECT_RATIO = 1.25;
	// couleur bleu franc lorsque le moteur est allumé
	private static final Color BLUE = Color.BLUE;
	// couleur rouge franc lorsque le moteur est allumé
	private static final Color RED = Color.RED;
	// couleur bleu grisé lorsque le moteur est éteint
	private static final Color GRAYED_BLUE = new Color(0, 0, 255, 50);
	// couleur rouge grisé lorsque le moteur est éteint
	private static final Color GRAYED_RED = new Color(255, 0, 0, 50);
	// les vitesses affichées sont celles, entre 0 et model.getMaxSpeed(), qui sont
	// les multiples de SPLIT_SIZE
	private static final int SPLIT_SIZE = 10;
	
	// ATTRIBUTS
	
	private SpeedometerModel sm;
	
	// CONSTRUCTEUR
	
	
	public GraphicSpeedometer(SpeedometerModel model) {
		sm = model;
		int width = (int) (ASPECT_RATIO * ARROW_BASE * (
				sm.getMaxSpeed() / SPLIT_SIZE) + 2 * MARGIN);
		Dimension d = new Dimension(width, PREFERRED_HEIGHT);
		setPreferredSize(d);
		((Observable) model).addObserver(new Observer() {
			@Override
			public void update(Observable o, Object arg) {
				repaint();
			}
		});
	}
	
	// REQUETES
	
	public SpeedometerModel getModel() {
		return sm;
	}
	
	// COMMANDES
	
	@Override
	protected void paintComponent(Graphics g) {
		g.setColor(Color.WHITE);
		g.fillRect(0, 0, getWidth(), getHeight());
		paintArrow(g);
		paintGraduation(g);
	}
	
	// OUTILS
	
	private void paintArrow(Graphics g) {
		if (sm.isOn()) {
			g.setColor(RED);
		} else {
			g.setColor(GRAYED_RED);
		}
		double w = this.getWidth() - 2 * MARGIN;
		double h = this.getHeight() / 3;
		double xA = MARGIN + (w * sm.getSpeed())
				/ sm.getMaxSpeed() - ARROW_BASE / 2;
		double yA = h + THICK + 2 * MARK + ARROW_BASE / 2;
		double xB = xA + ARROW_BASE / 2;
		double yB = yA - ARROW_BASE / 2;
		double xC = xA + ARROW_BASE;
		double yC = yA;
		int[] xPoints = new int[] { (int) xA, (int) xB, (int) xC };
		int[] yPoints = new int[] { (int) yA, (int) yB, (int) yC };
		int nPoints = xPoints.length;
		g.fillPolygon(xPoints, yPoints, nPoints);
		int x = (int) (xA + (xC - xA) / 2 - ARROW_THICK / 2);
		int y = (int) yA;
		g.fillRect(x, y, ARROW_THICK, ARROW_HEIGHT);
	}
	
	private void paintGraduation(Graphics g) {
		if (sm.isOn()) {
			g.setColor(BLUE);
		} else {
			g.setColor(GRAYED_BLUE);
		}
		double w = this.getWidth() - 2 * MARGIN;
		double h = this.getHeight() / 3;
		g.fillRect(MARGIN, (int) h, (int) w, THICK);
		for (int i = 0; i <= sm.getMaxSpeed() / SPLIT_SIZE; ++i) {
			FontMetrics fm = g.getFontMetrics();
			String s = String.valueOf(i * SPLIT_SIZE);
			int sWidth = fm.stringWidth(s);
			int xQ = (int) (MARGIN + i * w * SPLIT_SIZE / sm.getMaxSpeed()
					- sWidth / 2);
			int yQ = (int) (h - 2 * MARK);
			g.drawString(s, xQ, yQ);
			int xE = xQ + sWidth / 2;
			int yE = yQ + MARK;
			int xF = xE;
			int yF = yE + 2 * MARK + THICK;
			g.drawLine(xE, yE, xF, yF);
		}
	}
}
