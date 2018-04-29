/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 *
 * Referenced webcast for help writing tests:
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=956436f9-b748-4ab7-b7b7-2e27605f1667
 */

#include <iostream>

#include <array>

#include "circle.h"
#include "polygon.h"
#include "reuleauxtriangle.h"

static void result(const char *msg, bool expected, bool got) {
  std::cout << msg;
  if(expected == got)
    std::cout << "PASS\n";
  else
    std::cout << "FAIL\n";
}


static void circleInCircle() {
  // inside
  Circle inner = Circle(Point2D(0.0,0.0), 4.0);
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  result("Circle-in-Circle: ", true, inner.containedWithin(outer));

}

static void circleTouchCircle() {
  // touching
  Circle inner = Circle(Point2D(0.0,0.0), 8.0);
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  result("Circle-touching-Circle: ", true, inner.containedWithin(outer));

}

static void circleIntersectCircle() {
  // intersection
  Circle inner = Circle(Point2D(4.0,4.0), 8.0);
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  result("Circle-intersecting-Circle: ", false, inner.containedWithin(outer));

}

static void circleOutsideCircle() {
  // outside
  Circle inner = Circle(Point2D(15,15), 4.0);
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  result("Circle-outside-Circle: ", false, inner.containedWithin(outer));
}

static void testCirclesInCircles() {
  circleInCircle();
  circleTouchCircle();
  circleOutsideCircle();
  circleIntersectCircle();

}

static void circleInSquare() {
  // inside
  Circle inner = Circle(Point2D(0.0,0.0), 4.0);
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-12, -12), Point2D(-12,12), Point2D(12,12), Point2D(12,-12)});
  result("Circle-In-Square: ", true, inner.containedWithin(outer));

}

static void circleIntersectSquare() {
  // intersect
  Circle inner = Circle(Point2D(9,0.0), 4.0);
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-12, -12), Point2D(-12,12), Point2D(12,12), Point2D(12,-12)});
  result("Circle-Intersect-Square: ", false, inner.containedWithin(outer));
}

static void circleTouchSquare() {
  // touching
  Circle inner = Circle(Point2D(7.0,0.0), 5.0);
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-12, -12), Point2D(-12,12), Point2D(12,12), Point2D(12,-12)});
  result("Circle-Touch-Square: ", true, inner.containedWithin(outer));
}

static void circleOutsideSquare() {
  // outside
  Circle inner = Circle(Point2D(50,30), 1.0);
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-12, -12), Point2D(-12,12), Point2D(12,12), Point2D(12,-12)});
  result("Circle-Outside-Square: ", false, inner.containedWithin(outer));
}

static void testCirclesInSquares() {
  circleInSquare();
  circleTouchSquare();
  circleIntersectSquare();
  circleOutsideSquare();
}

static void triangleInCircle() {
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  // inside
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-1, -1), Point2D(0,2), Point2D(1,-1)});
  result("Triangle-in-Circle: ", true, inner.containedWithin(outer));

}

static void triangleTouchCircle() {
  // touching
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-8, 0), Point2D(0,2), Point2D(8,0)});
  result("Triangle-Touch-Circle: ", true, inner.containedWithin(outer));

}

static void triangleIntersectCircle() {
  // intersect
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-9, -9), Point2D(0,4.5), Point2D(9,-9)});
  result("Triangle-Intersect-Circle: ", false, inner.containedWithin(outer));

}

static void triangleOutsideCircle() {
  // outside
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(10, 10), Point2D(0, 5), Point2D(15,10)});
  result("Triangle-Outside-Circle: ", false, inner.containedWithin(outer));

}

static void testTrianglesInCircles() {
  triangleInCircle();
  triangleTouchCircle();
  triangleIntersectCircle();
  triangleOutsideCircle();
}

static void squareInSquare() {
  // inside
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-8, -8), Point2D(-8,8), Point2D(8,8), Point2D(8,-8)});
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-3, -3), Point2D(-3,3), Point2D(3,3), Point2D(3,-3)});
  result("Square-Inside-Square: ", true, inner.containedWithin(outer));
}

static void squareTouchSquare() {
  // touching
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-8, -8), Point2D(-8,8), Point2D(8,8), Point2D(8,-8)});
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-8, -8), Point2D(-8,8), Point2D(8,8), Point2D(8,-8)});
  result("Square-Touch-Square: ", true, inner.containedWithin(outer));
}

static void squareIntersectSquare() {
  // intersection
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-8, -8), Point2D(-8,8), Point2D(8,8), Point2D(8,-8)});
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-12, -2), Point2D(-12, 6), Point2D(-4, 6), Point2D(-4,-2)});
  result("Square-Intersect-Square: ", false, inner.containedWithin(outer));
}

static void squareOutsideSquare() {
  // outside
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-8, -8), Point2D(-8,8), Point2D(8,8), Point2D(8,-8)});
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-30, -30), Point2D(-30,-25), Point2D(30,-25), Point2D(30,-30)});
  result("Square-Outside-Square: ", false, inner.containedWithin(outer));

  //outside 2
  inner = RegularConvexPolygon({
    Point2D(-8, -8), Point2D(-8,8), Point2D(8,8), Point2D(8,-8)});
  outer = RegularConvexPolygon({
    Point2D(-3, -3), Point2D(-3,3), Point2D(3,3), Point2D(3,-3)});
  result("Square-Outside-Square-spcase: ", false, inner.containedWithin(outer));
}

static void testSquaresInSquares() {
  squareInSquare();
  squareTouchSquare();
  squareIntersectSquare();
  squareOutsideSquare();
}

static void trianglesInSquares() {
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-8, -8), Point2D(-8,8), Point2D(8,8), Point2D(8,-8)});
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-1, -1), Point2D(0,2), Point2D(1,-1)});
    result("triangle-in-square: ", true, inner.containedWithin(outer));
}

static void trianglesTouchSquares() {
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-1, -1), Point2D(-1,1), Point2D(1,1), Point2D(1,-1)});
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-1, -1), Point2D(0,1), Point2D(1,0)});
    result("triangle-touch-square: ", true, inner.containedWithin(outer));
}

static void testTrianglesInSquares() {
  trianglesInSquares();
  trianglesTouchSquares();
}

static void rtInCircle() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);

  Circle outer = Circle(Point2D(0.0,0.0), 9.0);
  result("RT-in-Circle: ", true, inner.containedWithin(outer));

  outer = Circle(Point2D(0.0,0.0), 7.0);
  result("RT-in-Circle: ", true, inner.containedWithin(outer));
}

static void rtIntersectCircle() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);
  Circle outer = Circle(Point2D(0.0,0.0), 6.0);
  result("RT-Intersect-Circle: ", false, inner.containedWithin(outer));
}

static void rtTouchCircle() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);
  Circle outer = Circle(Point2D(0.0,0.0), 6.928204);
  result("RT-Touch-Circle: ", true, inner.containedWithin(outer));
}

static void rtOutsideCircle() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);
  Circle outer = Circle(Point2D(-50.0,0.0), 8);
  result("RT-Touch-Circle: ", false, inner.containedWithin(outer));
}

static void rtInCircleOverlap() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);
  Circle outer = Circle(Point2D(-24.5, -16.18), 34);
  result("Arc-overlap: ", true, inner.containedWithin(outer));

}

static void smallRT() {
  // FIX: vertices inside but edges overlap
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);
  Circle outer = Circle(Point2D(0.0, 19.2), 24);
  result("RT-edge-overlap: ", true, inner.containedWithin(outer));
}

static void testRTinCircles() {
  rtInCircle();
  rtIntersectCircle();
  rtTouchCircle();
  rtOutsideCircle();
  smallRT();
  rtInCircleOverlap();
}

static void circleInRT() {
  Circle inner = Circle(Point2D(3.7,2.17), 0.5);
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT);
  result("circle-in-RT-between: ", true, inner.containedWithin(outer));

  inner = Circle(Point2D(-3.34,1.15), 1);
  result("circle-in-RT-tri: ", true, inner.containedWithin(outer));
}

static void circleIntersectRT() {
  Circle inner = Circle(Point2D(-3.34,1.15), 4);
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT);
  result("circle-intersect-RT: ", false, inner.containedWithin(outer));
}

static void circleTouchRT() {
  Circle inner = Circle(Point2D(-3.98,1.15), 1);
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT);
  result("circle-intersect-RT: ", true, inner.containedWithin(outer));
}

static void circleOutsideRT() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT);
  Circle inner = Circle(Point2D(-8,5), 2);
  result("circle-outside-RT: ", false, inner.containedWithin(outer));
}

static void testCirclesInRT() {
  circleInRT();
  circleTouchRT();
  circleOutsideRT();
  circleIntersectRT();
}

static void sqInRT() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT);
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-2, -2), Point2D(-2,2), Point2D(2,2), Point2D(2,-2)});
  result("Square-in-RT: ", true, inner.containedWithin(outer));
}

static void sqIntersectRT() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT);
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-6.64, -0.58), Point2D(-6.64,2.34), Point2D(-3.71,2.34), Point2D(-3.71,0.58)});
  result("Square-intersect-RT: ", false, inner.containedWithin(outer));

}

static void sqOutsideRT() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT);
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-50, -50), Point2D(-50,50), Point2D(50,50), Point2D(50,-50)});
  result("Square-outside-RT: ", false, inner.containedWithin(outer));

  inner = RegularConvexPolygon({
    Point2D(-10,5), Point2D(-10, -10), Point2D(-5,10), Point2D(-5,5)});
  result("Square-outside-RT: ", false, inner.containedWithin(outer));
}

static void triangleInRT() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT);
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)});
  result("triangle-in-RT-spCase: ", true, inner.containedWithin(outer));

  inner = RegularConvexPolygon({
    Point2D(-1, -1), Point2D(0,2), Point2D(1,-1)});
  result("triangle-in-RT: ", true, inner.containedWithin(outer));
}

static void testPolyinRT() {
  sqInRT();
  sqIntersectRT();
  sqOutsideRT();
  triangleInRT();
}

static void RTinPoly() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-8.17, -7.96), Point2D(-8.17,16.15), Point2D(15.93,16.15), Point2D(15.93,-7.96)});
  result("RT-in-poly: ", true, inner.containedWithin(outer));
}

static void rtIntersectPoly() {

}

static void rtOutsidePoly() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-10,5), Point2D(-10, -10), Point2D(-5,10), Point2D(-5,5)});
  result("RT-outside-poly: ", false, inner.containedWithin(outer));
}

static void rtTouchPoly() {

}

static void testRTinPoly() {
  RTinPoly();
  rtIntersectPoly();
  rtOutsidePoly();
  rtTouchPoly();
}



static void RTinRT() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);

  Point2D RT_out[3] = {Point2D(-14.14, -8.17), Point2D(-0.29, 16.12), Point2D(13.83, -8.03)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT_out);

  result("RT-in-RT: ", true, inner.containedWithin(outer));
}

static void RToutRT() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);

  Point2D RT_out[3] = {Point2D(8.09, 3.9), Point2D(16.99, 19.32), Point2D(25.9, 3.9)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT_out);

  result("RT-in-RT: ", false, inner.containedWithin(outer));
}

static void RTintersectRT() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);

  Point2D RT_out[3] = {Point2D(-1.08, -3.46), Point2D(2.46, -9.6), Point2D(6, -3.46)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT_out);

  result("RT-in-RT: ", false, inner.containedWithin(outer));
}

static void RTsameRT() {
  Point2D RT[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle inner = ReuleauxTriangle(RT);

  Point2D RT_out[3] = {Point2D(0,6.928204), Point2D(6,-3.46410161514), Point2D(-6,-3.46410161514)};
  ReuleauxTriangle outer = ReuleauxTriangle(RT_out);

  result("RT-same-RT: ", true, inner.containedWithin(outer));
}

static void testRTinRT() {
  RTinRT();
  RToutRT();
  RTintersectRT();
  RTsameRT();
}


int main(int argc, char *argv[])
{
    testCirclesInCircles();
    testCirclesInSquares();
    testTrianglesInCircles();
    testSquaresInSquares();
    testTrianglesInSquares();
    testRTinCircles();
    testCirclesInRT();
    testPolyinRT();
    testRTinPoly();
    testRTinRT();

}
