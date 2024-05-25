package pet.model;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.EventListenerList;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import javax.swing.text.PlainDocument;

import util.Contract;

public class StdPetModel implements PetModel {
    
    // ATTRIBUTS
    
    private Document document;
    private File file;
    private boolean sync;
    private DocumentListener docListener;
    private final EventListenerList listenerList;
    private final ChangeEvent changeEvent;

    // CONSTRUCTEURS
    
    public StdPetModel() {
        document = null;
        file = null;
        sync = false;
        listenerList = new EventListenerList();
        changeEvent = new ChangeEvent(this);
        docListener = new DocumentListener() {
            @Override
            public void changedUpdate(DocumentEvent e) {
                // rien ici
            }
            @Override
            public void insertUpdate(DocumentEvent e) {
                setSyncAndNotify(false);
            }
            @Override
            public void removeUpdate(DocumentEvent e) {
                setSyncAndNotify(false);
            }
        };
    }
    
    // REQUETES

	@Override
	public ChangeListener[] getChangeListeners() {
		return listenerList.getListeners(ChangeListener.class);
	}

    @Override
    public Document getDocument() {
        return document;
    }
    
    @Override
    public File getFile() {
        return file;
    }
    
    @Override
    public boolean isSynchronized() {
        return sync;
    }

	@Override
	public void addChangeListener(ChangeListener listener) {
		Contract.checkCondition(listener != null);
		listenerList.add(ChangeListener.class, listener);
	}
    
    // COMMANDES
    
    @Override
    public void clearDocument() {
        Contract.checkCondition(document != null);
        
        setText("", document);
        setSyncAndNotify(false);
    }

	@Override
	public void removeChangeListener(ChangeListener listener) {
		Contract.checkCondition(listener != null);
		listenerList.remove(ChangeListener.class, listener);
	}
    
    @Override
    public void removeDocAndFile() {
        transferDocumentListener(document, null);
        document = null;
        file = null;
        setSyncAndNotify(false);
    }
    
    @Override
    public void resetCurrentDocWithCurrentFile() throws IOException {
        Contract.checkCondition(document != null);
        Contract.checkCondition(file != null);
        
        load(file, document);
        setSyncAndNotify(true);
    }
    
    @Override
    public void saveCurrentDocIntoCurrentFile() throws IOException {
        Contract.checkCondition(document != null);
        Contract.checkCondition(file != null);
        
        save(document, file);
        setSyncAndNotify(true);
    }
    
    @Override
    public void saveCurrentDocIntoFile(File f) throws IOException {
        Contract.checkCondition(document != null);
        Contract.checkCondition(f != null && f.isFile());
        Contract.checkCondition(f.canRead() && f.canWrite());

        save(document, f);
        file = f;
        setSyncAndNotify(true);
    }
    
    @Override
    public void setNewDocAndNewFile(File f) throws IOException {
        Contract.checkCondition(f != null && f.isFile());
        Contract.checkCondition(f.canRead() && f.canWrite());

        Document newDoc = new PlainDocument();
        load(f, newDoc);
        transferDocumentListener(document, newDoc);
        document = newDoc;
        file = f;
        setSyncAndNotify(true);
    }
    
    @Override
    public void setNewDocFromFile(File f) throws IOException {
        Contract.checkCondition(f != null && f.isFile());
        Contract.checkCondition(f.canRead());

        Document newDoc = new PlainDocument();
        load(f, newDoc);
        transferDocumentListener(document, newDoc);
        document = newDoc;
        file = null;
        setSyncAndNotify(false);
    }
    
    @Override
    public void setNewDocWithoutFile() {
        Document newDoc = new PlainDocument();
        transferDocumentListener(document, newDoc);
        document = newDoc;
        file = null;
        setSyncAndNotify(false);
    }

    // OUTILS
    
    /**
     * @pre <pre>
     *     d != null </pre>
     * @post <pre>
     *     result est le contenu de d </pre>
     */
    private static String getText(Document d) {
        assert d != null;
        
        String content;
        try {
            content = d.getText(0, d.getLength());
        } catch (BadLocationException e) {
            // Ne devrait pas survenir
            throw new InternalError(e.getMessage());
        }
        return content;
    }

    /**
     * @pre <pre>
     *     s != null && d != null </pre>
     * @post <pre>
     *     le contenu de d est s </pre>
     */
    private static void setText(String s, Document d) {
        assert s != null && d != null;
        
        try {
            d.remove(0, d.getLength());
            d.insertString(0, s, null);
        } catch (BadLocationException e) {
            // Ne devrait pas survenir
            throw new InternalError(e.getMessage());
        }
    }

    /**
     * @pre <pre>
     *     f != null && d != null </pre>
     * @post <pre>
     *     le contenu de d a été remplacé par celui de f </pre>
     * @throws
     *     IOException si erreur de lecture dans f (et alors d reste inchangé)
     */
    private static void load(File f, Document d) throws IOException {
        assert f != null;
        assert d != null;
        
        StringBuilder sb = new StringBuilder();
        BufferedReader source = new BufferedReader(new FileReader(f));
        try {
            String line = source.readLine();
            if (line != null) {
                sb.append(line);
                line = source.readLine();
            }
            while (line != null) {
                sb.append("\n" + line);
                line = source.readLine();
            }
        } finally {
            source.close();
        }
        setText(sb.toString(), d);
    }

    /**
     * @pre <pre>
     *     f != null && d != null </pre>
     * @post <pre>
     *     le contenu de f a été remplacé par celui de d </pre>
     * @throws
     *     IOException si erreur d'écriture dans f
     */
    private static void save(Document d, File f) throws IOException {
        assert f != null;
        assert d != null;
        
        String content = getText(d);
        BufferedWriter dest = new BufferedWriter(new FileWriter(f));
        String[] lines = content.split("\n", -1);
        try {
            for (String line : lines) {
                dest.write(line);
                dest.newLine();
            }
        } finally {
            dest.close();
        }
    }

    /**
     * Fixe l'état de synchronisation du modèle et notifie les observateurs.
     */
    private void setSyncAndNotify(boolean on) {
        sync = on;
        /*******************************************************/
        /** A COMPLETER avec la notification des observateurs **/
        fireStateChanged();
        /*******************************************************/
    }
    
    private void transferDocumentListener(Document from, Document to) {
        if (from != null) {
            from.removeDocumentListener(docListener);
        }
        if (to != null) {
            to.addDocumentListener(docListener);
        }
    }
    
    private void fireStateChanged() {
    	Object[] listeners = listenerList.getListenerList();
    	for (int i = listeners.length - 2; i >= 0; i -= 2) {
    		if (listeners[i] == ChangeListener.class) {
    			((ChangeListener) listeners[i+1]).stateChanged(changeEvent);
    		}
    	}
    }
}
