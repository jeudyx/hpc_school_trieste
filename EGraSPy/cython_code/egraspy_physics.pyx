"""
Cython Module to contains physics
"""

cimport numpy as np
import numpy as np
cdef double G = 6.67E-11

DTYPE = np.double

cpdef np.ndarray[double, ndim=1] gravitational_acceleration(double mass1, double mass2,
                                                           np.ndarray[double, ndim=1] r_diff,
                                                           double r_diff_scalar_inv, int N):
    """
    Calculates gravitational acceleration. As much calculations done
    outside the function as possible
    :param mass1 and mass2: masses of the bodies
    :param r_diff:      numpy array with 3D coordinates of distance vector r1 - r2
    :param r_diff_scalar_inv: the norm of r_diff
    :return:
    """

    cdef np.ndarray resp = np.zeros(3, dtype=DTYPE)

    while N > 0:
        resp += G * mass1 * mass2 * r_diff * r_diff_scalar_inv**3
        N -= 1

    return resp

