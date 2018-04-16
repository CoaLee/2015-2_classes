#include <stdio.h>

int main(){
  int sum=0, input=-1;

	while(input!=0){
    printf("Current sum = %d, put a number to add, 0 to exit : ",sum);
		scanf("%d", &input);
		sum+=input;
	}
	printf("\nTotal: %d\n\n", sum);
	return 1;
}
