import numpy as np
from numba import njit,prange

@njit
def make_cube(d:tuple[float,float,float],n:tuple[int,float,float],start:tuple[float,float,float])->np.ndarray:
  nx,ny,nz=n;
  dx,dy,dz=d;
  ret=np.zeros((nx,ny,nz,3));
  for i in prange(nx):
    for j in prange(ny):
      for k in prange(nz):
        ret[i,j,k,0]=i*dx;
        ret[i,j,k,1]=j*dy;
        ret[i,j,k,2]=k*dz;
  return ret;

@njit
def make_simple_mapping(positionArray:np.ndarray,positionDataCube:np.ndarray)->np.ndarray:
  dx=positionDataCube[1,0,0,0];
  dy=positionDataCube[0,1,0,1];
  dz=positionDataCube[0,0,0,2];
  (nx,ny,nz,discard)=positionDataCube.shape;
  (nPoint,discard)=positionArray.shape;
  mapping=np.empty((nx,ny,nz),dtype=np.int32);
  mapping.fill(-1);
  for i in prange(nPoint):
    iData=np.int32(np.floor(positionArray[i,0]/dx));
    jData=np.int32(np.floor(positionArray[i,1]/dy));
    kData=np.int32(np.floor(positionArray[i,2]/dz));
    mapping[iData,jData,kData]=i;
  return mapping;

@njit
def amr_fix(simpleMap:np.ndarray,amrLevels:np.ndarray)->None:
  (nx,ny,nz)=simpleMap.shape;
  levelMax=np.maximum(amrLevels);
  for i in range(nx):
    for j in range(ny):
      for k in range(nz):
        if simpleMap[i,j,k]>=0:
          iMap=simpleMap[i,j,k]
          if amrLevels[iMap]==levelMax:
            simpleMap[i+1,j,k]=iMap;
            simpleMap[i,j+1,k]=iMap;
            simpleMap[i,j,k+1]=iMap;
            simpleMap[i+1,j+1,k]=iMap;
            simpleMap[i,j+1,k+1]=iMap;
            simpleMap[i+1,j,k+1]=iMap;
            simpleMap[i+1,j+1,k+1]=iMap;
