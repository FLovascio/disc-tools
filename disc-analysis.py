import osyris
import numpy as np
from numba import njit, stencil

@stencil
def grad_P(P,x):
  hix=x[1,0,0]-x[0,0,0]
  hmx=x[0,0,0]-x[-1,0,0]
  hfx=hix/hmx
  dpx=P[1,0,0]-((hfx)**2)*P[-1,0,0]-(1.0-(hfx)**2)*P[0,0,0]
  hiy=x[0,1,0]-x[0,0,0]
  hmy=x[0,0,0]-x[0,-1,0]
  hfy=hiy/hmy
  dpy=P[0,1,0]-((hfx)**2)*P[0,-1,0]-(1.0-(hfy)**2)*P[0,0,0]
  hiz=x[0,0,1]-x[0,0,0]
  hmz=x[0,0,0]-x[0,0,-1]
  hfz=hiz/hmz
  dpz=P[0,0,1]-((hfx)**2)*P[0,0,-1]-(1.0-(hfz)**2)*P[0,0,0]
  return np.array((dpx,dpy,dpz))

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

@njit
def angular_momentum(v,rho,V,x):
  return rho*V*np.cross(x,v)

@njit
def angular_momentum_sphere(v,rho,V,x):
  total_angular_momentum=np.array((0,0,0))
  for vi,rhoi,Vi,xi in zip(v,rho,V,x):
    total_angular_momentum+=angular_momentum(vi,rhoi,Vi,xi)
  return total_angular_momentum
