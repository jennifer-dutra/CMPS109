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

   while(stream >> token) {

     if(token == "Circle") {
       stream >> token;
       Point2D center = getPoint(token);
       printf("center: %f, %f \n", center.x, center.y);
       stream >> token;
       double radius = getRadius(token);
       printf("radius: %f \n", radius);

       if(innerCircle == NULL && innerRT == NULL && innerPoly == NULL) {
         innerCircle = new Circle(center, radius);
         printf("got inner circle\n");
       }
       else {
         outerCircle = new Circle(center, radius);
         printf("got outer circle\n");
       }
      }

      if(token == "ReuleauxTriangle") {
        stream >> token;
        Point2D p1 = getPoint(token);
        printf("p1: %f, %f \n", p1.x, p1.y);

        stream >> token;
        Point2D p2 = getPoint(token);
        printf("p2: %f, %f \n", p2.x, p2.y);

        stream >> token;
        Point2D p3 = getPoint(token);
        printf("p3: %f, %f \n", p3.x, p3.y);

        Point2D RT[3] = {p1, p2, p3};

        if(innerRT == NULL && innerCircle == NULL && innerPoly == NULL) {
          innerRT = new ReuleauxTriangle(RT);
          printf("got inner RT\n");
        }
        else {
          outerRT = new ReuleauxTriangle(RT);
          printf("got outer RT\n");
        }
      }

      // square test
      if(token == "RegularConvexPolygon") {
        stream >> token;
        Point2D p1 = getPoint(token);
        printf("p1: %f, %f \n", p1.x, p1.y);

        stream >> token;
        Point2D p2 = getPoint(token);
        printf("p2: %f, %f \n", p2.x, p2.y);

        stream >> token;
        Point2D p3 = getPoint(token);
        printf("p3: %f, %f \n", p3.x, p3.y);

        stream >> token;
        Point2D p4 = getPoint(token);
        printf("p4: %f, %f \n", p4.x, p4.y);

        if(innerRT == NULL && innerCircle == NULL && innerPoly == NULL) {
          innerPoly = new RegularConvexPolygon({p1, p2, p3, p4});
          printf("got inner poly\n");
        }
        else {
          outerPoly = new RegularConvexPolygon({p1, p2, p3, p4});
          printf("got outer poly\n");
        }

      }

      if(token == "true")
        expected = true;
    }

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
    else {
      printf("invalid input!!\n");
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
        if(line[0] != '#')
          parse(line);
      }
    }
    return 0;
}
