package ds.powergame;

public class ParPtrTree {
	private Integer[] array; // Node array

	public ParPtrTree(int size) {
		array = new Integer[size]; // Create node array
		for (int i = 0; i < size; i++)
			array[i] = null;
	}

	/** Determine if nodes are in different trees */
	public boolean differ(int a, int b) {
		Integer root1 = findWithoutCompression(a);     // Find root of node a
		Integer root2 = findWithoutCompression(b);     // Find root of node b
		return root1 != root2;       // Compare roots
	}

	/** Merge two subtrees */
	public void UNION(int a, int b) {
		Integer root1 = FIND(a);     // Find root of node a
		Integer root2 = FIND(b);     // Find root of node b
		if (root1 != root2) array[root2] = root1; // Merge
	}

	public Integer FIND(Integer curr) {
		if(array[curr]==null) return curr;
		return array[curr]=FIND(array[curr]);
	}
	
	private Integer findWithoutCompression(Integer curr){
		if(array[curr]==null) return curr;
		return findWithoutCompression(array[curr]);
	}
	
}
