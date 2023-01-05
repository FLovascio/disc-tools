use std::num;

pub struct vector<T:Float>{
  e1: T,
  e2: T,
  e3: T,
}

pub fn cross<T>(a:vector<T>,b:vector<T>)->vector<T>{
  return vector<T>(a.e2*b.e3-a.e3*b.e2, a.e3*b.e1-b.e1*a.e3, a.e1*b.e2-a.e2*b.e1);
}

pub fn dot<T>(a:vector<T>,b:vector<T>)->T{
  return a.e1*b.e1+a.e2*b.e2+a.e3*b.e3;
}

pub fn minus<T>(a:vector<T>,b:vector<T>)->vector<T>{
  return vector<T>(a.e1-b.e1, a.e2-b.e2, a.e3-b.e3);
}

pub fn plus<T>(a:vector<T>,b:vector<T>)->vector<T>{
  return vector<T>(a.e1+b.e1, a.e2+b.e2, a.e3+b.e3);
}

pub fn norm<T>(a:vector<T>)->T{
  return num::sqrt(a.e1*a.e1+ a.e2*a.e2+ a.e3*a.e3);
}

pub fn direction<T>(a:vector<T>)->vector<T>{
  vNorm:T=norm(a);
  return vector<T>(a.e1/vNorm,a.e2/vNorm,a.e3/vNorm)
}