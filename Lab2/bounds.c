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
      printf("intersect");
      return false;
    }
    // circle touches line or is outside line
    else if(inner->radius == dist || inner->radius < dist) {
      printf("touch or outside");

      // GET CENTRIOD

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
    Circle *inner = (Circle *)shape;
    inner->center.x = point->x;
    inner->center.y = point->y;
    return isCircleInPolygon((Polygon *)arena, (Circle *)shape);
  }

  return true;
}
