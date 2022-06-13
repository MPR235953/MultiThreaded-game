#include "library.h"

void* routine(void* arg){
    struct server_t* serv = (struct server_t*)arg;

    int x_rand, y_rand;
    do {
        x_rand = rand() % (MAP_X - 2) + 1;
        y_rand = rand() % (MAP_Y - 2) + 1;
    }while(serv->map[y_rand][x_rand] != ' ');

    int i;
    for(i = 0; serv->bis[i].x_curr != 0 && serv->bis[i].y_curr != 0; i++);

    serv->bis[i].x_curr = x_rand;
    serv->bis[i].y_curr = y_rand;
    for (int j = 0; j < MINI_MAP_XY; j++) {
        for (int k = 0; k < MINI_MAP_XY; k++) {
            if (!(serv->bis[i].x_curr - 2 + k < 0 || serv->bis[i].x_curr - 2 + k >= MAP_X || serv->bis[i].y_curr - 2 + j < 0 || serv->bis[i].y_curr - 2 + j >= MAP_Y))
                serv->bis[i].mini_map[j][k] = serv->map[serv->bis[i].y_curr - 2 + j][serv->bis[i].x_curr - 2 + k];
        }
    }
    serv->bis[i].mini_map[2][2] = '*';
    serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';


    int is_dead = 0, move;
    do{
        pthread_mutex_lock(&mutex);
        move = 0;
        for (int j = 0; j < MINI_MAP_XY; j++) {
            for (int k = 0; k < MINI_MAP_XY; k++) {
                if(serv->bis[i].mini_map[j][k] >= '1' && serv->bis[i].mini_map[j][k] <= '4'){
                    if(j == 2 || k == 2){
                        if(j == 2 && k < 2 && !strchr("_#A",serv->bis[i].mini_map[j][1])){
                            serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                            serv->bis[i].x_curr--;
                            serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                            move = 1;
                        }
                        else if(j == 2 && k > 2 && !strchr("_#A",serv->bis[i].mini_map[j][3])){
                            serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                            serv->bis[i].x_curr++;
                            serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                            move = 1;
                        }
                        else if(j < 2 && k == 2 && !strchr("_#A",serv->bis[i].mini_map[1][k])){
                            serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                            serv->bis[i].y_curr--;
                            serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                            move = 1;
                        }
                        else if(j > 2 && k == 2 && !strchr("_#A",serv->bis[i].mini_map[3][k])){
                            serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                            serv->bis[i].y_curr++;
                            serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                            move = 1;
                        }
                        else move = 0;
                    }
                    else{
                        if(j > 2 && k > 2){
                            if(!strchr("_#A",serv->bis[i].mini_map[2][3])){
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                                serv->bis[i].x_curr++;
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                                move = 1;
                            }
                            else if(!strchr("_#A",serv->bis[i].mini_map[3][2])){
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                                serv->bis[i].y_curr++;
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                                move = 1;
                            }
                            else move = 0;
                        }
                        else if(j < 2 && k < 2){
                            if(!strchr("_#A",serv->bis[i].mini_map[1][2] != '_')){
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                                serv->bis[i].y_curr--;
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                                move = 1;
                            }
                            else if(!strchr("_#A",serv->bis[i].mini_map[2][1])){
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                                serv->bis[i].x_curr--;
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                                move = 1;
                            }
                            else move = 0;
                        }
                        else if(j < 2 && k > 2){
                            if(!strchr("_#A",serv->bis[i].mini_map[3][2])){
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                                serv->bis[i].y_curr++;
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                                move = 1;
                            }
                            else if(!strchr("_#A",serv->bis[i].mini_map[1][2])){
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                                serv->bis[i].y_curr--;
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                                move = 1;
                            }
                            else move = 0;
                        }
                        else if(j > 2 && k < 2){
                            if(!strchr("_#A",serv->bis[i].mini_map[2][1])){
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                                serv->bis[i].x_curr--;
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                                move = 1;
                            }
                            else if(!strchr("_#A",serv->bis[i].mini_map[3][2])){
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                                serv->bis[i].y_curr++;
                                serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
                                move = 1;
                            }
                            else move = 0;
                        }
                    }
                    if(move) usleep(700000);
                }
                if(move) break;
            }
            if(move) break;
        }

        serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
        while(!move){
            int dir = rand() % 4;
            switch(dir){
                case 0:{if(!strchr("_#A",serv->bis[i].mini_map[1][2])) serv->bis[i].y_curr--; move = 1; break;}
                case 1:{if(!strchr("_#A",serv->bis[i].mini_map[2][1])) serv->bis[i].x_curr--; move = 1; break;}
                case 2:{if(!strchr("_#A",serv->bis[i].mini_map[3][2])) serv->bis[i].y_curr++; move = 1; break;}
                case 3:{if(!strchr("_#A",serv->bis[i].mini_map[2][3])) serv->bis[i].x_curr++; move = 1; break;}
                default: break;
            }
        }
        serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = '*';
        if(move) usleep(700000);

        for (int j = 0; j < MINI_MAP_XY; j++) {
            for (int k = 0; k < MINI_MAP_XY; k++) {
                if (!(serv->bis[i].x_curr - 2 + k < 0 || serv->bis[i].x_curr - 2 + k >= MAP_X || serv->bis[i].y_curr - 2 + j < 0 || serv->bis[i].y_curr - 2 + j >= MAP_Y))
                    serv->bis[i].mini_map[j][k] = serv->map[serv->bis[i].y_curr - 2 + j][serv->bis[i].x_curr - 2 + k];
            }
        }

        for(int j = 0; j < MAX_PLAYER_NUM; j++){
            if(serv->pls[j].x_curr == serv->bis[i].x_curr && serv->pls[j].y_curr == serv->bis[i].y_curr){
                //serv->map[serv->bis[i].y_curr][serv->bis[i].x_curr] = ' ';
                is_dead = 1;
                //serv->pls[j].sigs.killed = 1;
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
    }while(!is_dead);

    //sleep(5);
    return NULL;
}

//################################################################################################################################################################ BEAST

int main() {
    char* queue_path = "queue";
    int fd_q = shm_open(queue_path, O_CREAT | O_EXCL | O_RDWR, 0666);
    if(errno == EEXIST){
        shm_unlink(queue_path);
        fd_q = shm_open(queue_path, O_CREAT | O_RDWR, 0666);
        if(fd_q == -1) err_show(1, queue_path);
    }
    if (ftruncate(fd_q, sizeof(struct queue_t)) == -1){
        close(fd_q);
        shm_unlink(queue_path);
        err_show(1, queue_path);
    }
    struct queue_t *queue = mmap(NULL, sizeof(*queue), PROT_READ | PROT_WRITE, MAP_SHARED, fd_q, 0);
    if (queue == MAP_FAILED){
        close(fd_q);
        shm_unlink(queue_path);
        err_show(1, queue_path);
    }

    struct shm_t shm[MAX_PLAYER_NUM];
    memset(&shm, 0, sizeof(shm));
    for(int i = 0; i < MAX_PLAYER_NUM; i++){
        strcpy((shm + i)->pl_path, "player");
        *((shm + i)->pl_path + strlen("player")) = '0' + i + 1;
    }

    for(int i = 0; i < MAX_PLAYER_NUM; i++){
        (shm + i)->fd = shm_open((shm + i)->pl_path, O_CREAT | O_EXCL | O_RDWR, 0666);
        if(errno == EEXIST){
            shm_unlink((shm + i)->pl_path);
            (shm + i)->fd = shm_open((shm + i)->pl_path, O_CREAT | O_RDWR, 0666);
            if((shm + i)->fd == -1){
                close_shared_memory(shm);
                return i;
            }
        }
        if (ftruncate((shm + i)->fd, sizeof(struct player_t)) == -1){
            close_shared_memory(shm);
            return i;
        }
        (shm + i)->pl_info = mmap(NULL, sizeof(struct player_t), PROT_READ | PROT_WRITE, MAP_SHARED, (shm + i)->fd, 0);
        if((shm + i)->pl_info == MAP_FAILED){
            close_shared_memory(shm);
            return i;
        }
    }

    memset(queue, 0, sizeof(*queue));

//################################################################################################################################################################ SHARED MEM

    srand(time(NULL));

    sem_init(&queue->serv_sem, 1, 1);
    sem_init(&queue->pl_sem, 1, 0);
    queue->player_id = 1;

    int x_rand, y_rand, start_counting = 0;
    struct server_t serv;
    memset(&serv, 0, sizeof(serv));
    serv.server_pid = getpid();
    shm->pl_info->server_pid = serv.server_pid;


    pthread_t bth[MAX_BEAST_NUM];


    FILE* fp = fopen("map.txt", "r");
    if(!fp){
        printf("Couldn't open file\n");
        close_shared_memory(shm);
        munmap(queue, sizeof(struct queue_t));
        close(fd_q);
        shm_unlink(queue_path);
        return 1;
    }
    for(int i = 0; i < MAP_Y && !feof(fp); i++){
        for(int j = 0; j < MAP_X && !feof(fp); j++) {
            fscanf(fp, "%c", &serv.map[i][j]);
            if(serv.map[i][j] == '\n'){
                serv.map[i][j] = '\0';
                break;
            }
        }
        /*fseek(fp, i * MAP_X, SEEK_SET);
        fscanf(fp, "%51[^\n]s", serv.map[i]);*/
    }
    fclose(fp);


    do {
        x_rand = rand() % (MAP_X - 2) + 1;
        y_rand = rand() % (MAP_Y - 2) + 1;
    }while(serv.map[y_rand][x_rand] != ' ');
    serv.map[y_rand][x_rand] = 'A';


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

    char action;
    do{
        for (int i = 0; i < MAP_Y; i++) {
            for(int j = 0; j < MAP_X; j++) {
                switch(serv.map[i][j]){
                    case '_':{wattron(map_win,COLOR_PAIR(REV_FLOOR_PAIR));mvwprintw(map_win, i, j, &serv.map[i][j]);wattroff(map_win,COLOR_PAIR(REV_FLOOR_PAIR)); break;}
                    case ' ':{wattron(map_win,COLOR_PAIR(SPACE_PAIR));mvwprintw(map_win, i, j, &serv.map[i][j]);wattroff(map_win,COLOR_PAIR(SPACE_PAIR)); break;}
                    case '1': case '2': case '3': case '4':{wattron(map_win,COLOR_PAIR(PLAYER_PAIR));mvwprintw(map_win, i, j, &serv.map[i][j]);wattroff(map_win,COLOR_PAIR(PLAYER_PAIR));break;}
                    case '*':{wattron(map_win,COLOR_PAIR(BEAST_PAIR));mvwprintw(map_win, i, j, &serv.map[i][j]);wattroff(map_win,COLOR_PAIR(BEAST_PAIR));break;}
                    case 'c': case 't': case 'T':{wattron(map_win,COLOR_PAIR(TREASURE_PAIR));mvwprintw(map_win, i, j, &serv.map[i][j]);wattroff(map_win,COLOR_PAIR(TREASURE_PAIR));break;}
                    case 'D':{wattron(map_win,COLOR_PAIR(LOOT_PAIR));mvwprintw(map_win, i, j, &serv.map[i][j]);wattroff(map_win,COLOR_PAIR(LOOT_PAIR));break;}
                    case 'A':{wattron(map_win,COLOR_PAIR(CAMPSITE_PAIR));mvwprintw(map_win, i, j, &serv.map[i][j]);wattroff(map_win,COLOR_PAIR(CAMPSITE_PAIR));break;}
                    case '#':{wattron(map_win,COLOR_PAIR(BUSH_PAIR));mvwprintw(map_win, i, j, &serv.map[i][j]);wattroff(map_win,COLOR_PAIR(BUSH_PAIR));break;}
                    default: break;
                }
            }
        }

        mvwprintw(info_win, 1, 1, "Server's PID %d      ", serv.server_pid);
        mvwprintw(info_win, 2, 1, "Campsite %d/%d      ", serv.x_camp, serv.y_camp);
        mvwprintw(info_win, 3, 1, "Round number: %d      ", serv.round);
        mvwprintw(info_win, 5, 1, "Parameter:  Player1  Player2  Player3  Player4");
        for (int i = 0, sep = 0; i < MAX_PLAYER_NUM; i++, sep += 10) {
            if (i == 0) {
                mvwprintw(info_win, 6, 1, "PID:");
                mvwprintw(info_win, 7, 1, "Type:");
                mvwprintw(info_win, 8, 1, "Curr:");
                mvwprintw(info_win, 9, 1, "Deaths:");
            }
            if ((serv.pls + i)->player_pid == 0) {
                mvwprintw(info_win, 6, 13 + sep - i, "-      ");
                mvwprintw(info_win, 7, 13 + sep - i, "-      ");
                mvwprintw(info_win, 8, 13 + sep - i, "--/--      ");
                mvwprintw(info_win, 9, 13 + sep - i, "-      ");
            } else {
                mvwprintw(info_win, 6, 13 + sep - i, "%d      ", (serv.pls + i)->player_pid);
                if((serv.pls + i)->type == 0) mvwprintw(info_win, 7, 13 + sep - i, "HUMAN      ");
                else mvwprintw(info_win, 7, 13 + sep - i, "CPU      ");
                mvwprintw(info_win, 8, 13 + sep - i, "%d/%d      ", (serv.pls + i)->x_curr, (serv.pls + i)->y_curr);
                mvwprintw(info_win, 9, 13 + sep - i, "%d      ", (serv.pls + i)->deaths);
            }
        }
        mvwprintw(info_win, 11, 1, "Coins");
        for (int i = 0, sep = 0; i < MAX_PLAYER_NUM; i++, sep += 10) {
            if (i == 0) {
                mvwprintw(info_win, 12, 1, "carried:");
                mvwprintw(info_win, 13, 1, "brought:");
            }
            if ((serv.pls + i)->player_pid == 0) {
                mvwprintw(info_win, 12, 13 + sep - i, "-      ");
                mvwprintw(info_win, 13, 13 + sep - i, "-      ");
            } else {
                mvwprintw(info_win, 12, 13 + sep - i, "%d      ", (serv.pls + i)->carried);
                mvwprintw(info_win, 13, 13 + sep - i, "%d      ", (serv.pls + i)->budget);
                wrefresh(info_win);
            }
        }

        mvwprintw(command_win, 1, 1, "COMMANDS:");
        mvwprintw(command_win, 2, 1, "B/b - dodanie jednej bestii w losowym miejscu");
        mvwprintw(command_win, 3, 1, "c/t/T - monety, skarbu, dużego skarbu");
        mvwprintw(command_win, 4, 1, "Q/q - zakonczenie gry.");

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

        halfdelay(DELAY);
        action = wgetch(enter_win);
        switch(action){
            case 'b':
            case 'B':{
                int i;
                for(i = 0; i < MAX_BEAST_NUM && serv.bis[i].x_curr != 0 && serv.bis[i].y_curr != 0; i++);
                if(i == MAX_BEAST_NUM){
                    printf("\ncan't create a beast\n");
                    usleep(600000);
                    wclear(enter_win);
                    break;
                }
                else pthread_create(bth + i, NULL, routine, &serv);
                break;
            }
            case 'c':
            case 't':
            case 'T':{
                do {
                    x_rand = rand() % (MAP_X - 2) + 1;
                    y_rand = rand() % (MAP_Y - 2) + 1;
                }while(serv.map[y_rand][x_rand] != ' ');
                serv.map[y_rand][x_rand] = action;
                break;
            }
            default: break;
        }



        for(int i = 0; i < MAX_PLAYER_NUM; i++){
            //usleep(300000);
            //printf("|%d|\n", (shm + i)->pl_info->sigs.active);
            if((shm + i)->pl_info->sigs.active){
                //printf("here1 %d\n", rand() % 10);
                if(kill((shm + i)->pl_info->player_pid, 0) == -1 && errno == ESRCH){
                    serv.map[serv.pls[i].y_curr][serv.pls[i].x_curr] = ' ';
                    queue->player_id = i + 1;
                    memset((shm + i)->pl_info, 0, sizeof(struct player_t));
                    memset(&serv.pls[i], 0, sizeof(struct player_t));
                    sem_post(&queue->serv_sem);
                    continue;
                }
                sem_wait(&queue->serv_sem);
                //printf("here2 %d\n", rand() % 10);
                if((shm + i)->pl_info->sigs.new){
                    //printf("here3 %d\n", rand() % 10);
                    memset(serv.pls[i].mini_map, '_', sizeof(serv.pls[i].mini_map));
                    serv.pls[i].server_pid = serv.server_pid;
                    serv.pls[i].round = serv.round;
                    serv.pls[i].player_pid = (shm + i)->pl_info->player_pid;
                    serv.pls[i].sigs.active = 1;
                    serv.pls[i].sigs.new = 0;
                    serv.pls[i].player_id = (shm + i)->pl_info->player_id;

                    queue->player_id++;

                    do {
                        x_rand = rand() % (MAP_X - 2) + 1;
                        y_rand = rand() % (MAP_Y - 2) + 1;
                    } while (serv.map[y_rand][x_rand] != ' ');

                    serv.pls[i].x_spawn = x_rand;
                    serv.pls[i].y_spawn = y_rand;
                    serv.pls[i].x_curr = x_rand;
                    serv.pls[i].y_curr = y_rand;
                    for (int j = 0; j < MINI_MAP_XY; j++) {
                        for (int k = 0; k < MINI_MAP_XY; k++) {
                            if (!(serv.pls[i].x_curr - 2 + k < 0 || serv.pls[i].x_curr - 2 + k >= MAP_X || serv.pls[i].y_curr - 2 + j < 0 || serv.pls[i].y_curr - 2 + j >= MAP_Y))
                                serv.pls[i].mini_map[j][k] = serv.map[serv.pls[i].y_curr - 2 + j][serv.pls[i].x_curr - 2 + k];
                        }
                    }
                    serv.pls[i].mini_map[2][2] = '0' + i + 1;
                    serv.map[serv.pls[i].y_curr][serv.pls[i].x_curr] = '0' + i + 1;
                }
                else {
                    serv.pls[i].round = serv.round;
                    int y_curr = serv.pls[i].y_curr;
                    int x_curr = serv.pls[i].x_curr;
                    switch ((shm + i)->pl_info->move) {
                        case KEY_UP: {y_curr--;break;}
                        case KEY_DOWN: {y_curr++;break;}
                        case KEY_LEFT: {x_curr--;break;}
                        case KEY_RIGHT: {x_curr++;break;}
                        default:break;
                    }

                    int flag = 0;
                    if (serv.pls[i].y_prev && serv.pls[i].x_prev && serv.map[y_curr][x_curr] != '_') {
                        if (serv.pls[i].sigs.bush) serv.map[serv.pls[i].y_prev][serv.pls[i].x_prev] = '#';
                        else if (serv.pls[i].sigs.campsite) serv.map[serv.pls[i].y_prev][serv.pls[i].x_prev] = 'A';
                        serv.pls[i].x_prev = 0;
                        serv.pls[i].y_prev = 0;
                        serv.pls[i].sigs.wait = 0;
                        serv.pls[i].sigs.bush = 0;
                        serv.pls[i].sigs.campsite = 0;
                        flag = 1;
                    }

                    if (serv.map[y_curr][x_curr] != '_') {
                        switch (serv.map[y_curr][x_curr]) {
                            case 'c': {serv.pls[i].carried++;break;}
                            case 't': {serv.pls[i].carried += 10;break;}
                            case 'T': {serv.pls[i].carried += 50;break;}
                            case '#': {
                                serv.pls[i].sigs.wait = 1;
                                serv.pls[i].sigs.bush = 1;
                                serv.pls[i].x_prev = x_curr;
                                serv.pls[i].y_prev = y_curr;
                                break;
                            }
                            case 'A': {
                                serv.pls[i].sigs.campsite = 1;
                                serv.pls[i].x_prev = x_curr;
                                serv.pls[i].y_prev = y_curr;
                                serv.pls[i].budget += serv.pls[i].carried;
                                serv.pls[i].carried = 0;
                                break;
                            }
                            case '*': {
                                int j;
                                for(j = 0; j < MAX_BEAST_NUM && x_curr != serv.bis[j].x_curr && y_curr != serv.bis[j].y_curr; j++);
                                pthread_join(*(bth + j), NULL);
                                memset(&serv.bis[j], 0, sizeof(serv.bis[j]));

                                serv.map[y_curr][x_curr] = 'D';

                                for (j = 0; serv.dead_points[j].x != 0; j++);
                                serv.dead_points[j].x = x_curr;
                                serv.dead_points[j].y = y_curr;
                                serv.dead_points[j].value = serv.pls[i].carried;

                                serv.pls[i].carried = 0;
                                x_curr = serv.pls[i].x_spawn;
                                y_curr = serv.pls[i].y_spawn;
                                serv.pls[i].deaths++;

                                //serv.pls[i].sigs.killed = 1;
                                flag = 1;
                                break;
                            }
                            case '1': case '2': case '3': case '4':{
                                if(serv.map[y_curr][x_curr] - '0' == serv.pls[i].player_id) break;
                                //if(!serv.pls[i].sigs.killed){
                                    int id = serv.map[y_curr][x_curr] - '0';

                                    serv.map[y_curr][x_curr] = 'D';
                                    int j;
                                    for (j = 0; serv.dead_points[j].x != 0; j++);
                                    serv.dead_points[j].x = x_curr;
                                    serv.dead_points[j].y = y_curr;
                                    serv.dead_points[j].value = serv.pls[i].carried;

                                    int m;
                                    for(m = 0; m < MAX_PLAYER_NUM; m++){
                                        if(x_curr == serv.pls[m].x_curr && y_curr == serv.pls[m].y_curr && serv.pls[i].player_pid != serv.pls[m].player_pid)
                                            break;
                                    }
                                    serv.dead_points[j].value += serv.pls[m].carried;

                                    serv.pls[i].carried = 0;
                                    x_curr = serv.pls[i].x_spawn;
                                    y_curr = serv.pls[i].y_spawn;
                                    serv.pls[i].deaths++;



                                    serv.pls[m].carried = 0;
                                    serv.pls[m].x_curr = serv.pls[m].x_spawn;
                                    serv.pls[m].y_curr = serv.pls[m].y_spawn;
                                    serv.pls[m].deaths++;

                                    for (j = 0; j < MINI_MAP_XY; j++) {
                                        for (int k = 0; k < MINI_MAP_XY; k++) {
                                            if (!(serv.pls[k].x_curr - 2 + k < 0 || serv.pls[k].x_curr - 2 + k >= MAP_X ||
                                                  serv.pls[k].y_curr - 2 + j < 0 || serv.pls[k].y_curr - 2 + j >= MAP_Y))
                                                serv.pls[k].mini_map[j][k] = serv.map[serv.pls[k].y_curr - 2 + j][
                                                        serv.pls[k].x_curr - 2 + k];
                                        }
                                    }
                                    serv.pls[m].mini_map[2][2] = '0' + id;
                                    serv.map[serv.pls[m].y_curr][serv.pls[m].x_curr] = '0' + id;

                                    memcpy((shm + m)->pl_info, &serv.pls[m], sizeof(struct player_t));
                                    break;
                                //}
                                //break;
                            }
                            case 'D': {
                                int j;
                                for(j = 0; serv.dead_points[j].x != x_curr && serv.dead_points[j].y != y_curr; j++);
                                serv.pls[i].carried += serv.dead_points[j].value;
                                memset(&serv.dead_points[j], 0, sizeof(serv.dead_points[j]));
                            }
                            default: break;
                        }
                        if (!flag) serv.map[serv.pls[i].y_curr][serv.pls[i].x_curr] = ' ';

                        serv.pls[i].x_curr = x_curr;
                        serv.pls[i].y_curr = y_curr;

                        for (int j = 0; j < MINI_MAP_XY; j++) {
                            for (int k = 0; k < MINI_MAP_XY; k++) {
                                if (!(serv.pls[i].x_curr - 2 + k < 0 || serv.pls[i].x_curr - 2 + k >= MAP_X ||
                                      serv.pls[i].y_curr - 2 + j < 0 || serv.pls[i].y_curr - 2 + j >= MAP_Y))
                                    serv.pls[i].mini_map[j][k] = serv.map[serv.pls[i].y_curr - 2 + j][
                                            serv.pls[i].x_curr - 2 + k];
                            }
                        }
                        serv.pls[i].mini_map[2][2] = '0' + i + 1;
                        serv.map[serv.pls[i].y_curr][serv.pls[i].x_curr] = '0' + i + 1;

                        for (int j = 0; j < MAP_Y; j++) {
                            for (int k = 0; k < MAP_X; k++) {
                                if(serv.map[j][k] == '*' && serv.bis[0].x_curr == 0)
                                    serv.map[j][k] = ' ';
                            }
                        }
                    }
                }
                memcpy((shm + i)->pl_info, &serv.pls[i], sizeof(struct player_t));
                start_counting = 1;
                //printf("here4 %d\n", rand() % 10);
                sem_post(&queue->pl_sem);
            }
            else{
                if(i == 0) queue->player_id = 1;
            }
        }


        if(start_counting){
            //usleep(50000);
            serv.round++;
        }

    }while(action != 'q' && action != 'Q');

    endwin();

    close_shared_memory(shm);
    munmap(queue, sizeof(struct queue_t));
    close(fd_q);
    shm_unlink(queue_path);

    return 0;
}
