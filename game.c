#include "game.h"
#include "save.h"
#include "load.h"
#include "computer.h"

void display_menu() {
    // clear screen
    clrscr();
    set_colour(INTENSITY);
    printf("\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\t\t 1- Start New Game\n");
    printf("\t\t\t\t\t\t 2- Load Game\n");
    printf("\t\t\t\t\t\t 3- List Top 10\n");
    printf("\t\t\t\t\t\t 4- Exit\n");

    int option = -1;
    while(option < 1 || option > 4) {   // loop until the user enter the right option
        printf("\t\t\t\t\t\t   Enter Your Choice: ");
        scanf("%d", &option);
        if(option < 1 || option > 4) {
            printf("\t\t\t\t\t\t    try again!!\n");
        }
    }

    if(option == 1) {
        int op1 = -1, op2 = -1;
        char name1[10];
        char name2[10];

        clrscr();       // clear screen
        printf("\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t\t   Choose the Game Type: \n");
        printf("\t\t\t\t\t\t 1- Beginner\n");
        printf("\t\t\t\t\t\t 2- Expert\n");
        while(op1 < 1 || op1 > 2) {
            printf("\t\t\t\t\t\t   Enter Your Choice: ");
            scanf("%d", &op1);
            if(op1 < 1 || op1 > 2) {
                printf("\t\t\t\t\t\t    try again!!\n");
            }
        }

        clrscr();
        printf("\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t\t   Choose The Game Mode: \n");
        printf("\t\t\t\t\t\t 1- Human vs Human\n");
        printf("\t\t\t\t\t\t 2- Human vs Computer\n");
        while(op2 < 1 || op2 > 2) {
            printf("\t\t\t\t\t\t   Enter Your Choice: ");
            scanf("%d", &op2);
            if(op2 < 1 || op2 > 2) {
                printf("\t\t\t\t\t\t    try again!!\n");
            }
        }

        clrscr();
        fflush(stdin);           // flush input buffer to remove any unneeded input
        printf("First Player Name: ");
        scanf("%s", name1);

        if(op2 == 1) {
            printf("Second Player Name: ");
            scanf("%s", name2);
        }
        else {
            strncpy(name2, "Computer", 10);
        }

        // initialize game
        init((Game_Type) op1, (Game_Mode) op2, name1, name2);
    }
    else if(option == 2) {
        load_game();
    }
    else if(option == 3) {
        load_top_ten();
        clrscr();
        set_colour(GREEN);
        for(int i = 0; i < 10; i++) {
            char spaces[16];
            int l = strlen(top_ten[i].name);
            int j = 0;
            while(l < 15) {
                spaces[j++] = ' ';
                l++;
            }
            spaces[15] = '\0';
            l = strlen(top_ten[i].name);
            printf("%2d- Player Name: %.*s %.*s Rank: %d\n", i + 1, l, top_ten[i].name, j, spaces, top_ten[i].rank);
        }
        char input;
        set_colour(MAGENTA);
        while(input != 'R' && input != 'Q' && input != 'r' && input != 'q') {
            printf("\"R\" to Return to Main Menu or \"Q\" to Quit.\n");
            fflush(stdin);
            scanf("%c", &input);
        }
        if(input == 'R' || input == 'r') {
            display_menu();
        }
        else if(input == 'Q' || input == 'q') {
            exit_game(0);
        }
    }
    else {
        exit_game(0);
    }
}

void play_game() {
    display_menu();
    while(game_winner == NONE) {
        print_grid();
        play(turn == FIRST_PLAYER ? player1 : player2);
    }
    print_grid();

    if(game_winner == PLAYER1) {
        ++ (*player1).rank;
        printf("%s won!\n", player1->name);
    }
    else if(game_winner == PLAYER2) {
        ++ (*player2).rank;
        printf("%s won!\n", player2->name);
    }
    else if(game_winner == DRAW) {
        printf("Draw!!\n");
    }

    // save players and top ten
    save_players();
    save_top_ten();

    char input;
    set_colour(MAGENTA);
    while(input != 'R' && input != 'P' && input != 'Q' && input != 'r' && input != 'p' && input != 'q') {
        printf("\"R\" to Return to Main Menu \"P\" to Play Again or \"Q\" to Quit.\n");
        fflush(stdin);
        scanf("%c", &input);
    }
    if (input == 'R' || input == 'r') {
        play_game();
    }
    else if(input == 'p' || input == 'p') {
        reset();
        play_game();
    }
    else if(input == 'Q' || input == 'q') {
        exit_game(0);
    }
}

void init(Game_Type type, Game_Mode mode, char player1_name[15], char player2_name[15]) {
    game_type = type;
    game_mode = mode;
    game_winner = NONE;

    N = game_type == BEGINNER ? 3 : 6;
    total_moves = 2 * N * (N - 1);
    num_of_played_moves = 0;
    num_of_players = 0;
    num_of_redo = 0;

    // assign grid with 0
    for(int i = 0; i < 40; i++) {
        for(int j = 0; j < 40; j++) {
            grid[i][j] = 0;
        }
    }

    // load players
    load_players();
    load_top_ten();

    _Bool f1 = 0, f2 = 0;
    for(int i = 0; i < num_of_players; ++i) {
        if(player_list[i] != NULL) {
            if(strcmp(player1_name, player_list[i]->name) == 0) {
                player1 = player_list[i];
                player1->score = 0;
                f1 = 1;
            }
            if(strcmp(player2_name, player_list[i]->name) == 0) {
                player2 = player_list[i];
                player2->score = 0;
                f2 = 1;
            }
        }
    }


    if(!f1) {
        player1 = (Player *) malloc(sizeof(struct Player));
        strncpy((*player1).name, player1_name, 15);
        player1->score = 0;
        player1->colour = RED;
        player1->rank = 0;
        player1->type = HUMAN;

        player_list[num_of_players++] = (Player *) malloc(sizeof(struct Player));
        player_list[num_of_players - 1] = player1;
    }
    if(!f2) {
        player2 = (Player *) malloc(sizeof(Player));
        strncpy((*player2).name, player2_name, 15);
        player2->score = 0;
        player2->colour = BLUE;
        player2->rank = 0;
        player2->type = game_mode == HUMAN_VS_COMPUTER ? COMPUTER : HUMAN;

        if(player2->type == HUMAN) {
            player_list[num_of_players++] = (Player *) malloc(sizeof(struct Player));
            player_list[num_of_players - 1] = player2;
        }
    }

    turn = FIRST_PLAYER;
    game_start_time = time(NULL);
    player_start_time = time(NULL);
    first_move = 1;
}

void reset() {
    // clear the board
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            grid[i][j] = 0;
        }
    }

    // reset turn
    turn = FIRST_PLAYER;

    // reset total remaining moves
    total_moves = 2 * N * (N - 1);

    // reset players score
    player1->score = 0;
    player2->score = 0;

    // reset time & first_move
    game_start_time = time(NULL);
    player_start_time = time(NULL);
    first_move = 1;
}

void print_grid() {
    clrscr();      // clear screen
    // print players score
    set_colour(RED);
    printf("%s's Score = %d\n", player1->name, player1->score);
    set_colour(BLUE);
    printf("%s's Score = %d\n", player2->name, player2->score);
    set_colour(GREEN);
    printf("Total Number of Remaining Moves: %d", total_moves);

    // print grid
    printf("\n\n\n");
    int c = 0;
    for(int i = 0; i < N; i++) {
        for(int j = c; j < c + N; j++) {
            set_colour(INTENSITY);
            printf("%c", 254);
            if(j + 1 < N * N) {
                if(grid[j][j + 1]) {
                    if(grid[j][j + 1] == 1)
                        set_colour(RED);
                    else
                        set_colour(BLUE);
                    printf(" --- ");
                }
                else {
                    printf("     ");
                }
            }
        }
        printf("\n");
        for(int j = c; j < c + N; j++) {
            if(j + N < N * N) {
                if(grid[j][j + N]) {
                    if(grid[j][j + N] == 1)
                        set_colour(RED);
                    else
                        set_colour(BLUE);
                    printf("|     ");
                }
                else {
                    printf("      ");
                }
            }
        }
        printf("\n");
        c += N;
    }
    printf("\n");

    // print time
    set_colour(GREEN);
    Time t1 = passed_time(game_start_time);
    Time t2 = passed_time(player_start_time);
    printf("Time passed from the start of the game: %d minutes : %d seconds\n", t1.minutes, t1.seconds);
    if(!first_move) {
        printf("%s's time taken: %d minutes : %d seconds\n", turn == FIRST_PLAYER? player2->name : player1->name, t2.minutes, t2.seconds);
    }
}

void play(Player* player) {
    set_colour(INTENSITY);
    printf("Enter a move in the form \"1 1 1 2\" or \"U\" for Undo or \"R\" for Redo or \"S\" to Save \"Q\" to Quit.\n");

    Move m;
    char* input = (char *) malloc(10 * sizeof(char));
    player_start_time = time(NULL);
    if(player->type == HUMAN) {
        get_input(player, input);
        if(strcasecmp(input, "U") == 0) {
            undo();
        }
        else if(strcasecmp(input, "R") == 0) {
            redo();
        }
        else if(strcasecmp(input, "S") == 0){
            save_game();
            save_players();
            save_top_ten();

            char c = ' ';
            while(c != 'Y' && c != 'y' && c != 'N' && c != 'n') {
                printf("Do you wanna continue playing?[Y/N] ");
                fflush(stdin);
                scanf("%c", &c);
            }
            if(c == 'Y' || c == 'y') {
                return;
            }
            else {
                free(input);
                exit_game(0);
            }
        }
        else if(strcasecmp(input, "Q") == 0) {
            free(input);
            exit_game(0);
        }
        m = parse_move(input);
        // free(input);
    }
    else {
        printf("%s's Turn: ", player->name);
        sleep(3);
        m = computer_move();
    }
    int i = ((m.r1 - 1) * N + m.c1 - 1);
    int j = ((m.r2 - 1) * N + m.c2 - 1);
    grid[min(i, j)][max(i, j)] = player->colour;
    total_moves--;
    // if this move doesn't make a box then change the turn
    int num = is_making_box(m);
    if(num == 0) {
        turn = turn == FIRST_PLAYER ? SECOND_PLAYER : FIRST_PLAYER;
    }
    else {
        player->score += num;
    }
    check_for_winner();

    // store move in moves array
    moves[num_of_played_moves++] = m;

    first_move = 0;
    free(input);
}

void get_input(Player* player, char* input) {
    int size = 10;
    _Bool valid = 0;
    while(!valid) {
        printf("%s's Turn: ", player->name);
        fflush(stdin);
        getline(&input, &size, stdin);

        // trim right, left
        char* start = input;
        char* end = input + strlen(input);
        while(--end) {               // trim right
            if(!isspace(*end))
                break;
        }
        *(++end) = '\0';
        while(isspace(*start)) {       // trim left
            start++;
        }
        if(start != end) {
            memmove(input, start, end - start + 1);
        }

        Move m = parse_move(input);
        if(strcasecmp(input, "U") == 0 || strcasecmp(input, "R") == 0 ||
           strcasecmp(input, "S") == 0 || strcasecmp(input, "Q") == 0 ||
           is_valid_move(m)) {
            valid = 1;
        }
        if(!valid) {
            printf("try again!!\n");
        }
    }
}

Move parse_move(char* input) {
    Move m;
    char* copy = (char *) malloc(10 * sizeof(char));
    strcpy(copy, input);

    // split string by space
    char* s = strtok(copy, " ");
    int i = 0;
    int arr[10];
    while(s != NULL) {
        arr[i++] = atoi(s);
        s = strtok(NULL, " ");
    }
    if(i != 4) {
        return m;
    }
    m.r1 = arr[0], m.c1 = arr[1], m.r2 = arr[2], m.c2 = arr[3];

    // free memory
    free(copy);

    // return move
    return m;
}

_Bool is_valid_move(Move move) {
    _Bool valid = 0;
    if((move.r1 != (int) NULL && move.c1 != (int) NULL && move.r2 != (int) NULL && move.c2 != (int) NULL) &&
       (move.r1 >= 1 && move.r1 <= N) && (move.r2 >= 1 && move.r2 <= N) &&
       (move.c1 >= 1 && move.c1 <= N) && (move.c2 >= 1 && move.c2 <= N)) {

        int i = ((move.r1 - 1) * N + move.c1 - 1);
        int j = ((move.r2 - 1) * N + move.c2 - 1);
        if((max(i, j) - min(i, j) == 1 || max(i, j) - min(i, j) == N) && grid[min(i, j)][max(i, j)] == 0) {
            valid = 1;
        }
    }
    return valid;
}

_Bool check_box_side(Move move) {
    _Bool valid = 0;
    if(move.c2 <= N + 1 && move.c2 >= 1 && move.r2 <= N + 1 && move.r2 >= 1) {
        int i = ((move.r1 - 1) * N + move.c1 - 1);
        int j = ((move.r2 - 1) * N + move.c2 - 1);
        valid = grid[min(i, j)][max(i, j)] != 0;
    }
    return valid;
}

int is_making_box(Move move) {
    int i = ((move.r1 - 1) * N + move.c1 - 1);
    int j = ((move.r2 - 1) * N + move.c2 - 1);

    // if the move is vertical move
    if(max(i, j) - min(i, j) == N) {
        // check left box
        _Bool flag1 =
                    check_box_side((Move) {.r1 = move.r1, .c1 = move.c1, .r2 = move.r1, .c2 = move.c1 + 1}) &&
                    check_box_side((Move) {.r1 = move.r1, .c1 = move.c1 + 1, .r2 = move.r2, .c2 = move.c1 + 1}) &&
                    check_box_side((Move) {.r1 = move.r2, .c1 = move.c1 + 1, .r2 = move.r2, .c2 = move.c2});

        // check right box
        _Bool flag2 =
                    check_box_side((Move) {.r1 = move.r1, .c1 = move.c1, .r2 = move.r1, .c2 = move.c1 - 1}) &&
                    check_box_side((Move) {.r1 = move.r1, .c1 = move.c1 - 1, .r2 = move.r2, .c2 = move.c1 - 1}) &&
                    check_box_side((Move) {.r1 = move.r2, .c1 = move.c1 - 1, .r2 = move.r2, .c2 = move.c2});

        if(flag1 && flag2)
            return 2;
        if(flag1 || flag2)
            return 1;
        else
            return 0;
    }
    else { // if the move is horizontal one
        // check top box
        _Bool flag1 =
                    check_box_side((Move) {.r1 = move.r1, .c1 = move.c1, .r2 = move.r1 + 1, .c2 = move.c1}) &&
                    check_box_side((Move) {.r1 = move.r1 + 1, .c1 = move.c1, .r2 = move.r1 + 1, .c2 = move.c2}) &&
                    check_box_side((Move) {.r1 = move.r1 + 1, .c1 = move.c2, .r2 = move.r2, .c2 = move.c2});

        // check down box
        _Bool flag2 =
                    check_box_side((Move) {.r1 = move.r1, .c1 = move.c1, .r2 = move.r1 - 1, .c2 = move.c1}) &&
                    check_box_side((Move) {.r1 = move.r1 - 1, .c1 = move.c1, .r2 = move.r1 - 1, .c2 = move.c2}) &&
                    check_box_side((Move) {.r1 = move.r1 - 1, .c1 = move.c2, .r2 = move.r2, .c2 = move.c2});

        if(flag1 && flag2)
            return 2;
        if(flag1 || flag2)
            return 1;
        else
            return 0;
    }
}

void check_for_winner() {
    if(total_moves == 0) {
        if(player1->score > player2->score)
            game_winner = PLAYER1;
        else if(player2->score > player1->score)
            game_winner = PLAYER2;
        else
            game_winner = DRAW;
    }
}

void set_colour(Colour colour) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    switch(colour) {
    case RED:
        SetConsoleTextAttribute(console, FOREGROUND_RED);
        break;
    case BLUE:
        SetConsoleTextAttribute(console, FOREGROUND_BLUE);
        break;
    case GREEN:
        SetConsoleTextAttribute(console, FOREGROUND_GREEN);
        break;
    case CYAN:
        SetConsoleTextAttribute(console, 11);
        break;
    case MAGENTA:
        SetConsoleTextAttribute(console, 13);
        break;
    case INTENSITY:
        SetConsoleTextAttribute(console, FOREGROUND_INTENSITY);
        break;
    default:
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    }
}

void swap_two_players(Player* player1, Player* player2) {
    Player* temp = (Player *) malloc(sizeof(Player));
    memcpy(temp, player1, sizeof(Player));
    memcpy(player1, player2, sizeof(Player));
    memcpy(player2, temp, sizeof(Player));

    free(temp);
}

void sort_player_list() {
    // sort players list based on their score
    for(int i = 0; i < num_of_players; i++) {
        int mx = player_list[i]->rank;
        int index = i;
        for(int j = i + 1; j < num_of_players; j++) {
            if(player_list[j]->rank > mx) {
                mx = player_list[j]->rank;
                index = j;
            }
        }
        swap_two_players(player_list[i], player_list[index]);
    }
}

void update_top_ten_list() {
    sort_player_list();
    for(int i = 0; i < min(num_of_players, 10); i++) {
        top_ten[i] = *player_list[i];
    }
}

void undo() {
    if(num_of_played_moves > 0) {
        Move move = moves[--num_of_played_moves];

        int i = ((move.r1 - 1) * N + move.c1 - 1);
        int j = ((move.r2 - 1) * N + move.c2 - 1);

        // check if this move make or not
        if(is_making_box(move)) {
            if(turn == FIRST_PLAYER) {
                -- (*player1).score;
            }
            else if(turn == SECOND_PLAYER) {
                -- (*player2).score;
            }
        }
        else {
            turn = turn == FIRST_PLAYER ? SECOND_PLAYER : FIRST_PLAYER;
        }

        total_moves++;
        grid[min(i, j)][max(i, j)] = 0;

        // save move in redo list
        redo_list[num_of_redo++] = move;
    }
    else {
        printf("You have to make a move first!\n");
        sleep(2);
    }
}

void redo() {
    if(num_of_redo > 0) {
        Move move = redo_list[--num_of_redo];

        // put move in moves array
        moves[num_of_played_moves ++] = move;

        int i = ((move.r1 - 1) * N + move.c1 - 1);
        int j = ((move.r2 - 1) * N + move.c2 - 1);

        if(turn == FIRST_PLAYER) {
            grid[min(i,j)][max(i, j)] = player1->colour;
            if(is_making_box(move)) {
                ++ (*player1).score;
            }
            else {
                turn = SECOND_PLAYER;
            }
        }
        else if(turn == SECOND_PLAYER) {
            grid[min(i,j)][max(i, j)] = player1->colour;
            if(is_making_box(move)) {
                ++ (*player2).score;
            }
            else {
                turn = FIRST_PLAYER;
            }
        }

        total_moves--;
    }
    else {
        printf("The redo list is empty!\n");
        sleep(2);
    }
}

Time passed_time(time_t start) {
    time_t end = time(NULL);

    Time t;
    int diff = end - start;
    t.minutes = diff / 60;
    t.seconds = diff % 60;

    return t;
}

void exit_game(int s) {
    restore_resources();
    exit(s);
}

void restore_resources() {
    // free the memory
    for(int i = 0; i < num_of_players; ++i) {
        free(player_list[i]);
    }
    free(player1);
    free(player2);
}
