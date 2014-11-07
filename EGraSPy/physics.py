"""
Module to contains physics
"""

from constants import G
import numpy as np
from math import sqrt


def norm(vector):
    return sqrt((vector[0] * vector[0]) + (vector[1] * vector[1]) + (vector[2] * vector[2]))


def gravitational_acceleration(mass_j, position_i, position_j):
    result = [0., 0., 0.]
    r_diff = [0., 0., 0.]

    r_diff[0] = position_i[0] - position_j[0]
    r_diff[1] = position_i[1] - position_j[1]
    r_diff[2] = position_i[2] - position_j[2]
    norm_diff = norm(r_diff)

    result[0] = -1.0 * G * mass_j * r_diff[0] / (norm_diff * norm_diff * norm_diff)
    result[1] = -1.0 * G * mass_j * r_diff[1] / (norm_diff * norm_diff * norm_diff)
    result[2] = -1.0 * G * mass_j * r_diff[2] / (norm_diff * norm_diff * norm_diff)

    return result


def gravitational_acceleration_np(mass_j, position_i, position_j):
    r_diff = position_i - position_j
    return -1.0 * (G * mass_j * r_diff) / (np.linalg.norm(r_diff)**3)


def energy(masses, velocities):
    return 0.5 * masses * np.linalg.norm(velocities)**2