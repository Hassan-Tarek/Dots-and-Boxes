#include "save.h"

void save_game() {
    FILE* save;
    save = fopen("save.bin", "wb");
    if(save == NULL) {
        fprintf(stderr, "Failed to open file!!\n");
        exit(EXIT_FAILURE);
    }

    fwrite(grid, 1600, 1, save);
    fwrite(&game_type, sizeof(int), 1, save);
    fwrite(&game_mode, sizeof(int), 1, save);
    fwrite(&turn, sizeof(int), 1, save);
    fwrite(&num_of_played_moves, sizeof(int), 1, save);
    fwrite(&num_of_redo, sizeof(int), 1, save);
    fwrite(moves, (total_moves - num_of_played_moves) * sizeof(Move), 1, save);
    fwrite(redo_list, (num_of_redo) * sizeof(Move), 1, save);
    int t = time(NULL) - game_start_time;
    fwrite(&t, sizeof(int), 1, save);
    t = time(NULL) - player_start_time;
    fwrite(&t, sizeof(int), 1, save);
    fwrite(player1, sizeof(struct Player), 1, save);
    fwrite(player2, sizeof(struct Player), 1, save);
    fclose(save);
}

void save_players() {
    FILE* save;
    save = fopen("players.bin", "wb");
    if(save == NULL) {
        fprintf(stderr, "Failed to open file!!\n");
        exit(EXIT_FAILURE);
    }

    fwrite(&num_of_players, sizeof(int), 1, save);
    for(int i = 0; i < num_of_players; ++i) {
        fwrite(player_list[i], sizeof(struct Player), 1, save);
    }
    fclose(save);
}

void save_top_ten() {
    FILE* save;
    save = fopen("top_ten.bin", "wb");
    if(save == NULL) {
        fprintf(stderr, "Failed to open file!!\n");
        exit(EXIT_FAILURE);
    }

    update_top_ten_list();
    int n = min(10, num_of_players);
    fwrite(&n, sizeof(int), 1, save);
    for(int i = 0; i< n; ++i) {
        fwrite(&top_ten[i], sizeof(struct Player), 1, save);
    }
    fclose(save);
}
