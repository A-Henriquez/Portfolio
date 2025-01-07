
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



// Superblock Structure
struct __attribute__((__packed__)) superblock_t {
    uint8_t fs_id[8];
    uint16_t block_size;
    uint32_t file_system_block_count;
    uint32_t fat_start_block;
    uint32_t fat_block_count;
    uint32_t root_dir_start_block;
    uint32_t root_dir_block_count;
};

// Time and Date Entry Structure
struct __attribute__((__packed__)) dir_entry_timedate_t {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

// Directory Entry Structure
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

// Function prototype
int htonsblock(int i, void* addr, int size);
int htonlblock(int i, void* addr, int size);
int copyBlockContents(int sizeblock, int blocknum, void* addr, int curBlock, FILE* fp, int startFAT, int curr);
void diskget(int argc, char* argv[], char* addr, struct stat buffer);
void fileAvailable(void* addr, int i, FILE* fp, int sizeblock, int sFAT, int pos);


int main(int argc, char* argv[]) {
	int fh = open(argv[1], O_RDWR);
	struct stat buffer;

	if (fh == -1) {
		printf("Error\n");
		exit(EXIT_FAILURE);
	}
	if (fstat(fh, &buffer) == -1) {
		printf("Error\n");
		exit(EXIT_FAILURE);
	}
	
	char* addr = mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fh, 0);

	if (addr == MAP_FAILED) {
		printf("Error");
		exit(EXIT_FAILURE);
        
	}

    diskget(argc, argv, addr, buffer);

	close(fh);
    return 0;
}



void diskget(int argc, char* argv[], char* addr, struct stat buffer) {
	
	char* tokens[128];
	char* args = strtok(argv[2], "/");
	int i = 0;
	while (args) {
		tokens[i++] = args;
		args = strtok(NULL, "/");
	}	
	tokens[i] = NULL;
	int rootBlock = htonsblock(24, addr, 2) * htonsblock(8, addr, 2);

    while(1){
        for (int i = rootBlock; i < rootBlock + htonsblock(8, addr, 2); i += 64) {
       		int num = 0;
       		memcpy(&num, addr + i, 1);
       		if (num == 3) {
            	char fileName[31];
        		memcpy(&fileName, addr + i + 27, 31);
       			if (!strcmp(fileName, tokens[0])) {
               		FILE* fp = fopen(argv[3], "wb");
					if (fp == NULL) {
						return;
					}
					fileAvailable(addr, i, fp, htonsblock(8, addr, 2), htonsblock(16, addr, 2), num);
                   	munmap(addr, buffer.st_size);
            		fclose(fp);
           			return;
       			}
       		}
    	}

		int sizeblock = htonsblock(8, addr, 2);
		int sFAT = htonsblock(16, addr, 2);
			
		memcpy(&rootBlock, addr + ((rootBlock/sizeblock)*4) + (sFAT*sizeblock), 4);
		rootBlock = htonl(rootBlock);
		if (rootBlock == -1){ 
			break;
		}
		rootBlock = rootBlock * sizeblock;
	}
	//file Not Found - cant find it
	printf("File not found.\n");
	munmap(addr, buffer.st_size);
    return;
}


void fileAvailable(void* addr, int i, FILE* fp, int sizeblock, int sFAT, int pos) {
  	int addrFAT = 0;
	int blocknum = htonlblock(i+5, addr, 4);
	int curBlock = htonlblock(i+1, addr, 4);

	curBlock = copyBlockContents(sizeblock, blocknum, addr, curBlock, fp, sFAT, pos);
    int sizefile = htonlblock(i+9, addr, 4);
    
	int write_size;
	if (sizefile % sizeblock == 0) {
		write_size = sizeblock;
	} else {
		write_size = sizefile % sizeblock;
	}

	fwrite(addr + (sizeblock * curBlock), 1, write_size, fp);
	addrFAT = sFAT * sizeblock + curBlock * 4;
    memcpy(&pos, addr + addrFAT, 4);
}


int copyBlockContents(int sizeblock, int blocknum, void* addr, int curBlock, FILE* fp, int startFAT, int curr) {
	int fat_address = 0;
	for (int i = 0; i < blocknum-1; i++) {
        fwrite(addr + (sizeblock * curBlock), 1, sizeblock, fp);
		fat_address = startFAT * sizeblock + curBlock * 4;
        memcpy(&curr, addr + fat_address, 4);
        curr = htonl(curr);
        curBlock = curr;
    }
	return curBlock;
}


int htonsblock(int i, void* addr, int size) {
	int block = 0;
	memcpy(&block, addr + i, size);
	return htons(block);
}

int htonlblock(int i, void* addr, int size) {
	int block = 0;
	memcpy(&block, addr + i, size);
	return htonl(block);
}