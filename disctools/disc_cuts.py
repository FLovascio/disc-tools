import numpy as np
from numba import njit,prange
from typing import Callable

@njit
def make_nan_array(template:np.ndarray)->np.ndarray:
  ret=np.empty(template.shape);
  ret.fill(np.nan);
  return ret;

def make_cut_array(toCut:np.ndarray,conditionFunction:Callable[[np.ndarray],np.ndarray],conditionArg:np.ndarray)->np.ndarray:
  cutArray=make_nan_array(toCut);
  cutArray[conditionFunction(conditionArg)]=toCut[conditionFunction(conditionArg)];
  return cutArray;

def make_outside_zeros(toCut:np.ndarray,conditionFunction:Callable[[np.ndarray],np.ndarray],conditionArg:np.ndarray)->np.ndarray:
  cutArray=np.zeros_like(toCut);
  cutArray[conditionFunction(conditionArg)]=toCut[conditionFunction(conditionArg)];
  return cutArray; 
  
@njit
def in_domain(i:int,j:int,k:int)->bool:
  if i==0:
    return False;
  if j==0:
    return False;
  if k==0:
    return False;
  return True;

@njit
def has_nan_neighbor(stencil:np.ndarray)->bool:
  if np.isnan(stencil[1,1,1]):
    return False;
  if np.isnan(stencil[0,1,1]) or np.isnan(stencil[2,1,1]):
    return True;
  if np.isnan(stencil[1,0,1]) or np.isnan(stencil[1,2,1]):
    return True;
  if np.isnan(stencil[1,1,0]) or np.isnan(stencil[1,1,2]):
    return True;
  return False;

@njit
def get_cut_surface(cutArray:np.ndarray)->np.ndarray:
  nx,ny,nz=cutArray.shape;
  surface=np.full((nx,ny,nz),False);
  for i in prange(nx):
    for j in range(ny):
      for k in range(nz):
        if(in_domain(i,j,k)):
          surface[i,j,k]=has_nan_neighbor(cutArray[i-1:i+1,j-1:j+1,k-1:k+1]);
  return surface;

def get_surface(toCut:np.ndarray,conditionFunction:Callable[[np.ndarray],np.ndarray],conditionArg:np.ndarray)->np.ndarray:
  surface=get_cut_surface(make_cut_array(toCut,conditionFunction,conditionArg));
  return surface;