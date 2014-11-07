"""
Cython Module to contains physics
"""

cdef double G = 6.67E-11
from math import sqrt

cdef norm(double [:] vector):
    return sqrt((vector[0] * vector[0]) + (vector[1] * vector[1]) + (vector[2] * vector[2]))

def gravitational_acceleration(mass_j, position_i, position_j):
    """
    Calculates gravitational acceleration on particle i, by particle j
    :param mass_j: masses of particle j
    :param position_i: numpy array with 3D coordinates of particle i
    :param position_j: numpy array with 3D coordinates of particle j
    :return: numpy array with 3 components of gravitational acceleration
    """

    resp = [0., 0., 0.]
    cdef double[3] r_diff
    r_diff[0] = position_i[0] - position_j[0]
    r_diff[1] = position_i[1] - position_j[1]
    r_diff[2] = position_i[2] - position_j[2]

    resp[0] += -1.0 * (G * mass_j * r_diff[0]) / (norm(r_diff)**3)
    resp[1] += -1.0 * (G * mass_j * r_diff[1]) / (norm(r_diff)**3)
    resp[2] += -1.0 * (G * mass_j * r_diff[2]) / (norm(r_diff)**3)

    return resp

