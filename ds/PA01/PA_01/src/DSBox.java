/**
 * Implement this class using SingleLinkedList
 * @author DMLab
 *
 */
public class DSBox {
	private SingleLinkedList<String> list;
	
	DSBox(){
		list=new SingleLinkedList<>();
	}
	
	/**
	 * 
	 * @param document
	 */
	public void submit(String document){
		list.insert(document);
	}
	
	/**
	 * 
	 * @return
	 */
	public String get_top(){
		if(list.cnt==0) return "Box is Empty";
		list.moveToStart();
		return list.remove();
	}
	
	/**
	 * 
	 * @return
	 */
	public String get_bottom(){
		if(list.cnt==0) return "Box is Empty";
		list.moveToEnd();
		list.prev();
		return list.remove();
	}
	
	/**
	 * 
	 * @return
	 */
	public String view_top(){
		if(list.cnt==0) return "Box is Empty";
		list.moveToStart();
		return list.getValue();
	}
	
	/**
	 * 
	 * @return
	 */
	public String view_bottom(){
		if(list.cnt==0) return "Box is Empty";
		list.moveToEnd();
		list.prev();
		return list.getValue();
	}
	
	/**
	 * 
	 * @return
	 */
	public int size(){
		return list.cnt;
	}
}
