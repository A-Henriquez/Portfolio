# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

EXEC_DISKINFO = diskinfo
EXEC_DISKLIST = disklist
EXEC_DISKPUT = diskput
EXEC_DISKGET = diskget

SRC_DISKINFO = diskinfo.c
SRC_DISKLIST = disklist.c
SRC_DISKPUT = diskput.c
SRC_DISKGET = diskget.c


OBJ_DISKINFO = $(SRC_DISKINFO:.c=.o)
OBJ_DISKLIST = $(SRC_DISKLIST:.c=.o)
OBJ_DISKPUT = $(SRC_DISKPUT:.c=.o)
OBJ_DISKGET = $(SRC_DISKGET:.c=.o)



all: $(EXEC_DISKINFO) $(EXEC_DISKLIST) $(EXEC_DISKPUT) $(EXEC_DISKGET)


$(EXEC_DISKINFO): $(OBJ_DISKINFO)
	$(CC) $(OBJ_DISKINFO) -o $(EXEC_DISKINFO)

$(EXEC_DISKLIST): $(OBJ_DISKLIST)
	$(CC) $(OBJ_DISKLIST) -o $(EXEC_DISKLIST)

$(EXEC_DISKPUT): $(OBJ_DISKPUT)
	$(CC) $(OBJ_DISKPUT) -o $(EXEC_DISKPUT)

$(EXEC_DISKGET): $(OBJ_DISKGET)
	$(CC) $(OBJ_DISKGET) -o $(EXEC_DISKGET)


# Rule to compile .c files into .o object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executables
clean:
	rm -f $(OBJ_DISKINFO) $(OBJ_DISKLIST) $(OBJ_DISKPUT) $(OBJ_DISKGET)
	rm -f $(EXEC_DISKINFO) $(EXEC_DISKLIST) $(EXEC_DISKPUT) $(EXEC_DISKGET)

# Phony targets (not actual files)
.PHONY: all clean
