#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write


// Vector Operation
int dot(int v1[], int v2[])
{
	int sum = 0;
	for (int i = 0; i < 5; i++)
		sum += v1[i] * v2[i];
	
	return sum;
}

// Print
void print(int vec[]){
	printf("***********Vector***********:\n");
	for (int i = 0; i < 5; i++){
		printf("%d ",vec[i]);
	}
	printf("\n");
}


int bufferSize;
int main(int argc , char *argv[])
{

	int socket_desc , new_socket, c;
	struct sockaddr_in server, client;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc < 0)
	{
		printf("Could not create socket");
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 1025 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("binding done");
	
	//Listen
	listen(socket_desc , 1);
	
	//Accepts an incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
    	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c); 
    
    // Accept incoming connection

	// Receive vector size 
	recv(new_socket, &bufferSize, sizeof(bufferSize), 0);

	// Initalize vector
	int V1[bufferSize];
	int V2[bufferSize];

	// Receive vector
	if (recv(new_socket, V1, sizeof(V1), 0) < 0){
		perror("Error receiving");
	} 

	if (recv(new_socket, V2, sizeof(V1), 0) < 0){
		perror("Error receiving");
	} 
	
	printf("Vectors Received\n");
	print(V1);
	print(V2);
	
	// Perform dot product on partial vector
	int sum = dot(V1, V2);

	// Send sum
	send(new_socket, &sum, sizeof(sum), 0);

	// close
	close(socket_desc);
	
	return 0;
}
