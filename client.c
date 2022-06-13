#include "library.h"

int main() {
    char *queue_path = "queue";
    int fd_q = shm_open(queue_path, O_RDWR, 0666);
    if (fd_q == -1) err_show(2, "Server not found");
    if (ftruncate(fd_q, sizeof(struct queue_t)) == -1) err_show(1, queue_path);
    struct queue_t *queue = mmap(NULL, sizeof(*queue), PROT_READ | PROT_WRITE, MAP_SHARED, fd_q, 0);
    if (queue == MAP_FAILED) err_show(1, queue_path);

    int fd;
    switch(queue->player_id){
        case 0: {printf("No server found\n"); return 1;}
        case 1:{fd = shm_open("player1", O_RDWR, 0666);if (fd == -1) err_show(2, "FULL, player1");break;}
        case 2:{fd = shm_open("player2", O_RDWR, 0666);if (fd == -1) err_show(2, "FULL, player2");break;}
        case 3:{fd = shm_open("player3", O_RDWR, 0666);if (fd == -1) err_show(2, "FULL, player3");break;}
        case 4:{fd = shm_open("player4", O_RDWR, 0666);if (fd == -1) err_show(2, "FULL, player4");break;}
        default: {printf("Server is full\n"); return 1;}
    }
    if (ftruncate(fd, sizeof(struct player_t)) == -1) err_show(1, "player");
    struct player_t* pl = mmap(NULL, sizeof(*pl), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pl == MAP_FAILED) err_show(1, "player");

    if(kill(pl->server_pid, 0) == -1 && errno == ESRCH){
        printf("No server found\n");
        return 1;
    }
    //printf("here1 %d\n", rand() % 10);
    pl->player_pid = getpid();
    pl->sigs.new = 1;
    pl->sigs.active = 1;
    pl->player_id = queue->player_id;

    initscr();

    start_color();
    init_pair(REV_FLOOR_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(SPACE_PAIR, COLOR_BLACK, COLOR_BLACK);
    init_pair(PLAYER_PAIR, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(BEAST_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(TREASURE_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(LOOT_PAIR, COLOR_GREEN, COLOR_YELLOW);
    init_pair(CAMPSITE_PAIR, COLOR_YELLOW, COLOR_GREEN);
    init_pair(BUSH_PAIR, COLOR_WHITE, COLOR_BLACK);

    WINDOW* map_win = newwin(MAP_Y, MAP_X, 0,0);
    WINDOW* info_win = newwin(MAP_Y - 10, MAP_X, 0, MAP_X + 10);
    WINDOW* command_win = newwin(MAP_Y - 19, MAP_X, 15, MAP_X + 10);
    WINDOW* enter_win = newwin(MAP_Y - 20, MAP_X, 21, MAP_X + 10);
    refresh();

    int action;
    do {
        //printf("here2 %d\n", rand() % 10);
        if(kill(pl->server_pid, 0) == -1 && errno == ESRCH){
            endwin();
            printf("Disconected\n");
            sem_post(&queue->pl_sem);
            return 2;
        }
        sem_wait(&queue->pl_sem);
        //printf("here3 %d\n", rand() % 10);
        for (int i = 0; i < MINI_MAP_XY; i++){
            for (int j = 0; j < MINI_MAP_XY; j++){
                //usleep(300000);
                //printf("%c", pl->mini_map[i][j]);
                //mvwprintw(map_win, i, j, &pl->mini_map[i][j]);
                switch(pl->mini_map[i][j]){
                    case '_':{wattron(map_win,COLOR_PAIR(REV_FLOOR_PAIR));mvwprintw(map_win, i, j, &pl->mini_map[i][j]);wattroff(map_win,COLOR_PAIR(REV_FLOOR_PAIR)); break;}
                    case ' ':{wattron(map_win,COLOR_PAIR(SPACE_PAIR));mvwprintw(map_win, i, j, &pl->mini_map[i][j]);wattroff(map_win,COLOR_PAIR(SPACE_PAIR)); break;}
                    case '1': case '2': case '3': case '4':{wattron(map_win,COLOR_PAIR(PLAYER_PAIR));mvwprintw(map_win, i, j, &pl->mini_map[i][j]);wattroff(map_win,COLOR_PAIR(PLAYER_PAIR));break;}
                    case '*':{wattron(map_win,COLOR_PAIR(BEAST_PAIR));mvwprintw(map_win, i, j, &pl->mini_map[i][j]);wattroff(map_win,COLOR_PAIR(BEAST_PAIR));break;}
                    case 'c': case 't': case 'T':{wattron(map_win,COLOR_PAIR(TREASURE_PAIR));mvwprintw(map_win, i, j, &pl->mini_map[i][j]);wattroff(map_win,COLOR_PAIR(TREASURE_PAIR));break;}
                    case 'D':{wattron(map_win,COLOR_PAIR(LOOT_PAIR));mvwprintw(map_win, i, j, &pl->mini_map[i][j]);wattroff(map_win,COLOR_PAIR(LOOT_PAIR));break;}
                    case 'A':{wattron(map_win,COLOR_PAIR(CAMPSITE_PAIR));mvwprintw(map_win, i, j, &pl->mini_map[i][j]);wattroff(map_win,COLOR_PAIR(CAMPSITE_PAIR));break;}
                    case '#':{wattron(map_win,COLOR_PAIR(BUSH_PAIR));mvwprintw(map_win, i, j, &pl->mini_map[i][j]);wattroff(map_win,COLOR_PAIR(BUSH_PAIR));break;}
                    default: break;
                }
            }
            mvwprintw(map_win, i, 5, "                                                  ");
        }

        mvwprintw(info_win, 1, 1, "Server's PID %d", pl->server_pid);
        mvwprintw(info_win, 2, 1, "Campsite unknown");
        mvwprintw(info_win, 3, 1, "Round number: %d", pl->round);
        mvwprintw(info_win, 5, 1, "Player:");
        mvwprintw(info_win, 6, 1, "Number: %d", pl->player_id);
        if (pl->type == 0) mvwprintw(info_win, 7, 1, "Type: HUMAN");
        else mvwprintw(info_win, 7, 1, "Type: CPU");
        mvwprintw(info_win, 8, 1, "Curr x/y: %d/%d      ", pl->x_curr, pl->y_curr);
        mvwprintw(info_win, 9, 1, "Deaths: %d      ", pl->deaths);
        mvwprintw(info_win, 10, 1, "Coins found: %d      ", pl->carried);
        mvwprintw(info_win, 11, 1, "Coins brought: %d      ", pl->budget);

        mvwprintw(command_win, 1, 1, "COMMANDS:");
        mvwprintw(command_win, 2, 1, "KEY_UP / KEY_DOWN / KEY_LEFT / KEY_RIGHT");
        mvwprintw(command_win, 3, 1, "Q/q - quit");

        mvwprintw(enter_win, 1, 1, "Enter command");
        mvwprintw(enter_win, 2, 1, ": ");

        box(info_win, 0, 0);
        box(command_win, 0, 0);
        box(enter_win, 0, 0);
        wrefresh(map_win);
        wrefresh(info_win);
        wrefresh(command_win);
        wrefresh(enter_win);
        refresh();

        if(pl->sigs.wait) usleep(1000000);

        keypad(enter_win, 1);

        halfdelay(DELAY);
        action = wgetch(enter_win);
        pl->move = action;
        //pl->sigs.new = 0;

        sem_post(&queue->serv_sem);
        //printf("here4 %d\n", rand() % 10);

    }while(action != 'q' && action != 'Q');

    //pl->sigs.active = 0;
    endwin();

    return 0;
}
