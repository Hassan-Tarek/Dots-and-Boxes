#include "computer.h"

void get_remaining_moves(Move_Priority* moves, int* size) {
    for(int i = 1; i <= N; ++i) {
        for(int j = 1; j <= N; ++j) {
            Move m[2];
            m[0] = (Move) {.r1 = i, .c1 = j, .r2 = i, .c2 = j + 1};
            m[1] = (Move) {.r1 = i, .c1 = j, .r2 = i + 1, .c2 = j};

            for(int k = 0; k < 2; ++k) {
                if(is_valid_move(m[k])) {
                    _Bool v1 = 0, v2 = 0, v3 = 0;
                    _Bool v4 = 0, v5 = 0, v6 = 0;

                    int mi = ((m[k].r1 - 1) * N + m[k].c1 - 1);
                    int mj = ((m[k].r2 - 1) * N + m[k].c2 - 1);

                    // if move line is horizontal
                    if(max(mi, mj) - min(mi, mj) == 1) {
                        // check the 3 lines of the top box
                        v1 = check_box_side((Move) {.r1 = m[k].r1, .c1 = m[k].c1, .r2 = m[k].r1 - 1, .c2 = m[k].c1});
                        v2 = check_box_side((Move) {.r1 = m[k].r1 - 1, .c1 = m[k].c1, .r2 = m[k].r1 - 1, .c2 = m[k].c1 + 1});
                        v3 = check_box_side((Move) {.r1 = m[k].r1 - 1, .c1 = m[k].c1 + 1, .r2 = m[k].r1, .c2 = m[k].c1 + 1});

                        // check the 3 lines of the down box
                        v4 = check_box_side((Move) {.r1 = m[k].r1, .c1 = m[k].c1, .r2 = m[k].r1 + 1, .c2 = m[k].c1});
                        v5 = check_box_side((Move) {.r1 = m[k].r1 + 1, .c1 = m[k].c1, .r2 = m[k].r1 + 1, .c2 = m[k].c1 + 1});
                        v6 = check_box_side((Move) {.r1 = m[k].r1 + 1, .c1 = m[k].c1 + 1, .r2 = m[k].r1, .c2 = m[k].c1 + 1});
                    }
                    else {  // if move line is vertical
                        // check the 3 lines of the right box
                        v1 = check_box_side((Move) {.r1 = m[k].r1, .c1 = m[k].c1, .r2 = m[k].r1, .c2 = m[k].c1 + 1});
                        v2 = check_box_side((Move) {.r1 = m[k].r1, .c1 = m[k].c1 + 1, .r2 = m[k].r1 + 1, .c2 = m[k].c1 + 1});
                        v3 = check_box_side((Move) {.r1 = m[k].r1 + 1, .c1 = m[k].c1 + 1, .r2 = m[k].r1 + 1, .c2 = m[k].c1});

                        // check the 3 lines of the left box
                        v4 = check_box_side((Move) {.r1 = m[k].r1, .c1 = m[k].c1, .r2 = m[k].r1, .c2 = m[k].c1 - 1});
                        v5 = check_box_side((Move) {.r1 = m[k].r1, .c1 = m[k].c1 - 1, .r2 = m[k].r1 + 1, .c2 = m[k].c1 - 1});
                        v6 = check_box_side((Move) {.r1 = m[k].r1 + 1, .c1 = m[k].c1 - 1, .r2 = m[k].r1 + 1, .c2 = m[k].c1});
                    }
                    if((v1 && v2 && v3) ||
                       (v4 && v5 && v6)) {
                        moves[(*size)++] = (Move_Priority) {.move = m[k], .priority = 3};
                    }
                    else if(((v1 && v2) || (v2 && v3) || (v3 && v1)) ||
                            ((v4 && v5) || (v5 && v6) || (v6 && v4))) {
                        moves[(*size)++] = (Move_Priority) {.move = m[k], .priority = 0};
                    }
                    else if((v1 || v2 || v3) ||
                            (v4 || v5 || v6)) {
                        moves[(*size)++] = (Move_Priority) {.move = m[k], .priority = 2};
                    }
                    else {
                        moves[(*size)++] = (Move_Priority) {.move = m[k], .priority = 1};
                    }
                }
            }
        }
    }
}

void swap_two_moves(Move_Priority* move1, Move_Priority* move2) {
    Move_Priority* temp = (Move_Priority *) malloc(sizeof(Move_Priority));
    memcpy(temp, move1, sizeof(Move_Priority));
    memcpy(move1, move2, sizeof(Move_Priority));
    memcpy(move2, temp, sizeof(Move_Priority));

    free(temp);
}

void shuffle(Move_Priority* moves, size_t size) {
    srand(time(NULL));
    for(size_t i = 0; i < size; i++) {
        size_t j = rand() % size;
        swap_two_moves(&moves[i], &moves[j]);
    }
}

Move computer_move() {
    Move_Priority* moves;
    moves = (Move_Priority *) malloc(60 * sizeof(Move_Priority));
    int size = 0;
    get_remaining_moves(moves, &size);

    // shuffle array
    shuffle(moves, size);

    Move move;
    int indx = -1;
    int mx = INT_MIN;
    for(int i = 0; i < size; ++i) {
        if(moves[i].priority > mx) {
            indx = i;
            mx = moves[i].priority;
        }
    }
    // copy wanted move
    memcpy(&move, &moves[indx].move, sizeof(Move));
    free(moves);
    return move;
}
