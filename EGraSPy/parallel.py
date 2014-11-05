import mpi4py.MPI as MPI
import numpy as np
import time
from utils import generate_sphere_position_distribution, \
    generate_mass_distribution, generate_velocities_distribution, save_points
from physics import gravitational_acceleration
from constants import AU, SUN_MASS, SECONDS_DAY

if not MPI.Is_initialized():
    MPI.Init()

comm = MPI.COMM_WORLD
myid = comm.Get_rank()
size = comm.Get_size()

N = 1000
STEPS = 1
DT = SECONDS_DAY

if myid == 0:
    # System initialization done only on rot
    particles_positions = generate_sphere_position_distribution(2*AU, N)
    masses = generate_mass_distribution(SUN_MASS/5000., N, variability=0.5)
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

start = myid * chunk
end = start + chunk + (rest if myid == size - 1 else 0)

print 'I am node %s and my calculation range is from %s to %s' % (myid, start, end)

# Euler to start TODO use leap frog or Velet
while itr <= STEPS:
    start_time = time.time()
    for i in range(start, end):
        p_i = particles_positions[i]
        m_i = masses[i]
        for j in range(0, N):
            if i == j:
                # not interacting with itself
                continue
            m_j = masses[j]
            p_j = particles_positions[j]
            acc = gravitational_acceleration(m_j, p_i, p_j)
            accelerations[i] += acc
            # TODO put back III Newton
            # accelerations[j] -= (acc * m_i) / m_j

    end_time = time.time()

    accelerations_sum = np.zeros((N, 3))

    # Reduce the accelerations
    comm.Reduce([accelerations, MPI.DOUBLE], [accelerations_sum, MPI.DOUBLE], op=MPI.SUM, root=0)

    if myid == 0:
        # Update velocities and positions in root after syncronization
        velocities += (accelerations * DT)
        particles_positions += (velocities * DT)

    comm.Bcast([particles_positions, MPI.DOUBLE], root=0)
    comm.Bcast([velocities, MPI.DOUBLE], root=0)
    accelerations = np.zeros((N, 3))

    itr += 1
    if itr % 500 == 0:
        print "Voy por %s" % itr
        print "Max velocity: %s -  Min velocity: %s" % (velocities.max(), velocities.min())
        save_points(particles_positions, './data/positions_%s.csv' % itr)

print ' %s done. Avg velocity in system %s. Total mass: %s' % (myid,
                                                               velocities.mean(),
                                                               masses.sum() / SUN_MASS)

MPI.Finalize()