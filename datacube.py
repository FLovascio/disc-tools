import numpy as np
from numba import njit,prange

@njit(parallel=True)
def make_cube(d:tuple[float,float,float],l:tuple[float,float,float],start:tuple[float,float,float])->np.ndarray:
  lx,ly,lz=l;
  dx,dy,dz=d;
  nx,ny,nz=(int(np.floor(lx/dx)),int(np.floor(ly/dy)),int(np.floor(lz/dz)));
  ret=np.zeros((nx,ny,nz,3));
  for i in prange(nx):
    for j in prange(ny):
      for k in prange(nz):
        ret[i,j,k,0]=i*dx;
        ret[i,j,k,1]=j*dy;
        ret[i,j,k,2]=k*dz;
  return ret;

@njit(parallel=True)
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

@njit(parallel=True)
def amr_fix_upres(simpleMap:np.ndarray,amrLevels:np.ndarray)->None:
  (nx,ny,nz)=simpleMap.shape;
  levelMax=np.max(amrLevels);
  for i in range(nx):
    for j in range(ny):
      for k in range(nz):
        if simpleMap[i,j,k]>=0:
          iMap=simpleMap[i,j,k];
          n=levelMax-amrLevels[iMap];
          nMinus=np.int8(np.floor(n/2));
          nPlus=np.int8(np.ceil(n/2));
          simpleMap[i-nMinus:i+nPlus,j-nMinus:j+nPlus,k-nMinus:k+nPlus]=iMap;
          
@njit
def make_mapping(positionArray:np.ndarray,amrLevels:np.ndarray,positionDataCube:np.ndarray,amrMapMethod:str="upres")->np.ndarray:
  mapping=make_simple_mapping(positionArray,positionDataCube);
  if amrMapMethod=="upres":
    amr_fix_upres(mapping,amrLevels);
  else:
    raise(UserWarning("if you have any level jumps, using no amrMapMethod will lead to broken mappings"));
  return mapping;
