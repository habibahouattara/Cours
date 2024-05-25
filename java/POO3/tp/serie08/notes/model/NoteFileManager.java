package notes.model;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.List;

import notes.util.event.ResourceListener;
import notes.util.event.ResourceSupport;
import util.Contract;

class NoteFileManager {

    // CONSTANTES STATIQUES

    private static final String NL = System.getProperty("line.separator");

    // ATTRIBUTS
    
    private final ResourceSupport support;

    // CONSTRUCTEURS

    NoteFileManager() {
        support = new ResourceSupport(this);
    }

    // REQUETES
    
    ResourceListener[] getResourceListeners() {
        return support.getListeners();
    }

    // COMMANDES

    /**
     * @pre
     *     lnr != null
     * @post
     *     lnr a été ajouté à la séquence des écouteurs
     */
    void addResourceListener(ResourceListener lnr) {
        Contract.checkCondition(lnr != null);
        
        support.add(lnr);
    }

    /**
     * Scanne le fichier de texte associé à f pour en extraire des lignes
     *  de texte.
     * Les ResourceListener enregistrés seront notifiés pour chaque ligne lue
     *  par activation de lineLoaded.
     * Ils seront aussi notifiés par activation de progressUpdate
     *  (régulièrement) et de failureOccurred (si nécessaire).
     * @pre
     *     f != null
     * @post
     *     le scan du fichier associé à f a été effectué
     *     les notifications nécessaires ont été émises
     */
    void scanFile(File f) {
        /*****************/
        /** A COMPLETER **/
    	BufferedReader br = null;
    	int n = 0;
    	int fSize = (int) f.length();
    	try {
    		br = new BufferedReader(new FileReader(f));
    		String line = br.readLine();
    		while (line != null) {
    			delayAction();
    			support.fireLineLoaded(line);
    			n += line.getBytes().length;
    			support.fireProgressUpdated((n * 100) / fSize);
    			line = br.readLine();
    		}
    		support.fireProgressUpdated(100);
    	} catch (Exception ex) {
    		support.fireFailureOccurred(ex);
    	} finally {
    		try {
    			br.close();
    		} catch (IOException io) {
    			support.fireFailureOccurred(io);
    		}
    	}
        /*****************/
    }

    /**
     * @pre
     *     lnr != null
     * @post
     *     lnr a été retiré de la séquence des écouteurs
     */
    void removeResourceListener(ResourceListener lnr) {
        Contract.checkCondition(lnr != null);
        
        support.remove(lnr);
    }

    /**
     * Écrit une liste de lignes dans le fichier de texte associé à f.
     * Les ResourceListener enregistrés seront notifiés une fois l'écriture
     *  terminée par activation de dataSaved.
     * Ils seront aussi notifiés par activation de progressUpdate
     *  (régulièrement) et de failureOccurred (si nécessaire).
     * @pre
     *     f != null && lines != null
     * @post
     *     la liste de lignes a été écrite dans le fichier associé à f
     *     les notifications nécessaires ont été émises
     */
    void saveListToFile(List<String> lines, File f) {
        /*****************/
        /** A COMPLETER **/
    	BufferedWriter bw = null;
    	int sum = 0;
    	try {
    		bw = new BufferedWriter(new FileWriter(f));
    		for (String line : lines) {
    			bw.write(line + NL);
    			sum += 1;
    			support.fireProgressUpdated((sum * 100) / lines.size());
    			delayAction();
    		}
    	} catch (Exception ex) {
    		support.fireFailureOccurred(ex);
    	} finally {
    		try {
    			bw.close();
    		} catch (IOException io) {
    			support.fireFailureOccurred(io);
    		}
    	}
    	support.fireDataSaved(f.getAbsolutePath());
        /*****************/
    }

    // OUTILS

    /**
     * Pour ralentir les actions de lecture ou d'écriture.
     */
    private void delayAction() {
        final int delay = 100;
        try {
            Thread.sleep(delay);
        } catch (InterruptedException e) {
            // rien, on quitte
        }
    }
}
