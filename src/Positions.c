#include "../include/Positions.h"

#include <stdlib.h>

Positions *Positions_create (Vector2 start, Vector2 target, Vector2 selected) {
    Positions *positions = (Positions *)malloc (sizeof (Positions));
    positions->start     = start;
    positions->target    = target;
    positions->selected  = selected;
    positions->path      = Vector2Queue_create ();
    return positions;
}

void Positions_destroy (Positions *positions) {
    Vector2Queue_destroy (positions->path);
    free (positions);
}
