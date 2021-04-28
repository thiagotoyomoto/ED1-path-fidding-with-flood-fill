#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "./Vector2.h"

// Se quiser verificar os valores no mapa, defina esta constante como 1.
#define DEBUG_MODE 0

#define DEFAULT_VALUE 0
#define WALL_VALUE    -1

#define DEFAULT_COLOR          1
#define START_COLOR            2
#define TARGET_COLOR           3
#define WALL_COLOR             4
#define PATH_COLOR             5
#define SELECTED_DEFAULT_COLOR 6
#define SELECTED_START_COLOR   7
#define SELECTED_TARGET_COLOR  8
#define SELECTED_WALL_COLOR    9
#define SELECTED_PATH_COLOR    10

static Vector2 offset[4] = { { 0, -1 }, { -1, 0 }, { 0, 1 }, { 1, 0 } };

#endif /* CONSTANTS_H */
