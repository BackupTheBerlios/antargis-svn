#ifndef __antargis_vector_h
#define __antargis_vector_h

#include <algorithm>
#include <iostream>
#include <assert.h>
#include <cmath>

template<typename T,int N>
class Vector
{
 public:
  typedef T value_type;
  
  static inline int dim()
    {
      return N; 
    }
  
  inline Vector() 
    {
    }
  
  explicit inline Vector(const T& v0) 
    {
      assert(N==1); 
      values_[0] = v0; 
    }
  
  inline Vector(const T& v0,const T& v1) 
    {
      assert(N==2); 
      values_[0] = v0;
      values_[1] = v1; 
    }
  
  inline Vector(const T& v0,const T& v1,const T& v2) 
    {
      assert(N==3);
      values_[0]=v0;
      values_[1]=v1;
      values_[2]=v2; 
    }
  
  inline Vector(const T& v0,const T& v1,const T& v2,
		const T& v3)
    {
      assert(N==4); 
      values_[0]=v0;
      values_[1]=v1; 
      values_[2]=v2; 
      values_[3]=v3; 
    }
  
  inline Vector(const T& v0,const T& v1,const T& v2,
		const T& v3,const T& v4) 
    { 
      assert(N==5); 
      values_[0]=v0;
      values_[1]=v1;
      values_[2]=v2;
      values_[3]=v3;
      values_[4]=v4; 
    }
  
  inline Vector(const T& v0,const T& v1,const T& v2,
		const T& v3,const T& v4,const T& v5) 
    {
      assert(N==6);
      values_[0]=v0; 
      values_[1]=v1;
      values_[2]=v2;
      values_[3]=v3;
      values_[4]=v4; 
      values_[5]=v5; 
    }
  
  explicit inline Vector(const T _values[N]) 
    {
      memcpy(values_,_values,N*sizeof(T)); 
    }
  
  inline Vector(const Vector<T,N> &o) 
    { 
      memcpy(values_, o.values_, N*sizeof(T)); 
    }
  
  template<typename otherTType>
    inline Vector(const Vector<otherTType,N> &o)
    {
      operator=(o); 
    }
  
  
  
  template<typename otherTType>
    inline Vector<T,N>& operator=(const Vector<otherTType,N> &o) 
    {
      for(int i=0; i<N; i++) 
	values_[i] = T(o[i]); 
      return *this; 
    }
  
  
  inline operator T*() 
    {
      return values_; 
    }

  
  inline (operator const T*)() const 
    {
      return values_; 
    }




  //--- element access ---

  /// get i'th element read-write
  inline T& operator[](int i) {
    assert(i>=0 && i<N); return values_[i]; 
  }

  /// get i'th element read-only
  inline const T& operator[](int i) const {
    assert(i>=0 && i<N); return values_[i]; 
  }




  //--- assignment, comparsion ---
  
  /// assignment from a vector of the same kind
  inline Vector<T,N> &operator=(const Vector<T,N> &o) {
    memcpy(values_, o.values_, N*sizeof(T)); 
    return *this;
  }

  /// component-wise comparison
  inline bool operator==(const Vector<T,N> &other) const {
    for(int i=0; i<N; i++) if(values_[i]!=other.values_[i]) return false;
    return true; 
  }

  /// component-wise comparison
  inline bool operator!=(const Vector<T,N> &other) const {
    for(int i=0; i<N; i++) if(values_[i]!=other.values_[i]) return true;
    return false; 
  }




  //--- overloaded operators ---

  /// compute scalar product with another vector of same type
  inline T operator|(const Vector<T,N> &other) const {
    T p(values_[0]*other.values_[0]);
    for(int i=1; i<N; i++)
      p += values_[i]*other.values_[i]; 
    return p; 
  }

  /// component-wise self-multiplication with scalar
  inline Vector<T,N>& operator*=(const T &s) {
    for(int i=0; i<N; i++) values_[i] *= s; 
    return *this; 
  }

  /// component-wise multiplication with scalar
  inline Vector<T,N> operator*(const T &s) const {
    return Vector<T,N>(*this) *= s;
  }

  /// component-wise self-multiplication 
  inline const Vector<T,N>& operator*=(const Vector<T,N> &rhs) {
    for(int i=0; i<N; i++) values_[i] *= rhs[i]; 
    return *this; 
  }

  /// component-wise multiplication
  inline Vector<T,N> operator*(const Vector<T,N> &rhs) const {
    return Vector<T,N>(*this) *= rhs;
  }

  /// component-wise self-division by scalar
  inline const Vector<T,N> &operator/=(const T &s) {
    for(int i=0; i<N; i++) values_[i] /= s; 
    return *this; 
  }

  /// component-wise division by scalar
  inline Vector<T,N> operator/(const T &s) const {
    return Vector<T,N>(*this) /= s;
  }

  /// component-wise self-division
  inline const Vector<T,N> &operator/=(const Vector<T,N> &rhs) {
    for(int i=0; i<N; i++) values_[i] /= rhs[i]; 
    return *this; 
  }

  /// component-wise division 
  inline Vector<T,N> operator/(const Vector<T,N> &rhs) const {
    return Vector<T,N>(*this) /= rhs;
  }

  /// vector difference from this
  inline Vector<T,N> &operator-=(const Vector<T,N> &other) {
    for(int i=0; i<N; i++) values_[i] -= other.values_[i]; 
    return *this; 
  }
  
  /// vector difference
  inline Vector<T,N> operator-(const Vector<T,N> &other) const {
    return Vector<T,N>(*this) -= other;
  }
  
  /// vector self-addition
  inline Vector<T,N> &operator+=(const Vector<T,N> &other) {
    for(int i=0; i<N; i++) values_[i] += other.values_[i]; 
    return *this; 
  }
  
  /// vector addition
  inline Vector<T,N> operator+(const Vector<T,N> &other) const {
    return Vector<T,N>(*this) += other; 
  }
  
  /// unary minus
  inline Vector<T,N> operator-(void) const {
    Vector<T,N> v;
    for(int i=0; i<N; i++) v.values_[i] = -values_[i]; 
    return v; 
  }

  /// cross product: only defined for Vec3f/Vec3d as specialization
  inline Vector<T,N> operator%(const Vector<T,N> &other) const;





  //--- norm stuff ---

  /// compute norm; leave vector unchanged
  inline T norm() const {
    return (T)sqrt(sqrnorm()); }

  /// compute squared norm; leave vector unchanged 
  inline T sqrnorm() const {
    T s(values_[0]*values_[0]);
    for(int i=1; i<N; i++) s += values_[i]*values_[i];
    return s;
  }

  /// normalize vector, return normalized vector
  inline Vector<T,N> &normalize() {
    T n = norm();
    if (n != 0.0)
      *this *= 1.0/n;
    return *this;
  }
  


  //--- max min mean ---

  /// return the maximal component
  inline T max() const {
    T m(values_[0]);
    for(int i=1; i<N; i++) if(values_[i]>m) m=values_[i];
    return m; 
  }
    
  /// return the minimal component
  inline T min() const {
    T m(values_[0]); 
    for(int i=1; i<N; i++) if(values_[i]<m) m=values_[i];
    return m; 
  }
    
  /// return arithmetic mean
  inline T arith_mean() const {
    T m(values_[0]); 
    for(int i=1; i<N; i++) m+=values_[i];
    return m/T(N); 
  }

  /// component-wise min
  inline Vector<T,N> min(const Vector<T,N> &other) {
    Vector<T,N> res;
    for (int i = 0; i < N; i++)
      res[i] = std::min(values_[i],other[i]);
    return res;
  }
  
  /// component-wise max
  inline Vector<T,N> max(const Vector<T,N> &other) {
    Vector<T,N> res;
    for (int i = 0; i < N; i++)
      res[i] = std::max(values_[i],other[i]);
    return res;
  }
  
  /// minimize values: same as *this = min(*this,other), but faster
  inline Vector<T,N> minimize(const Vector<T,N> &other) {
    for (int i = 0; i < N; i++)
      if (other[i] < values_[i]) values_[i] = other[i];
    return *this;
  }

  /// maximize values: same as *this = max(*this,other), but faster
  inline Vector<T,N> maximize(const Vector<T,N> &other) {
    for (int i = 0; i < N; i++)
      if (other[i] > values_[i]) values_[i] = other[i];
    return *this;
  }
  
  /// component-wise apply function object with T operator()(T).
  template<typename func>
    inline Vector<T,N> apply(const func& f) const 
    {
      Vector<T,N> result;
      for(int i=0; i<N; i++) result[i] = f(values_[i]);
      return result; 
    }
  
  /// compose vector containing the same value in each component
    static inline Vector<T,N> vectorize(T value) 
      {
	Vector<T,N> result;
	for(int i=0; i<N; i++)
	  result[i] = value;
	return result; 
      }



protected:

  /** The N values of the template T type are the only data members
      of this class. This guarantees 100% compatibility with arrays of type
      T and size N, allowing us to define the cast operators to and from
      arrays and array pointers */
  T values_[N];
};

//== TEMPLATE SPECIALIZATIONS =================================================


/// cross product for Vec3f
template<>
inline Vector<float,3>
Vector<float,3>::operator%(const Vector<float,3> &other) const 
{
  return 
    Vector<float,3>(values_[1]*other.values_[2]-values_[2]*other.values_[1],
		     values_[2]*other.values_[0]-values_[0]*other.values_[2],
		     values_[0]*other.values_[1]-values_[1]*other.values_[0]);
}
  

/// cross product for Vec3d
template<>
inline Vector<double,3>
Vector<double,3>::operator%(const Vector<double,3> &other) const
{
 return 
   Vector<double,3>(values_[1]*other.values_[2]-values_[2]*other.values_[1],
		     values_[2]*other.values_[0]-values_[0]*other.values_[2],
		     values_[0]*other.values_[1]-values_[1]*other.values_[0]);
}



//== FUNCTIONS ================================================================


/// read the space-separated components of a vector from a stream
template<typename T,int N>
inline std::istream& operator>>(std::istream& is, Vector<T,N>& vec) {
  for(int i=0; i<N; i++) is >> vec[i];
  return is;
}


/// output a vector by printing its space-separated compontens
template<typename T,int N>
inline std::ostream& 
operator<<(std::ostream& os, const Vector<T,N>& vec) {
  for(int i=0; i<N-1; i++) os << vec[i] << " ";
  os << vec[N-1];
  return os;
}

  
/// scalar * vector
template<typename T,int N>
inline Vector<T,N> operator*(T s, const Vector<T,N>& v ) {
  return v*s;
}


//== TYPEDEFS =================================================================


/** 2-byte signed vector */
typedef Vector<signed char,2> Vec2c;
/** 2-byte unsigned vector */
typedef Vector<unsigned char,2> Vec2uc;
/** 2-short signed vector */
typedef Vector<signed short int,2> Vec2s;
/** 2-short unsigned vector */
typedef Vector<unsigned short int,2> Vec2us;
/** 2-int signed vector */
typedef Vector<signed int,2> Vec2i;
/** 2-int unsigned vector */
typedef Vector<unsigned int,2> Vec2ui;
/** 2-float vector */
typedef Vector<float,2> Vec2f;
/** 2-double vector */
typedef Vector<double,2> Vec2d;

/** 3-byte signed vector */
typedef Vector<signed char,3> Vec3c;
/** 3-byte unsigned vector */
typedef Vector<unsigned char,3> Vec3uc;
/** 3-short signed vector */
typedef Vector<signed short int,3> Vec3s;
/** 3-short unsigned vector */
typedef Vector<unsigned short int,3> Vec3us;
/** 3-int signed vector */
typedef Vector<signed int,3> Vec3i;
/** 3-int unsigned vector */
typedef Vector<unsigned int,3> Vec3ui;
/** 3-float vector */
typedef Vector<float,3> Vec3f;
/** 3-double vector */
typedef Vector<double,3> Vec3d;

/** 4-byte signed vector */
typedef Vector<signed char,4> Vec4c;
/** 4-byte unsigned vector */
typedef Vector<unsigned char,4> Vec4uc;
/** 4-short signed vector */
typedef Vector<signed short int,4> Vec4s;
/** 4-short unsigned vector */
typedef Vector<unsigned short int,4> Vec4us;
/** 4-int signed vector */
typedef Vector<signed int,4> Vec4i;
/** 4-int unsigned vector */
typedef Vector<unsigned int,4> Vec4ui;
/** 4-float vector */
typedef Vector<float,4> Vec4f;
/** 4-double vector */
typedef Vector<double,4> Vec4d;


#endif
