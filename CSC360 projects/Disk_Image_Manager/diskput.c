

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

// Function prototypes
void diskput(int argc, char* argv[], char* addr, struct stat buf);
int numBlocksAllocate(int sizebuf, int sizeblock);
int insertSpaceBlock(struct superblock_t* sb, void* addr, FILE* fp, struct stat buf);
void insertFile(int block_size, void* address, struct stat buf, int startBlock, int fat_start_block, struct superblock_t* sb, char* tokens[]);
int copyFile(int block, int sizeblock, void* addr, struct stat buf, char* filePosition, int sizefile, int blocknum, int firstblock, int firstFATB, int curFAT);
void changeTime(void* addr, int i, struct stat buf);


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

    struct stat buf;
	int f = open(argv[2], O_RDONLY);
	if (fstat(f, &buf) < 0) {
		printf("File not found.\n");
		exit(EXIT_FAILURE);
	}
    
    diskput(argc, argv, addr, buf);

    
	close(fh);
    return 0;
}



void diskput(int argc, char* argv[], char* addr, struct stat buf) {
	char* tokens[128];
    
	char* args = strtok(argv[3], "/");
	int i = 0;
	while (args) {
		tokens[i++] = args;
		args = strtok(NULL, "/");
	}	
	tokens[i] = NULL;

    FILE* fh = fopen(argv[2], "rb");
	if (fh == NULL) {
		return;
	}
    struct superblock_t* sb = (struct superblock_t*) addr;

	int calculatedStartBlocknum = ntohl(sb->fat_start_block) * htons(sb->block_size);
	
	insertFile(htons(sb->block_size), addr, buf, insertSpaceBlock(sb, addr, fh, buf), calculatedStartBlocknum, sb, tokens);
    return;
}


int insertSpaceBlock(struct superblock_t* sb, void* addr, FILE* fp, struct stat buf) {
	
	int blocknum = 0;
	int startBlock = 0;
 	
	int calculatedStartBlocknum = ntohl(sb->fat_start_block) * htons(sb->block_size);
	int calculatedEndBlocknum = ntohl(sb->fat_block_count) * htons(sb->block_size);
	int calcStartandEndNum = calculatedEndBlocknum + calculatedStartBlocknum;
	
	int prevFAT = 0;
	char buffer[512];

    for (int i = calculatedStartBlocknum; i < calcStartandEndNum; i += 4) {
        int count = -1;
    	memcpy(&count, addr + i, 4);
    	count = htonl(count);
    	if (count == 0) {
       		if (blocknum == 0) {

				startBlock = (i - calculatedStartBlocknum) / 4;


               	prevFAT = i;
               	blocknum++;
               	fread(buffer, htons(sb->block_size), 1, fp);
               	memcpy(addr+(startBlock*htons(sb->block_size)), &buffer, htons(sb->block_size));
               	continue;
           	}
        
			int spaceMem = (i - calculatedStartBlocknum) / 4;

           	fread(buffer, htons(sb->block_size), 1, fp);
           	memcpy(addr+(spaceMem*htons(sb->block_size)), &buffer, htons(sb->block_size));

           	int space = htonl(spaceMem);
           	memcpy(addr+prevFAT, &space, 4);
           	prevFAT = i;
           	blocknum++;
			
           	if (blocknum == numBlocksAllocate(buf.st_size, htons(sb->block_size))){
				
            	int hex = 0xFFFFFFFF;
	            memcpy(addr + i, &hex, 4);
               	break;
           	}
        }
    }

    return startBlock;	
}




void insertFile(int block_size, void* address, struct stat buf, int startBlock, int fat_start_block, struct superblock_t* sb, char* tokens[]) {
	
	int calculatedStartBlocknum = ntohl(sb->fat_start_block) * htons(sb->block_size);
	int calculatedEndBlocknum = ntohl(sb->fat_block_count) * htons(sb->block_size);
	int calcStartandEndNum = calculatedEndBlocknum + calculatedStartBlocknum;

	int num = 0;
	int space = 0;

	while (num < 2) {
		if (num == 1) {

            for (int i = space; i < calcStartandEndNum; i += 4) {
				
				int count = -1;
				memcpy(&count, address + i, 4);
				count = htonl(count);
				if (count == 0) {

					int allocate = (i - calculatedStartBlocknum) / 4;
					memcpy(address + space, &allocate, 4);
			
					int hex = 0xFFFFFFFF;
					memcpy(address + i, &hex, 4);

					int root = ntohl(ntohl(sb->root_dir_block_count) + 1);
					memcpy(address+26, &root, 4);
					break;
        		}
    		}
            
		}

		int calcRootBlocknum = ntohl(sb->root_dir_start_block) * htons(sb->block_size);

		int result = copyFile(calcRootBlocknum, htons(sb->block_size), address, buf, tokens[0], 
                     buf.st_size, numBlocksAllocate(buf.st_size, htons(sb->block_size)), 
                     startBlock, calculatedStartBlocknum, space);
		
		if (result) {
   			 return;
		}

		num++;
	}

	exit(EXIT_FAILURE);
}




int copyFile(int block, int sizeblock, void* addr, struct stat buf, char* filePosition, int sizefile, int blocknum, int firstblock, int firstFATB, int curFAT) {
	
	int jump = 3;
   	while(1){
      	for (int i = block; i < block + sizeblock; i += 64) {
         	int count = 0;
         	memcpy(&count, addr + i, 1);
         	if (count == 0){
                memcpy(addr+i, &jump, 1);

                firstblock = ntohl(firstblock);
	            memcpy(addr + (i+1), &firstblock, 4);

                blocknum = ntohl(blocknum);
	            memcpy(addr + (i+5), &blocknum, 4);

                sizefile = ntohl(sizefile);
	            memcpy(addr + (i+9), &sizefile, 4);


                changeTime(addr, i, buf);
                strncpy(addr+i+27, filePosition, 31);

                int hex = 0xFFFFFFFF;
	            memcpy(addr + (i+58), &hex, 4);
                
                hex = 0xFFFF;
                memcpy(addr + (i + 62), &hex, 2);

                return 0;
          	}
        }

		curFAT = (block/sizeblock)*4 + (firstFATB);
		memcpy(&block, addr + curFAT, 4);

		block = htonl(block);
		if (block == -1){ 
			break; 
		}

		block = block * sizeblock;
	}

   return curFAT;
}




int numBlocksAllocate(int sizebuf, int sizeblock) {
    if (sizebuf % sizeblock != 0) {
    return (sizebuf / sizeblock) + 1;
    } else {
        return (sizebuf / sizeblock);
    }
}


void changeTime(void* addr, int i, struct stat buf) {
    char timeStr[20];
    struct tm* timeinfo = localtime(&buf.st_mtime);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
    memcpy(addr + i, timeStr, 20);
}
