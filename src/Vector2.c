#include "../include/Vector2.h"

bool Vector2_equals(Vector2 a, Vector2 b) { return a.x == b.x && a.y == b.y; }

Vector2 Vector2_add(Vector2 a, Vector2 b) {
  return (Vector2){a.x + b.x, a.y + b.y};
}
