
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h>


char client_message[2000];
char buffer[1024];

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * socketThread(void *arg) 
{

    int newSocket = *((int *) arg);   
    
    while (1) {
    
    char* temp_msg = malloc((sizeof(char)*2000));
    //Receive from client thread, 
    //remove mutex here cuz we assume the thread speed will be faster than incoming traffic  
    //that destroys the buffer (due to the scope of this exercise)
    //pthread_mutex_lock(&lock);	
    memset(client_message,'\0',sizeof(client_message));
    read(newSocket, client_message, sizeof(client_message));
    strcpy(temp_msg,client_message);
    //pthread_mutex_unlock(&lock);

    printf("Message received from socket %d: %s",newSocket,temp_msg);
    free(temp_msg);
    
    if (strcmp(temp_msg,"STOP\n") == 0) {
	break;
    }

    
    
    
    	
    
    int size = sizeof(char)*strlen(client_message)+20;
    char *msg = malloc(size);
    printf("Enter msg to client on socket %d: ",newSocket);

    //Sending message back to server
    pthread_mutex_lock(&lock);	
    fgets(msg,size, stdin);
    send(newSocket,msg,strlen(msg),0);
    pthread_mutex_unlock(&lock);

    //Clearing things up
    fflush(stdin);	
    printf("Sending to client on socket %d: %s",newSocket,msg);
    free(msg);
    }
    //pthread_mutex_unlock(&lock);
    printf("Exit socketThread\n");
    
    //close(newSocket);
    //pthread_exit(NULL); 
}

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address,cli_addr; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; //buffer to take in msg from client
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    printf("Socket created\n");
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }
     
    printf("Setting socket option\n");
    
    int port = atoi(argv[1]);

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( port ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    printf("Socket binding created\n");
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    printf("Listening on port %d\n",port);


    pthread_t tid[4];

    int i = 0;
    struct sockaddr_storage serverStorage;

    while(1)

    {
//Accept call creates a new socket for the incoming connection

        //addr_size = sizeof serverStorage;

        
	int cli_len;
    cli_len = sizeof(cli_addr);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&cli_addr,  
                       (socklen_t*)&cli_len))<0) 
    { 
        perror("Accept err: "); 
        exit(EXIT_FAILURE); 
    } 
    printf("Accepted connection on socket\n");
        //for each client request creates a thread and assign the client request to it to process

       //so the main thread can entertain next request

        if( pthread_create(&tid[i], NULL, socketThread, &new_socket) != 0 )

           printf("Failed to create thread\n");

        if( i >= 4)

        {

          i = 0;

          while(i < 4)

          {

            pthread_join(tid[i++],NULL);

          }

          i = 0;

        }

    }

  return 0;

} 



