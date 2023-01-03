import numpy as np
from numba import njit,prange
from typing import Callable

@njit
def spherical_integral(position:np.ndarray,data:np.ndarray,center:np.ndarray,radius:np.float64,phiRange:tuple[np.float64,np.float64])->np.float64:
  integral:np.float64=np.float64(0.0);
  return integral;

@njit
def cylindrical_integral(position:np.ndarray,data:np.ndarray,center:np.ndarray,radius:np.float64,zRange:tuple[np.float64,np.float64])->np.float64:
  integral:np.float64=np.float64(0.0);
  for 


