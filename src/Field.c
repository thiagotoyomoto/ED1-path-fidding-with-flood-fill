#include "../include/Field.h"

#include "../include/constants.h"

#include <ncurses.h>
#include <stdlib.h>

Field *Field_create (int width, int height) {
    Field *field = (Field *)malloc (sizeof (Field));

    field->data = (int **)malloc (height * sizeof (int *));
    int i;
    for (i = 0; i < height; ++i)
        field->data[i] = (int *)malloc (width * sizeof (int));

    field->width  = width;
    field->height = height;

    int j;
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            field->data[i][j] = DEFAULT_VALUE;
        }
    }

    return field;
}

void Field_destroy (Field *field) {
    int i;
    for (i = 0; i < field->height; ++i)
        free (field->data[i]);
    free (field->data);
    free (field);
}

void Field_draw (Field *field, Positions *positions) {
    int i, j;
    for (i = 0; i < field->height; ++i) {
        for (j = 0; j < field->width; ++j) {
            Vector2 pos   = { j, i };
            int     value = field->data[j][i];

            bool is_start_pos    = Vector2_equals (pos, positions->start);
            bool is_target_pos   = Vector2_equals (pos, positions->target);
            bool is_selected_pos = Vector2_equals (pos, positions->selected);
            bool is_wall         = value == WALL_VALUE;

            // Default
            char ch    = ' ';
            int  color = DEFAULT_COLOR;

            if (is_wall) {
                color = is_selected_pos ? SELECTED_WALL_COLOR : WALL_COLOR;
                ch    = 'w';
            } else if (is_start_pos) {
                color = is_selected_pos ? SELECTED_START_COLOR : START_COLOR;
                ch    = 's';
            } else if (is_target_pos) {
                color = is_selected_pos ? SELECTED_TARGET_COLOR : TARGET_COLOR;
                ch    = 't';
            } else if (is_selected_pos) {
                color = SELECTED_DEFAULT_COLOR;
            }

            attron (COLOR_PAIR (color));
            // mvprintw(j, i, "%c", ch);
            mvprintw (i, j, "%d", value);
            attroff (COLOR_PAIR (color));
        }
    }
}
