// School on parallel programming, ICTP, Trieste, 2014
// Jeudy Blanco - UCR

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define BUFFER_SIZE 100

int main(int argc, char *argv[]){

	int myid, size, i, received;
	int buffer[BUFFER_SIZE];

	MPI_Status status;

	MPI_Init(&argc, &argv);	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	for(i = 0; i < BUFFER_SIZE; i++){
		buffer[i] = myid;
	}

	printf("Soy el proceso %d de %d\n", myid, size);

	// Process 0 sends its buffer to process 1 (assumes only 2 processes?)

	if(myid == 0){
		MPI_Send(buffer, BUFFER_SIZE, MPI_INT, 1, 100, MPI_COMM_WORLD);
	}
	else if(myid == 1){
		MPI_Recv(buffer, BUFFER_SIZE, MPI_INT, 0, 100, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &received);
		printf("Received %d bytes from process 0\n", received);
	}

	printf("I am process %d and this is my buffer:\n", myid);
	for(i = 0; i < BUFFER_SIZE; i++){
		printf("%d,", buffer[i]);
	}

	MPI_Finalize();
	return 0;
}
