import numpy as np
from numba import njit,prange
@njit(parallel=True)
def make_cube(d:tuple[float,float,float],l:tuple[float,float,float],start:tuple[float,float,float])->np.ndarray:
  #print(l)
  #print(d)
  lx,ly,lz=l;
  dx,dy,dz=d;
  nx,ny,nz=(int(np.floor(lx/dx)),int(np.floor(ly/dy)),int(np.floor(lz/dz)));
  ret=np.zeros((nx,ny,nz,3));
  startx,starty,startz=start;

  for i in prange(nx):
    for j in prange(ny):
      for k in prange(nz):
        ret[i,j,k,0]=i*dx+startx+0.5*dx;
        ret[i,j,k,1]=j*dy+starty+0.5*dy;
        ret[i,j,k,2]=k*dz+startz+0.5*dz;
  return ret;

@njit(parallel=True)
def make_simple_mapping(positionArray:np.ndarray,positionDataCube:np.ndarray)->np.ndarray:
  dx=positionDataCube[1,0,0,0]-positionDataCube[0,0,0,0];
  dy=positionDataCube[0,1,0,1]-positionDataCube[0,0,0,1];
  dz=positionDataCube[0,0,1,2]-positionDataCube[0,0,0,2];
  (nx,ny,nz,discard)=positionDataCube.shape;
  (nPoint,discard)=positionArray.shape;
  mapping=np.empty((nx,ny,nz),dtype=np.int32);
  mapping.fill(-1);
  x0,y0,z0=positionDataCube[1,0,0,0],positionDataCube[0,1,0,1],positionDataCube[0,0,1,2]
  for i in prange(nPoint):
    iData=np.int32(np.around((positionArray[i,0]-x0-0.5*dx)/dx));
    jData=np.int32(np.around((positionArray[i,1]-y0-0.5*dy)/dy));
    kData=np.int32(np.around((positionArray[i,2]-z0-0.5*dz)/dz));
    mapping[iData,jData,kData]=i;
  return mapping;

@njit(parallel=True)
def make_amr_mapping(positionArray:np.ndarray,positionDataCube:np.ndarray,amrLevels:np.ndarray)->np.ndarray:
  dx=positionDataCube[1,0,0,0]-positionDataCube[0,0,0,0];
  dy=positionDataCube[0,1,0,1]-positionDataCube[0,0,0,1];
  dz=positionDataCube[0,0,1,2]-positionDataCube[0,0,0,2];
  (nx,ny,nz,discard)=positionDataCube.shape;
  (nPoint,discard)=positionArray.shape;
  mapping=np.empty((nx,ny,nz),dtype=np.int32);
  mapping.fill(-1);
  x0,y0,z0=positionDataCube[0,0,0,0],positionDataCube[0,0,0,1],positionDataCube[0,0,0,2]
  levelMax=np.max(amrLevels);
  levelMin=np.min(amrLevels);
  for L in range(levelMax-levelMin,0,-1):
    iLevel=2**(L);
    print("building amr level", levelMax-L);
    for i in prange(nPoint):
      iData=np.int32(iLevel*np.around((positionArray[i,0]-x0)/(iLevel*dx)-0.5));
      jData=np.int32(iLevel*np.around((positionArray[i,1]-y0)/(iLevel*dy)-0.5));
      kData=np.int32(iLevel*np.around((positionArray[i,2]-z0)/(iLevel*dz)-0.5));
      mapping[iData-iLevel//2:iData+iLevel//2,jData-iLevel//2:jData+iLevel//2,kData-iLevel//2:kData+iLevel//2]=i;
  print("building amr level", levelMax);
  for i in prange(nPoint):
    iData=np.int32(np.floor((positionArray[i,0]-x0)/dx-0.5));
    jData=np.int32(np.floor((positionArray[i,1]-y0)/dy-0.5));
    kData=np.int32(np.floor((positionArray[i,2]-z0)/dz-0.5));
    mapping[iData,jData,kData]=i;
  return mapping;

@njit(parallel=True)
def amr_fix_upres(simpleMap:np.ndarray,amrLevels:np.ndarray)->np.ndarray:
  (nx,ny,nz)=simpleMap.shape;
  newMap=np.zeros((nx,ny,nz),dtype=np.int32);
  levelMax=np.max(amrLevels);
  for i in prange(nx):
    for j in range(ny):
      for k in range(nz):
        iMap=simpleMap[i,j,k];
        newMap[i,j,k]=iMap
        if iMap>=0:
          n=levelMax-amrLevels[iMap];
          if n>0:
            #print("fixing ",n,"at ",i,j,k,"imap= ",iMap)
            newMap[i-n:i+n,j-n:j+n,k-n:k+n]=iMap;
  return newMap;

@njit
def make_mapping(positionArray:np.ndarray,amrLevels:np.ndarray,positionDataCube:np.ndarray,amrMapMethod:str="upres")->np.ndarray:
  mapping=make_simple_mapping(positionArray,positionDataCube);
  if amrMapMethod=="upres":
    mapping=make_amr_mapping(positionArray,positionDataCube,amrLevels);
  else:
    raise(UserWarning("if you have any level jumps, using no amrMapMethod will lead to broken mappings"));
  return mapping;

@njit
def fill_array_scalar(incomingData,mapping)->np.ndarray:
  nx,ny,nz=mapping.shape;
  output=np.zeros((nx,ny,nz));
  for i in range(nx):
    for j in range(ny):
      for k in range(nz):
        output[i,j,k]=incomingData[mapping[i,j,k]];
  return output;

@njit
def fill_array_vector(incomingData_x,incomingData_y,incomingData_z,mapping)->np.ndarray:
  nx,ny,nz=mapping.shape;
  output=np.zeros((nx,ny,nz,3));
  for i in range(nx):
    for j in range(ny):
      for k in range(nz):
        output[i,j,k,0]=incomingData_x[mapping[i,j,k]];
        output[i,j,k,1]=incomingData_y[mapping[i,j,k]];
        output[i,j,k,2]=incomingData_z[mapping[i,j,k]];
  return output;