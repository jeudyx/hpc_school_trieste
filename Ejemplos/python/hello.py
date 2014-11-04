"""
Hola mundo en python
"""

import mpi4py.MPI as MPI

if not MPI.Is_initialized():
    MPI.Init()

comm = MPI.COMM_WORLD
myid = comm.Get_rank()
size = comm.Get_size()

print "Soy proceso %s de %s" % (myid, size)

MPI.Finalize()