#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "prj_types.h"
#include "server_rw.h"
#define PORT 8080

//TODO Add argument parser here

//init info in order to start the server
void init_server(struct sockaddr_in* address, int* server_fd, int* opt);
//Function to handle each client separately in a new thread
void* handleClient(void* args);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;//Thread options
    int addrlen = sizeof(address);
    
    init_server(&address, &server_fd, &opt);
    
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    while(1)
    {
 		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		{
		    perror("Accept error");
		    exit(EXIT_FAILURE);
		}
		
		pthread_t write_thr;
		pthread_create(&write_thr, NULL, handleClient, (void*) &new_socket);
		pthread_join(write_thr, NULL);
		
		close(new_socket);   
    }
   
    close(server_fd);
    return 0;
}

void* handleClient(void* args)
{
	package_t pack;
	int* sock = (int*) args;
	
	//Read the package the client has sent
	read(*sock, (package_t*) &pack, sizeof(package_t));
	if(pack.isMp3)//Parse the information
	{
		dum_mp3_t mp3;
		memset(&mp3, 0, sizeof(mp3));
		if(readMp3(pack.fileName, &mp3) == 1)
		{
			printf("Succesfully sent %s\n", pack.fileName);
		}
		else
		{
			printf("Cannot find %s\n", pack.fileName);
		}
		send(*sock, (void*) &mp3, sizeof(dum_mp3_t), 0);
	}
	else
	{
		dum_jpg_t jpg;
		memset(&jpg, 0, sizeof(jpg));
		if(readJpg(pack.fileName, &jpg) == 1)
		{
			printf("Succesfully sent %s\n", pack.fileName);
		}
		else
		{
			printf("Cannot find %s\n", pack.fileName);
			
		}
		send(*sock, (void*) &jpg, sizeof(dum_jpg_t), 0);
	}
}

void init_server(struct sockaddr_in* address, int* server_fd, int* opt)
{
	// Creating socket file descriptor
    if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(*opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons( PORT );
}
