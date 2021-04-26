#include "../include/Vector2Queue.h"
#include <stdlib.h>

Vector2Queue *Vector2Queue_create() {
  Vector2Queue *queue = (Vector2Queue *)malloc(sizeof(Vector2Queue));
  queue->head = queue->tail = NULL;
  return queue;
}

void Vector2Queue_destroy(Vector2Queue *queue) {
  if (queue == NULL)
    return;

  Vector2QueueNode *next = NULL;
  while (queue->head != NULL) {
    next = queue->head->next;
    free(queue->head);
    queue->head = next;
  }
}

bool Vector2Queue_empty(Vector2Queue *queue) {
  return queue == NULL || queue->head == NULL;
}

Vector2 Vector2Queue_front(Vector2Queue *queue) { return queue->head->data; }

void Vector2Queue_enqueue(Vector2Queue *queue, Vector2 data) {
  if (queue == NULL)
    return;

  Vector2QueueNode *new = (Vector2QueueNode *)malloc(sizeof(Vector2QueueNode));
  if (new != NULL) {
    if (Vector2Queue_empty(queue)) {
      queue->head = queue->tail = new;
    } else {
      new->next = NULL;
      new->data = data;
      queue->tail->next = new;
      queue->tail = new;
    }
  }
}

void Vector2Queue_dequeue(Vector2Queue *queue) {
  if (queue == NULL)
    return;

  if (!Vector2Queue_empty(queue)) {
    Vector2QueueNode *next = queue->head->next;
    free(queue->head);
    queue->head = next;
    if (next == NULL)
      queue->tail = NULL;
  }
}
