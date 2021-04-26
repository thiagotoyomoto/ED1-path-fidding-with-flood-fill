#ifndef POSITIONS_H
#define POSITIONS_H

#include "./Vector2.h"
#include "Vector2Queue.h"

typedef struct Positions Positions;

struct Positions {
  Vector2 start;
  Vector2 target;
  Vector2 selected;
  Vector2Queue path;
};

#endif /* POSITIONS_H */
