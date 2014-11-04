// School on parallel programming, ICTP, Trieste, 2014
// Jeudy Blanco - UCR

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define TAG 1000

int main(int argc, char *argv[]){

	int myid, size, i, received, tag;
	int token[2];

	MPI_Status status;

	MPI_Init(&argc, &argv);	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	token[0] = myid;
	token[1] = myid;

	// Send section

	if(myid == size - 1){
		// If last, must send to process 0
		MPI_Send(token, 2, MPI_INT, 0, TAG, MPI_COMM_WORLD);
	}
	else{
		// Send data to process N + 1
		MPI_Send(token, 2, MPI_INT, (myid + 1), TAG, MPI_COMM_WORLD);
	}
	
	// Receive section

	if(myid == 0){
		MPI_Recv(token, 2, MPI_INT, size - 1, TAG, MPI_COMM_WORLD, &status);
	}
	else{
		MPI_Recv(token, 2, MPI_INT, (myid - 1), TAG, MPI_COMM_WORLD, &status);
	}

	printf("I am process %d and this is what I got [%d]:\n", myid, token[0]);

	MPI_Finalize();
	return 0;
}
