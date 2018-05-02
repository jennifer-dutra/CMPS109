#ifndef _CUBE_H_
#define _CUBE_H_

#include <array>

#include "containable.h"
#include "point.h"
#include "circle.h"
#include "line.h"


// C++ requires forward declaration of any cyclically dependent classes

class Cube : public Containable3D {

    public:
      Cube(Point3D upperFace[4], Point3D lowerFace[4]);

};

#endif
