#include<stdio.h>
#include<string.h>	//strlen memcpy
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<pthread.h> //for threading , link with lpthread
#include<unistd.h>
#include<stdlib.h>

/*
This is client program that connects with 2 servers and distributes vector between them to perform dot product of 2 vectors

*/


#define MAX 10 // Vector size
#define WORKERS 2 // Working threads

// int step = (MAX/WORKERS);
int sums[WORKERS] = { 0 };
int Ports[WORKERS] = {1024, 1025}; // Server ports

// Global Initalizations and Declarations
int V1[MAX];
int V2[MAX];
int rem = MAX % WORKERS;
int size = 0; // Size of vectors to be sent to the workers


// This structure holds arguments to pass to threads
struct thread_args{
	char *ADDR;
	int PORT;
	int INDEX;
};

void* create_connection(void *_args)
{
	// Get Socket Parameters
	struct thread_args *args = (struct thread_args *) _args;

	int socket_desc;
	struct sockaddr_in server;

	//Create socket
	// SOCK_DGRAM = UDP
	// SOCK_STREAM = TCP
	// socket returns connection handler
	// It returns a positive integer
	socket_desc = socket(AF_INET , SOCK_STREAM , 0); // 3rd arg = 0 detects the protocol from 2nd arg
	if (socket_desc == -1)
	{
		// puts() prints an input string on stout and appends a newline
		puts("Could not create socket"); 
	}
	
	server.sin_addr.s_addr = inet_addr(args->ADDR); // inet_addr() converts ip to long format
	server.sin_family = AF_INET;
	server.sin_port = htons( args->PORT ); // sender converts port number host to network short/long

	//Connect to remote server
	// It is blocking function and here 3way handshake is done
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
		pthread_exit(NULL);
	}
	
	puts("Connected\n");
	
	// Set vectors
	int slicedV1[size];
	int slicedV2[size];


	// Send vector size
	send(socket_desc, &size, sizeof(size), 0);

	// Send vectors
    memcpy(slicedV1, V1+(args->INDEX * size), size * sizeof(int));
	send(socket_desc, slicedV1, sizeof(slicedV1), 0);

    memcpy(slicedV2, V2+(args->INDEX * size), size * sizeof(int));
	send(socket_desc, slicedV2, sizeof(slicedV2), 0);

	// Receive
	int res;
	recv(socket_desc, &res, sizeof(res), 0 );

	sums[args->INDEX] = res;

	//Close connection
	close(socket_desc);
	pthread_exit(NULL);
}

// Vector Initialization
void initVector(){
    for(int i = 0; i<MAX; i++){
       V1[i] = i*2; //even
	V2[i] = (i*2)+1; //odd
    }
}


int main(int argc , char *argv[]){	
	// Init vectors
	initVector();


	pthread_t workers[WORKERS];


	// Check if load is balanced evenly

	if (rem == 0){
		size = MAX / WORKERS;
		for (int i = 0; i < WORKERS; i++){
			struct thread_args *args = malloc (sizeof (struct thread_args));
			args->ADDR = "127.0.0.1"; // local loopback address
			args->PORT = Ports[i];
			args->INDEX = i;

			if (pthread_create(&workers[i], NULL, create_connection, args) != 0){
				printf("Thread Creation Failed\n");
			}
		}
	}
	// Else
	else{
		size = MAX - rem;
		size = size / WORKERS;
		for (int i = 0; i < WORKERS; i++){
			struct thread_args *args = malloc (sizeof (struct thread_args));
			args->ADDR = "127.0.0.1";
			args->PORT = Ports[i];
			args->INDEX = i;

			if (pthread_create(&workers[i], NULL, create_connection, args) != 0){
				printf("Thread Creation Failed\n");
			}
		}
	}

	// Wait for threads execution
	for (int i = 0; i < WORKERS; i++){
		pthread_join(workers[i], NULL);
	}

	int total = 0;		
	if (rem != 0)
	{
		// Perform Dot operation on remaining elements
		// Start from next element the last thread counted to
		for(int i = MAX - rem; i < MAX; i++){
			total += V1[i] * V2[i];
		}		
		// Add total
		for(int i = 0; i < WORKERS; i++){
			total += sums[i];
		}
	}
	else
	{
		// Add sum and display
		for(int i = 0; i < WORKERS; i++){
			total += sums[i];
		}
	}
	printf("DOT PRODUCT IS: %d", total);
	return 0;
}