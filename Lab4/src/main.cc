/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "circle.h"
#include "reuleauxtriangle.h"
#include "polygon.h"

#define USAGE "USAGE %s test-file\n"

/*
 * Referenced webcast for help in parsing file
 * Source: https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=f1b2136b-19ef-4971-b4c6-504eadc46eac
 */

 static Point2D getPoint(std::string &token) {
   double x,y;
   std::stringstream vstream(token);
   std::string value;
   if(std::getline(vstream, value, ',')) {
     std::stringstream(value) >> x;
     if(std::getline(vstream, value, ',')) {
       std::stringstream(value) >> y;
       return Point2D(x,y);
     }
   }
   throw "invalid point" + token;
 }

 static double getRadius(std::string &token) {
   double radius;
   std::stringstream vstream(token);
   std::string value;
   if(std::getline(vstream, value)) {
     std::stringstream(value) >> radius;
     return radius;
   }
   throw "invalid radius" + token;
 }


 static void parse(const std::string line) {

   std::stringstream stream(line);
   std::string token;

   Circle *innerCircle = NULL;
   Circle *outerCircle = NULL;

   ReuleauxTriangle *innerRT = NULL;
   ReuleauxTriangle *outerRT = NULL;

   RegularConvexPolygon *innerPoly = NULL;
   RegularConvexPolygon *outerPoly = NULL;

   bool expected = false;
   bool threeD = false;

   while(stream >> token) {

     Circle:
       if(token == "Circle") {
         stream >> token;
         Point2D center = getPoint(token);
         // printf("center: %f, %f \n", center.x, center.y);
         stream >> token;
         double radius = getRadius(token);
         // printf("radius: %f \n", radius);

         if(innerCircle == NULL && innerRT == NULL && innerPoly == NULL) {
           innerCircle = new Circle(center, radius);
           // printf(" got inner circle ");
         }
         else {
           outerCircle = new Circle(center, radius);
           // printf(" got outer circle ");
         }
        }

      ReuleauxTriangle:
        if(token == "ReuleauxTriangle") {
          // printf(" RT ");

          stream >> token;
          Point2D p1 = getPoint(token);
          // printf("p1: %f, %f \n", p1.x, p1.y);

          stream >> token;
          Point2D p2 = getPoint(token);
          // printf("p2: %f, %f \n", p2.x, p2.y);

          stream >> token;
          Point2D p3 = getPoint(token);
          // printf("p3: %f, %f \n", p3.x, p3.y);

          Point2D RT[3] = {p1, p2, p3};

          if(innerRT == NULL && innerCircle == NULL && innerPoly == NULL) {
            innerRT = new ReuleauxTriangle(RT);
            // printf(" got inner RT ");
          }
          else {
            outerRT = new ReuleauxTriangle(RT);
            // printf(" got outer RT ");
          }
        }

      RegularConvexPolygon:
        if(token == "RegularConvexPolygon") {

          // printf(" entering poly ");

          std::vector<Point2D> polyVector;
          stream >> token;

          Point2D p;
          while(token != "true" && token != "false" && token != "RegularConvexPolygon"
          && token != "Circle" && token != "ReuleauxTriangle") {
            p = getPoint(token);
            polyVector.push_back(p);
            stream >> token;
          }

          if(innerRT == NULL && innerCircle == NULL && innerPoly == NULL) {
            innerPoly = new RegularConvexPolygon(polyVector);
            // printf(" got inner poly ");
          }
          else {
            outerPoly = new RegularConvexPolygon(polyVector);
            // printf(" got outer poly ");
          }

          // if next token is a shape
          if(token == "Circle")
            goto Circle;
          else if(token == "ReuleauxTriangle")
            goto ReuleauxTriangle;
          else if(token == "RegularConvexPolygon")
            goto RegularConvexPolygon;
        }


      if(token == "Cube" || token == "Sphere" || token == "ReuleauxTetrahedron") {
        threeD = true;
      }

      // get expected result
      if(token == "true")
        expected = true;
    }

    // get actual result
    bool result;
    if(innerCircle != NULL && outerCircle != NULL) {
      result = innerCircle->containedWithin(*outerCircle);
    }
    else if(innerRT != NULL && outerCircle != NULL) {
      result = innerRT->containedWithin(*outerCircle);
    }
    else if(innerCircle != NULL && outerRT != NULL) {
      result = innerCircle->containedWithin(*outerRT);
    }
    else if(innerRT != NULL && outerRT != NULL) {
      result = innerRT->containedWithin(*outerRT);
    }
    else if(innerPoly != NULL && outerPoly != NULL) {
      result = innerPoly->containedWithin(*outerPoly);
    }
    else if(innerCircle != NULL && outerPoly != NULL) {
      result = innerCircle->containedWithin(*outerPoly);
    }
    else if(innerPoly != NULL && outerCircle != NULL) {
      result = innerPoly->containedWithin(*outerCircle);
    }
    else if(innerRT != NULL && outerPoly != NULL) {
      result = innerRT->containedWithin(*outerPoly);
    }
    else if(innerPoly != NULL && outerRT != NULL) {
      result = innerPoly->containedWithin(*outerRT);
    }
    else if(threeD) {
      result = false;
      threeD = false;
    }
    else {
      throw "Badly formed input";
    }

    std::cout << (result == expected ? "PASS" : "FAIL") << std::endl;

    // std::cout << token << std::endl;
 }

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf(USAGE, argv[0]);
        return -1;
    }

    std::cout << "Test file: " << argv[1] << std::endl;

    std::ifstream ifs(argv[1]);
    if (ifs) {
      std::string line;
      while(std::getline(ifs, line)) {
        // if line is empty
        if (line.empty())
          continue;
        // ignore any comment line
        if(line[0] != '#') {
          parse(line);
        }
      }
    }
    return 0;
}
