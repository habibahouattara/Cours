package serie10.model;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Observable;

import serie02.util.FileStateTester;
import util.Contract;

public class StdSplitManager extends Observable implements SplitManager {
	
	// ATTRIBUTS
	
	private File file;
	private long[] gSS;
	
	// CONSTRUCTEUR
	
	public StdSplitManager() {
		file = null;
		gSS = null;
	}
	
	public StdSplitManager(File file) {
		Contract.checkCondition(file != null);
		this.file = file;
		if (canSplit()) {
			gSS = new long[1];
			gSS[0] = file.length();
		} else {
			gSS = null;
		}
	}
	
	// REQUETES
	
	@Override
	public boolean canSplit() {
		return FileStateTester.isSplittable(file);
	}

	@Override
	public String getDescription() {
		return FileStateTester.describe(file);
	}

	@Override
	public File getFile() {
		return file;
	}

	@Override
	public int getMaxFragmentNb() {
		if (!canSplit()) {
			return 0;
		}
		return (int) Math.min(MAX_FRAGMENT_NB, Math.ceil(
				(double) file.length() / (double) MIN_FRAGMENT_SIZE));
	}

	@Override
	public long getMinFragmentSize() {
		if (!canSplit()) {
			return 0;
		}
		return (int) Math.max(MIN_FRAGMENT_SIZE, Math.ceil(
				(double) file.length() / getMaxFragmentNb()));
	}

	@Override
	public String[] getSplitsNames() {
		Contract.checkCondition(canSplit());
		String[] names = new String[gSS.length];
		for (int i = 0; i < gSS.length; i++) {
			names[i] = file.getAbsolutePath() + "." + (i+1);
		}
		return names;
	}

	@Override
	public long[] getSplitsSizes() {
		Contract.checkCondition(canSplit());
		long[] sizes = new long[gSS.length];
		for (int i = 0; i < gSS.length; i++) {
			sizes[i] = gSS[i];
		}
		return sizes;
	}
	
	// COMMANDES

	@Override
	public void changeFor(File f) {
		Contract.checkCondition(f != null);
		file = f;
		if (canSplit()) {
			gSS = new long[1];
			gSS[0] = file.length();
		} else {
			gSS = null;
		}
		setChanged();
		notifyObservers(Splitter.ChangeConfig.FILE_CHANGE);
	}

	@Override
	public void close() {
		file = null;
		gSS = null;
	}

	@Override
	public void setSplitsNumber(int splitsNb) {
		Contract.checkCondition(canSplit());
		Contract.checkCondition(1 <= splitsNb
				&& splitsNb <= getMaxFragmentNb());
		gSS = new long[splitsNb];
		long q = getFile().length() / splitsNb;
		long r = getFile().length() % splitsNb;
		for (int i = 0; i < r; i++) {
			gSS[i] = q + 1;
		}
		for (int i = (int) r; i < gSS.length; i++) {
			gSS[i] = q;
		}
		setChanged();
		notifyObservers(Splitter.ChangeConfig.CONFIG_CHANGE);
	}

	@Override
	public void setSplitsSizes(long size) {
		Contract.checkCondition(canSplit());
		Contract.checkCondition(getMinFragmentSize() <= size
				&& size <= getFile().length());
		long k = getFile().length() / size;
		long r = getFile().length() % size;
		if (r > 0) {
			gSS = new long[(int) k+1];
			gSS[gSS.length - 1] = r;
		}
		if (r == 0) {
			gSS = new long[(int) k];
			gSS[gSS.length - 1] = size;
		}
		for (int i = 0; i < gSS.length; i++) {
			gSS[i] = size;
		}
		setChanged();
		notifyObservers(Splitter.ChangeConfig.CONFIG_CHANGE);
	}

	@Override
	public void setSplitsSizes(long[] sizes) {
		Contract.checkCondition(canSplit());
		Contract.checkCondition(sizes != null);
		Contract.checkCondition(1 <= sizes.length);
		long sum = 0;
		for (long i : sizes) {
			Contract.checkCondition(sizes.length >= MIN_FRAGMENT_SIZE);
			sum += i;
		}
		long n = getFile().length();
		if (sum < n) {
			Contract.checkCondition(sizes.length < getMaxFragmentNb());
			gSS = new long[sizes.length + 1];
			for (int i = 0; i < sizes.length; i++) {
				gSS[i] = sizes[i];
			}
			gSS[sizes.length] = n - sum;
		} else {
			Contract.checkCondition(sizes.length <= getMaxFragmentNb());
			gSS = new long[sizes.length];
			for (int i = 0; i < sizes.length - 1; i++) {
				gSS[i] = sizes[i];
			}
			gSS[sizes.length-1] = n - (sum - sizes[sizes.length - 1]);
		}
	}

	@Override
	public void split() throws IOException {
		Contract.checkCondition(canSplit());
		BufferedInputStream bis = new BufferedInputStream(
				new FileInputStream(file.getAbsolutePath()));
		int c = bis.read();
		for (int i = 0; i < getSplitsNames().length; i++) {
			BufferedOutputStream bos = new BufferedOutputStream(
					new FileOutputStream(getSplitsNames()[i]));
			long k = 0;
			while (c != -1 && k < gSS[i]) {
				bos.write(c);
				k++;
				c = bis.read();
			}
			bos.close();
		}
		bis.close();
		setChanged();
		notifyObservers();
	}

}
