package notes.gui;

import static notes.model.ColumnFeature.SUBJECT;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

import notes.model.NoteSheetModel;
import notes.view.NoteSheet;

public class NotesAppli {

    // ATTRIBUTS

    private final NoteSheet noteSheetBean;
    private final JButton load;
    private final JButton save;
    private final JLabel mean;
    private final JLabel points;
    private final JFrame frame;
    private final JFileChooser chooser;
    private final JProgressBar progress;

    // CONSTRUCTEURS

    public NotesAppli() {
        // VUE
        frame = new JFrame("Gestionnaire de notes");
        noteSheetBean = new NoteSheet();
        load = new JButton("Load...");
        save = new JButton("Save as...");
        mean = new JLabel();
        points = new JLabel();
        chooser = new JFileChooser(System.getProperty("user.home"));
        progress = new JProgressBar();
        progress.setMinimum(0);
        progress.setMaximum(100);
        progress.setStringPainted(true);
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

    private void placeComponents() {
        JPanel p = new JPanel();
        { //--
            p.add(load);
            p.add(save);
        } //--
        frame.add(p, BorderLayout.NORTH);
        
        frame.add(noteSheetBean, BorderLayout.CENTER);
        
        p = new JPanel(new GridLayout(2, 0));
        { //--
            JPanel q = new JPanel(new GridLayout(1, 0));
            { //--
                JPanel r = new JPanel();
                { //--
                    r.add(new JLabel("Moyenne :"));
                    r.add(mean);
                } //--
                q.add(r);

                r = new JPanel();
                { //--
                    r.add(new JLabel("Total des points :"));
                    r.add(points);
                } //--
                q.add(r);
            } //--
            p.add(q);
            
            q = new JPanel(new GridLayout(0, 1));
            { //--
                q.add(progress);
            } //--
            p.add(q);
        } //--
        frame.add(p, BorderLayout.SOUTH);
    }

    private void connectControllers() {
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        // écoute du bouton load,
        // pour charger le contenu d'un fichier dans la table
        load.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                File f = askForFile(OpenCommands.LOAD);
                if (f == null) {
                    return;
                }
                
                noteSheetBean.loadTableFromFile(f);
            }
        });
        
        // écoute du bouton save,
        // pour sauvegarder le contenu de la table dans un fichier
        save.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                File f = askForFile(OpenCommands.SAVE);
                if (f == null) {
                    return;
                }
                
                noteSheetBean.saveTableToFile(f);
            }
        });
        
        // observation de la propriété progress du bean,
        // pour modifier la barre de progression de l'application
        /*****************/
        /** A COMPLETER **/
        noteSheetBean.addPropertyChangeListener(NoteSheetModel.PROP_PROGRESS,
        		new PropertyChangeListener() {
			@Override
			public void propertyChange(PropertyChangeEvent evt) {
				progress.setValue((Integer) evt.getNewValue());
			}
        });
        /*****************/
        
        // observation du modèle de données de la table,
        // pour mettre à jour les champs de texte mean et points
        /*****************/
        /** A COMPLETER **/
        noteSheetBean.getTableModel().addTableModelListener(
        		new TableModelListener() {
			@Override
			public void tableChanged(TableModelEvent e) {
				points.setText(String.valueOf(noteSheetBean.getPoints()));
				mean.setText(String.valueOf(noteSheetBean.getMean()));
			}
        });
        /*****************/
    }

    /**
     * Demande à l'utilisateur une nouvelle référence de fichier.
     * Retourne une valeur non null uniquement si l'utilisateur a bien choisi
     *  un fichier à la souris ou tapé un nom de fichier valide.
     */
    private File askForFile(OpenCommands type) {
        int result = (type == OpenCommands.LOAD)
                ? chooser.showOpenDialog(null)
                : chooser.showSaveDialog(null);
        if (result == JFileChooser.APPROVE_OPTION) {
            return chooser.getSelectedFile();
        } else {
            return null;
        }
    }

    // TYPES IMBRIQUES

    private enum OpenCommands { LOAD, SAVE }
}
