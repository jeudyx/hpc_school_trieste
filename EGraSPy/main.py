import numpy as np
import time
from utils import generate_sphere_position_distribution, \
    generate_mass_distribution, generate_velocities_distribution, save_points
from physics import gravitational_acceleration
from constants import AU, SUN_MASS, SECONDS_DAY

N = 1000
STEPS = 1
DT = 10 * SECONDS_DAY

particles_positions = generate_sphere_position_distribution(AU, N)
masses = generate_mass_distribution(SUN_MASS/10000., N)
velocities = generate_velocities_distribution(N, -100.0, 100.0)
acelerations = np.zeros((N, 3))

print "System initialized"

# Euler to start TODO use leap frog or Velet
while STEPS > 0:
    start = time.time()
    for i, p_i in enumerate(particles_positions):
        m_i = masses[i]
        for j, p_j in enumerate(particles_positions):
            if i == j:
                # not interacting with itself
                continue
            m_j = masses[j]
            acc = gravitational_acceleration(m_j, p_i, p_j)
            acelerations[i] += acc
            # TODO apply 3rd Newton

    end = time.time()

    print "Forces calculation done in %s" % (end - start)

    start = time.time()

    velocities += (acelerations * DT)
    particles_positions += (velocities * DT)
    acelerations = np.zeros((N, 3))

    end = time.time()

    print "Velocities and positions update done in %s" % (end - start)

    STEPS -= 1
    if False and STEPS % 100 == 0:
        print "Voy por %s" % STEPS
        print "Max velocity: %s" % velocities.max()
        save_points(particles_positions, './data/positions_%s.csv' % STEPS)

print 'Done. Avg velocity in system %s' % (velocities.mean())