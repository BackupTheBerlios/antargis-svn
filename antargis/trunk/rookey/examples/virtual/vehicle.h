#ifndef VEHICLE_H
#define VEHICLE_H

// SWIG_INCLUDE

#include <rk_rubyobj.h>

class Vehicle:public AGRubyObject
{
  public:
    virtual size_t countOfWheels() const;
    virtual void travel(float time);

    float getWayTraveled() const;
  protected:
    void travelWay(float way);
  private:

    float wayTravelled;
};

#endif
