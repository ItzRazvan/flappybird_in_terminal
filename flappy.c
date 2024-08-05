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

    while(1){
        printf("%s", empty);
        printf("%s", empty);
        printf("%s", empty);

        printf("%s", bird);

        printf("%s", empty);
        printf("%s", empty);
        printf("%s", empty);

        printf("%s", flooor);


        //we ll make it sleep 0.8 seconds, so it looks nice and its updated quite fast
        usleep(80000);
    }
}