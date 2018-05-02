#ifndef _REULEAUXTETRAHEDRON_H_
#define _REULEAUXTETRAHEDRON_H_

#include <array>

#include "containable.h"
#include "point.h"
#include "circle.h"
#include "line.h"


// C++ requires forward declaration of any cyclically dependent classes

class ReuleauxTetrahedron : public Containable3D {

    public:
      ReuleauxTetrahedron(Point3D vertices[4]);

};

#endif
