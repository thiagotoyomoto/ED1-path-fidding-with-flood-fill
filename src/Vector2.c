#include "../include/Vector2.h"

bool Vector2_equals (Vector2 a, Vector2 b) {
    return (a.x == b.x) && (a.y == b.y);
}

Vector2 Vector2_add (Vector2 a, Vector2 b) {
    return (Vector2) { a.x + b.x, a.y + b.y };
}

int Vector2_sqr_distance (Vector2 a, Vector2 b) {
    int x = b.x - a.x;
    int y = b.y - a.y;
    return (x * x) + (y * y);
}
