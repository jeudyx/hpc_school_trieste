import numpy as np
import time
from utils import generate_sphere_position_distribution, \
    generate_mass_distribution, generate_velocities_distribution, save_points
from physics import gravitational_acceleration, energy
from constants import AU, SUN_MASS, SECONDS_DAY

np.random.seed(1000)

N = 10000
STEPS = 1
DT = 10 * SECONDS_DAY

particles_positions = generate_sphere_position_distribution(AU, N)
masses = generate_mass_distribution(SUN_MASS/1000., N)
velocities = generate_velocities_distribution(N, -100.0, 100.0, no_z=True)
accelerations = np.zeros((N, 3))
itr = 0

save_points(particles_positions, './data/positions_initial.csv')

print "System initialized. Total mass: %s" % (masses.sum() / SUN_MASS)

# Euler to start TODO use leap frog or Velet
while itr < STEPS:
    interactions = 0
    start = time.time()
    for i in xrange(0, N):
        p_i = particles_positions[i]
        m_i = masses[i]
        for j in xrange(i+1, N):
            m_j = masses[j]
            p_j = particles_positions[j]
            acc = gravitational_acceleration(m_j, p_i, p_j)
            interactions += 1
            accelerations[i] += acc
            accelerations[j] -= acc

    end = time.time()

    if itr == 0:
        print "Forces calculation done in %s for first iteration. Interactions: %s" % (end - start, interactions)

    velocities += (accelerations * DT)
    particles_positions += (velocities * DT)
    accelerations = np.zeros((N, 3))

    itr += 1
    if itr % 500 == 0:
        print "Voy por %s" % itr
        print "Max velocity: %s -  Min velocity: %s" % (velocities.max(), velocities.min())
        save_points(particles_positions, './data/positions_%s.csv' % itr)

print 'Done. Total energy: %s' % (energy(masses, velocities).sum())