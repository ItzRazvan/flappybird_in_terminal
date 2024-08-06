#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <termios.h>

#define bird_init_y 14
#define bird_init_x 8

#define num_rows 29
#define num_colums 100

#define lower_column_bound 18
#define upper_column_bound 4
#define y_dist_between_col 8

#define gen_col_interval 25

typedef struct{
    int x;
    int upper_const;
    int upper_y;
    int lower_y;
    int lower_const;
} tower_pair;

typedef struct{
    int x;
    int y;
} Bird;

char empty[num_colums + 2];
char flooor[num_colums + 2];
tower_pair towers[10];
Bird bird;
int tower_number;

char c;


void empty_init(){
    memset(empty, ' ', sizeof(empty) - 2);
    empty[(sizeof(empty)) - 2] = '\n';
    empty[(sizeof(empty)) - 1] = '\0';
}

void floor_init(){
    memset(flooor, '-', sizeof(empty) - 2);
    flooor[(sizeof(empty)) - 2] = '\n';
    flooor[(sizeof(empty)) - 1] = '\0';
}

void bird_init(){
    bird.x = bird_init_x;
    bird.y = bird_init_y;
}

void generate_random_column_coord(int* upper_col, int* lower_col){
    srand(time(NULL));

    int upper_y = rand() % (lower_column_bound - upper_column_bound + 1) + upper_column_bound;

    int lower_y = upper_y + y_dist_between_col + 1;

    *upper_col = upper_y;
    *lower_col = lower_y;
}

void tower_init(int tower_number){
    tower_number--;
    towers[tower_number].x = num_colums;
    generate_random_column_coord(&towers[tower_number].upper_y, &towers[tower_number].lower_y);

    towers[tower_number].upper_const = 1;
    towers[tower_number].lower_const = num_rows-1;
}

void tower_regen(int tower_number){
    towers[tower_number].x = num_colums;
    generate_random_column_coord(&towers[tower_number].upper_y, &towers[tower_number].lower_y);
}


struct termios orig_termios;

void disable_raw_mode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode(){

    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;

    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void gen_and_print_lines(){
    for(int y = 1; y < num_rows; ++y){
        for(int x = 1; x <= num_colums; ++x){
            bool printed = 0;
            if(bird.x == x && bird.y == y){
                printf(">");
                printed = 1;
            }
            for(int i = 0; i < tower_number; ++i){
                if(!printed && towers[i].x == x && ((y >= towers[i].upper_const && y <= towers[i].upper_y) || (y >= towers[i].lower_y && y <= towers[i].lower_const))){
                    printf("|");
                    printed = 1;
                }
            }
            if(!printed)
                printf(" ");
        }
        printf("\n");
    }

    printf("%s\n", flooor);
}



int main() {
    enable_raw_mode();

    empty_init();
    floor_init();
    bird_init();

    tower_number = 1;
    tower_init(tower_number);

    int timer = 0;
    int tower_reuse = 0;

    const int max_nr_towers = (num_colums / gen_col_interval) + 2;

    int score = 0;
    bool pause = 0;

    bool alive = 1;
    while(alive){
        c = '\0';

if(!pause){
        printf("\033[H\033[J");

        if(bird.y >= num_rows || bird.y <= 0)
            alive = 0;
}

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 80000;

        int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if(result > 0){
            read(STDIN_FILENO, &c, 1);
        }

if(!pause){
        timer = (timer + 1) % gen_col_interval;

        if(timer == 0){
            if(tower_number < max_nr_towers){
                tower_number++;
                tower_init(tower_number);
            }
            if(tower_number == max_nr_towers){
                tower_regen(tower_reuse);

                tower_reuse = (tower_reuse + 1) % max_nr_towers;
            } 
        }
        
        for(int i = 0; i < tower_number; ++i)
            towers[i].x--;


       gen_and_print_lines();

        if(c == ' '){
            bird.y-=5;
        }else{    
            bird.y+=1;
        }

        for(int i = 0; i < tower_number; ++i){
            if(towers[i].x == bird.x){
                if((bird.y <= towers[i].upper_y) || (bird.y >= towers[i].lower_y)){
                    alive = 0;
                }else{
                    score++;
                }
            }
        }


        printf("(Pause on ESC, Quit on q)\nYour score is: %d\n", score);
} 
        if(c == 'q'){
            break;
        }else if((int)c == 27 && !pause){
            pause = 1;
        }else if((int)c == 27 && pause){
            pause = 0;
        }

        usleep(80000);
    }

    if(!alive)
        printf("YUU LOST!!!\n");
    else   
        printf("You quit!\n");

    return 0;
} 