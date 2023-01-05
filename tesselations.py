import numpy as np
from numba import stencil,njit,prange

@njit
def normal(triangle:tuple[np.ndarray,np.ndarray,np.ndarray])->np.ndarray:
  a:np.ndarray,b:np.ndarray,c:np.ndarray=triangle;
@stencil
def normal_x(cube):

def normal_y(cube):

def normal_z(cube):
