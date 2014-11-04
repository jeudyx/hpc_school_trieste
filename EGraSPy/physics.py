"""
Module to contains physics
"""

from scipy.constants import G


def gravitational_acceleration(mass1, mass2, r_diff, r_diff_scalar_inv):
    """
    Calculates gravitational acceleration. As much calculations done
    outside the function as possible
    :param mass1 and mass2: masses of the bodies
    :param r_diff:      numpy array with 3D coordinates of distance vector r1 - r2
    :param r_diff_scalar_inv: the norm of r_diff
    :return:
    """
    return G * mass1 * mass2 * r_diff * r_diff_scalar_inv**3