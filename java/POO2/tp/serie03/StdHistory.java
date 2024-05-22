package serie03;

import java.util.ArrayDeque;
import java.util.Collections;
import java.util.Deque;
import java.util.Queue;

import util.Contract;

public final class StdHistory<E> implements History<E> {
	
	// ATTRIBUTS
	
	private int max;
	private int curPos;
	private int endPos;
	private Queue<E> tail;
	private Deque<E> head;
	
	// CONSTRUCTEUR
	
	public StdHistory(int maxHeight) {
		Contract.checkCondition(maxHeight > 0);
		max = maxHeight;
		curPos = 0;
		endPos = 0;
		head = new ArrayDeque<E>();
		tail = Collections.asLifoQueue(new ArrayDeque<E>());
	}
	
	// REQUETES
	
	@Override
	public int getMaxHeight() {
		return max;
	}

	@Override
	public int getCurrentPosition() {
		return curPos;
	}

	@Override
	public E getCurrentElement() {
		Contract.checkCondition(getCurrentPosition() > 0);
		return head.element();
	}

	@Override
	public int getEndPosition() {
		return endPos;
	}

	@Override
	public boolean isEmpty() {
		return getEndPosition() == 0;
	}
	
	// COMMANDES

	@Override
	public void add(E e) {
		Contract.checkCondition(e != null);
		if (getCurrentPosition() == getMaxHeight()) {
			head.removeLast();
		} else {
			curPos++;
		}
		head.addFirst(e);
		tail = Collections.asLifoQueue(new ArrayDeque<E>());
		endPos = curPos;
	}

	@Override
	public void goForward() {
		Contract.checkCondition(getCurrentPosition() < getEndPosition());
		head.addFirst(tail.remove());
		curPos++;
	}

	@Override
	public void goBackward() {
		Contract.checkCondition(getCurrentPosition() > 0);
		tail.add(head.remove());
		curPos--;
	}

}
