#ifndef VECTOR2_H
#define VECTOR2_H

#include <stdbool.h>

typedef struct Vector2 Vector2;

struct Vector2 {
    int x;
    int y;
};

bool    Vector2_equals (Vector2, Vector2);
Vector2 Vector2_add (Vector2, Vector2);
int     Vector2_sqr_distance (Vector2, Vector2);

#endif /* VECTOR2_H */
