#ifndef GAME_FINAL_LIBRARY_H
#define GAME_FINAL_LIBRARY_H

#include <stdio.h>
#include <ncurses.h>
#include <curses.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#define MAP_X 52
#define MAP_Y 25
#define MINI_MAP_XY 5
#define MAX_PLAYER_NUM 2
#define MAX_BEAST_NUM 1
#define MAX_DEAD_POINTS_NUM 100
#define DELAY 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

enum pair_t{
    REV_FLOOR_PAIR = 1,
    SPACE_PAIR,
    PLAYER_PAIR,
    BEAST_PAIR,
    TREASURE_PAIR,
    LOOT_PAIR,
    CAMPSITE_PAIR,
    BUSH_PAIR,
};

struct queue_t{
    int player_id;
    sem_t serv_sem;
    sem_t pl_sem;
};

struct signals_t{
    int new;
    int active;
    int wait;
    int bush;
    int campsite;
    int killed;
};

struct player_t{
    struct signals_t sigs;
    int server_pid;
    int player_pid;
    int player_id;
    char mini_map[MINI_MAP_XY][MINI_MAP_XY];
    int x_curr;
    int y_curr;
    int x_prev;
    int y_prev;
    int x_spawn;
    int y_spawn;
    int move;
    int type;        //0 - user, 1 - CPU
    int carried;
    int budget;
    int is_dead;
    int deaths;
    int round;
};

struct beasts_t{
    int x_curr;
    int y_curr;
    int x_prev;
    int y_prev;
    char mini_map[MINI_MAP_XY][MINI_MAP_XY];
};

struct loot_t{
    int x;
    int y;
    int value;
};

struct server_t{
    int server_pid;
    char map[MAP_Y][MAP_X];
    int round;
    int x_camp;
    int y_camp;
    struct loot_t dead_points[MAX_DEAD_POINTS_NUM];
    struct player_t pls[MAX_PLAYER_NUM];
    struct beasts_t bis[MAX_BEAST_NUM];
};

struct shm_t{
    char pl_path[8];
    int fd;
    struct player_t* pl_info;
};

void close_shared_memory(struct shm_t* shm){
    if(!shm) return;
    for(int i = 0; i < MAX_PLAYER_NUM; i++){
        munmap(&(shm + i)->pl_info, sizeof(struct player_t));
        close((shm + i)->fd);
        shm_unlink((shm + i)->pl_path);
    }
}

void err_show(int err_code, const char* msg) {
    if(err_code == 1) {
        printf("errcode: %d\n", err_code);
        perror(msg);
    }
    else printf("%s\n", msg);
    exit(err_code);
}

#endif //GAME_FINAL_LIBRARY_H
