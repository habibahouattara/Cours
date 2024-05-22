package serie06;

import java.util.HashMap;
import java.util.Map;

public enum CoinTypes {
	ONE(1),
	TWO(2),
	FIVE(5),
	TEN(10),
	TWENTY(20),
	FIFTY(50),
	ONE_HUNDRED(100),
	TWO_HUNDRED(200);
	
	private final int faceValue;
	private static final Map<Integer, CoinTypes> COINS_MAP;
	
	static {
		COINS_MAP = new HashMap<Integer, CoinTypes>();
		for (CoinTypes c : CoinTypes.values()) {
			COINS_MAP.put(c.faceValue, c);
		}
	}
	
	CoinTypes(int value) {
		faceValue = value;
	}
	
	public int getFaceValue() {
		return faceValue;
	}
	
	public String toString() {
		return faceValue + " ct" + (faceValue != 1 ? "s" : "");
	}
	
	public static CoinTypes getCoinType(int val) {
		return COINS_MAP.get(val);
	}
}
