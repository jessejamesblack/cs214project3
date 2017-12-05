	#include "socket.h"
	#include <string.h>
	#include <pthread.h>
	#define PORT 8080
	void * clientHandler(void *);
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	
	int main(int argc, char const *argv[])
	{
	    int server_fd, new_socket, valread;
	    struct sockaddr_in address;
	    int opt = 1;
	    int addrlen = sizeof(address);
	    char buffer[1024] = {0};
	
		 printf("waiting for client...\n");
		 

	    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	    {
	        perror("socket failed");
	        exit(EXIT_FAILURE);
	    }
	
	    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
	                   &opt, sizeof(opt)))
	    {
	        perror("setsockopt");
	        exit(EXIT_FAILURE);
	    }
	    
	    address.sin_family = AF_INET;
	    address.sin_addr.s_addr = INADDR_ANY;
	    address.sin_port = htons(PORT);
	
	    // Forcefully attaching socket to the port 8080
	
	 	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	   {
	        perror("bind failed");
	        exit(EXIT_FAILURE);
	    }
	   
	 	if (listen(server_fd, 3) < 0)
	   {
	        perror("listen");
	        exit(EXIT_FAILURE);
	   }
	   	   
	   
		pthread_t tids[1000];
		
		int tidCount = 0;
		
		
  while ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t *)&addrlen)))
	   {
			puts("connection accepted, spawning thread to process the client");
			tidCount++;
			void * arg = (void *) new_socket;
			pthread_create(&tids[tidCount - 1], NULL, clientHandler, arg); 
			pthread_join(tids[tidCount-1], NULL);
	   }
	      
		// Need to fix this, will never be reachable here, need to move into the while loop above, but have a condition to enter and join.
		int j = 0;
		for(j = 0; j < tidCount; j++){
			pthread_join(tids[j], NULL);	
		}
	 return 0;
	}
	
	
void * clientHandler (void * args){
		pthread_mutex_lock(&mutex);
		   int socketFD = (int) args;			
			int valread;	
			FILE * file = fopen("gather.txt", "wb");
			char buffer[1024];
			int tot = 0;
			int b = 0;
			while(1) {
				while((b = recv(socketFD, buffer, 1024, 0)) > 0) {	
					tot+= b;
					fwrite(buffer, 1, b ,file);	
				}
				fclose(file);
				break;
			}
			
			
			
	     pthread_mutex_unlock(&mutex);
	   
		pthread_exit(NULL);
}



