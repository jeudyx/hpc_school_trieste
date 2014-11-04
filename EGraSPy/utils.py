import numpy as np


def contains_point(r, point):
    return r >= (np.linalg.norm(point))


def get_position_from_a_to_b(a, b):
    """ Generates 3D positions from a to b using: (b - a) * random_sample() + a
    Assumes that a is less than b

    :param a: lower limit of range
    :param b: upper limit of range
    :return: numpy array of 3D vector between a and b
    """
    return (b - a) * np.random.random_sample(3) + a


def generate_sphere_position_distribution(radius, N):

    """Generate particle distribution in the shape of s sphere

    :param radius: radius of sphere
    :param center: 3D vector for the center of the sphere (numpy array)
    :param N: number of particles
    :return: numpy array of n_particles x 3 elements with 3D position
    """
    positions = []

    while len(positions) < N:
        position = get_position_from_a_to_b(-radius, radius)
        if contains_point(radius, position):
            positions.append(position)

    return np.array(positions)


def save_points(points, path):
    f = open(path, 'w')

    for point in points:
        line = ",".join(str(c) for c in point)
        f.write(line)
        f.write('\n')

    f.close()