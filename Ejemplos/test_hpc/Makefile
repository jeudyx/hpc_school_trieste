
MKL_ROOT=/opt/intel/mkl

CC=icc
FC=ifort
MPIF90=mpif90

LIBS_THREADS=-L${MKL_ROOT}/lib/intel64/ -lmkl_intel_lp64  -lmkl_intel_thread -lmkl_core
LIBS_MKL=-L${MKL_ROOT}/lib/intel64/ -lmkl_intel_lp64  -lmkl_sequential -lmkl_core
OPT_FLAGS=-O3 -openmp
CPU_OBJS=clib.o

all: bench_dgemm.x bench_daxpy.x bench_dgemm_threads.x bench_mpi_daxpy.x

clib.o: cptimer.c
	${CC} ${OPT_FLAGS} -c cptimer.c -o ./clib.o

bench_dgemm.x: bench_dgemm.f90 ${CPU_OBJS}
	${FC} ${OPT_FLAGS} -o bench_dgemm.x bench_dgemm.f90 ${CPU_OBJS} ${LIBS_MKL}

bench_dgemm_threads.x: bench_dgemm.f90 ${CPU_OBJS}
	${FC} ${OPT_FLAGS} -o bench_dgemm_threads.x bench_dgemm.f90 ${CPU_OBJS} ${LIBS_THREADS}

bench_daxpy.x: bench_daxpy.f90 ${CPU_OBJS}
	${FC} ${OPT_FLAGS} -o bench_daxpy.x bench_daxpy.f90 ${CPU_OBJS} ${LIBS_MKL}

bench_mpi_daxpy.x: bench_mpi_daxpy.f90 ${CPU_OBJS}
	${MPIF90} ${OPT_FLAGS} -cpp -D__MPI -o bench_mpi_daxpy.x bench_mpi_daxpy.f90 ${CPU_OBJS} ${LIBS_MKL}

clean:
	rm -rf *.x *.o *~
