#include "vehicle.h"

size_t Vehicle::countOfWheels() const
{
  return 0;
}
void Vehicle::travel(float time)
{
  travelWay(0);
}

float Vehicle::getWayTraveled() const
{
  return wayTravelled;
}

void Vehicle::travelWay(float way)
{
  wayTravelled+=way;
}
