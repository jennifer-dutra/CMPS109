#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bounds.h"

/*
 * Intialise a two or thre dimensional space of shape ARENA containing NUMSHAPES
 * SHAPES, all of which may or may not be entirely contained within ARENA.
 *
 * Starter code from Prof. Harrison's webcast:
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=785d50a3-c653-46b8-b9f6-0c468e959ac7
 */

static Shape *arena;

void setup(Shape *a, Shape *shapes[], int numShapes) {
  arena = a;
}

// creates a Point object
Point makePoint(float x, float y, float z) {
    Point p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}

// Calculates the centroid of a polygon
// Referenced source: https://stackoverflow.com/questions/19766485/how-to-calculate-centroid-of-polygon-in-c
Point findCentriod(Point centroid, Polygon *inner) {
  float a, cx, cy, t;
  int i1;

  // Calculate area of polygon
  a = 0.0;
  i1 = 1;
  for(int i = 0; i < inner->numVertices; i++) {
    a += (inner->vertices)[i].x * (inner->vertices)[i1].y - (inner->vertices)[i1].x * (inner->vertices)[i].y;
    i1 = (i1 + 1) % inner->numVertices;
  }
  a *= 0.5;

  // calculate the x and y coordinates of centriod
  cx = cy = 0.0;
  i1 = 1;
  for(int i = 0; i < inner->numVertices; i++) {
    t = (inner->vertices)[i].x * (inner->vertices)[i1].y - (inner->vertices)[i1].x * (inner->vertices)[i].y;
    cx += ((inner->vertices)[i].x + (inner->vertices)[i1].x) * t;
    cy += ((inner->vertices)[i].y + (inner->vertices)[i1].y) * t;
    i1 = (i1 + 1) % inner->numVertices;
  }
  centroid.x = cx / (6.0 * a);
  centroid.y = cy / (6.0 * a);
  return centroid;
}


/*
 * Checks if edge of polygon intersects circle
 * Sources:
 * https://www.geeksforgeeks.org/check-line-touches-intersects-circle/
 * https://www.geeksforgeeks.org/program-find-line-passing-2-points/
 *
 */
static bool polyCircleIntersect(Circle *circle, Polygon *poly) {
  for(int i = 0; i < poly->numVertices; i++) {
    bool restart = false;

    // two vertices form a line
    Point P = (poly->vertices)[i];
    Point Q = (poly->vertices)[i + 1];

    if(i == poly->numVertices - 1) {
      restart = true;
      P = (poly->vertices)[poly->numVertices - 1];
      Q = (poly->vertices)[0];
    }

    // equation for a line is ax + by + c = 0
    float a = Q.y - P.y;
    float b = P.x - Q.x;
    float c = (a * P.x) + (b * P.y);

    // center of circle
    float x = circle->center.x;
    float y = circle->center.y;

    // check: does circle intersect side of polygon (line)
    int dist = (abs(a * x + b * y + c)) / sqrt(a * a + b * b);

    // printf("dist: %d vs radius: %f ", dist, circle->radius);

    if(circle->radius > dist) {
      return true;
    }
    if(restart == true) {
      break;
    }
  }
  return false;
}

bool lineSegIntersect(Polygon *outer, Polygon *inner) {
  /* check if any sides of outer polygon intersect any sides of inner polygon
  (intersection of line segments ) */
  bool restartOuter = false;
  bool restartInner = false;

  for(int i = 0; i < outer->numVertices ; i++) {
    for(int j = 0; j < inner->numVertices; j++) {

      // two vertices form a line
      Point P = (outer->vertices)[i];
      Point Q = (outer->vertices)[i + 1];

      Point S = (inner->vertices)[j];
      Point T = (inner->vertices)[j + 1];

      if(i == outer->numVertices - 1) {
        restartOuter = true;
        P = (outer->vertices)[outer->numVertices - 1];
        Q = (outer->vertices)[0];
      }

      if(j == inner->numVertices - 1) {
        restartInner = true;
        S = (inner->vertices)[inner->numVertices - 1];
        T = (inner->vertices)[0];
      }

      /* Find equation for inner polygon edge y  =mx + b form */
      float A1 = (P.y - Q.y) / (P.x - Q.x);   // Pay attention to not dividing by zero
      float A2 = (S.y - T.y) / (S.x - T.x);   // Pay attention to not dividing by zero
      float b1 = P.y - (A1 * Q.x);
      float b2 = S.y - (A2 * T.x);


      // check bounds of line segments
      // if (fmax(P.x,Q.x) < fmin(S.x,T.x)) {
      //   printf("abcisses?? wtf");
      //   return true; // There is no mutual abcisses
      // }

      //
      // if (A1 == A2) {
      //   printf("same slope");
      //   return false; // same slope = parallel segments
      // }

      // get point of X intersection

      float Xa = (b2 - b1) / (A1 - A2);     // pay attention to not dividing by 0

      // find point of Y intersection
      float Ya1 = A1 * Xa + b1;
      float Ya2 = A2 * Xa + b2;

      // doesn't satisfy both formulas
      // ALSO CHECK : is box outside or inside
      if(Ya1 != Ya2) {
        printf("doesnt satisfy formulas");
        return true;
      }


      // make sure the x intersection is within bounds
      if ( (Xa < fmax( fmin(P.x,Q.x), fmin(S.x,T.x) )) || (Xa > fmin( fmax(P.x,Q.x), fmax(S.x,T.x) )) ) {
        printf("not in bounds");
        return true; // intersection is out of bound
      }
      else {
        return false;
      }

      // all sides of one poly or both have been checked
      if(restartInner == true || restartOuter == true)
        printf("checked all sides no intersect");
        return true;
    }
  }
  return false;
}





// is a circle inside a circle
static bool isCircleInCircle(Circle *outer, Circle *inner) {
  float distance = sqrt (
      pow(outer->center.x - inner->center.x, 2) +
      pow(outer->center.y - inner->center.y, 2));
      return distance <= outer->radius - inner->radius;
}

// is a polygon inside of a circle
static bool isPolygoninCircle(Circle *outer, Polygon *inner) {
  /* check if the distance from any vertex to the center of the circle
  is greater than the radius of the circle */
  for(int i = 0; i < inner->numVertices; i++) {
    float distToVertex = sqrt (
      pow((inner->vertices)[i].x - outer->center.x, 2) +
      pow((inner->vertices)[i].y - outer->center.y, 2));

    // printf("distToVertex: %f vs radius: %f ", distToVertex, outer->radius);

    if(distToVertex > outer->radius) {
      return false;
    }
  }
  // printf("%d: , x:%f, y:%f ", 0, (inner->vertices)[0].x, (inner->vertices)[0].y);
  return true;
}


static bool isCircleInPolygon(Polygon *outer, Circle *inner) {
  if(polyCircleIntersect(inner, outer)) {
    return false;
  }
  else {
    Point centroid = makePoint(0.0, 0.0, 0.0);
    centroid = findCentriod(centroid, outer);

    // printf("height: %f, ", centroid.x);
    // printf("width: %f, ", centroid.y);

    // check if distance between center of circle and poly < dist from poly center to edge
    float distCenters = sqrt (
      pow(centroid.x - inner->center.x, 2) +
      pow(centroid.y - inner->center.y, 2));

    // NEEDS CONDITIONS FOR DIFF POLYGONS
    float halfHeight = abs((outer->vertices)[0].y - (outer->vertices)[1].y) / 2;
    // printf("distbtwn: %f vs half-height: %f ", distCenters, halfHeight);

    return distCenters < halfHeight;
  }
}


/* Check intersection of line segments
 * Source: https://stackoverflow.com/questions/3838329/how-can-i-check-if-two-segments-intersect
 */
static bool isPolygoninPolygon(Polygon *outer, Polygon *inner) {

  return true;
}


/*
 * Move SHAPE to POINT.
 *
 * Return TRUE if SHAPE remains entirely within the space defined by the
 * ARENA parameter of setup(), FALSE otherwise.
 */
bool move(Shape *shape, Point *point) {
  if(shape->type == CIRCLE) {
    Circle *inner = (Circle *)shape;
    inner->center.x = point->x;
    inner->center.y = point->y;

    if(arena->type == CIRCLE) {
      return isCircleInCircle((Circle *)arena, (Circle *)shape);
    }
    else if(arena->type == POLYGON) {
      return isCircleInPolygon((Polygon *)arena, (Circle *)shape);
    }
  }

  else if(shape->type == POLYGON) {
    Polygon *inner = (Polygon *)shape;
    Point centroid = makePoint(0.0, 0.0, 0.0);
    centroid = findCentriod(centroid, (Polygon *)shape);

    // printf(" Current centroid: %f, %f ", centroid.x, centroid.y);

    // find diff between centroid and point
    float xMove = point->x - centroid.x;
    float yMove = point->y - centroid.y;

    // printf(" xMove: %f, yMove: %f ", xMove, yMove);

    // move all vertecies by this vector
    for(int i = 0; i < inner->numVertices; i++) {
      (inner->vertices)[i].x += xMove;
      (inner->vertices)[i].y += yMove;
    }

    if(arena->type == CIRCLE) {
      return isPolygoninCircle((Circle *)arena, inner);
    }
    else if(arena->type == POLYGON) {
      return isPolygoninPolygon((Polygon *)arena, inner);
    }
  }
  return true;
}
