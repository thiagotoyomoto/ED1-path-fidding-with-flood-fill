#include "../include/Field.h"
#include "../include/Vector2Queue.h"
#include "../include/constants.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

/* Verifica se n está em [min, max]. */
bool in_range (int n, int min, int max);

/* Verifica se pos.x está em [0, w-1] e verifica se pos.y está em [0, h-1]. */
bool in_boundary (Vector2 pos, int w, int h);

/* Algoritmo que preenche o campo com os valores. */
void flood_fill (Field *, Positions *);

/* Algoritmo que determina o caminho entre o ponto inicial e final. */
void set_path (Field *, Positions *);

int main (int argc, char *argv[]) {
    Field *field = Field_create (40, 20);

    Positions *pos =
        Positions_create ((Vector2) { 0, 0 },
                          (Vector2) { field->width - 1, field->height - 1 },
                          (Vector2) { field->width / 2, field->height / 2 });

    // Flags
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

    curs_set (0);
    cbreak ();
    noecho ();
    keypad (stdscr, true);

    while (run_loop) {
        if (was_modified) {
            flood_fill (field, pos);
            set_path (field, pos);
            was_modified = false;
        }

        erase ();
        Field_draw (field, pos);

        // Imprime as posições ao lado do mapa ---------------------------------
        mvprintw (2, field->width + 4, "Posicoes:");
        mvprintw (3,
                  field->width + 4,
                  "  Inicial     = (%2d; %2d)\n",
                  pos->start.x,
                  pos->start.y);
        mvprintw (4,
                  field->width + 4,
                  "  Final       = (%2d; %2d)\n",
                  pos->target.x,
                  pos->target.y);
        mvprintw (5,
                  field->width + 4,
                  "  Selecionado = (%2d; %2d)\n",
                  pos->selected.x,
                  pos->selected.y);
        // -----------------------------------------------------------

        refresh ();

        int *selected_value = &field->data[pos->selected.y][pos->selected.x];
        bool is_start_pos   = Vector2_equals (pos->selected, pos->start);
        bool is_target_pos  = Vector2_equals (pos->selected, pos->target);
        bool is_wall        = *selected_value == WALL_VALUE;

        switch (getch ()) {
            case KEY_UP: // Selecionar a célula de cima
                if (pos->selected.y > 0)
                    pos->selected.y -= 1;
                break;
            case KEY_LEFT: // Selecionar a célula da esquerda
                if (pos->selected.x > 0)
                    pos->selected.x -= 1;
                break;
            case KEY_DOWN: // Selecionar a célula de baixo
                if (pos->selected.y < field->height - 1)
                    pos->selected.y += 1;
                break;
            case KEY_RIGHT: // Selecionar a célula da direita
                if (pos->selected.x < field->width - 1)
                    pos->selected.x += 1;
                break;
            case 'a': // Mudar a posição inicial
                if (!is_target_pos && !is_wall) {
                    pos->start   = pos->selected;
                    was_modified = true;
                }
                break;
            case 's': // Mudar a posição alvo
                if (!is_start_pos && !is_wall) {
                    pos->target  = pos->selected;
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
    Positions_destroy (pos);

    endwin ();

    return 0;
}

bool in_range (int n, int min, int max) {
    return n >= min && n <= max;
}

bool in_boundary (Vector2 pos, int w, int h) {
    return in_range (pos.x, 0, w - 1) && in_range (pos.y, 0, h - 1);
}

void flood_fill (Field *field, Positions *pos) {
    int i, j;

    Vector2Queue *queue = Vector2Queue_create ();

    // Resetando os valores
    for (i = 0; i < field->height; ++i)
        for (j = 0; j < field->width; ++j)
            if (field->data[i][j] != WALL_VALUE)
                field->data[i][j] = DEFAULT_VALUE;

    field->data[pos->start.y][pos->start.x] = 1;
    Vector2Queue_enqueue (queue, pos->start);

    while (!Vector2Queue_empty (queue)) {
        Vector2 current_position = Vector2Queue_front (queue);
        int new_value = field->data[current_position.y][current_position.x] + 1;

        for (i = 0; i < 4; ++i) {
            // posição do vizinho
            Vector2 neightbour_position =
                Vector2_add (current_position, offset[i]);

            // verifica a posição do vizinho
            if (in_boundary (neightbour_position,
                             field->width,
                             field->height)) {
                // valor do vizinho
                int *neighbour_value =
                    &field->data[neightbour_position.y][neightbour_position.x];

                // se não tiver valor, recebe um novo valor
                if (*neighbour_value == 0) {
                    *neighbour_value = new_value;
                    Vector2Queue_enqueue (queue, neightbour_position);
                }
            }
        }

        Vector2Queue_dequeue (queue);
    }

    Vector2Queue_destroy (queue);
}

void set_path (Field *field, Positions *pos) {
    Vector2 current_position = pos->target;
    Vector2Queue_clear (pos->path);

    bool has_next;
    do {
        has_next = true;
        int i, neightbour_counter = 0;
        int curr_value = field->data[current_position.y][current_position.x];
        Vector2 neighbour_positions[4];
        for (i = 0; i < 4; ++i) {
            Vector2 neighbour_position =
                Vector2_add (current_position, offset[i]);
            if (in_boundary (neighbour_position, field->width, field->height)) {
                int neighbour_value =
                    field->data[neighbour_position.y][neighbour_position.x];
                if (neighbour_value > DEFAULT_VALUE &&
                    neighbour_value < curr_value) {
                    if (Vector2_equals (neighbour_position, pos->start)) {
                        neightbour_counter = 0;
                        break;
                    }
                    neighbour_positions[neightbour_counter++] =
                        neighbour_position;
                }
            }
        }

        bool has_neighbours = neightbour_counter > 0;
        if (has_neighbours) {
            // menor distância
            int shortest_distance =
                Vector2_sqr_distance (neighbour_positions[0], pos->start);
            // índice do menor
            int shortest_index = 0;
            for (i = 1; i < neightbour_counter; ++i) {
                int current_distance =
                    Vector2_sqr_distance (neighbour_positions[i], pos->start);
                if (current_distance < shortest_distance) {
                    shortest_distance = current_distance;
                    shortest_index    = i;
                }
            }
            current_position = neighbour_positions[shortest_index];
            Vector2Queue_enqueue (pos->path, current_position);
        } else {
            has_next = false;
        }
    } while (has_next);
}
