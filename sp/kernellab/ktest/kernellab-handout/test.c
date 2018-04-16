#include <stdio.h>

int main(){
	char test[10][10]={0};
	int i;
	
	sprintf(test[0], "hello!");
	
	for(i=0;i<9;i++) printf("%d\n", *test[i]);
}
