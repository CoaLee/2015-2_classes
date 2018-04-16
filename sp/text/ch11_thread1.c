#include <stdio.h>
#include <pthread.h>

#define N 100

int main(){
  pthread_t tid[N]; 
	int i;
	
	for(i=0;i<N;i++){
	  pthread_create(&tid[i], NULL, thread, &i);
	}

	for(i=0;i<N;i++)
	  pthread_join(tid[i], NULL);

	return 0;
}

void *thread(void *vargp){
  int myid = *((int*)vargp);
	printf("Hello from thread %d\n", myid);
	return NULL;
}
