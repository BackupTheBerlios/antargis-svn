#ifndef PATH_VECTOR_SORT_H
#define PATH_VECTOR_SORT_H

class DistanceOrder
{
public:
  DistanceOrder(const AGVector2 &pBase):mBase(pBase)
  {
  }
  bool operator()(const AGVector2 &a,const AGVector2 &b) const
  {
    return (a-mBase).length2()<(b-mBase).length2();
  }
private:
  AGVector2 mBase;

};

#endif
