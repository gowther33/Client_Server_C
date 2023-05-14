#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write


/*
Socket programming structures in C/C++:
// IPv4 AF_INET sockets:
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET, AF_INET6
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr;          // load with inet_pton()
};

struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
};
*/



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
	server.sin_family = AF_INET; //Address Family - AF_INET (this is IP version 4) 
	server.sin_addr.s_addr = INADDR_ANY; // OS will assign any address
	server.sin_port = htons( 1024 );

	//Bind
	// Returns a positve int
	if( bind(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("binding done");
	
	//Listen
	listen(socket_desc , 1);
	
	//Accepts an incoming connection
	// Here the server starts listening , its a blocking call
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
    new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c); 
    
    // Accept incoming connection

	// Recv should be written in loop as it is not certain that we will receive all the data in one go
	// so after receiving buffersize we must keep checking how much data we have received
	// recv returns how much bytes were received
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
