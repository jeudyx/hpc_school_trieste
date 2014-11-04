import numpy as np
from numpy.linalg import norm
# from physics import gravitational_acceleration
import time

N_ITER = 1000000

r1 = np.array([0., 0., 0.])
r2 = np.array([1., 1., 0.])

SUN_MASS = 1.9891E30   # Sun
EARTH_MASS = 5.972E24    # Earth

i = 0

diff = r1 - r2
diff_scalar_inv = 1. / (norm(diff))

sum = np.zeros(3)

start = time.time()

# while i < N_ITER:
sum = gravitational_acceleration(SUN_MASS, EARTH_MASS, diff, diff_scalar_inv, N_ITER)


end = time.time()

print 'Time taken: %s' % (end - start)

print sum