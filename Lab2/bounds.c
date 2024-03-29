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

    if(circle->radius > dist) {
      return true;
    }
    if(restart == true) {
      break;
    }
  }
  return false;
}


/* Source for onSegment, orientation, & doIntersect:
 * https://www.geeksforgeeks.org/orientation-3-ordered-points/
 *
 */

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r) {
    if (q.x <= fmax(p.x, r.x) && q.x >= fmin(p.x, r.x) &&
        q.y <= fmax(p.y, r.y) && q.y >= fmin(p.y, r.y))
       return true;
    return false;
}


// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
float orientation(Point p, Point q, Point r) {
    float val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2;  // clock or counterclock wise
}


// returns true if line segments intersect
bool doIntersect(Point p1, Point q1, Point p2, Point q2) {
    // Find the four orientations needed for general and
    // special cases
    float o1 = orientation(p1, q1, p2);
    float o2 = orientation(p1, q1, q2);
    float o3 = orientation(p2, q2, p1);
    float o4 = orientation(p2, q2, q1);

    // Special Cases - only vertex is touching
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return false;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return false;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return false;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return false;

    // General case
    if (o1 != o2 && o3 != o4) {
      return true;
    }
    return false; // Doesn't fall in any of the above cases
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

    if(distToVertex > outer->radius) {
      return false;
    }
  }
  return true;
}


static bool isCircleInPolygon(Polygon *outer, Circle *inner) {
  if(polyCircleIntersect(inner, outer)) {
    return false;
  }
  else {
    Point centroid = makePoint(0.0, 0.0, 0.0);
    centroid = findCentriod(centroid, outer);

    if(outer->numVertices == 4) {
      // check if distance between center of circle and poly < dist from poly center to edge
      float distCenters = sqrt (
        pow(centroid.x - inner->center.x, 2) +
        pow(centroid.y - inner->center.y, 2));

      float halfHeight = abs((outer->vertices)[0].y - (outer->vertices)[1].y) / 2;
      return distCenters < halfHeight;
    }
  }
  return true;
}

bool isCircleInReuleux(ReuleauxTriangle *outer, Circle *inner) {

  return true;
}

bool isReuleuxinCircle(Circle *outer, ReuleauxTriangle *inner) {

  return true;
}

/* Check intersection of line segments
 * Source: https://stackoverflow.com/questions/3838329/how-can-i-check-if-two-segments-intersect
 */
static bool isPolygoninPolygon(Polygon *outer, Polygon *inner) {
  /* check if any sides of outer polygon intersect any sides of inner polygon
  (intersection of line segments ) */
  // bool restartOuter = false;
  bool restartInner = false;

  for(int i = 0; i < outer->numVertices ; i++) {
    for(int j = 0; j < inner->numVertices; j++) {

      // two vertices form a line
      Point P = (outer->vertices)[i];
      Point Q = (outer->vertices)[i + 1];

      Point S = (inner->vertices)[j];
      Point T = (inner->vertices)[j + 1];

      if(i == outer->numVertices - 1) {
        // restartOuter = true;
        P = (outer->vertices)[outer->numVertices - 1];
        Q = (outer->vertices)[0];
      }

      if(j == inner->numVertices - 1) {
        restartInner = true;
        S = (inner->vertices)[inner->numVertices - 1];
        T = (inner->vertices)[0];
      }

      if(doIntersect(P, Q, S, T)) {
        return false;
      }
      else if(outer->numVertices == 4) {

        Point outerCenter = makePoint(0.0, 0.0, 0.0);
        outerCenter = findCentriod(outerCenter, outer);

        Point innerCenter = makePoint(0.0, 0.0, 0.0);
        innerCenter = findCentriod(innerCenter, inner);

        // check if distance between center of circle and poly < dist from poly center to edge
        float distCenters = sqrt (
          pow(outerCenter.x - innerCenter.x, 2) +
          pow(outerCenter.y - innerCenter.y, 2));

        float halfHeight = abs((outer->vertices)[0].y - (outer->vertices)[1].y) / 2;

        if(distCenters > halfHeight) {
          return false;
        }
      }

      // all sides of one poly or both have been checked
      if(restartInner == true) {
        restartInner = false;
        break;
      }
    }
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
    if(arena->type == REULEUX_TRIANGLE) {
      return isCircleInReuleux((ReuleauxTriangle *)arena, (Circle *)shape);
    }
  }

  else if(shape->type == POLYGON) {
    Polygon *inner = (Polygon *)shape;
    Point centroid = makePoint(0.0, 0.0, 0.0);
    centroid = findCentriod(centroid, (Polygon *)shape);

    // find diff between centroid and point
    float xMove = point->x - centroid.x;
    float yMove = point->y - centroid.y;

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

  else if(shape->type == REULEUX_TRIANGLE) {
    // ReuleauxTriangle *inner = (ReuleauxTriangle *)shape;
    // Point centroid = makePoint(0.0, 0.0, 0.0);
    // centroid = findCentriod(centroid, (ReuleauxTriangle *shape);
    //
    // // find diff between centroid and point
    // float xMove = point->x - centroid.x;
    // float yMove = point->y - centroid.y;
    //
    // // move all vertecies by this vector
    // for(int i = 0; i < inner->numVertices; i++) {
    //   (inner->vertices)[i].x += xMove;
    //   (inner->vertices)[i].y += yMove;
    // }
    // return isReuleuxinCircle((Circle *)arena, (ReuleauxTriangle *)shape);
  }
  return true;
}
