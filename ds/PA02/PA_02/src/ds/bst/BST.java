package ds.bst;

import java.lang.Comparable;
import ds.list.SingleLinkedList;

/** Binary Search Tree implementation for Dictionary ADT */
public class BST<Key extends Comparable<? super Key>, E>
implements Dictionary<Key, E> {
	private BSTNode<Key,E> root; // Root of the BST
	int nodecount;             // Number of nodes in the BST

	/** Constructor */
	public BST() { 
		root = null; nodecount = 0;
	}

	/** Reinitialize tree */
	public void clear() { 
		root = null; nodecount = 0;
	}

	/** Insert a record into the tree.
      @param k Key value of the record.
      @param e The record to insert. */
	public void insert(Key k, E e) {
		root = inserthelp(root, k, e);
		nodecount++;
	}

	/** Remove a record from the tree.
      @param k Key value of record to remove.
      @return The record removed, null if there is none. */
	public E remove(Key k) {
		E tobeRemoved = findhelp(root, k);
		if(tobeRemoved!=null) root=removehelp(root, k);
		return tobeRemoved;
	}

	/** Remove and return the root node from the dictionary.
      @return The record removed, null if tree is empty. */
	public E removeAny() {
		if (root == null) return null;
		E temp = root.element();
		root = removehelp(root, root.key());
		nodecount--;
		return temp;
	}

	/** @return The number of records in the dictionary. */
	public int size() { 
		return nodecount;
	}
	
	/** @return Record with key value k, null if none exist.
      @param k The key value to find. */
	public E find(Key k) { 
		return findhelp(root, k);
	}

	private E findhelp(BSTNode<Key,E> rt, Key k) {
		if(rt==null) return null;
		else if(rt.key().compareTo(k)==0) return rt.element();
		else if(rt.key().compareTo(k)>0) return findhelp(rt.left(), k);
		else return findhelp(rt.right(), k);
	}

	public void inorder(SingleLinkedList<Key> list) { 
		if(root==null) return;
		inorderHelper(root, list); 
	}
	
	private void inorderHelper(BSTNode<Key, E> rt, SingleLinkedList<Key> list){
		if(rt.left()!=null) inorderHelper(rt.left(), list);
		list.append(rt.key());
		if(rt.right()!=null) inorderHelper(rt.right(), list);
	}
	
	public int rangeSearch(Key from, Key to) { 
		return rangeSearchHelper(root, from, to); 
	}
	
	private int rangeSearchHelper(BSTNode<Key, E> rt, Key from, Key to){
		int first, last, cnt =0;
		SingleLinkedList<Key> rangelist = new SingleLinkedList<Key>();
		if(from.equals(to)) {
			if(findhelp(rt, from)==null) return 0;
			else return 1;
		}
		inorderHelper(root, rangelist);
		while(rangelist!=null && rangelist.getValue().compareTo(from)<0) rangelist.next();
		first=rangelist.currPos();
		rangelist.moveToStart();
		while(rangelist!=null && rangelist.getValue().compareTo(to)<0) rangelist.next();
		last=rangelist.currPos();
		if(rangelist.getValue().equals(to)) cnt=1;
		
		return last-first+cnt;
	}
	
	
	/** @return The current subtree, modified to contain
   the new item */
	private BSTNode<Key,E> inserthelp(BSTNode<Key,E> rt, Key k, E e) {
		if (rt == null) return new BSTNode<Key,E>(k, e);
		if (rt.key().compareTo(k) > 0)
			rt.setLeft(inserthelp(rt.left(), k, e));
		else
			rt.setRight(inserthelp(rt.right(), k, e));
		return rt;
	}
	
	/** Remove a node with key value k
    @return The tree with the node removed */
	private BSTNode<Key,E> removehelp(BSTNode<Key,E> rt,Key k) {
		if(rt==null) return null;
		else if(rt.key().compareTo(k)==0) {
			nodecount--;
			if(rt.right()!=null) {
				BSTNode<Key, E> rightMin = rt.right();
				BSTNode<Key, E> parentOf = rt;
				while(rightMin.left()!=null) {
					parentOf=rightMin;
					rightMin=rightMin.left();
				}
				if(rt==parentOf) {
					rightMin.setLeft(rt.left());
					return rightMin;
				} else {
					parentOf.setLeft(rightMin.right());
					rightMin.setLeft(rt.left());
					return rightMin;
				}
			} else {
				return rt.left();
			}
			
		}
		else if(rt.key().compareTo(k)>0) {
			rt.setLeft(removehelp(rt.left(), k));
			return rt;
		}
		else {
			rt.setRight(removehelp(rt.right(), k));
			return rt;
		}
	}
	
	public BSTNode<Key,E> getmin() {
		if(root==null) return null;
		return getminhelp(root);
	}
	
	private BSTNode<Key,E> getminhelp(BSTNode<Key,E> rt) {
		if(rt.left()!=null) return getminhelp(rt.left());
		else return rt;
	}
	
	public BSTNode<Key,E> getmax() {
		if(root==null) return null;
		return getmaxhelp(root);
	}
	
	private BSTNode<Key,E> getmaxhelp(BSTNode<Key,E> rt) {
		if(rt.right()!=null) return getmaxhelp(rt.right());
		else return rt;
	}
	
	private BSTNode<Key,E> deletemin(BSTNode<Key,E> rt) {
		BSTNode<Key, E> tmpP = null;
		BSTNode<Key, E> tmpMin = rt;
		while(tmpMin!=null) {
			tmpP=tmpMin;
			tmpMin=tmpMin.left();
		}
		//when root is min
		if(tmpP==null) root = tmpMin.right();
		else tmpP.setLeft(tmpMin.right());
		
		tmpMin.setRight(null);
		return tmpMin;
	}
}