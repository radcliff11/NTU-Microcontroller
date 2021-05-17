#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int g_var1 = 0;

void *inc_gv() {
	int i,j;
	for (i = 0;i<10;i++) {
		g_var1++;
		for(j=0;j<5000000;j++) ; //delay
		printf(" %d", g_var1);
		fflush(stdout);	
	}
}

int main() {
	pthread_t t1,t2;
	pthread_create(&t1,NULL,inc_gv,NULL);
	pthread_create(&t2,NULL,inc_gv,NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("DONE");

}
