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

static bool isCircleInCircle(Circle *outer, Circle *inner) {
  float distance = sqrt (
      pow(outer->center.x - inner->center.x, 2) +
      pow(outer->center.y - inner->center.y, 2));
      return distance <= outer->radius - inner->radius;
}

static bool isPolygoninCircle(Circle *outer, Polygon *inner) {
  for(int i = 0; i < inner->numVertices; i++) {
    float distToVertex = sqrt (
      pow((inner->vertices)[i].x - outer->center.x, 2) +
      pow((inner->vertices)[i].y - outer->center.y, 2));

    printf("distToVertex: %f vs radius: %f ", distToVertex, outer->radius);

    if(distToVertex >= outer->radius) {
      return false;
    }
  }
  // printf("%d: , x:%f, y:%f ", 0, (inner->vertices)[0].x, (inner->vertices)[0].y);
  return true;
}

/*
 * Sources:
 * https://www.geeksforgeeks.org/check-line-touches-intersects-circle/
 * https://www.geeksforgeeks.org/program-find-line-passing-2-points/
 *
 */
static bool isCircleInPolygon(Polygon *outer, Circle *inner) {
  for(int i = 0; i < outer->numVertices; i++) {
    // two vertices form a line
    Point P = (outer->vertices)[i];
    Point Q = (outer->vertices)[i + 1];

    bool restart = false;
    if(i == outer->numVertices - 1) {
      restart = true;
      P = (outer->vertices)[outer->numVertices - 1];
      Q = (outer->vertices)[0];
    }

    // equation for a line is ax + by + c = 0
    float a = Q.y - P.y;
    float b = P.x - Q.x;
    float c = (a * P.x) + (b * P.y);

    // center of circle
    float x = inner->center.x;
    float y = inner->center.y;

    // check: does circle intersect side of polygon (line)
    int dist = (abs(a * x + b * y + c)) / sqrt(a * a + b * b);

    // circle intersects line
    if(inner->radius > dist) {
      return false;
    }
    // circle touches line or is outside line
    else if(inner->radius == dist || inner->radius < dist) {

      // GET CENTRIOD
      float centX = ((outer->vertices)[0].y + (outer->vertices)[1].y) / 2;
      float centY = ((outer->vertices)[0].x + (outer->vertices)[3].x) / 2;

      printf("height: %f, ", centY);
      printf("width: %f, ", centX);

      // check if distance between center of circle and poly < radius
      float distCenters = sqrt (
        pow(centX - inner->center.x, 2) +
        pow(centY - inner->center.y, 2));
      float halfHeight = abs((outer->vertices)[0].y - (outer->vertices)[1].y) / 2;
      printf("distbtwn: %f vs half-height: %f ", distCenters, halfHeight);
      return distCenters < halfHeight;
    }

    // all sides of poly have been checked
    if(restart == true) {
      break;
    }
    // printf("vertex [%d]: %f, %f", i, (outer->vertices)[i].x, (outer->vertices)[i].y);  // TESTING
  }
  return true;
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
    printf("RADIUS: %f, ", outer->radius);

    // calc current centroid
    // Source: https://stackoverflow.com/questions/19766485/how-to-calculate-centroid-of-polygon-in-c

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

    // calculate the centroid coordinates
    cx = cy = 0.0;
    i1 = 1;
    for(int i = 0; i < inner->numVertices; i++) {
      t = (inner->vertices)[i].x * (inner->vertices)[i1].y - (inner->vertices)[i1].x * (inner->vertices)[i].y;
      cx += ((inner->vertices)[i].x + (inner->vertices)[i1].x) * t;
      cy += ((inner->vertices)[i].y + (inner->vertices)[i1].y) * t;
      i1 = (i1 + 1) % inner->numVertices;
    }
    cx = cx / (6.0 * a);
    cy = cy / (6.0 * a);

    printf(" Current centroid: %f, %f ", cx, cy);

    // find diff between centroid and point
    float xMove = point->x - cx;
    float yMove = point->y - cy;

    printf(" xMove: %f, yMove: %f ", xMove, yMove);

    // move all vertecies by this vector
    for(int i = 0; i < inner->numVertices; i++) {
      (inner->vertices)[i].x += xMove;
      (inner->vertices)[i].y += yMove;

      // printf("%d: , x:%f, y:%f ", i, (inner->vertices)[i].x, (inner->vertices)[i].y);  // TESTING
    }
    return isPolygoninCircle(outer, inner);
  }
  return true;
}
