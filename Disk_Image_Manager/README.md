# Disk Image Manager

## Project Overview

This tool is designed to manage a disk image that implements a custom file system. The program allows users to inspect, list, retrieve, and insert files into the disk image. It operates on a custom disk structure, which includes components like the superblock, FAT (File Allocation Table), and directory entries. This project provides insights into file system management, focusing on disk structures and their manipulation. This project was developed for the CSC360 Operating Systems course. 

### Key Features:
- **Inspect Disk Image**: View information about the disk structure, including the superblock, FAT, and root directory.
- **List Directory Contents**: Display files and directories within the disk image.
- **Retrieve Files**: Extract a file from the disk image and save it locally.
- **Insert Files**: Add new files into the disk image, updating the FAT and directory entries.

### Functions:

- **diskinfo**: 
  - Processes the disk image to retrieve superblock information and classifies FAT entries (free, reserved, or allocated blocks).
  - Displays this information to the user, offering insights into the disk's structure and status.
  
- **display_disk_info**: 
  - Displays detailed information about the superblock and the FAT, such as block size, block count, and the status of FAT blocks (free, reserved, or allocated).

- **disklist**:
  - Lists the contents of a directory within the disk image. It takes a directory path as an argument, displaying metadata for each file or directory, such as file size, modification time, etc.

- **diskget**:
  - Retrieves a file from the disk image by locating it within the directory structure and then writing the file's contents to a local file.

- **diskput**:
  - Inserts a new file into the disk image by writing the file contents from the local machine, updating the FAT and directory entries as necessary.

### Technologies Used:
- **C Programming Language**: The tool is implemented in C.
- **Custom Disk Image Structure**: The disk image consists of a superblock, FAT, and directory entries, all of which are manipulated by the tool.
