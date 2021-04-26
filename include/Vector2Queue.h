#ifndef VECTOR2QUEUE_H
#define VECTOR2QUEUE_H

#include "./Vector2.h"

typedef struct Vector2QueueNode Vector2QueueNode;
typedef struct Vector2Queue Vector2Queue;

struct Vector2QueueNode {
  Vector2QueueNode *next;
  Vector2 data;
};

struct Vector2Queue {
  Vector2QueueNode *head;
  Vector2QueueNode *tail;
};

Vector2Queue *Vector2Queue_create();
void Vector2Queue_destroy(Vector2Queue *);
bool Vector2Queue_empty(Vector2Queue *);
Vector2 Vector2Queue_front(Vector2Queue *);
void Vector2Queue_enqueue(Vector2Queue *, Vector2);
void Vector2Queue_dequeue(Vector2Queue *);

#endif /* VECTOR2QUEUE_H */
