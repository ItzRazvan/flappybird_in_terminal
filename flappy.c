#include <stdio.h>
#include <unistd.h>
#include <string.h>


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

int main() {
    empty_init();
    floor_init();
    bird_init();

    int bird_position = 3;

    while(1){
        printf("\033[H\033[J");

        if(bird_position == 7)
            break;

        for(int i = 0; i < bird_position; ++i)
            printf("%s", empty);

        printf("%s", bird);

        for(int i = bird_position + 1; i < 7; ++i){
            printf("%s", empty); 
        }

        printf("%s", flooor);


        //here we will check if the space key is hit


        bird_position++;


        //we ll make it sleep 0.8 seconds, so it looks nice and its updated quite fast
        sleep(1);
    }

    printf("YUU LOST\n");

    return 0;
} 