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


/*
 * Referenced webcast :
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=956436f9-b748-4ab7-b7b7-2e27605f1667
 *
 */
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
        if(Geom::intersects(edgeInner, edgeOuter, false)) {
          return false;
        }
      }
    }

    // check if completely outside or inside
    Line line(Geom::center(polygon), Geom::center(*this));
    for (Line const &edge: edges()) {
      if (Geom::intersects(edge, line, false)) {
        return false;
      }
    }

    std::vector<Point2D> polyInner = vertices();
    std::vector<Point2D> polyOuter = polygon.vertices();

    // if outer is completely inside inner
    if(Geom::distance(polyInner[0], Geom::center(*this)) > Geom::distance(polyOuter[0], Geom::center(polygon))) {
      return false;
    }

    return true;
}

bool RegularConvexPolygon::containedWithin(ReuleauxTriangle &rt) {
  // check: is poly in RT
  std::array<Point2D, 3> RTV = rt.vertices();
  // get radii of circle centered at each vertex
  double RTradius = Geom::distance(RTV[0], RTV[1]);

  for (Point2D const &vertexSq: vertices()) {
    for(Point2D const &vertexRT: rt.vertices()) {
      // if vertex of polygon is not inside or touching circles at RT vertices
      if(Geom::distance(vertexSq, vertexRT) > RTradius) {
        return false;
      }
    }
  }

  return true;
}
