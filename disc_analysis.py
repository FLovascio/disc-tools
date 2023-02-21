import numpy as np
from numba import njit, prange, stencil

@stencil(parallel=True)
def grad_P_x(P,x):
  hix=x[1,0,0]-x[0,0,0]
  hmx=x[0,0,0]-x[-1,0,0]
  hfx=hix/hmx
  dpx=P[1,0,0]-((hfx)**2)*P[-1,0,0]-(1.0-(hfx)**2)*P[0,0,0]
  return dpx
@stencil(parallel=True)
def grad_P_y(P,x):
  hiy=x[0,1,0]-x[0,0,0]
  hmy=x[0,0,0]-x[0,-1,0]
  hfy=hiy/hmy
  dpy=P[0,1,0]-((hfy)**2)*P[0,-1,0]-(1.0-(hfy)**2)*P[0,0,0]
  return dpy
@stencil(parallel=True)
def grad_P_z(P,x):
  hiz=x[0,0,1]-x[0,0,0]
  hmz=x[0,0,0]-x[0,0,-1]
  hfz=hiz/hmz
  dpz=P[0,0,1]-((hfz)**2)*P[0,0,-1]-(1.0-(hfz)**2)*P[0,0,0]
  return dpz

@njit(parallel=True)
def grad_P(P:np.ndarray,X:np.ndarray)->np.ndarray:
  return np.stack((grad_P_x(P,X[:,:,:,0]),grad_P_y(P,X[:,:,:,1]),grad_P_z(P,X[:,:,:,2])),axis=-1)

@njit
def pressure_support(gradP:np.ndarray,x:np.ndarray,sink_pos:np.ndarray)->np.ndarray:
  r=x-sink_pos
  direction=r/np.linalg.norm(r)
  return np.dot(gradP,direction)

@njit
def make_basis(direction:np.ndarray)->np.ndarray:
  epsilon1=direction/np.sqrt(np.linalg.norm(direction),axis=-1)
  epsilon2=np.cross(epsilon1,np.array([1.0,0.0,0.0]),axis=-1)
  epsilon2=epsilon2/np.sqrt(np.linalg.norm(epsilon2,axis=-1))
  epsilon3=np.cross(epsilon2,epsilon1,axiz=-1)
  return np.stack((epsilon1,epsilon2,epsilon3),axis=-1)

@njit
def to_basis(a:np.ndarray,basis:np.ndarray)->np.ndarray:
  return np.array((np.dot(a,basis[0,:]),np.dot(a,basis[1,:]),np.dot(a,basis[2,:])))

@njit
def rotational_support(v:np.ndarray,x:np.ndarray,sink_pos:np.ndarray)->np.float64:
  r=x-sink_pos
  v_tan=to_basis(v,make_basis(r))
  v_tan[0]*=0.0
  omega=np.linalg.norm(v_tan)/(2.0*np.pi*np.linalg.norm(r))
  return omega*omega*np.linalg.norm(r)

@njit
def rotational_fraction(gradP:np.ndarray,v:np.ndarray,x:np.ndarray,sink_pos:np.ndarray)->np.float64:
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

@njit(parallel=True)
def orbital_time(x,sinkPos,sinkM):
  nx,ny,nz,discard=x.shape
  T=np.zeros((nx,ny,nz))
  mu=6.67430e-8*sinkM
  for i in prange(nx):
    for j in range(ny):
      for k in range(nz):
        r=np.linalg.norm(x[i,j,k,:]-sinkPos)
        T[i,j,k]=2*np.pi*np.sqrt(r**3/mu)

@njit
def z_rotate(theta):
  return np.matrix([[np.cos(theta),-np.sin(theta),0],[np.sin(theta),np.cos(theta),0],[0.,0.,1.]])
@njit
def y_rotate(theta):
  return np.matrix([[np.cos(theta),0.,np.sin(theta)],[0.,0., 1.],[-np.sin(theta),0.,np.cos(theta)]])
#@njit
#def z_rotate(theta)
@njit
def find_rotors_to_z(vector):
  theta=np.arcos(vector[1]/np.linalg.norm(vector[0:2]))
  phi=np.arcsin(np.linalg.norm(vector[0:2])/np.linalg.norm(vector))