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

#include<fcntl.h>
#include<sys/types.h>
#include<sys/mman.h>

// Super block
struct __attribute__((__packed__)) superblock_t {
    uint8_t fs_id[8];
    uint16_t block_size;
    uint32_t file_system_block_count;
    uint32_t fat_start_block;
    uint32_t fat_block_count;
    uint32_t root_dir_start_block;
    uint32_t root_dir_block_count;
};

// Time and date entry
struct __attribute__((__packed__)) dir_entry_timedate_t {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

// Directory entry
struct __attribute__((__packed__)) dir_entry_t {
    uint8_t status;
    uint32_t starting_block;
    uint32_t block_count;
    uint32_t size;
    struct dir_entry_timedate_t create_time;
    struct dir_entry_timedate_t modify_time;
    char filename[31];
    uint8_t unused[6];
    uint8_t more_unused;
    uint8_t even_more_unused;
};

int diskinfo(int argc, char* argv[]) {
    printf("arg: %s\n", argv[1]);

    int fd;
    char *data;

    fd = open(argv[1], O_RDWR);

    if (fd < 0) {
        printf("device not opened\n");
        exit(1);
    }

    data = mmap(NULL, 0x100, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    printf("%p\n", data);
    printf("%c\n", data[0]);
    printf("%c\n", data[1]);

    close(fd);


    return 0;
}

int disklist(int argc, char* argv[]) {


    return 0;
}

int diskget(int argc, char* argv[]) {


    return 0;
}

int diskput(int argc, char* argv[]) {


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
    #error "PART[1234] must be defined"
    #endif

    return 0;
}