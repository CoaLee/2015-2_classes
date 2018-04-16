#include <stdio.h>
#include <string.h>

int move2front(const char* arr, const int idx);
int transpose(const char* arr, const int idx);
void swap(const char* arr, const int a, const int b);

int main(int argc, char* argv[]){
  int i=0, cnt=0;
  char[] sequence = "DHHGHEGHGHECEHG";
	char[] array = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	char* ptr=sequence;
  
	for(i=0;i<15;i++){
    move2front(array,(*ptr++)-'A');
		cnt++
	}

	printf("%s / comparisons: %d\n", array, cnt);

/*
	switch (argc){
	case 1:
	case 2:
	  printf("put the way ('count', 'move-to-front', 'transpose')\nand # of letters and sequence\n ex)$cmd move-to-front 8 DHHGHEGHGHECEHG\n");
		return 1;
	case 3:
    break;    
  }
  return 0;
	*/
}

int move2front(const char* arr, const int idx){
  int i, len, tmp=arr[idx]; 

	if(idx==0) return 1;
	for(len=0;arr[len]!='\0';len++);
	if(idx>len) return -1;

	for(i=idx;i>idx;i--) arr[i]=arr[i-1];
  arr[0]=tmp;	
  return 1;
}

int transpose(const char* arr, const int idx){
  if(idx==0) return 1;
	swap(arr, idx-1, idx);
	return 1;
}

void swap(const char* arr, const int a, const int b){
  int tmp=arr[a];
	arr[a]=arr[b];
	arr[b]=tmp;
}
