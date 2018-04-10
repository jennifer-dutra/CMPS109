#include <stdio.h>
#include <stdlib.h>

#include "bounds.h"

/*
 * Intialise a two or thre dimensional space of shape ARENA containing NUMSHAPES 
 * SHAPES, all of which may or may not be entirely contained within ARENA.
 */
void setup(Shape *arena, Shape *shapes[], int numShapes) {
    int ununsed; // here to fail warnings test, remove it
}

/*
 * Move SHAPE to POINT. 
 *
 * Return TRUE if SHAPE remains entirely within the space defined by the 
 * ARENA parameter of setup(), FALSE otherwise.
 */
bool move(Shape *shape, Point *point) {
    printf("Not implemented!\n");
    exit(-1);
}
