/*
 *     It evolves the equation:
 *                             u,t + u,x + u,y = 0
 *     Using a Lax scheme.
 *     The initial data is a cruddy gaussian distributed by columns
 *     and data allocated dynamically
 *     Using periodic boundaries     
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NX 100
#define NY 100
#define LX 2.0
#define LY 2.0

#define DT 0.01
#define STEPS 15000

#define sigma 0.1
#define tmax 100


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

// Set to zero all values that I'm not concerned with
void zero_values(float *temp, int startY, int endY){ 
  int ix,iy;
  float x,y;

  /*for(iy=1;iy<=NY;++iy)
    for(ix=1;ix<=NrowsX;++ix){
      x=ix2x_specific(ix, NrowsX);
      y=iy2y(iy);
      temp[((NX+2)*iy)+ix] = 0.0;
    }*/
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


int evolve(float dtfact, float *temp, float *temp_new){
  float dx, dt;
  int ix, iy;
  float temp0;

  dx = 2*LX/NX;
  dt = dtfact*dx/sqrt(3.0);
  for(iy=1;iy<=NY;++iy)
    for(ix=1;ix<=NX;++ix){
      temp0 = temp[((NX+2)*iy)+ix];
      temp_new[((NX+2)*iy)+ix] = temp0-0.5*dt*(temp[((NX+2)*(iy+1))+ix]
					       -temp[((NX+2)*(iy-1))+ix]+temp[((NX+2)*iy)+(ix+1)]-temp[((NX+2)*iy)+(ix-1)])/dx;
    }

  for(iy=0;iy<=NY+1;++iy)
    for(ix=0;ix<=NX+1;++ix)
      temp[((NX+2)*iy)+ix] = temp_new[((NX+2)*iy)+ix];


  return 0;
}

int main(int argc, char* argv[]){
  
  int i=0, nRow=NX+2, nCol=NY+2;
  float *temp, *temp_new;
  double tinit, tstart, tstop, tio;  

  tinit = seconds();
    
  temp = (float *) malloc (nRow*nCol*sizeof(float));
  temp_new = (float *) malloc (nRow*nCol*sizeof(float));

  init_transport(temp);
  update_boundaries_PBC(temp);

#ifdef __DEBUG
  float before=summa(NX, NY, temp);
  printf(" sum temp before: %f\n",before);
#endif

  tstop = seconds();

  printf("\ninitialization done\n");
  printf("cpu time in seconds %.3g\n", tstop-tinit );

  tstart = seconds();
  save_gnuplot("transport-serial.dat", temp);
  tstop = seconds();

  tio = tstop - tstart;

  tstart = seconds();
  for(i=1;i<=STEPS;++i) {
    evolve(DT, temp, temp_new);
    update_boundaries_PBC(temp);
  }
  tstop = seconds();

  printf("\nevolution done\n");
  printf("cpu time in seconds %.3g\n", tstop-tstart);


#ifdef __DEBUG
  float after=summa(NX, NY, temp);
  printf(" sum temp after: %f\n",after);
#endif

  tstart = seconds();
  save_gnuplot("transport-end-serial.dat", temp);
  tstop = seconds();

  tio = tio + tstop - tstart;
  printf("\nsave_data done\n");
  printf("IO time in seconds %.3g\n", tio);
     
  printf("\ntotal cpu time in seconds %.3g\n", tstop - tinit);

  free(temp);
  free(temp_new);
  return 0;
}
