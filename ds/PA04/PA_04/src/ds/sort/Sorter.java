package ds.sort;

public class Sorter{
	private int[] array;
	//private int newloc;
	private int cnt;
	//private LinkedList<Integer> removed; //to avoid moving elements around each add/remove operations.
	public final int NOTSORTED = 0;
	public final int ASCENDING = 1;
	public final int DESCENDING = -1;
	public final int LARGEST = 1;
	public final int SMALLEST = -1;
	private int howSorted = NOTSORTED;	
	
	public Sorter(int n){
		array = new int[n];
	//	newloc = 0;
		cnt = 0;
	//	removed = new LinkedList<Integer>();
	}
	
	public void add(int value){
		/*if(newloc==100000)
			if(!compression()) return;
		if(removed.isEmpty()) array[newloc++]=value;
		else array[removed.removeLast()]=value;*/
		howSorted=NOTSORTED;
		array[cnt++]=value;
		return;
	}
	/*
	private boolean compression(){ 
		if(cnt==100000) return false;
		while(!removed.isEmpty()) 
			array[removed.removeLast()]=array[--newloc];
		return true;
	}
	*/
	public boolean remove(int value){
		Integer k = findindex(array, value);
		if(k==null) return false;
		else {
			//removed.add(k);
			if(k!=cnt-1) array[k]=array[cnt-1];
			howSorted=NOTSORTED;
			cnt--;
			return true;
		}
	}
	
	private Integer findindex(int[] arr, int val){
		int i;
		for(i=0;i<cnt;i++) if(arr[i]==val) return i;
		/*
		for(i=0;i<newloc;i++) 
			if(arr[i]==val) 
				if(!removed.contains(i)) return i;*/
		return null;
	}
	
	public void sort(int ordering){
		//compression();
		switch (ordering){
		case ASCENDING:
			qsortAsc(array, 0, cnt-1);
			howSorted=ASCENDING;
			break;
		case DESCENDING:
			qsortDes(array, 0, cnt-1);
			howSorted=DESCENDING;
			break;
		}
		return;
	}
	
	private void qsortAsc(int[] arr, final int l, final int r){
		int pivot, mvL=l, mvR=r;
		
		if(l>=r) return; //filtering meaningless cases out
		
		if(r-l<=5){		//Selection sort for few elements
			int i, j, minPos;
			for(i=l;i<r;i++){
				minPos=i;
				for(j=i+1;j<=r;j++) if(arr[j]<arr[minPos]) minPos=j;
				if(i!=minPos) swap(arr, i, minPos);
			}
		} else {
			pivot=l+(int)Math.random()*(r-l);
			swap(arr, pivot, r);
			while(mvL!=mvR){
				while(arr[mvL]<arr[r]) mvL++;
				while(arr[mvR]>arr[r]) mvR++;
				swap(arr,mvL,mvR);
			}
			swap(arr,mvR,r);
			qsortAsc(arr,l,mvL-1);
			qsortAsc(arr,mvR+1,r);
		}
		return;
	}
	
	private void qsortDes(int[] arr, final int l, final int r){
		int pivot, mvL=l, mvR=r;
		
		if(l>=r) return; //filtering meaningless cases out
		
	
		if(r-l<=5){		//Selection sort for few elements
			int i, j, maxPos;
			for(i=l;i<r;i++){
				maxPos=i;
				for(j=i+1;j<=r;j++) if(arr[j]>arr[maxPos]) maxPos=j;
				if(i!=maxPos) swap(arr, i, maxPos);
			}
		} else {
			pivot=l+(int)Math.random()*(r-l);
			swap(arr, pivot, r);
			while(mvL!=mvR){
				while(arr[mvL]>arr[r]) mvL++;
				while(arr[mvR]<arr[r]) mvR++;
				swap(arr,mvL,mvR);
			}
			swap(arr,mvR,r);
			qsortDes(arr,l,mvL-1);
			qsortDes(arr,mvR+1,r);
		}
		return;
	}
	
	private void swap(int[] arr, int a, int b){
		int tmp=arr[a];
		arr[a]=arr[b];
		arr[b]=tmp;
		return;
	}
	
	public void top(int k, int isLargest){
		String res="";
		int limit = (k<cnt)? k:cnt;
		
		if(howSorted==NOTSORTED) sort(ASCENDING);
		if(howSorted*isLargest==LARGEST) //from last
			for(int i=1;i<=limit;i++) res=res+" "+array[cnt-i];
		else 							//from first
			for(int i=0;i<limit;i++) res=res+" "+array[i];
		System.out.println("TOP:"+res);
		return;
	}
	
	public void sequencing(){
		for(int i=0;i<cnt;) System.out.print(" "+array[i++]);
		return;
	}
	
	public void sequencingReverse(){
		for(int i=cnt;i>0;) System.out.print(" "+array[--i]);
		return;
	}
	
	public int getCnt(){
		return cnt;
	}
	
	public int getHowSorted(){
		return howSorted;
	}
}