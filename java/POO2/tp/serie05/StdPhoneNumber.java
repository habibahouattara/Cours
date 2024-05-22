package serie05;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import util.Contract;

public class StdPhoneNumber implements PhoneNumber {
	
	// ATTRIBUTS
	
	private final char[] digits;
	private String extension;
	private static final String PATTERN;
	static {
		PATTERN = SEPARATORS
				+ "(?:" + NATIONAL_PREFIX + "|\\"
				+ INTERNATIONAL_PREFIX + ")?"
				+ "((?:" + SEPARATORS + "\\d){"
				+ DIGITS_NB + "})"
				+ SEPARATORS + "(\\d*)" + SEPARATORS;
	}
	
	// CONSTRUCTEUR
	
	public StdPhoneNumber(String n) {
		Contract.checkCondition(n != null);
		Pattern p = Pattern.compile(NUMBER_PATTERN);
		Matcher matcher = p.matcher(n);
		Contract.checkCondition(matcher.matches());
		digits = new char[DIGITS_NB];
		matcher.usePattern(Pattern.compile(PATTERN));
		matcher.reset();
		if (matcher.matches()) {
			extension = matcher.group(2);
			final String numbers = matcher.group(1);
			Matcher m = Pattern.compile("\\d").matcher(numbers);
			boolean find = m.find();
			int i = 0;
			while (find) {
				Contract.checkCondition(!find || i < DIGITS_NB);
				digits[i] = m.group().charAt(0);
				find = m.find();
				i += 1;
			}
			assert i == DIGITS_NB;
		}
	}
	
	// REQUETES
	
	@Override
	public char digit(int i) {
		Contract.checkCondition(1 <= i && i <= DIGITS_NB);
		return digits[i-1];
	}

	@Override
	public String extension() {
		return extension;
	}

	@Override
	public String international() {
		return "+33"
				+ " "
				+ new String(digits)
				+ (extension.equals("") ? "" : "~" + extension);
	}

	@Override
	public String national() {
		String national = "0";
		for (int i = 0; i < DIGITS_NB; i++) {
			national += new String(digits, i, 1)
					+ (i % 2 == 0 && i < DIGITS_NB - 1 ? " " : "");
		}
		return national + (extension.equals("") ? "" : "(" + extension + ")");
	}
	
	@Override
	public String toString() {
		String s = "";
		for (int i = 0; i < DIGITS_NB; i++) {
			s += new String(digits, i, 1)
					+ (i < DIGITS_NB - 1 ? "," : "");
		}
		return s + (extension.equals("") ? "" : ";" + extension);
	}
}
