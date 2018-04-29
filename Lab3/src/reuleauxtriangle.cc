#include <iostream>

#include "reuleauxtriangle.h"
#include "geom.h"


ReuleauxTriangle::ReuleauxTriangle(Point2D vertices[3]) {
  vertices_ = {vertices[0], vertices[1], vertices[2]};
}

std::array<Point2D, 3> ReuleauxTriangle::vertices() {
    return vertices_;
}

// edges of "regular" triangle formed by making lines between vertecies
std::vector<Line> ReuleauxTriangle::edges() {
    std::vector<Line> edges;
    for (unsigned int i = 0; i < 2; i++)
        edges.push_back(Line(vertices_[i],vertices_[i+1]));
    edges.push_back(Line(vertices_[2],vertices_[0]));
    return edges;
}

bool ReuleauxTriangle::containedWithin(Circle &circle) {
  // if not all vertices are inside, RT is not in cirlce
  std::array<Point2D, 3> RTV = vertices();
  // get radii of circle centered at each vertex
  double RTradius = Geom::distance(RTV[0], RTV[1]);

  for(Point2D const &vertex: vertices()) {
    if(Geom::distance(vertex, circle.center()) > circle.radius()) {
      return false;
    }
  }

  // check if center of circle is contained in RT
  Circle centerPoint = Circle(circle.center(), 0);

  // if RT radius is bigger than circle radius and center of circle is not in RT
  if(RTradius < circle.radius() && !centerPoint.containedWithin(*this)) {
    return true;
  }

  return true;
}

bool ReuleauxTriangle::containedWithin(RegularConvexPolygon &polygon) {

  std::array<Point2D, 3> RTV = vertices();
  // get radii of circle centered at each vertex
  double RTradius = Geom::distance(RTV[0], RTV[1]);

  // see if any RT vertices are outside the polygon
  for(Point2D const &polyVertex: polygon.vertices()) {
    // create line from vertex to center of RT
    Line line(polyVertex, Geom::center(*this));
    printf("vertices outside poly");

    for(Point2D const &RTvertex: vertices()) {
      Circle c = Circle(RTvertex, RTradius);
      if(Geom::intersects(line, c)) {
        printf("line intersection");
        return false;
      }
    }
  }
  return true;
}

bool ReuleauxTriangle::containedWithin(ReuleauxTriangle &rt) {

  std::array<Point2D, 3> RTV = rt.vertices();
  // get radii of circle centered at each vertex
  double RTradius = Geom::distance(RTV[0], RTV[1]);

  std::array<Point2D, 3> RTVInner = (*this).vertices();
  // get radii of circle centered at each vertex
  double RTradiusInner = Geom::distance(RTVInner[0], RTVInner[1]);

  // check if all inner vertices contained within all outer circles
  for(Point2D const &vertexIn: vertices()) {
    for(Point2D const &vertexOut: rt.vertices()) {
      Circle c = Circle(vertexOut, RTradius);
      if(Geom::distance(vertexIn, c.center()) > c.radius()) {
        return false;
      }
    }
  }

  // if radius of inner RT is greater than outer RT
  if(RTradiusInner > RTradius) {
    return false;
  }

  return true;

}
