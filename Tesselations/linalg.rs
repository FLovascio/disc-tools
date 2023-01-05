mod linalg{
  struct vector<T>{
    e1: T,
    e2: T,
    e3: T,
  }
  fn cross<T>(a:vector<T>,b:vector<T>)->vector<T>{
    return vector<T>(a.e2*b.e3-a.e3*b.e2, a.e3*b.e1-b.e1*a.e3, a.e1*b.e2-a.e2*b.e1);
  }
  fn dot<T>(a:vector<T>,b:vector<T>)->T{
    return a.e1*b.e1+a.e2*b.e2+a.e3*b.e3;
  }
}