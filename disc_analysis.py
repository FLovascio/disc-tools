import osyris
import numpy as np
from numba import njit, prange, stencil


@stencil
def grad_P_x(P:np.float64,x:np.ndarray)->np.float64:
  hix=x[1,0,0]-x[0,0,0]
  hmx=x[0,0,0]-x[-1,0,0]
  hfx=hix/hmx
  dpx=P[1,0,0]-((hfx)**2)*P[-1,0,0]-(1.0-(hfx)**2)*P[0,0,0]
  return dpx
@stencil
def grad_P_y(P:np.float64,x:np.ndarray)->np.float64:
  hiy=x[0,1,0]-x[0,0,0]
  hmy=x[0,0,0]-x[0,-1,0]
  hfy=hiy/hmy
  dpy=P[0,1,0]-((hfy)**2)*P[0,-1,0]-(1.0-(hfy)**2)*P[0,0,0]
  return dpy
@stencil
def grad_P_z(P:np.float64,x:np.ndarray)->np.float64:
  hiz=x[0,0,1]-x[0,0,0]
  hmz=x[0,0,0]-x[0,0,-1]
  hfz=hiz/hmz
  dpz=P[0,0,1]-((hfz)**2)*P[0,0,-1]-(1.0-(hfz)**2)*P[0,0,0]
  return dpz

@njit
def pressure_support(gradP,x,sink_pos):
  r=x-sink_pos
  direction=r/np.linalg.norm(r)
  return np.dot(gradP,direction)

@njit
def make_basis(direction):
  epsilon1=direction/np.sqrt(np.linalg.norm(direction))
  epsilon2=np.cross(epsilon1,np.array([1.0,0.0,0.0]))
  epsilon2=epsilon2/np.sqrt(np.linalg.norm(epsilon2))
  epsilon3=np.cross(epsilon2,epsilon1)
  return np.vstack((epsilon1,epsilon2,epsilon3))

@njit
def to_basis(a,basis):
  return np.array((np.dot(a,basis[0,:]),np.dot(a,basis[1,:]),np.dot(a,basis[2,:])))

@njit
def rotational_support(v,x,sink_pos):
  r=x-sink_pos
  v_tan=to_basis(v,make_basis(r))
  v_tan[0]*=0.0
  omega=np.linalg.norm(v_tan)/(2.0*np.pi*np.linalg.norm(r))
  return omega*omega*np.linalg.norm(r)

@njit
def rotational_fraction(gradP,v,x,sink_pos):
  return rotational_support(v,x,sink_pos)/pressure_support(gradP,x,sink_pos)

def get_disc():
  return 0

@njit(parallel=True)
def angular_momentum(v,rho,V,x):
  nx,ny,nz=rho.shape
  angMom=np.zeros_like(v)
  for i in prange(nx):
    for j in range(ny):
      for k in range(nz):
        angMom[i,j,k,:]=rho[i,j,k]*V*np.cross(x[i,j,k,:],v[i,j,k,:])
  return angMom

@njit
def angular_momentum_sphere(v,rho,V,x):
  total_angular_momentum=angular_momentum(v,rho,V,x)
  return np.sum(np.sum(np.sum(total_angular_momentum,axis=0),axis=0),axis=0)