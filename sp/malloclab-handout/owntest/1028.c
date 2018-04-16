#include <stdlib.h>
#include <stdio.h>

int main(){
	int *p, i = 0;

	p = (int *)malloc(10*sizeof(int));

	for(;i<10;i++){
		p[i]=i;
		printf("%d\n", p[i]);
	}
	

	free(p);
	return 0;
}
