#ifndef _REULEAUXTRIANGLE_H_
#define _REULEAUXTRIANGLE_H_

#include <array>

#include "containable.h"
#include "point.h"
#include "circle.h"
#include "line.h"


// C++ requires forward declaration of any cyclically dependent classes

class ReuleauxTriangle : public Containable2D {

    private:
         std::array<Point2D, 3> vertices_;

    public:

        // ReuleauxTriangle(std::array<Point2D, 3> vertices);
        ReuleauxTriangle(Point2D vertices[3]);

        std::array<Point2D, 3> vertices();

        std::vector<Line> edges();
        int vertexCount = 3;

        bool containedWithin(Circle &circle);
        bool containedWithin(RegularConvexPolygon &polygon);
        bool containedWithin(ReuleauxTriangle &rt);

};

#endif
