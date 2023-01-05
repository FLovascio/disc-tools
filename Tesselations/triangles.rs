mod linalg
use linalg::vector

pub struct triangle<T>{
  point1:vector<T>,
  point2:vector<T>,
  point3:vector<T>,
}

pub fn normal<T>(A:triangle<T>)->vector<T>{
  return cross(minus(A.point1,A.point2),minus(A.point1,A.point3));
}