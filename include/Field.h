#ifndef FIELD_H
#define FIELD_H

#include "./Positions.h"

typedef struct Field Field;

struct Field {
  int **data;
  int width;
  int height;
};

Field *Field_create(int, int);
void Field_draw(Field *, Positions *);
void Field_reset(Field *);
void Field_destroy(Field *);

#endif /* FIELD_H */
