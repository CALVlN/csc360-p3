#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

int diskinfo(int argc, char** argv) {


    return 0;
}

int disklist(int argc, char** argv) {


    return 0;
}

int diskget(int argc, char** argv) {


    return 0;
}

int diskput(int argc, char** argv) {


    return 0;
}

int main(int argc, char* argv[]) {
    #if defined(PART1)
    diskinfo(argc, argv);
    #elif defined(PART2)
    disklist(argc, argv);
    #elif defined(PART3)
    diskget(argc, argv);
    #elif defined(PART4)
    diskput(argc,argv);
    #else
    # error "PART[1234] must be defined"
    #endif

    return 0;
}