import numpy as np
from numba import njit,prange
from typing import Callable


@njit
def crossed_by_circle(position,h,circle_radius,height):
  if (position[2]+h)<height:
    return False
  if (position[2]-h)>height:
    return False
  if ((np.linalg.norm(position[0:2])+h)<circle_radius):
    return False
  if ((np.linalg.norm(position[0:2])-h)>circle_radius):
    return False
  return True

@njit(parallel=True)
def cells_intersected_circle(positions,h,circle_radius,height):
  nx,ny,nz,_=positions.shape
  ilist=np.empty((nx*ny,3),dtype=int)
  element=0
  for i in prange(nx):
    for j in range(ny):
      for k in range(nz):
        if crossed_by_circle(positions[i,j,k],h,circle_radius,height):
          ilist[element]=(np.array((i,j,k)))
          element+=1
  return ilist[:element]


  
