from distutils.core import setup
from Cython.Build import cythonize

# To run this: python setup.py build_ext --inplace

setup(
  name = 'Physics module for EGraSPy',
  ext_modules = cythonize("egraspy_physics.pyx"),
)