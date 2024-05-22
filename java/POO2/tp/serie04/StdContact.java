package serie04;

import util.Contract;

public class StdContact implements Contact {
	
	// ATTRIBUTS
	
	private String lastName;
	private String firstName;
	private Civ civ;
	
	// CONSTRUCTEUR
	
	public StdContact(String n, String p) {
		Contract.checkCondition(n != null && p != null);
		Contract.checkCondition(!n.equals("") || !p.equals(""));
		lastName = n;
		firstName = p;
		civ = Civ.UKN;
	}
	
	public StdContact(Civ c, String n, String p) {
		this(n, p);
		Contract.checkCondition(c != null);
		civ = c;
	}
	
	// REQUETES
	
	public int compareTo(Contact ct) {
		if (ct == null) {
			throw new NullPointerException();
		}
		int r = this.getLastName().compareTo(ct.getLastName());
		if (r == 0) {
			r = this.getFirstName().compareTo(ct.getFirstName());
			if (r == 0) {
				r = this.getCivility().compareTo(ct.getCivility());
			}
		}
		return r;
	}
	
	public boolean canEquals(Object other) {
		return other instanceof StdContact;
	}
	
	public boolean equals(Object other) {
		boolean b = false;
		if (other instanceof StdContact) {
			StdContact that = (StdContact) other;
			b = that.canEquals(this) && (compareTo(that) == 0);
		}
		return b;
	}

	@Override
	public Civ getCivility() {
		return civ;
	}

	@Override
	public String getFirstName() {
		return firstName;
	}

	@Override
	public String getLastName() {
		return lastName;
	}
	
	public int hashCode() {
		return toString().hashCode();
	}
	
	@Override
	public String toString() {
		return getCivility() + " " + getFirstName() + " " + getLastName();
	}
	
	// COMMANDES
	
	@Override
	public Contact evolve(Civ civility) {
		Contract.checkCondition(civility != null
				&& getCivility().canEvolveTo(civility));
		return new StdContact(civility, lastName, firstName);
	}

}
