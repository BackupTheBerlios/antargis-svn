#ifndef AG_SERIAL_VEC_H
#define AG_SERIAL_VEC_H

#include <ag_serial.h>
#include <ag_geometry.h>

AGEXPORT BinaryIn &operator>>(BinaryIn &i,AGVector2 &v);
AGEXPORT BinaryIn &operator>>(BinaryIn &i,AGVector3 &v);
AGEXPORT BinaryIn &operator>>(BinaryIn &i,AGVector4 &v);


#endif
