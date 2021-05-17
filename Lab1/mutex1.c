#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int g_var1 = 0;
pthread_mutex_t mutexA;


void *inc_gv(void *k) {
	int i,j;
	int t_id = *((int *) k);
	for (i = 0;i<10;i++) {
		pthread_mutex_lock(&mutexA);
		g_var1++;
		for(j=0;j<10000000;j++) ; //delay
		printf("Thread %d prints  %d\n",t_id, g_var1);
		fflush(stdout);	
		pthread_mutex_unlock(&mutexA);
	}
	free(k);
}

int main() {

	if (pthread_mutex_init(&mutexA,NULL)!=0) {
		printf("\nMutex init has failed\n");
		return 1;
	
	}

	pthread_t t1,t2;

	int *arg_p1 = malloc(sizeof(*arg_p1));
	int *arg_p2 = malloc(sizeof(*arg_p2));
	
	*arg_p1 = 1;
	*arg_p2 = 2;
	pthread_create(&t1,NULL,inc_gv, arg_p1);
	pthread_create(&t2,NULL,inc_gv, arg_p2);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("DONE\n");

}
