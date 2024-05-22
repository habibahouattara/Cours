package serie06;

public enum DrinkTypes {
	COFFEE(30),
	CHOCOLATE(45),
	ORANGE_JUICE(110);
	
	private final int price;
	
	DrinkTypes(int p) {
		price = p;
	}
	
	public int getPrice() {
		return price;
	}
	
	public String toString() {
		return this.name().toLowerCase().replace('_', ' ');
	}
}
