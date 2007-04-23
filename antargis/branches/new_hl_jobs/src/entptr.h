#ifndef ENTPTR_H
#define ENTPTR_H

#include "entity.h"

class AntEntityPtr
{
 public:
  AntEntityPtr(const AntEntityPtr &p);
  AntEntityPtr(AntEntity *p);
  AntEntityPtr();

  AntEntity *get();

 private:
  AntEntity *p;
};


#endif
