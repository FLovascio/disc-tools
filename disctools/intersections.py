import numpy as np
from numba import njit,prange
from typing import Callable
from numba.experimental import jitclass

@jitclass
class Plane:
  def __init__(self,ax,by,cz,rhs):
    self.Ax=ax
    self.By=by
    self.Cz=cz
    self.rhs=rhs
  
  @staticmethod
  def plane_from_vector(vector,at_point):
    A=vector[0]
    B=vector[1]
    C=vector[2]
    rhs=A*at_point[0]+B*at_point[1]+C*at_point[2]
    return Plane(A,B,C,rhs)
    
  def point(self,X):
    return self.Ax*X[0]+self.By*X[1]+self.Cz*X[2]-self.rhs

@njit
def crossed_by_circle(position,h,circle_radius,height):
  if (position[2]+0.5*h)<height:
    return False
  if (position[2]-0.5*h)>height:
    return False
  if ((np.linalg.norm(position[0:2])+0.5*h)<circle_radius):
    return False
  if ((np.linalg.norm(position[0:2])-0.5*h)>circle_radius):
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

@njit
def crossed_by_plane(position,h,plane):
  cornerPositions=np.zeros(8,3)
  cornerPositions[0]=np.array((position[0]-0.5*h,position[1]-0.5*h,position[2]-0.5*h))
  cornerPositions[1]=np.array((position[0]+0.5*h,position[1]-0.5*h,position[2]-0.5*h))
  cornerPositions[2]=np.array((position[0]-0.5*h,position[1]+0.5*h,position[2]-0.5*h))
  cornerPositions[3]=np.array((position[0]+0.5*h,position[1]+0.5*h,position[2]-0.5*h))
  cornerPositions[4]=np.array((position[0]-0.5*h,position[1]-0.5*h,position[2]+0.5*h))
  cornerPositions[5]=np.array((position[0]+0.5*h,position[1]-0.5*h,position[2]+0.5*h))
  cornerPositions[6]=np.array((position[0]-0.5*h,position[1]+0.5*h,position[2]+0.5*h))
  cornerPositions[7]=np.array((position[0]+0.5*h,position[1]+0.5*h,position[2]+0.5*h))
  plane_0=plane.point(cornerPositions[0])
  for i in range(1,8):
    if (plane_0* plane.point(cornerPositions[i])) < 0.0:
      return True
  return False

@njit(parallel=True)
def cells_intersected_plane(positions,h,plane):
  nx,ny,nz,_=positions.shape
  ilist=np.empty((nx*ny,3),dtype=int)
  element=0
  for i in prange(nx):
    for j in range(ny):
      for k in range(nz):
        if crossed_by_plane(positions[i,j,k],h,plane):
          ilist[element]=(np.array((i,j,k)))
          element+=1
  return ilist[:element]