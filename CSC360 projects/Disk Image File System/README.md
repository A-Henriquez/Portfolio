### CSC360 A3

Overview

This project is a tool for managing a disk image that implements a custom file system. The code allows users to inspect, list, retrieve, and insert files into the disk image. The program operates on a custom disk structure that includes superblock information, FAT (File Allocation Table), and directory entries. The disk image can be opened, analyzed, and manipulated with a variety of functions provided by the tool.

Key Features

Inspect Disk Image: View basic information about the disk structure (superblock, FAT, and root directory).
List Directory Contents: List files and directories in a given path within the disk image.
Retrieve Files: Retrieve a file from the disk image and save it locally.
Insert Files: Insert new files into the disk image and update the relevant FAT and directory entries.

Functions

diskinfo

This function processes the disk image, retrieves superblock information, and classifies FAT entries (free, reserved, or allocated blocks). It then displays this information to the user.

display_disk_info

This function displays the superblock and FAT information, such as block size, block count, and the status of the FAT blocks (free, reserved, or allocated).

disklist

This function lists the contents of a directory in the disk image. The directory path is specified as a command-line argument. It handles both files and directories and displays their metadata (e.g., file size, modification time, etc.).


diskget

This function allows the user to retrieve a file from the disk image. It locates the file by traversing the directory structure and then writes the file contents to a local file.

diskput

This function inserts a new file into the disk image. It writes the contents of a local file into the image, updating the FAT and directory entries as necessary.

