#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "modIO_define.h"

char msg_buf[BUF_LEN];

void print_menu(void){
	printf("\n\n--------------------------------------\n\n\tWELCOME TO LKM MODULE\n\tAUTHOR: %s\n\tDESC: %s\n\tVERSION: %s\n\n--------------------------------------\n\n\n",AUTHOR,DESC,VER);
}

int main(int argc, char ** argv) {
	print_menu();
	char msg[10] = "Hello LKM";
	static char rx_buf[BUF_LEN];

	int fd, i;
	printf("Opening file /dev/kern_modIO\n");
	fd = open("/dev/kern_modIO",O_RDWR);
	if (fd < 0) {
		printf("open /dev/kern_modIO failed.\n YEET\n");
		exit (-1);	
	}
	printf("Open /dev/kern_modIO successfully.\n");

	//temp1 = '*';		
	i = write(fd,msg,10);
	printf ("Sent this shit to kernel: \'%s\'\n",msg);
	memset(rx_buf,'\0',BUF_LEN);
	i = read(fd,rx_buf,BUF_LEN);
	printf("number of char read = %d\nMsg is %s\n",i,rx_buf);
	if (i<0) {
		printf( "Error in read\n");
		close(fd);
		exit (-1);
	}
	else {
		printf ("Message received from Kernel: \'%s\'\n", rx_buf);
		close(fd);
		exit(0);
	}

}
