package pet.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.util.EnumMap;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.border.EtchedBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.text.Document;

import pet.model.PetModel;
import pet.model.StdPetModel;
import util.Contract;

public class Pet {
    
    private final JFrame frame;
    private final JLabel statusBar;
    private final JTextArea editor;
    private final JScrollPane scroller;
    private final PetModel model;
    private final Map<Item, JMenuItem> menuItems;

    // CONSTRUCTEUR
    
    public Pet() {
        // MODELE
        model = new StdPetModel();
        // VUE
        frame = buildMainFrame();
        editor = buildEditor();
        scroller = new JScrollPane();
        statusBar = new JLabel();
        menuItems = buildMenuItemsMap();
        placeMenuItemsAndMenus();
        placeComponents();
        // CONTROLEUR
        connectControllers();
    }
    
    //COMMANDE
    
    public void display() {
        setItemsEnabledState();
        updateStatusBar();
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    // OUTILS
    
    private JFrame buildMainFrame() {
        final Dimension prefSize = new Dimension(640, 480);
        
        JFrame jf = new JFrame("Petit Éditeur de Texte");
        jf.setPreferredSize(prefSize);
        return jf;
    }
    
    private JTextArea buildEditor() {
        final int fontSize = 14;
        
        JTextArea jta = new JTextArea();
        jta.setBackground(Color.BLACK);
        jta.setForeground(Color.LIGHT_GRAY);
        jta.setCaretColor(Color.RED);
        jta.setFont(new Font("Courier New", Font.PLAIN, fontSize));
        return jta;
    }
    
    /**
     * Création de la correspondance Item -> JMenuItem.
     */
    private Map<Item, JMenuItem> buildMenuItemsMap() {
        /*****************/
        /** A COMPLETER **/
    	Map<Item, JMenuItem> map = new EnumMap<Item, JMenuItem>(Item.class);
    	for (Item item : Item.values()) {
    		map.put(item, new JMenuItem(item.label()));
    	}
    	return map;
        /*****************/
    }
    
    /**
     * Place les menus et les éléments de menu sur une barre de menus, et cette
     *  barre de menus sur la fenêtre principale.
     */
    private void placeMenuItemsAndMenus() {
        /*****************/
        /** A COMPLETER **/
    	JMenuBar jmb = new JMenuBar();
    	for (Menu m : Menu.values()) {
    		JMenu jm = new JMenu(m.label());
    		for (Item item : Menu.STRUCT.get(m)) {
    			if (item == null) {
    				jm.addSeparator();
    			} else {
    				jm.add(menuItems.get(item));
    			}
    		}
    		jmb.add(jm);
    	}
    	frame.setJMenuBar(jmb);
        /*****************/
    }
    
    private void placeComponents() {
        frame.add(scroller, BorderLayout.CENTER);
        
        JPanel p = new JPanel(new GridLayout(1, 0));
        p.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createEtchedBorder(EtchedBorder.RAISED),
                BorderFactory.createEmptyBorder(3, 5, 3, 5)));
        { //--
            p.add(statusBar);
        } //--
        
        frame.add(p, BorderLayout.SOUTH);
    }
    
    private void connectControllers() {
        /*
         * L'opération de fermeture par défaut ne doit rien faire car on se
         *  charge de tout dans un écouteur qui demande confirmation puis
         *  libère les ressources de la fenêtre en cas de réponse positive.
         */
        /*****************/
        /** A COMPLETER **/
    	frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
    	
    	frame.addWindowListener(new WindowAdapter() {
    		@Override
    		public void windowClosing(WindowEvent e) {
    			if (!model.isSynchronized() && model.getDocument() != null) {
            		if (confirmAction()) {
            			if (model.getFile() != null) {
            				if (confirmSave()) {
            					try {
									model.saveCurrentDocIntoCurrentFile();
								} catch (IOException e1) {
									displayError("Problème lors de la sauvegarde");
									return;
								}
            				}
            			} else {
            				if (confirmSave()) {
            					File f = selectSaveFile();
            					if (f == null) {
            						return;
            					}
            					try {
									model.saveCurrentDocIntoFile(f);
								} catch (IOException e1) {
									displayError("Problème lors de la sauvegarde");
									return;
								}
            				}
            			}
            			frame.dispose();
            			System.exit(0);
            		}
            	} else {
            		frame.dispose();
            	}
    		}
    	});
    	
    	editor.addFocusListener(new FocusAdapter() {
    		@Override
    		public void focusLost(FocusEvent e) {
    			editor.requestFocus();
    		}
    	});
        /*****************/
        
        /*
         * Observateur du modèle.
         */
        /*****************/
        /** A COMPLETER **/
    	model.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
		        setItemsEnabledState();
		        updateScrollerAndEditorComponents();
		        updateStatusBar();
			}
    	});
        /*****************/
        
        /*
         * Écouteurs des items du menu
         */
        menuItems.get(Item.NEW).addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /*****************/
                /** A COMPLETER **/
            	if (confirmAction()) {
            		model.setNewDocWithoutFile();
            	}
                /*****************/
            }
        });
        menuItems.get(Item.NEW_FROM_FILE).addActionListener(
            new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    /*****************/
                    /** A COMPLETER **/
                	if (confirmAction()) {
                		File f = selectLoadFile();
                		if (f != null) {
                			try {
								model.setNewDocFromFile(f);
							} catch (IOException e1) {
								displayError("Problème de lecture du fichier");
							}
                		}
                	}
                    /*****************/
                }
            }
        );
        menuItems.get(Item.OPEN).addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /*****************/
                /** A COMPLETER **/
            	if (confirmAction()) {
            		File f = selectLoadFile();
            		if (f != null) {
            			try {
							model.setNewDocAndNewFile(f);
						} catch (IOException e1) {
							displayError("Problème de lecture du fichier");
						}
            		}
            	}
                /*****************/
            }
        });
        menuItems.get(Item.REOPEN).addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /*****************/
                /** A COMPLETER **/
            	if (confirmAction()) {
            		File f = selectLoadFile();
            		if (f != null) {
            			try {
							model.resetCurrentDocWithCurrentFile();
						} catch (IOException e1) {
							displayError("Problème de lecture du fichier");
						}
            		}
            	}
                /*****************/
            }
        });
        menuItems.get(Item.SAVE).addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /*****************/
                /** A COMPLETER **/
            	if (confirmReplaceContent(model.getFile())) {
            		try {
						model.saveCurrentDocIntoCurrentFile();
					} catch (IOException e1) {
						displayError("Problème lors de la sauvegarde");
					}
            	}
                /*****************/
            }
        });
        menuItems.get(Item.SAVE_AS).addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /*****************/
                /** A COMPLETER **/
            	File f = selectSaveFile();
            	if (f != null) {
            		try {
						model.saveCurrentDocIntoFile(f);
					} catch (IOException e1) {
						displayError("Problème lors de la sauvegarde");
					}
            	}
                /*****************/
            }
        });
        menuItems.get(Item.CLOSE).addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /*****************/
                /** A COMPLETER **/
            	if (confirmAction()) {
            		model.removeDocAndFile();
            	}
                /*****************/
            }
        });
        menuItems.get(Item.CLEAR).addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /*****************/
                /** A COMPLETER **/
            	if (confirmAction()) {
            		model.clearDocument();
            	}
                /*****************/
            }
        });
        menuItems.get(Item.QUIT).addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                /*****************/
                /** A COMPLETER **/
            	if (!model.isSynchronized() && model.getDocument() != null) {
            		if (confirmAction()) {
            			if (model.getFile() != null) {
            				if (confirmSave()) {
            					try {
									model.saveCurrentDocIntoCurrentFile();
								} catch (IOException e1) {
									displayError("Problème lors de la sauvegarde");
									return;
								}
            				}
            			} else {
            				if (confirmSave()) {
            					File f = selectSaveFile();
            					if (f == null) {
            						return;
            					}
            					try {
									model.saveCurrentDocIntoFile(f);
								} catch (IOException e1) {
									displayError("Problème lors de la sauvegarde");
									return;
								}
            				}
            			}
            			frame.dispose();
            			System.exit(0);
            		}
            	} else {
            		frame.dispose();
            	}
                /*****************/
            }
        });
    }
    
    /**
     * Gère l'état de la disponibilité des éléments du menu en fonction de
     *  l'état du modèle.
     */
    private void setItemsEnabledState() {
        /*****************/
        /** A COMPLETER **/
    	Document d = model.getDocument();
    	File f = model.getFile();
    	boolean b = d != null && f != null && !model.isSynchronized();
    	menuItems.get(Item.CLEAR).setEnabled(d != null && d.getLength() > 0);
    	menuItems.get(Item.SAVE).setEnabled(b);
    	menuItems.get(Item.REOPEN).setEnabled(b);
    	menuItems.get(Item.SAVE_AS).setEnabled(d != null);
    	menuItems.get(Item.CLOSE).setEnabled(d != null);
        /*****************/
    }
    
    /**
     * Met à jour le Viewport du JScrollPane en fonction de la présence d'un
     *  document dans le modèle.
     * Remplace le document de la zone de texte par celui du modèle quand c'est
     *  nécessaire.
     */
    private void updateScrollerAndEditorComponents() {
        /*****************/
        /** A COMPLETER **/
    	if (model.getDocument() == null) {
    		scroller.setViewportView(null);
    	} else {
    		if (editor.getDocument() != model.getDocument()) {
    			editor.setDocument(model.getDocument());
    		}
    		scroller.setViewportView(editor);
    	}
        /*****************/
    }
    
    /**
     * Met à jour la barre d'état.
     */
    private void updateStatusBar() {
        /*****************/
        /** A COMPLETER **/
    	StringBuffer sb = new StringBuffer();
    	if (model.getDocument() != null && model.getFile() == null
    			|| !model.isSynchronized()) {
    		sb.append("* ");
    	}
    	if (model.getFile() == null) {
    		sb.append("<aucun>");
    	} else {
    		sb.append(model.getFile().getAbsolutePath());
    	}
    	statusBar.setText(sb.toString());
        /*****************/
    }
    
    /**
     * Demande une confirmation de poursuite d'action.
     * @post
     *     result == true <==>
     *         le modèle était synchronisé
     *         || il n'y avait pas de document dans le modèle
     *         || le document était en cours d'édition mais l'utilisateur
     *            a répondu positivement à la demande de confirmation
     */
    private boolean confirmAction() {
        /*****************/
        /** A COMPLETER **/
    	if (model.isSynchronized()
    			|| model.getDocument() == null
    			|| JOptionPane.showConfirmDialog(
    					null,
    					"Voulez-vous poursuivre votre action?",
    					"Confirmation",
    					JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
    		return true;
    	}
    	return false;
        /*****************/
    }
    
    /**
     * Demande une confirmation de sauvegarde.
     * @post
     *     result == true <==>
     *         l'utilisateur a répondu positivement à la demande de confirmation
     */
    private boolean confirmSave() {
    	return JOptionPane.showConfirmDialog(
    			null,
    			"Voulez-vous sauvegarder?",
    			"Confirmation",
    			JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION;
    }
    
    /**
     * Demande une confirmation d'écrasement de fichier.
     * @pre
     *     f != null
     * @post
     *     result == true <==>
     *         le fichier n'existe pas
     *         || le fichier existe mais l'utilisateur a répondu positivement
     *            à la demande de confirmation
     */
    private boolean confirmReplaceContent(File f) {
        /*****************/
        /** A COMPLETER **/
    	Contract.checkCondition(f != null);
    	if (!f.exists()
    			|| JOptionPane.showConfirmDialog(
    	    			null,
    	    			"Voulez-vous écraser le fichier?",
    	    			"Confirmation",
    	    			JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
    		return true;
    	}
    	return false;
        /*****************/
    }
    
    /**
     * Toute erreur de l'application due au comportement de l'utilisateur doit
     *  être interceptée et transformée en message présenté dans une boite de
     *  dialogue.
     */
    private void displayError(String m) {
        /*****************/
        /** A COMPLETER **/
    	JOptionPane.showConfirmDialog(
    			null,
    			m,
    			"Erreur",
    			JOptionPane.ERROR_MESSAGE);
        /*****************/
    }
    
    /**
     * Permet au client de sélectionner un fichier de sauvegarde en choisissant
     *  un nom de fichier à l'aide d'un JFileChooser.
     * Si le nom de fichier choisi n'existe pas encore, un nouveau fichier est
     *  créé avec ce nom.
     * Retourne null si et seulement si :
     * - l'utilisateur a annulé l'opération,
     * - le nom choisi correspond à un élément préexistant du système de fichier
     *   mais cet élément n'est pas un fichier
     * - le nom choisi ne correspond pas à un élément préexistant du système de
     *   fichier mais le fichier n'a pas pu être créé.
     * Dans les deux derniers cas, une boite de dialogue vient de plus décrire
     *  le problème.
     */
    private File selectSaveFile() {
        /*****************/
        /** A COMPLETER **/
    	JFileChooser jfc = new JFileChooser();
    	if (jfc.showSaveDialog(null) == JFileChooser.APPROVE_OPTION) {
    		File f = jfc.getSelectedFile();
    		if (f.exists() && !f.isFile()) {
    			displayError("Ce n'est pas un fichier");
    			return null;
    		} else if (!f.exists()) {
    			try {
					f.createNewFile();
				} catch (IOException e) {
					displayError("Le fichier n'a pas pu être crée");
					return null;
				}
    		}
    		return f;
    	}
    	return null;
        /*****************/
    }
    
    /**
     * Permet au client de sélectionner un fichier à charger en choisissant
     *  un nom de fichier à l'aide d'un JFileChooser.
     * Retourne null si et seulement si :
     * - l'utilisateur a annulé l'opération,
     * - le nom choisi ne correspond pas un fichier existant.
     * Dans ce dernier cas, une boite de dialogue vient de plus décrire
     *  le problème.
     */
    private File selectLoadFile() {
        /*****************/
        /** A COMPLETER **/
    	JFileChooser jfc = new JFileChooser();
    	if (jfc.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
    		File f = jfc.getSelectedFile();
    		if (!f.exists() || !f.isFile()) {
    			displayError("Ce n'est pas un fichier existant");
    			return null;
    		}
    		return f;
    	}
    	return null;
        /*****************/
    }
}
