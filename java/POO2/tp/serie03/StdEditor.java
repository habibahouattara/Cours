package serie03;

import serie03.cmd.CommandFactory;
import util.Contract;

public class StdEditor implements Editor {
	
	// ATTRIBUTS
	
	private Text text;
	private History<Command> history;
	
	// CONSTRUCTEUR
	
	public StdEditor() {
		text = new StdText();
		history = new StdHistory<Command>(DEFAULT_HISTORY_SIZE);
	}
	
	public StdEditor(int historySize) {
		Contract.checkCondition(historySize > 0);
		text = new StdText();
		history = new StdHistory<Command>(historySize);
	}
	
	// REQUETES

	@Override
	public int getTextLinesNb() {
		return text.getLinesNb();
	}

	@Override
	public String getTextContent() {
		return text.getContent();
	}

	@Override
	public int getHistorySize() {
		return history.getMaxHeight();
	}

	@Override
	public int nbOfPossibleUndo() {
		return history.getCurrentPosition();
	}

	@Override
	public int nbOfPossibleRedo() {
		return history.getEndPosition() - history.getCurrentPosition();
	}
	
	// COMMANDES
	
	@Override
	public void clear() {
		Command c = CommandFactory.createClear(text);
		c.act();
		history.add(c);
	}

	@Override
	public void insertLine(int numLine, String s) {
		Contract.checkCondition(s != null);
		Contract.checkCondition(1 <= numLine
				&& numLine <= getTextLinesNb() + 1);
		Command c = CommandFactory.createInsertLine(text, numLine, s);
		c.act();
		history.add(c);
	}

	@Override
	public void deleteLine(int numLine) {
		Contract.checkCondition(1 <= numLine && numLine <= getTextLinesNb());
		Command c = CommandFactory.createDeleteLine(text, numLine);
		c.act();
		history.add(c);
	}

	@Override
	public void redo() {
		Contract.checkCondition(nbOfPossibleRedo() > 0);
		history.goForward();
		history.getCurrentElement().act();
	}

	@Override
	public void undo() {
		Contract.checkCondition(nbOfPossibleUndo() > 0);
		history.getCurrentElement().act();
		history.goBackward();
	}

}
