#ifndef CAR_H
#define CAR_H

#include "vehicle.h"

// INCLUDE_SWIG

class Car:public Vehicle
{
public:
  virtual size_t countOfWheels() const;
  virtual void travel(float time);

  // a function being in this class alone
  size_t countOfWindows() const;

};

#endif
