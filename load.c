#include "load.h"

void load_game() {
    FILE* load;
    load = fopen("save.bin", "rb");
    if(load == NULL) {
        fprintf(stderr, "Failed to open file!!\n");
        exit(EXIT_FAILURE);
    }

    fread(grid, 1600, 1, load);
    fread(&game_type, sizeof(int), 1, load);
    N = game_type == BEGINNER ? 3 : 6;
    total_moves = 2 * N * (N - 1);
    fread(&game_mode, sizeof(int), 1, load);
    fread(&turn, sizeof(int), 1, load);
    fread(&num_of_played_moves, sizeof(int), 1, load);
    total_moves -= num_of_played_moves;
    fread(&num_of_redo, sizeof(int), 1, load);
    fread(moves, (total_moves - num_of_played_moves) * sizeof(Move), 1, load);
    fread(redo_list, (num_of_redo) * sizeof(Move), 1, load);
    int t;
    fread(&t, sizeof(int), 1, load);
    game_start_time = time(NULL) - t;
    fread(&t, sizeof(int), 1, load);
    player_start_time = time(NULL) - t;
    fread(&player1, sizeof(struct Player), 1, load);
    fread(&player2, sizeof(struct Player), 1, load);
    game_winner = NONE;
    first_move = num_of_played_moves == 0;
    fclose(load);

}

void load_players() {
    FILE* load;
    load = fopen("players.bin", "rb");
    if(load == NULL) {
        return;
    }

    fread(&num_of_players, sizeof(int), 1, load);
    for(int i = 0; i < num_of_players; ++i) {
        player_list[i] = (Player *) malloc(sizeof(Player));
        fread(player_list[i], sizeof(struct Player), 1, load);
    }
    fclose(load);
}

void load_top_ten() {
    FILE* load;
    load = fopen("top_ten.bin", "rb");
    if(load == NULL) {
        return;
    }

    int n;
    fread(&n, sizeof(int), 1, load);
    for(int i = 0; i < n; ++i) {
        fread(&top_ten[i], sizeof(struct Player), 1, load);
    }
    fclose(load);
}
