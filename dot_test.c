// C Program to find sum of array using multithreading
#include <stdio.h>
#include <stdlib.h>

// size of array
// Defines constant in C/C++
#define MAX 10 

int v1[MAX];
int v2[MAX];

void initVector(){
    for(int i = 0; i<MAX; i++){
        v1[i] =i*2;
		v2[i] = (i*2)+1;
    }
}

void print(){

    printf("VECTOR 1:");
	for (int i = 0; i < MAX; i++){
        printf("%d ", v1[i]);
    }
	printf("\n");
	printf("VECTOR 1:");
	for (int i = 0; i < MAX; i++){
        printf("%d ", v2[i]);
    }
	printf("\n");
}

void dot()
{
	int sum = 0;
	int n = sizeof(v1)/sizeof(v1[0]);
    for (int i = 0; i < n; i++)
		sum += v1[i] * v2[i];

	printf("Result:%d\n",sum);
}

// Driver Code
int main(){

    // Create vectors
    initVector();
    print();

    dot();

	return 0;
}