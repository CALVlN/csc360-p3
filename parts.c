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

#include <arpa/inet.h>

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
    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        printf("device not opened\n");
        exit(1);
    }

    struct stat fileStats;
    int err = fstat(fd, &fileStats);
    if (err < 0) {
        perror("Error getting file information\n");
        exit(1);
    }

    char* dataPtr = mmap(NULL, fileStats.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (dataPtr == MAP_FAILED) {
        perror("Error mapping file\n");
        exit(1);
    }

    // Extract data from file to block, accounting for endianness
    struct superblock_t block = {};
    char* blockPtr = dataPtr;
    memcpy(&block.fs_id, blockPtr, 8);
    blockPtr += 8;
    memcpy(&block.block_size, blockPtr, 2);
    block.block_size = htons(block.block_size);
    blockPtr += 2;
    memcpy(&block.file_system_block_count, blockPtr, 4);
    block.file_system_block_count = htonl(block.file_system_block_count);
    blockPtr += 4;
    memcpy(&block.fat_start_block, blockPtr, 4);
    block.fat_start_block = htonl(block.fat_start_block);
    blockPtr += 4;
    memcpy(&block.fat_block_count, blockPtr, 4);
    block.fat_block_count = htonl(block.fat_block_count);
    blockPtr += 4;
    memcpy(&block.root_dir_start_block, blockPtr, 4);
    block.root_dir_start_block = htonl(block.root_dir_start_block);
    blockPtr += 4;
    memcpy(&block.root_dir_block_count, blockPtr, 4);
    block.root_dir_block_count = htonl(block.root_dir_block_count);

    char* fatEntryData = dataPtr + block.fat_start_block*block.block_size;
    int freeFatBlocks = 0;
    int reservedFatBlocks = 0;
    int allocatedFatBlocks = 0;

    for (int i = 0; i < block.block_size*block.fat_block_count; i += 4) {
        uint32_t fatData;
        memcpy(&fatData, fatEntryData + i, 4);
        fatData = htonl(fatData);
        if (fatData == 0x00) {
            freeFatBlocks++;
        } else if (fatData == 0x01) {
            reservedFatBlocks++;
        } else if (fatData <= 0xFFFFFF00) {
            allocatedFatBlocks++;
        } else if (fatData == 0xFFFFFFFF) {
            allocatedFatBlocks++;
        }
    }

    printf(
        "\nSuper block information\n"
        "Block size: %d\n"
        "Block count: %d\n"
        "FAT starts: %d\n"
        "FAT blocks: %d\n"
        "Root directory starts: %d\n"
        "Root directory blocks: %d\n"
        "\nFAT information\n"
        "Free blocks: %d\n"
        "Reserved blocks: %d\n"
        "Allocated blocks: %d\n",
        block.block_size,
        block.file_system_block_count,
        block.fat_start_block,
        block.fat_block_count,
        block.root_dir_start_block,
        block.root_dir_block_count,
        freeFatBlocks,
        reservedFatBlocks,
        allocatedFatBlocks
    );

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