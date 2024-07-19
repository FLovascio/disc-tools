import numpy as np
from numba import njit, prange

import disc_analysis
import disctools
import constants

cgs_constants=constants.CGS()

@njit
def eccentricity_vector(star_mass,position,velocity,dens):
  k=star_mass*cgs_constants.G
  rhat=(position/np.linalg.mod(position))
  L=disc_analysis.angular_momentum(velocity,dens,1,position)
  p=velocity*dens
  Lcp=np.cross(L,p)
  e=(1.0/(dens*k))*Lcp-rhat
  return e

@njit
def complex_eccentricity(eccentricity_vector):
  return np.complex(eccentricity_vector[0],eccentricity_vector[1])

@njit
def mean_eccentricity(star_mass,positions,velocities,denities):
  e=np.zeros_like(positions)
  for (ei,position,velocity,dens) in zip(e,positions,velocities,denities):
    ei=eccentricity_vector(position,velocity,dens)
  emean=np.mean(e)
  return emean

