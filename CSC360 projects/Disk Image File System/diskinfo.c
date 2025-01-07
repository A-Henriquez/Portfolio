#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>

struct __attribute__((__packed__)) superblock_t {
    uint8_t fs_id [8];
    uint16_t block_size;
    uint32_t file_system_block_count; 
    uint32_t fat_start_block;
    uint32_t fat_block_count; 
    uint32_t root_dir_start_block;
    uint32_t root_dir_block_count;
};


// Time and Date Entry Struct
struct __attribute__((__packed__)) dir_entry_timedate_t {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};


// Directory Entry Struct
struct __attribute__((__packed__)) dir_entry_t {
    uint8_t status;
    uint32_t starting_block;
    uint32_t block_count;
    uint32_t size;
    struct dir_entry_timedate_t create_time;
    struct dir_entry_timedate_t modify_time;
    uint8_t filename[31];
    uint8_t unused[6];
};


// Function prototypes
void diskinfo(char* addr);
void display_disk_info();



int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <disk_image>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open the disk image file
    int fh = open(argv[1], O_RDWR);
    if (fh == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Get the size of the file
    struct stat buffer;
    if (fstat(fh, &buffer) == -1) {
        perror("Error getting file status");
        close(fh);
        return EXIT_FAILURE;
    }

    // Memory map the disk image
    char* addr = mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fh, 0);
    if (addr == MAP_FAILED) {
        perror("Error mapping file to memory");
        close(fh);
        return EXIT_FAILURE;
    }

    diskinfo(addr);


    munmap(addr, buffer.st_size);
    close(fh);

    return 0;
}


void diskinfo(char* addr) {

    struct superblock_t* superblock = (struct superblock_t*) addr;

    uint16_t block_size = ntohs(superblock->block_size);
    int startOfBlock = ntohl(superblock->fat_start_block) * block_size;
    int endOfBlock = ntohl(superblock->fat_block_count) *block_size;
    int startAndEndSize = startOfBlock + endOfBlock;

    int FATtable[3] = {0};

    // Loop over the FAT entries and classify them
    for (int i = startOfBlock; i < startAndEndSize; i += 4) {
        int temp = 0;
        memcpy(&temp, addr + i, 4); // Read the FAT entry at position i
        uint32_t entry = ntohl(temp); // Convert to host byte order

        // Classify the block type
        if (entry == 0) {
            FATtable[0]++;  // Free block
        } else if (entry == 1) {
            FATtable[1]++;  // Reserved block
        } else {
            FATtable[2]++;  // Allocated block
        }
    }

    // Display the disk information
    display_disk_info(superblock, FATtable);
}


// Function to print the disk information
void display_disk_info(struct superblock_t* superblock, int FATtable[]) {
    // Print other superblock information
    printf("Superblock Information:\n");
    printf("Block size: %u\n", ntohs(superblock->block_size));  // Use ntohl() for 32-bit values
    printf("Block count: %u\n", ntohl(superblock->file_system_block_count));  // Use ntohl()
    printf("FAT starts: %u\n", ntohl(superblock->fat_start_block));  // Use ntohl()
    printf("FAT blocks: %u\n", ntohl(superblock->fat_block_count));  // Use ntohl()
    printf("Root directory starts: %u\n", ntohl(superblock->root_dir_start_block));  // Use ntohl()
    printf("Root directory blocks: %u\n", ntohl(superblock->root_dir_block_count));  // Use ntohl()

    printf("\nFAT Information:\n");
    printf("Free Blocks: %d\n", FATtable[0]);
    printf("Reserved Blocks: %d\n", FATtable[1]);
    printf("Allocated Blocks: %d\n", FATtable[2]);
}

