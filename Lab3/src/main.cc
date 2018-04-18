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
  // inside
  Circle inner = Circle(Point2D(0.0,0.0), 4.0);
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);
  result("Circle-in-Circle: ", true, inner.containedWithin(outer));

  // touching
  inner = Circle(Point2D(0.0,0.0), 8.0);
  result("Circle-in-Circle: ", true, inner.containedWithin(outer));

  // outside
  inner = Circle(Point2D(15,15), 4.0);
  result("Circle-in-Circle: ", false, inner.containedWithin(outer));
}

static void testCirclesInSquares() {
  // inside
  Circle inner = Circle(Point2D(0.0,0.0), 4.0);
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-12, -12), Point2D(-12,12), Point2D(12,12), Point2D(12,-12)});
  result("Circle-In-Square: ", true, inner.containedWithin(outer));

  // intersect
  inner = Circle(Point2D(9,0.0), 4.0);
  result("Circle-In-Square: ", false, inner.containedWithin(outer));

  // touching
  inner = Circle(Point2D(7.0,0.0), 5.0);
  result("Circle-In-Square: ", true, inner.containedWithin(outer));

  // outside
  inner = Circle(Point2D(15,15), 1.0);
  result("Circle-In-Square: ", false, inner.containedWithin(outer));

}

static void testTrianglesInCircles() {
  Circle outer = Circle(Point2D(0.0,0.0), 8.0);

  // inside
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-1, -1), Point2D(0,2), Point2D(1,-1)});
  result("Triangle-in-Circle: ", true, inner.containedWithin(outer));

  // intersect
  inner = RegularConvexPolygon({
    Point2D(-9, -9), Point2D(0,4.5), Point2D(9,-9)});
  result("Triangle-in-Circle: ", false, inner.containedWithin(outer));

  // outside
  inner = RegularConvexPolygon({
    Point2D(10, 10), Point2D(0, 5), Point2D(15,10)});
  result("Triangle-in-Circle: ", false, inner.containedWithin(outer));

  // touching
  inner = RegularConvexPolygon({
    Point2D(-8, 0), Point2D(0,2), Point2D(8,0)});
  result("Triangle-in-Circle: ", true, inner.containedWithin(outer));
}

static void testSquaresInSquares() {
  // intersection
  RegularConvexPolygon outer = RegularConvexPolygon({
    Point2D(-8, -8), Point2D(-8,8), Point2D(8,8), Point2D(8,-8)});
  RegularConvexPolygon inner = RegularConvexPolygon({
    Point2D(-12, -2), Point2D(-12, 6), Point2D(-4, 6), Point2D(-4,-2)});

  result("Square-in-Square: ", false, inner.containedWithin(outer));

  // touching
  inner = RegularConvexPolygon({
    Point2D(-8, -8), Point2D(-8,8), Point2D(8,8), Point2D(8,-8)});

  result("Square-in-Square: ", true, inner.containedWithin(outer));

  // outside
  inner = RegularConvexPolygon({
    Point2D(-15, -0), Point2D(-15,5), Point2D(-10,5), Point2D(-10,-0)});

  // inside
  inner = RegularConvexPolygon({
    Point2D(-3, -3), Point2D(-3,3), Point2D(3,3), Point2D(3,-3)});

  result("Square-in-Square: ", true, inner.containedWithin(outer));
}

int main(int argc, char *argv[])
{
    testCirclesInCircles();
    testCirclesInSquares();
    testTrianglesInCircles();
    testSquaresInSquares();
}
