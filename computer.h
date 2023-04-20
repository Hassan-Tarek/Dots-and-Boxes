#ifndef COMPUTER_H_
#define COMPUTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "game.h"

typedef struct Move_Priority {
    Move move;
    int priority;
} Move_Priority;

void get_remaining_moves(Move_Priority*, int*);
void swap_two_moves(Move_Priority*, Move_Priority*);
void shuffle(Move_Priority*, size_t);
Move computer_move();

#endif // COMPUTER_H_
