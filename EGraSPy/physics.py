"""
Module to contains physics
"""

from constants import G
import numpy as np


def gravitational_acceleration(mass_j, position_i, position_j):
    """
    Calculates gravitational acceleration. As much calculations done
    outside the function as possible
    :param mass1 and mass2: masses of the bodies
    :param r_diff:      numpy array with 3D coordinates of distance vector r1 - r2
    :param r_diff_scalar_inv: the norm of r_diff
    :return:
    """

    r_diff = position_i - position_j

    return -1.0 * (G * mass_j * r_diff) / (np.linalg.norm(r_diff)**3)