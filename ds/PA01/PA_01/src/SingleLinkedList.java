/** Source code example for "A Practical Introduction to Data
    Structures and Algorithm Analysis, 3rd Edition (Java)" 
    by Clifford A. Shaffer
    Copyright 2008-2011 by Clifford A. Shaffer
 */

/** Linked list implementation */
class SingleLinkedList<E> implements List<E> {
	private Link<E> head;         // Pointer to list header
	private Link<E> tail;         // Pointer to last element
	protected Link<E> curr;       // Access to current element
	int cnt;		      // Size of list

	/** Constructors */
	SingleLinkedList(int size) { this(); }   // Constructor -- Ignore size
	SingleLinkedList() {
		curr = tail = head = new Link<E>(null);
		cnt = 0;
	}

	/** Remove all elements */
	public void clear() {
		curr = tail = head = new Link<E>(null);
		cnt = 0;
	}

	/** Insert "it" at current position */
	public void insert(E it) {
		curr.setNext(new Link<E>(it, curr.next()));
		if(tail==curr) tail=curr.next();
		cnt++;
	}

	/** Append "it" to list */
	public void append(E it) {
		tail.setNext(new Link<E>(it, null));
		tail=tail.next();
		cnt++;
	}

	/** Remove and return current element */
	public E remove() {
		if(curr==tail) return null;
		Link<E> tmpp = curr.next();
		if(tmpp==tail) tail=curr;
		curr.setNext(tmpp.next());
		cnt--;
		return tmpp.element();                         // Return value
	}

	/** Set curr at list start */
	public void moveToStart(){
		curr=head;
	}
	/** Set curr at list end */
	public void moveToEnd(){ 
		curr=tail;
	}

	/** Move curr one step left; no change if now at front */
	public void prev() {
		if(curr==head) return;
		Link<E> tmpp = head;
		while(tmpp.next()!=curr) tmpp=tmpp.next();
		curr = tmpp;
	}

	/** Move curr one step right; no change if now at end */
	public void next(){ 
		if(curr!=tail) curr=curr.next();
	}

	/** @return List length */
	public int length() {
		return cnt;
	}

	/** @return The position of the current element */
	public int currPos() {
		Link<E> tmpp=head;
		int i=1;
		while(tmpp!=curr) {
			tmpp=tmpp.next();
			i++;
		}
		return i;
	}

	/** Move down list to "pos" position */
	public void moveToPos(int pos) {
		assert(pos>=0&&pos<cnt): "Position out of range";
		curr=head;
		for(int i=1; i<pos; i++) curr = curr.next();
	}

	/** @return Current element value */
	public E getValue() {
		if(curr.next() == null) return null;
		return curr.next().element();
	}

}