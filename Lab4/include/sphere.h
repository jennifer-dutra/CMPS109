#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <array>

#include "containable.h"
#include "point.h"
#include "circle.h"
#include "line.h"


// C++ requires forward declaration of any cyclically dependent classes

class Sphere : public Containable3D {

    public:
      Sphere(Point3D center, double radius);

};

#endif
