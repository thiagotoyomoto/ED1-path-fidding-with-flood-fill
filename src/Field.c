#include "../include/Field.h"

#include "../include/constants.h"

#include <ncurses.h>
#include <stdlib.h>

Field *Field_create (int width, int height) {
    int    i, j;
    Field *field = (Field *)malloc (sizeof (Field));

    // Tamanho mínimo
    if (width < 5)
        width = 5;
    if (height < 5)
        height = 5;

    field->data = (int **)malloc (height * sizeof (int *));
    for (i = 0; i < height; ++i)
        field->data[i] = (int *)malloc (width * sizeof (int));

    field->width  = width;
    field->height = height;

    for (i = 0; i < height; ++i)
        for (j = 0; j < width; ++j)
            field->data[i][j] = DEFAULT_VALUE;

    return field;
}

void Field_destroy (Field *field) {
    int i;
    for (i = 0; i < field->height; ++i)
        free (field->data[i]);
    free (field->data);
    free (field);
}

void Field_draw (Field *field, Positions *pos) {
    int i, j;

    mvprintw (1, 1, "o");
    mvprintw (1, field->width + 2, "o");
    mvprintw (field->height + 2, 1, "o");
    mvprintw (field->height + 2, field->width + 2, "o");
    for (i = 0; i < field->width; ++i) {
        mvprintw (1, 2 + i, "-");
        mvprintw (field->height + 2, 2 + i, "-");
    }
    for (i = 0; i < field->height; ++i) {
        mvprintw (2 + i, 1, "|");
        mvprintw (2 + i, field->width + 2, "|");
    }

    for (i = 0; i < field->height; ++i) {
        for (j = 0; j < field->width; ++j) {
            Vector2 curr_pos = { j, i };
            int     value    = field->data[i][j];

            bool is_start_pos    = Vector2_equals (curr_pos, pos->start);
            bool is_target_pos   = Vector2_equals (curr_pos, pos->target);
            bool is_selected_pos = Vector2_equals (curr_pos, pos->selected);
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
#if !DEBUG_MODE
            mvprintw (i + 2, j + 2, "%c", ch);
#else
            mvprintw (i + 2, j + 2, "%d", value);
#endif
            attroff (COLOR_PAIR (color));
        }
    }

    Vector2Queue *temp = Vector2Queue_create ();
    while (!Vector2Queue_empty (pos->path)) {
        Vector2 curr_pos = Vector2Queue_front (pos->path);

        bool is_selected_pos = Vector2_equals (curr_pos, pos->selected);
        int  color = is_selected_pos ? SELECTED_PATH_COLOR : PATH_COLOR;

        attron (COLOR_PAIR (color));
#if !DEBUG_MODE
        mvprintw (2 + curr_pos.y, 2 + curr_pos.x, "p");
#else
        mvprintw (2 + curr_pos.y,
                  2 + curr_pos.x,
                  "%d",
                  field->data[curr_pos.y][curr_pos.x]);
#endif
        attroff (COLOR_PAIR (color));

        Vector2Queue_enqueue (temp, curr_pos);
        Vector2Queue_dequeue (pos->path);
    }
    while (!Vector2Queue_empty (temp)) {
        Vector2Queue_enqueue (pos->path, Vector2Queue_front (temp));
        Vector2Queue_dequeue (temp);
    }
}
