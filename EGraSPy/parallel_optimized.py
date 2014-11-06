# TURN ON THE VIRTUAL ENVIRONMENT FOR YOUR APPLICATION
try:
    activate_this = '/share/apps/.virtualenvs/dev/bin/activate_this.py'
    execfile(activate_this, dict(__file__=activate_this))
except:
    pass

# To run this on the cluster: qsub -pe mpich 30 run_parallel.sh

import mpi4py.MPI as MPI
import numpy as np
import time
from utils import generate_sphere_position_distribution, \
    generate_mass_distribution, generate_velocities_distribution, save_points
from physics import gravitational_acceleration, energy
from constants import AU, SUN_MASS, SECONDS_DAY

if not MPI.Is_initialized():
    MPI.Init()

np.random.seed(1000)

comm = MPI.COMM_WORLD
myid = comm.Get_rank()
size = comm.Get_size()

N = 10000
STEPS = 1
DT = 10 * SECONDS_DAY

if myid == 0:
    # System initialization done only on rot
    particles_positions = generate_sphere_position_distribution(AU, N)
    masses = generate_mass_distribution(SUN_MASS/1000., N)
    velocities = generate_velocities_distribution(N, -100.0, 100.0, no_z=True)
    print "System initialized. Total mass: %s" % (masses.sum() / SUN_MASS)
    save_points(particles_positions, './data/positions_initial.csv')
else:
    particles_positions = np.empty((N, 3), dtype='d')
    masses = np.empty(N, dtype='d')
    velocities = np.empty((N, 3), dtype='d')

# Broadcast system to all
comm.Bcast([particles_positions, MPI.DOUBLE], root=0)
comm.Bcast([masses, MPI.DOUBLE], root=0)
comm.Bcast([velocities, MPI.DOUBLE], root=0)

accelerations = np.zeros((N, 3))
itr = 0

chunk = N / size
rest = N % size

ranks = range(0, size)
ranks.reverse()

start = ranks[myid] * chunk
end = start + chunk + (rest if ranks[myid] == size - 1 else 0)

print 'I am node %s and my calculation range is from %s to %s' % (myid, start, end)

# Euler to start
# TODO use leap frog or Velet

start_sym = time.time()

while itr < STEPS:
    interactions = 0
    start_time = time.time()
    for i in xrange(start, end):
        p_i = particles_positions[i]
        m_i = masses[i]
        for j in xrange(i+1, N):
            m_j = masses[j]
            p_j = particles_positions[j]
            acc = gravitational_acceleration(m_j, p_i, p_j)
            interactions += 1
            accelerations[i] += acc
            accelerations[j] -= acc

    end_time = time.time()
    print "%s force calculation for range (%s to %s) in: %s. Total interactions: %s" % (myid, start, end, end_time - start_time, interactions)

    accelerations_sum = np.zeros((N, 3))

    comm.Reduce([accelerations, MPI.DOUBLE], [accelerations_sum, MPI.DOUBLE], op=MPI.SUM, root=0)

    if myid == 0:
        # Update velocities and positions in root after syncronization using Euler integration
        # TODO use a better method: Leapfrog or Velet
        velocities += (accelerations * DT)
        particles_positions += (velocities * DT)

    comm.Bcast([particles_positions, MPI.DOUBLE], root=0)
    comm.Bcast([velocities, MPI.DOUBLE], root=0)

    accelerations = np.zeros((N, 3))

    end_time = time.time()

    if itr == 0 and myid == 0:
        print "Forces calculation and comm completed in: %s" % (end_time - start_sym)

    itr += 1

    if myid == 0 and itr % 100 == 0:
        print "Processing iteration %s" % itr
        print "Max velocity: %s -  Min velocity: %s" % (velocities.max(), velocities.min())
        save_points(particles_positions, './data/positions_%s.csv' % itr)

MPI.Finalize()

end_time = time.time()

if myid == 0:
    print 'Done. Total energy: %s' % (energy(masses, velocities).sum())