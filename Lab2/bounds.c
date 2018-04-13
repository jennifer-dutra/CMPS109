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

static bool polyCircleIntersect(Circle *circle, Polygon *poly, bool circleInside) {
  for(int i = 0; i < poly->numVertices; i++) {
    // two vertices form a line
    Point P = (poly->vertices)[i];
    Point Q = (poly->vertices)[i + 1];

    bool restart = false;
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

    if(circleInside && (circle->radius > dist)) {
      return true;
    }

    if(!circleInside && (dist > circle->radius)) {
      return true;
    }

    if(restart == true) {
      break;
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
  if(polyCircleIntersect(outer,inner, false)) {
    return false;
  }
  else {
    Point centroid = makePoint(0.0, 0.0, 0.0);
    centroid = findCentriod(centroid, inner);

    // printf("height: %f, ", centroid.x);
    // printf("width: %f, ", centroid.y);

    // check if distance between center of circle and poly centroid < radius
    float distCenters = sqrt (
      pow(centroid.x - outer->center.x, 2) +
      pow(centroid.y - outer->center.y, 2));

    // printf("distbtwn: %f vs radius: %f ", distCenters, outer->radius);
    return distCenters < outer->radius;
  }
}

/*
 * Sources:
 * https://www.geeksforgeeks.org/check-line-touches-intersects-circle/
 * https://www.geeksforgeeks.org/program-find-line-passing-2-points/
 *
 */
static bool isCircleInPolygon(Polygon *outer, Circle *inner) {
  if(polyCircleIntersect(inner, outer, true)) {
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

    float halfHeight = abs((outer->vertices)[0].y - (outer->vertices)[1].y) / 2;
    // printf("distbtwn: %f vs half-height: %f ", distCenters, halfHeight);

    return distCenters < halfHeight;
  }
}


/*
 * Move SHAPE to POINT.
 *
 * Return TRUE if SHAPE remains entirely within the space defined by the
 * ARENA parameter of setup(), FALSE otherwise.
 */
bool move(Shape *shape, Point *point) {
  if(arena->type == CIRCLE && shape->type == CIRCLE) {
    Circle *inner = (Circle *)shape;
    inner->center.x = point->x;
    inner->center.y = point->y;
    return isCircleInCircle((Circle *)arena, (Circle *)shape);
  }

  if(arena->type == POLYGON && shape->type == CIRCLE) {
    // calculate centroid here instead
    Circle *inner = (Circle *)shape;
    inner->center.x = point->x;
    inner->center.y = point->y;
    return isCircleInPolygon((Polygon *)arena, (Circle *)shape);
  }

  if(arena->type == CIRCLE && shape->type == POLYGON) {
    Polygon *inner = (Polygon *)shape;
    Circle *outer = (Circle *)arena;
    // printf("RADIUS: %f, ", outer->radius);

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
    return isPolygoninCircle(outer, inner);
  }
  return true;
}
