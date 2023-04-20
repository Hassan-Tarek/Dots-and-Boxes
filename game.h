#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>

#define clrscr() \
            system("cls");

typedef enum Game_Type {
    BEGINNER = 1,
    EXPERT
} Game_Type;

typedef enum Game_Mode {
    HUMAN_VS_HUMAN = 1,
    HUMAN_VS_COMPUTER
} Game_Mode;

typedef enum Player_Type {
    HUMAN = 1,
    COMPUTER
} Player_Type;

typedef enum Turn {
    FIRST_PLAYER = 1,
    SECOND_PLAYER
} Turn;

typedef enum Game_Winner {
    PLAYER1 = 1,
    PLAYER2,
    DRAW,
    NONE
} Game_Winner;

typedef enum Colour {
    RED = 1,
    BLUE,
    GREEN,
    CYAN,
    MAGENTA,
    INTENSITY,
    DEFAULT
} Colour;

struct Player {
    char name[15];
    Player_Type type;
    Colour colour;
    int score;
    int rank;
};

struct Move {
   int r1;
   int c1;
   int r2;
   int c2;
};

struct Time {
    int minutes;
    int seconds;
};

typedef struct Player Player;
typedef struct Move Move;
typedef struct Time Time;

int N;
int num_of_players;
Game_Type game_type;
Game_Mode game_mode;
Game_Winner game_winner;
Turn turn;
Player* player1;
Player* player2;
int grid[40][40];
int total_moves;
int num_of_played_moves;
int num_of_redo;
Move moves[100];
Move redo_list[100];
Player* player_list[10000];
Player top_ten[10];
time_t game_start_time;
time_t player_start_time;
_Bool first_move;


void display_menu();
void play_game();
void init(Game_Type type, Game_Mode mode, char player1_name[15], char player2_name[15]);
void reset();
void print_grid();
void play(Player* p);
void get_input(Player* player, char* input);
Move parse_move(char* input);
_Bool is_valid_move(Move move);
_Bool check_box_side(Move move);
int is_making_box(Move move);
void check_for_winner();
void set_colour(Colour colour);
void swap_two_players(Player* player1, Player* player2);
void sort_player_list();
void update_top_ten_list();
void undo();
void redo();
Time passed_time(time_t start);
void exit_game(int s);
void restore_resources();

#endif // GAME_H_
