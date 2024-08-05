#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>

const int bird_init_y = 14;
const int bird_init_x = 8;
const int num_rows = 29;

const int lower_column_bound = 18;
const int upper_column_bound = 10;
const int y_dist_between_col = 8;

const int gen_col_interval = 12;


typedef struct{
    int x;
    int upper_y;
    int lower_y;
} tower_pair;

char empty[100];
char flooor[100];
char bird[100];
tower_pair towers[10];
int bird_position;

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
    bird_position = bird_init_y;
    memset(bird, ' ', sizeof(empty) - 2);
    bird[bird_init_x] = '>';
    bird[(sizeof(empty)) - 2] = '\n';
    bird[(sizeof(empty)) - 1] = '\0';
}

void generate_random_column_coord(int* upper_col, int* lower_col){
    int upper_y = rand() % (lower_column_bound - upper_column_bound + 1) + upper_column_bound;

    int lower_y = upper_y + y_dist_between_col;

    *upper_col = upper_y;
    *lower_col = lower_y;
}

void towers_init(int tower_number){
    tower_number--;
    towers[tower_number].x = 98;
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



int main() {
    enable_raw_mode();

    empty_init();
    floor_init();
    bird_init();

    int tower_number = 1;
    towers_init(tower_number);

    int timer = -1;

    while(1){
        printf("\033[H\033[J");
        c = '\0';

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


        timer = (timer + 1) % gen_col_interval;

        if(timer == 0){
            tower_number++;
            tower_init(tower_number);
        }
        
        for(int i = 0; i < tower_number; ++i)
            towers[i].x--;

        if(bird_position >= num_rows || bird_position < 0)
            break;

        for(int i = 0; i < bird_position; ++i)
            printf("%s", empty);

        printf("%s", bird);

        for(int i = bird_position + 1; i < num_rows; ++i){
            printf("%s", empty); 
        }

        printf("%s", flooor);

        if(c == 'q')
            return 0;

        if(c == ' '){
            bird_position-=5;
        }else{    
            bird_position+=1;
        }

        usleep(80000);
    }

    printf("YUU LOST\n");

    return 0;
} 