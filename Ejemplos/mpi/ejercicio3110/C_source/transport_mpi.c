/*
 *     It evolves the equation:
 *                             u,t + u,x + u,y = 0
 *     Using a Lax scheme.
 *     The initial data is a cruddy gaussian distributed by columns
 *     and data allocated dynamically
 *     Using periodic boundaries     
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define NX 100
#define NY 100
#define LX 2.0
#define LY 2.0

#define DT 0.01
#define STEPS 15000

#define sigma 0.1
#define tmax 100


void printMatrix(float *matrix, int myid){
  int ix,iy;

  printf("Printing matrix for process %d\n", myid);
  
  for(iy=0;iy<=NY+1;++iy){
    for(ix=0;ix<=NX+1;++ix){
      printf("%f,", matrix[((NX+2)*iy)+ix]);
    }
    printf("\n");
  }
  
  printf("-----------------\n");
}

/* Return the second elapsed since Epoch (00:00:00 UTC, January 1, 1970)                                                                    
 */
double seconds(){
  struct timeval tmp;
  double sec = 0.0;
  gettimeofday( &tmp, (struct timezone *)0 );
  sec = tmp.tv_sec + ((double)tmp.tv_usec)/1000000.0; 
  return sec;
}


/* 
 * conversions from discrete to real coordinates
 */

float ix2x(int ix){
  return ((ix-1) - (NX-1) / 2.0)*LX/(NX-1);
}

float iy2y(int iy){ 
  return ((iy-1) - (NY-1) / 2.0)*LY/(NY-1);
}

/* Function for evaluating the results that calculates the sum of the array values */
float summa(int nx, int ny, float* val){
  float summma=0.0;
  int ix,iy;
  for(iy=1;iy<=ny;++iy)
    for(ix=1;ix<=nx;++ix){
      summma+=val[((nx+2)*iy)+ix];
    } 
  return(summma);
}

/* 
 * initialize the system with a gaussian temperature distribution 
 */
int init_transport(float *temp){ 
  int ix,iy;
  float x,y;

  for(iy=1;iy<=NY;++iy)
    for(ix=1;ix<=NX;++ix){
      x=ix2x(ix);
      y=iy2y(iy);
      temp[((NX+2)*iy)+ix] = tmax*exp((-((x*x)+(y*y)))/(2.0*(sigma*sigma)));
    }
  return 0;
}


/*
 * save the temperature distribution
 * the ascii format is suitable for splot gnuplot function
 */
int save_gnuplot(char *filename, float *temp) {
  int ix,iy;
  FILE *fp;

  fp = fopen(filename, "w");   
  for(iy=1;iy<=NY;++iy){
    for(ix=1;ix<=NX;++ix)
      fprintf(fp, "\t%f\t%f\t%g\n", ix2x(ix), iy2y(iy), temp[((NX+2)*iy)+ix]);
    fprintf(fp, "\n");
  }
  fclose(fp);

  return 0;
}

int update_boundaries_PBC(float *temp){
  int ix=0, iy=0;

  for(iy=0;iy<=NY+1;++iy){
    temp[(NX+2)*iy] = temp[((NX+2)*iy)+NX];
    temp[((NX+2)*iy)+(NX+1)] = temp[((NX+2)*iy)+1];
  }

  for(ix=0;ix<=NX+1;++ix){
    temp[ix] = temp[((NX+2)*NY)+ix];
    temp[((NX+2)*(NY+1))+ix] = temp[(NX+2)+ix];
  }
  return 0;
}

int evolve(float dtfact, float *temp, float *temp_new, int startY, int endY){
  float dx, dt;
  int ix, iy;
  float temp0;

  dx = 2*LX/NX;
  dt = dtfact*dx/sqrt(3.0);
  
  // Calculate only for my designates range from startY to endY
  for(iy=startY;iy<=endY;++iy)
    for(ix=1;ix<=NX;++ix){
      temp0 = temp[((NX+2)*iy)+ix];
      temp_new[((NX+2)*iy)+ix] = temp0-0.5*dt*(temp[((NX+2)*(iy+1))+ix]
					       -temp[((NX+2)*(iy-1))+ix]+temp[((NX+2)*iy)+(ix+1)]-temp[((NX+2)*iy)+(ix-1)])/dx;
    }

  // Set temp to 0
  for(iy=0;iy<=NY+1;++iy)
    for(ix=0;ix<=NX+1;++ix)
      temp[((NX+2)*iy)+ix] = 0.0;
	
  for(iy=startY;iy<endY;++iy)
    for(ix=0;ix<NX+1;++ix)
      temp[((NX+2)*iy)+ix] = temp_new[((NX+2)*iy)+ix];

  return 0;
}

int main(int argc, char* argv[]){
  
  int i=0, nRow=NX+2, nCol=NY+2, myid, nProcs, Nrows, startY, endY, rest, dataSize, ix, iy;
  float *temp, *temp2, *temp_new, *temp_swap;
  double tinit, tstart, tstop, tio;  	

	char filename_0[25];
	char filename_1[25];

	MPI_Init(&argc, &argv);	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	// Number of rows that this process will use
	Nrows = NY / nProcs;

	rest = NY % nProcs;

	startY = 1 + (myid * Nrows);
	endY = startY + Nrows;

	if(myid == nProcs - 1)
		endY += rest;

	for(i=0;i<25;i++){
		filename_0[i] = 0;
		filename_1[i] = 0;
	}

	sprintf(filename_0, "transport-mpi_%d.dat", myid);
	sprintf(filename_1, "transport-mpi-end_%d.dat", myid);

	printf("I am process %d and my range is from %d to %d\n", myid, startY, endY);

	tinit = seconds();

	dataSize = nRow*nCol*sizeof(float);
	
	temp = (float *) malloc (dataSize);
	temp2 = (float *) malloc (dataSize);
	temp_new = (float *) malloc (dataSize);

	if(myid == 0){
		// Only root node does the initialization
		init_transport(temp);
		update_boundaries_PBC(temp);

		tstop = seconds();

		printf("\ninitialization done\n");
		printf("cpu time in seconds %.3g\n", tstop-tinit );

		tstart = seconds();
		save_gnuplot(filename_0, temp);
		tstop = seconds();

		tio = tstop - tstart;
		
	}

	MPI_Bcast(temp, nRow*nCol, MPI_FLOAT, 0, MPI_COMM_WORLD);	
	// At this point all processes have the initial matrix (CHECKED DATA IS OK)
	tstart = seconds();
	for(i=1;i<=STEPS;++i) {		
		evolve(DT, temp, temp_new, startY, endY);
	  
		if(myid == 0){
		  for(iy=1;iy<=NY;++iy)
		    for(ix=1;ix<=NX;++ix)
		      temp2[((NX+2)*iy)+ix] = 0.0;
		}

		MPI_Reduce(temp, temp2, nRow*nCol, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);		
		// Need to copy back to temp. Can I just swap the pointers?		
		if(myid == 0){
		  temp_swap = temp;
		  temp = temp2;
		  temp2 = temp_swap;
		  //printMatrix(temp, myid);
		  update_boundaries_PBC(temp);
		}				
												
		MPI_Bcast(temp, nRow*nCol, MPI_FLOAT, 0, MPI_COMM_WORLD);
		
	}
	
	tstop = seconds();
	
	if(myid == 0){
		printf("\nevolution done\n");
		printf("cpu time in seconds %.3g\n", tstop-tstart);
	}

#ifdef __DEBUG
	float after=summa(NX, NY, temp);
	printf(" sum temp after: %f\n",after);
#endif

	if(myid == 0){
		tstart = seconds();
		save_gnuplot(filename_1, temp);
		tstop = seconds();

		tio = tio + tstop - tstart;
		printf("\nsave_data done\n");
		printf("IO time in seconds %.3g\n", tio);
		tstop = seconds();
		printf("\ntotal cpu time in seconds %.3g\n", tstop - tinit);
	}

	free(temp);
	free(temp_new);

	MPI_Finalize();

  return 0;
}
