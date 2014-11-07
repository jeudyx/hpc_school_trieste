import cProfile
import time
import numpy as np
from physics import gravitational_acceleration, gravitational_acceleration_np

from constants import SUN_MASS

# p_1 = [0., 0., 0.]
# p_2 = [1., 1., 1.]
# suma = [0., 0., 0.]
#
N = 1000000


def run_pure_python():
    i = 0

    start_time = time.time()

    while i < N:
        result = gravitational_acceleration(SUN_MASS, p_1, p_2)
        suma[0] += result[0]
        suma[1] += result[1]
        suma[2] += result[2]

        i += 1

    end_time = time.time()

    print "Done pure python in %s: %s" % (end_time - start_time, suma)

def run_np():
    p_1 = np.array([0., 0., 0.])
    p_2 = np.array([1., 1., 1.])
    suma = np.array([0., 0., 0.])

    i = 0

    start_time = time.time()

    while i < N:
        result = gravitational_acceleration_np(SUN_MASS, p_1, p_2)
        suma[0] += result[0]
        suma[1] += result[1]
        suma[2] += result[2]

        i += 1

    end_time = time.time()

    print "Done with numpy in %s: %s" % (end_time - start_time, suma)

cProfile.run("run_np()")