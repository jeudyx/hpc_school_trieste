import numpy as np
import time
from utils import generate_sphere_position_distribution, \
    generate_mass_distribution, generate_velocities_distribution, save_points
from physics import gravitational_acceleration
from constants import AU, SUN_MASS, SECONDS_DAY

N = 200
STEPS = 50000
DT = SECONDS_DAY

particles_positions = generate_sphere_position_distribution(2*AU, N)
masses = generate_mass_distribution(SUN_MASS/5000., N, variability=0.5)
velocities = generate_velocities_distribution(N, -100.0, 100.0, no_z=True)
accelerations = np.zeros((N, 3))
itr = 0

save_points(particles_positions, './data/positions_initial.csv')

print "System initialized. Total mass: %s" % (masses.sum() / SUN_MASS)

# Euler to start TODO use leap frog or Velet
while itr <= STEPS:
    start = time.time()
    for i in range(0, N):
        p_i = particles_positions[i]
        m_i = masses[i]
        for j in range(i+1, N):
            if i == j:
                # not interacting with itself
                continue
            m_j = masses[j]
            p_j = particles_positions[j]
            acc = gravitational_acceleration(m_j, p_i, p_j)
            accelerations[i] += acc
            # Take advantage of Newton 3rd law
            accelerations[j] -= (acc * m_i) / m_j  # This may be unnecessary since all masses are equal

    end = time.time()

    if itr == 0:
        print "Forces calculation done in %s for first iteration..." % (end - start)

#    start = time.time()

    velocities += (accelerations * DT)
    particles_positions += (velocities * DT)
    accelerations = np.zeros((N, 3))

#    end = time.time()

#    print "Velocities and positions update done in %s" % (end - start)

    itr += 1
    if itr % 500 == 0:
        print "Voy por %s" % itr
        print "Max velocity: %s -  Min velocity: %s" % (velocities.max(), velocities.min())
        save_points(particles_positions, './data/positions_%s.csv' % itr)

print 'Done. Avg velocity in system %s' % (velocities.mean())