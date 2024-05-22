package serie04;

import java.util.EnumSet;

import util.Contract;

public enum Civ {
	UKN(""),
	MR("M."),
	MRS("Mme"),
	MS("Mlle");
	
	private final String name;
	private EnumSet<Civ> candidates;
	
	static {
		UKN.candidates = EnumSet.of(MR, MRS, MS);
		MR.candidates = EnumSet.noneOf(Civ.class);
		MRS.candidates = EnumSet.of(MS);
		MS.candidates = EnumSet.of(MRS);
	}
	
	Civ(String n) {
		name = n;
	}
	
	public boolean canEvolveTo(Civ candidate) {
		Contract.checkCondition(candidate != null);
		return this.candidates.contains(candidate);
	}
	
	public String toString() {
		return this.name;
	}
}
