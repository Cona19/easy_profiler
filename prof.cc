#include <stdio.h>
#include <unistd.h>
#define __PROF__
#include "prof.hpp"

#define PROF_FORLOOP 0
#define PROF_FUNC1 1
#define PROF_FUNC2 2

PROF_GLOBAL_INIT(
    "for loop", "func1", "func2"
);

void func1(){
    sleep(1);
}

int func2(int s, int e){
    int x = 0;
    for (int i = s; i < e; i++){
        x++;
    }
    return x;
}

int main(){
    int x = 0;
    int y;
    PROF_LOCAL_INIT;
    PROF(PROF_FORLOOP, 
    for (int i = 0; i < 100000000; i++){
        x++;
    });
    printf("%d\n", x);
    for (int i = 0; i < 5; i++){
        PROF(PROF_FUNC1, func1());
        PROF(PROF_FUNC2, y = func2(0, 50000000));
        printf("%d\n", y);
    }
    PROF_PRINT(PROF_FORLOOP, PROF_FUNC2);
    return 0;
}
