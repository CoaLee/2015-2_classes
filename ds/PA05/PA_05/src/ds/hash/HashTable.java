package ds.hash;

public class HashTable {
	private final int LINEAR =1;
	private final int QUADRA = 2;
	private int[] ht;
	private int policy;
	private int c1, c2, c3;
	
	public HashTable(int n) {
		ht = new int[n];
		for(int i=0;i<n;i++) ht[i]=-1;
	}

	public void Start(String policy, int constant1, int constant2, int constant3){
		if(policy.equals("linear")) {
			this.policy=LINEAR;
			c1=constant1;
			c2=0;
			c3=0;
		} else if (policy.equals("quadratic")){
			this.policy=QUADRA;
			c1=constant1;
			c2=constant2;
			c3=constant3;
		}
		return;
	}
	
	public void Insert(int value) {
		ht[resolution(value)]=value;
	}

	public int find(int value) {
		for(int i=0;i<1001;i++) if(ht[i]==value) return i;
		return -1;
	}
	
	private int h(int k){
		return k%1001;
	}
	
	private int resolution(int v){
		int idx=h(v);
		int i;
		
		switch(policy){
		case LINEAR:
			for(i=0;ht[idx]!=-1;i++){
				idx=(idx+c1)%1001;
			}
			break;
		case QUADRA:
			int home=idx;
			for(i=1;ht[idx]!=-1;i++){
				idx=(home+c1*mpow(i,2)+c2*i+c3)%1001;
			}
			break;
		}
		
		return idx;
	}
	
	private int mpow(int a, int b){
		int res=1;
		while(b>0){
			if((b&1)>0) res*=a;
			b>>=1;
			a*=a;
		}
		return res;
	}
}
