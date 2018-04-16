package ds.bst;

import ds.list.SingleLinkedList;

public class BookSearch {
	private BST<String, String> bst;
	
	public BookSearch(){
		bst=new BST<String, String>();
	}
	
	public void add(String name, String location){
		bst.insert(name, location);
	}
	
	public String remove(String name){
		return bst.remove(name);
	}
	
	public String get(String name){
		return bst.find(name);
	}
	
	public int size(){
		return bst.size();
	}
	
	public void order(){
		SingleLinkedList<String> booklist= new SingleLinkedList<String>();
		bst.inorder(booklist);
		if(booklist.getValue()==null) System.out.println("BookSearch does not have any book");
		else {
			while(booklist.getValue()!=null){
				System.out.println("ORDER: "+booklist.getValue());
				booklist.next();
			}
		}
	}
	
	public String first(){
		if(bst.size()==0) return null;
		return bst.getmin().key();
	}
	
	public String last(){
		if(bst.size()==0) return null;
		return bst.getmax().key();
	}
	
	public int range(String from, String to){
		if(bst.size()==0) return 0;
		return bst.rangeSearch(from, to);
	}
	
}
