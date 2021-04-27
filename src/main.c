#include "../include/Field.h"
#include "../include/Vector2Queue.h"
#include "../include/constants.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

void flood_fill (Field *, Positions *);
void get_path (Field *, Positions *);

int main (int argc, char *argv[]) {
    Field *   field = Field_create (5, 5);
    Positions pos   = {
        { 0, 0 },                                // Posição inicial
        { field->width - 1, field->height - 1 }, // Posição alvo
        { field->width / 2, field->height / 2 }  // Posição selecionada
    };

    bool run_loop     = true;
    bool was_modified = true;

    initscr ();

    // Inicialização das cores
    start_color ();

    init_pair (DEFAULT_COLOR, COLOR_BLACK, COLOR_BLACK);
    init_pair (START_COLOR, COLOR_GREEN, COLOR_BLACK);
    init_pair (TARGET_COLOR, COLOR_RED, COLOR_BLACK);
    init_pair (WALL_COLOR, COLOR_BLUE, COLOR_BLACK);
    init_pair (PATH_COLOR, COLOR_YELLOW, COLOR_BLACK);
    init_pair (SELECTED_DEFAULT_COLOR, COLOR_BLACK, COLOR_WHITE);
    init_pair (SELECTED_START_COLOR, COLOR_GREEN, COLOR_WHITE);
    init_pair (SELECTED_TARGET_COLOR, COLOR_RED, COLOR_WHITE);
    init_pair (SELECTED_WALL_COLOR, COLOR_BLUE, COLOR_WHITE);
    init_pair (SELECTED_PATH_COLOR, COLOR_YELLOW, COLOR_WHITE);

    cbreak ();
    noecho ();
    keypad (stdscr, true);

    while (run_loop) {
        erase ();
        if (was_modified) {
            flood_fill (field, &pos);
            was_modified = false;
        }

        Field_draw (field, &pos);

#if DEBUG_MODE
        mvprintw (field->height + 1,
                  1,
                  "pos.start     = (%2d; %2d)\n",
                  pos.start.x,
                  pos.start.y);
        mvprintw (field->height + 2,
                  1,
                  "pos.target    = (%2d; %2d)\n",
                  pos.target.x,
                  pos.target.y);
        mvprintw (field->height + 3,
                  1,
                  "pos.selected  = (%2d; %2d)\n",
                  pos.selected.x,
                  pos.selected.y);
#endif

        refresh ();

        int *selected_value = &field->data[pos.selected.y][pos.selected.x];
        bool is_start_pos   = Vector2_equals (pos.selected, pos.start);
        bool is_target_pos  = Vector2_equals (pos.selected, pos.target);
        bool is_wall        = *selected_value == WALL_VALUE;

        switch (getch ()) {
            case KEY_UP: // Selecionar a célula de cima
                if (pos.selected.y > 0)
                    pos.selected.y -= 1;
                break;
            case KEY_LEFT: // Selecionar a célula da esquerda
                if (pos.selected.x > 0)
                    pos.selected.x -= 1;
                break;
            case KEY_DOWN: // Selecionar a célula de baixo
                if (pos.selected.y < field->height - 1)
                    pos.selected.y += 1;
                break;
            case KEY_RIGHT: // Selecionar a célula da direita
                if (pos.selected.x < field->width - 1)
                    pos.selected.x += 1;
                break;
            case 'a': // Mudar a posição inicial
                if (!is_target_pos && !is_wall) {
                    pos.start    = pos.selected;
                    was_modified = true;
                }
                break;
            case 's': // Mudar a posição alvo
                if (!is_start_pos && !is_wall) {
                    pos.target   = pos.selected;
                    was_modified = true;
                }
                break;
            case 'd': { // Adicionar ou remover parede
                if (!is_start_pos && !is_target_pos) {
                    *selected_value = is_wall ? DEFAULT_VALUE : WALL_VALUE;
                    was_modified    = true;
                }
            } break;
            case 'q': // Sair
                run_loop = false;
                break;
        }
    }

    Field_destroy (field);
    endwin ();
    return 0;
}

void flood_fill (Field *field, Positions *pos) {
    Vector2Queue *queue = Vector2Queue_create ();

    if (queue == NULL)
        return;

    int     i, j, k = 0;
    Vector2 offset[4] = { { 0, -1 }, { -1, 0 }, { 0, 1 }, { 1, 0 } };

    // Resetando os valores
    for (i = 0; i < field->height; ++i) {
        for (j = 0; j < field->width; ++j) {
            if (field->data[i][j] != WALL_VALUE)
                field->data[i][j] = DEFAULT_VALUE;
        }
    }

    field->data[pos->start.y][pos->start.x] = 1;
    Vector2Queue_enqueue (queue, pos->start);

    while (!Vector2Queue_empty (queue)) {
        Vector2 curr_pos  = Vector2Queue_front (queue);
        int     new_value = field->data[curr_pos.y][curr_pos.x] + 1;

        for (i = 0; i < 4; ++i) {
            Vector2 neighbour_pos = Vector2_add (curr_pos, offset[i]);
            bool    in_boundary =
                neighbour_pos.x >= 0 && neighbour_pos.x < field->width &&
                neighbour_pos.y >= 0 && neighbour_pos.y < field->height;
            if (in_boundary) {
                int *neighbour_value =
                    &field->data[neighbour_pos.y][neighbour_pos.x];
                bool not_evaluated = *neighbour_value == 0;
                if (not_evaluated) {
                    *neighbour_value = new_value;
                    Vector2Queue_enqueue (queue, neighbour_pos);
                }
            }
        }

        Vector2Queue_dequeue (queue);
    }

    Vector2Queue_destroy (queue);
}

void get_path (Field *field, Positions *pos) {
    Vector2 neighbour_pos = { -1, -1 };
}
