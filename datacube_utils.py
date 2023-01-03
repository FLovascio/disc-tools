import numpy as np
from numba import njit,prange
from typing import Callable

@njit(parallel=True)
def brodcast(broadcastFunction:Callable,data:np.ndarray,args:any)->np.ndarray: #bradcastFunction must be pure!
  returnArray:np.ndarray=np.zeros_like(data);
  nx,ny,nz=(data.shape[0],data.shape[1],data.shape[2]);
  for i in prange(nx):
    for j in range(ny):
      for k in range(nz):
        returnArray[i,j,k]=broadcastFunction(data[i,j,k],*args);

@njit(parallel=True)
def spatial_brodcast(broadcastFunction:Callable,x:np.ndarray,data:np.ndarray,args:any)->np.ndarray: #bradcastFunction must be pure!
  returnArray:np.ndarray=np.zeros_like(data);
  nx,ny,nz,discard=x.shape;
  for i in prange(nx):
    for j in range(ny):
      for k in range(nz):
        returnArray[i,j,k]=broadcastFunction(x[i,j,k],data[i,j,k],*args);