#include <stdio.h>

float sum_elements (float a[], unsigned length){
	int i;
	float result = 0;
	
	for(i=0; i<=length-1; i++){
		result += a[i];
	}

	return result;
}

int main(void){
	int num;
	float arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	
	printf("how many numbers to sum up? <=10\n : ");
	scanf("%d", &num);
	
	while(num>10){
		printf("put number less than or equal to 10 : ");
		scanf("%d", &num);
	}

	printf("%1.2f\n", sum_elements(arr, num));

	return 0;
}
