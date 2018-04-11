#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bounds.h"

/*
 * Intialise a two or thre dimensional space of shape ARENA containing NUMSHAPES
 * SHAPES, all of which may or may not be entirely contained within ARENA.
 */

static Shape *arena;

void setup(Shape *a, Shape *shapes[], int numShapes) {
  arena = a;
}

static bool isCircleInsideCircle(Circle *outer, Circle *inner) {
  float distance = sqrt (
      pow(outer->center.x - inner->center.x,2) +
      pow(outer->center.y - inner->center.y,2));
      return distance <= outer->radius - inner->radius;
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
    return isCircleInsideCircle((Circle *)arena, (Circle *)shape );
  }
  return true;
}
