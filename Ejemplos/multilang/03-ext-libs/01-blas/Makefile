# compiler configuration
# C
CC=gcc
CFLAGS=-Wall -g
LFLAGS=-lm
# C++
CXX=g++
CXXFLAGS=-Wall -g
LXXFLAGS=
# F77
FC=gfortran
FFLAGS=-Wall -g -std=legacy
FLFLAGS=
# F95
F90=gfortran
F90FLAGS=-Wall -g -std=f95
F90LFLAGS=

#BLAS_LIB=-lblas
BLAS_LIB=-lopenblas -lgfortran
#BLAS_LIB=-L/usr/lib64/atlas-sse3/ -lf77blas -latlas -lgfortran

#############################################

all: blasex_f77 blasex_cxx
    

clean:
	rm -f blasex_f77 blasex_cxx
	rm -f *.dat *.mod *~ a.out *.o

#############################################


blasex_f77: blasex.f90 $(EXTRA_OBJS)
	$(FC) $(FFLAGS) -o $@ $^ $(FLFLAGS) $(BLAS_LIB)

blasex_cxx: blasex.cpp $(EXTRA_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LXXFLAGS) $(BLAS_LIB)


