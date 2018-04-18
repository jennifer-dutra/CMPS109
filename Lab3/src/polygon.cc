/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include <iostream>

#include "polygon.h"
#include "geom.h"

RegularConvexPolygon::RegularConvexPolygon(std::vector<Point2D> vertices) {
    vertices_ = vertices;
}

std::vector<Point2D> RegularConvexPolygon::vertices() {
    return vertices_;
}

std::vector<Line> RegularConvexPolygon::edges() {
    std::vector<Line> edges;
    for (unsigned int i = 0; i < vertices_.size()-1; i++)
        edges.push_back(Line(vertices_[i],vertices_[i+1]));
    edges.push_back(Line(vertices_[vertices_.size()-1],vertices_[0]));
    return edges;
}

int RegularConvexPolygon::vertexCount() {
    return vertices_.size();
}

bool RegularConvexPolygon::containedWithin(Circle &circle) {
    for(Point2D const &vertex: vertices()) {
      if(Geom::distance(vertex, circle.center()) > circle.radius()) {
        return false;
      }
    }
    return true;
}

bool RegularConvexPolygon::containedWithin(RegularConvexPolygon &polygon) {
    for(Line const &edgeOuter: edges()) {
      for(Line const &edgeInner: polygon.edges()) {
        if(Geom::intersects(edgeInner, edgeOuter)) {
          return false;
        }
      }
    }

    // check if completely outside or inside
    Line line(Geom::center(polygon), Geom::center(*this));
    for (Line const &edge: polygon.edges()) {
        if (Geom::intersects(edge, line))
            return false;
    }
    return true;
}

bool RegularConvexPolygon::containedWithin(ReuleauxTriangle &rt) {
    throw "Not implemented";
}
