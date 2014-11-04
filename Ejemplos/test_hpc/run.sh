#!/bin/bash

# This script is used to run dgemm and daxpy benchmarks during the smr2613
#
# Vectors and Matrixes sizes are hard-coded into the sources.
# It produces outputs running dgemm single and multi-thread as well as daxpy 
# on single and multiple process-cores
#

export OMP_NUM_THREADS=1
./bench_dgemm.x | tee dgemm_output_single-core.out

# finds the number of cores in the CPU platform 
N_CORES=$(grep processor /proc/cpuinfo | wc -l) 
export OMP_NUM_THREADS=2

while [ $OMP_NUM_THREADS -le $N_CORES ]
do
    export MKL_NUM_THREADS=${OMP_NUM_THREADS}
    export OMP_NUM_THREADS=${OMP_NUM_THREADS}
    ./bench_dgemm_threads.x | tee dgemm_output_${OMP_NUM_THREADS}-cores.out
    let "OMP_NUM_THREADS *= 2"
done

./bench_daxpy.x | tee daxpy_output_single-core.out

#runs #NCORES daxpy in parallel measuring the aggregate memory bandwidth 
NP=2
while [ $NP -le $N_CORES ]
do
    mpirun -np ${NP} ./bench_mpi_daxpy.x | tee daxpy_output_multicore.out
    let "NP *= 2"
done
