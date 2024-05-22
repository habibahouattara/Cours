package serie05;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.NavigableSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import serie04.Civ;
import serie04.Contact;
import serie04.PhoneBook;
import serie04.StdPhoneBook;
import util.Contract;

public class StdPersistentPhoneBook<C extends Contact & Comparable<C>, N
		extends PhoneNumber> extends StdPhoneBook<C, N>
		implements PersistentPhoneBook<C, N> {
	
	// ATTRIBUTS
	
	private File file;
	private final DataFactory<C, N> df;
	
	// CONSTRUCTEUR
	
	public StdPersistentPhoneBook(DataFactory<C, N> df, File file) {
		this(df);
		this.file = file;
	}
	
	public StdPersistentPhoneBook(DataFactory<C, N> df) {
		Contract.checkCondition(df != null);
		this.df = df;
	}
	
	// REQUETES
	
	@Override
	public File getFile() {
		return file;
	}
	
	// COMMANDES
	
	@Override
	public void load() throws IOException, BadSyntaxException {
		Contract.checkCondition(file != null);
		BufferedReader br = null;
		try {
			br = new BufferedReader(new FileReader(file));
			Pattern p = Pattern.compile(LINE_PATTERN);
			Civ[] civs = Civ.values();
			String line = null;
			while ((line = br.readLine()) != null) {
				Matcher m = p.matcher(line);
				if (m.matches()) {
					String[] fields = line.split(FSEP);
					Civ civ = civs[Integer.parseInt(fields[0].trim())];
					C c = df.createContact(civ, fields[1].trim(), fields[2].trim());
					String[] phoneNumbers = fields[3].trim().split(NSEP);
					List<N> pNumbers = new ArrayList<N>();
					for (String n : phoneNumbers) {
						N num = df.createPhoneNumber(n);
						pNumbers.add(num);
					}
					addEntry(c, pNumbers);
				} else {
					throw new BadSyntaxException("Fichier mal formé");
				}
			}
		} catch (FileNotFoundException ex) {
			throw ex;
		} catch (IOException io) {
			clear();
			throw io;
		} catch (Error e) {
			clear();
			throw e;
		} catch (BadSyntaxException syn) {
			clear();
			throw syn;
		} finally {
			if (br != null) {
				br.close();
			}
		}
	}

	@Override
	public void save() throws IOException {
		Contract.checkCondition(file != null);
		BufferedWriter bw = null;
		try {
			bw = new BufferedWriter(new FileWriter(file));
			NavigableSet<C> contacts = contacts();
			String line = "";
			for (C c : contacts) {
				line += c.getCivility().ordinal() + FSEP
						+ c.getLastName() + FSEP
						+ c.getFirstName() + FSEP;
				List<N> n = phoneNumbers(c);
				int k = n.size();
				int i = 0;
				while (i < k) {
					String[] tab = n.get(i).international().split("~");
					line += tab[0] + (i == k - 1 ? "" : NSEP);
					i += 1;
				}
				bw.write(line);
				bw.newLine();
			}
			bw.close();
		} catch (FileNotFoundException ex) {
			throw ex;
		} catch (IOException io) {
			throw io;
		} finally {
			if (bw != null) {
				bw.close();
			}
		}
	}

	@Override
	public void setFile(File file) {
		Contract.checkCondition(file != null);
		this.file = file;
	}

}
