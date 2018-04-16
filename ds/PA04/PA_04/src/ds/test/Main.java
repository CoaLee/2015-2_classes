package ds.test;

import java.io.FileNotFoundException;
import java.io.StringReader;
import java.util.Scanner;

import ds.sort.Sorter;


public class Main {
	private static final int ERROR = -1;
	private static final int ADD = 0;
	private static final int REMOVE = 1;
	private static final int SORT = 2;
	private static final int TOP = 3;

	public static void main(String[] args) throws FileNotFoundException{
		
		int size = 200000;
		Sorter sorter = new Sorter(size);
		Scanner scanner = new Scanner(System.in);
		
		while(scanner.hasNext()){
			String line = scanner.nextLine();
			Scanner i_scanner = new Scanner(new StringReader(line));
			String cmd = i_scanner.next();
			int value = 0;
			int k = 0;
			String type = null;
			
			switch(getCommandNum(cmd)){
			case ADD:
				value = i_scanner.nextInt();
								
				sorter.add(value);
				System.out.println("ADD: "+value);
				
				break;
			case REMOVE:
				value = i_scanner.nextInt();

				if(sorter.remove(value)) System.out.println("REMOVE: "+value);
				else System.out.println("The value does not exist");
				
				break;
			case SORT:
				type = i_scanner.next();
				
				if(sorter.getCnt()==0) System.out.println("ERROR: The array is empty.");
				else if(type.equals("descend")) {
					int howSorted=sorter.getHowSorted();
					System.out.print("SORT:");
					if(howSorted==sorter.NOTSORTED){
						sorter.sort(sorter.DESCENDING);
						sorter.sequencing();
					} else if(howSorted==sorter.DESCENDING){
						sorter.sequencing();
					} else {
						sorter.sequencingReverse();
					}
					System.out.println("");
				}
				else if(type.equals("ascend")) {
					int howSorted=sorter.getHowSorted();
					System.out.print("SORT:");
					if(howSorted==sorter.NOTSORTED){
						sorter.sort(sorter.ASCENDING);
						sorter.sequencing();
					} else if(howSorted==sorter.ASCENDING){
						sorter.sequencing();
					} else {
						sorter.sequencingReverse();
					}
					System.out.println("");
				}
				else System.out.println("ERROR: Wrong input. Put 'descend' or 'ascend'.");
				
				break;
			case TOP:
				k = i_scanner.nextInt();
				type = i_scanner.next();
								
				if(sorter.getCnt()==0) System.out.println("ERROR: The array is empty.");
				else if(type.equals("largest")) sorter.top(k, sorter.LARGEST);
				else if(type.equals("smallest")) sorter.top(k, sorter.SMALLEST);
				else System.out.println("ERROR: Wrong input. Put 'largest' or 'smallest'.");
				
				break;
			case ERROR:
				System.out.println("ERROR: put correct command. 'add', 'remove', 'sort', 'top'");
				
				break;
			}
			i_scanner.close();
		}
		
		scanner.close();
	}
	
	private static int getCommandNum(String cmd){
		if(cmd.equals("add"))
			return ADD;
		if(cmd.equals("remove"))
			return REMOVE;
		else if(cmd.equals("sort"))
			return SORT;
		else if(cmd.equals("top"))
			return TOP;
		else 
			return ERROR;
	}

}
