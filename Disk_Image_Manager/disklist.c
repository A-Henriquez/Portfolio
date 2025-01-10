
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

void printDirectory(struct dir_entry_t* root);
void disklist(int argc, char* argv[], char* addr);

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

    disklist(argc, argv, addr);

    //diskinfo(argc, argv, addr);
    
	close(fh);
        return 0;
}



void disklist(int argc, char* argv[], char* addr) {
	char* targetdir = argv[2];
    	char* tokens[128];
    	int dircount = 0;

    char* args = strtok(argv[2], "/");
	int i = 0;
	while (args) {
		tokens[i++] = args;
		args = strtok(NULL, "/");
	}	
	tokens[i] = NULL;

	while (tokens[dircount] != NULL) {
		dircount++;
	}

    struct superblock_t* sb = (struct superblock_t*) addr;
	int rootStart = ntohl(sb->root_dir_start_block) * htons(sb->block_size);
    struct dir_entry_t* rootBlock;
    int num = 0;
    if (argc == 3 && strcmp(targetdir, "/")) {
		for (int i = 0; i < dircount; i++) {
			for (int j = rootStart; j < rootStart + htons(sb->block_size); j += 64) {
				rootBlock = (struct dir_entry_t*) (addr+j);
				const char* name = (const char*)rootBlock->filename;
				if (!strcmp(name, tokens[num])) {
					num++;
					rootStart = ntohl(rootBlock->starting_block) * htons(sb->block_size);
					break;
				}
			}	
		}
   	}
    
    if (num == dircount || !strcmp(targetdir, "/")) {
    	for (int i = rootStart; i <= rootStart + htons(sb->block_size); i += 64) {
     	rootBlock = (struct dir_entry_t*) (addr+i);
        	if (ntohl(rootBlock->size) == 0) {
	 			continue;      
			}
    		printDirectory(rootBlock);
    	}			
	}    

}



void printDirectory(struct dir_entry_t* root) {
        char* DorF;
        if (root->status == 5){
            DorF = "D";

        }
        else{
            DorF = "F";
        }

        printf("%s %10d %30s %4d/%02d/%02d %02d:%02d:%02d\n",
		DorF, ntohl(root->size), root->filename, htons(root->modify_time.year),
		root->modify_time.month, root->modify_time.day, root->modify_time.hour,
		root->modify_time.minute, root->modify_time.second); 
}

