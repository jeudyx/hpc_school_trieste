#------------------------------------------------
#!/bin/bash
#
#$ -cwd
#$ -j y
#$ -S /bin/bash
#
MPI_DIR=/opt/openmpi/
$MPI_DIR/bin/mpirun -np $NSLOTS -machinefile $TMP/machines python parallel.py
#------------------------------------------------
