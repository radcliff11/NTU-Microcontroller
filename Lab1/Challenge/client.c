// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>
   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 

    char buffer[1024] = {0}; 

    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
    printf("Empty socket created\n");


    int port;
    port = atoi(argv[2]);

    struct hostent *server; 
    server = gethostbyname(argv[1]);
    
    memset((char *) &serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length); 
    serv_addr.sin_port = htons(port); 
       

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    printf("Connection established\n"); 
    
    char hello[1000];
    while (1) {
	    memset(hello,'\0',sizeof(hello));  // clean buffer for output msg
	    printf("Please enter the msg you want to send to server: ");
	    fgets(hello, 1000,stdin);
            if (strcmp(hello,"STOP\n") == 0) {
                send(sock , hello , strlen(hello) , 0 );
		return 0;
	    }	
	    send(sock , hello , strlen(hello) , 0 ); 
	    printf("Message sent\n"); 
            memset(buffer,'\0',sizeof(buffer));  //clean buffer for input msg from server
	    valread = read( sock , buffer, 1024); 
	    printf("Msg from server:%s\n",buffer ); 
    }
    return 0; 
} 
