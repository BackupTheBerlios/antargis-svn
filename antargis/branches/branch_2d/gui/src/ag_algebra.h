#ifndef AG_ALGEBRA_H
#define AG_ALGEBRA_H

#include <vector>
#include <ag_string.h>

class AGMatrixN
{
 public:
  AGMatrixN(size_t w,size_t h);
  AGMatrixN(const AGMatrixN &p);

  void set(size_t x,size_t y,float v);
  float get(size_t x,size_t y) const;

  AGMatrixN &operator*=(const AGMatrixN &p);
  AGMatrixN operator*(const AGMatrixN &p) const;

  AGMatrixN operator-(const AGMatrixN &p) const;

  float scalar() const;

  AGMatrixN inverse() const;
  AGMatrixN pseudoInverse() const;
  AGMatrixN makeQuadratic() const;

  AGMatrixN transposed() const;

  void makeUnitMatrix();

  void copyFrom(const AGMatrixN &p);

  void output() const;

  AGString toString() const;

  void swapRows(size_t a,size_t b);
  void swapCols(size_t a,size_t b);

 private:
  
  inline size_t index(size_t x,size_t y) const { return x+y*mW; }

  std::vector<float> m;
  size_t mW,mH;
};

#endif
