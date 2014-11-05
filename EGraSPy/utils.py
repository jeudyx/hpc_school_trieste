import numpy as np


def _contains_point(r, point):
    return r >= (np.linalg.norm(point))


def _get_position_from_a_to_b(a, b):
    """ Generates 3D positions from a to b using: (b - a) * random_sample() + a
    Assumes that a is less than b

    :param a: lower limit of range
    :param b: upper limit of range
    :return: numpy array of 3D vector between a and b
    """
    return (b - a) * np.random.random_sample(3) + a


def generate_sphere_position_distribution(radius, n):

    """Generate particle distribution in the shape of s sphere

    :param radius: radius of sphere
    :param center: 3D vector for the center of the sphere (numpy array)
    :param N: number of particles
    :return: numpy array of n_particles x 3 elements with 3D position
    """
    positions = []

    while len(positions) < n:
        position = _get_position_from_a_to_b(-radius, radius)
        if _contains_point(radius, position):
            positions.append(position)

    return np.array(positions)


def generate_velocities_distribution(n, vmin, vmax, no_z=False):
    velocities = (vmax - vmin) * np.random.random_sample((n, 3)) + vmin
    if no_z:
        velocities[:, 2] = 0.0
    return velocities


def generate_mass_distribution(total_mass, n, variability=0.0):
    particle_mass = total_mass / n
    if variability == 0.0:
        return np.ones(n) * particle_mass
    else:
        var_range = ((-1. * (variability * particle_mass) - particle_mass),
                     ((variability * particle_mass) + particle_mass))
        return np.ones(n) * particle_mass + \
               ((var_range[1] - var_range[0]) * np.random.random_sample(n) + var_range[0])


def save_points(points, path):
    f = open(path, 'w')

    for point in points:
        line = ",".join(str(c) for c in point)
        f.write(line)
        f.write('\n')

    f.close()