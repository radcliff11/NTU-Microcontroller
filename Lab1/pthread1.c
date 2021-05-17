#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_msg(void *ptr) {
	char *text;
	text = (char *) ptr;
	printf("%s \n",text);
}

int main() {
	pthread_t t1, t2;
	char *str1 = "I am thread 1";
	char *str2 = "I am thread 2";
	int T1ret, T2ret;

	T1ret = pthread_create(&t1,NULL, print_msg, (void *) str1);
	T2ret = pthread_create(&t2,NULL, print_msg, (void *) str2);

	pthread_join( t1, NULL);
	pthread_join( t2, NULL);

	printf("T1 and T2 return: %d, %d\n",T1ret, T2ret);
	return 0;
	
}
