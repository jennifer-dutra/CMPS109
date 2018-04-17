/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include <iostream>

#include "circle.h"
#include "polygon.h"

static void result(const char *msg, bool expected, bool got) {
  std::cout << msg;
  if(expected == got)
    std::cout << "PASS\n";
  else
    std::cout << "FAIL\n";
}

static void testCirclesInCircles() {
  Circle inner = Circle(Point2D(0.0,0.0), 4.0);
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  result("Circle-in-Circle: ", true, inner.containedWithin(outer));
}

static void testCirclesInSquares() {
  Circle inner = Circle(Point2D(0.0,0.0), 4.0);
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-12, -12), Point2D(-12,12), Point2D(12,12), Point2D(12,-12)});
  result("Square-in-Circle: ", true, inner.containedWithin(outer));
}

static void testTrianglesInCircles() {
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-1, -1), Point2D(0,2), Point2D(1,-1)});
  result("Triangle-in-Circle: ", true, inner.containedWithin(outer));
}

int main(int argc, char *argv[])
{
    testCirclesInCircles();
    testCirclesInSquares();
    testTrianglesInCircles();
}
