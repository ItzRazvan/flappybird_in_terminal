#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>


char empty[40];
char flooor[40];
char bird[40];

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
    memset(bird, ' ', sizeof(empty) - 2);
    bird[2] = 'x';
    bird[(sizeof(empty)) - 2] = '\n';
    bird[(sizeof(empty)) - 1] = '\0';
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

    int bird_position = 3;

    char c = '\0';

    while(1){
        printf("\033[H\033[J");


        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 600000; // 0.6 seconds

        int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if(result > 0){
            read(STDIN_FILENO, &c, 1);
        }

        if(bird_position == 7)
            break;

        for(int i = 0; i < bird_position; ++i)
            printf("%s", empty);

        printf("%s", bird);

        for(int i = bird_position + 1; i < 7; ++i){
            printf("%s", empty); 
        }

        printf("%s", flooor);

        if(c == 'q')
            return 0;

        if(c == ' ')
            bird_position--;
        else    
            bird_position++;

        c = '\0';

        usleep(400000);
    }

    printf("YUU LOST\n");

    return 0;
} 