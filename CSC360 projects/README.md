# CSC360 Operating Systems Projects

This folder contains three separate projects related to Operating Systems, demonstrating core concepts such as file system management, multithreading, and process management. These projects were developed as part of the CSC360 course and showcase different aspects of operating systems' functionalities and behaviors. Each project focuses on a distinct component of operating system theory and implementation.

### Projects Overview

1. **CSC360 A3 - Disk Image Manager**  
   This project is a tool for managing a custom disk image that implements a simple file system. It allows users to inspect, list, retrieve, and insert files into the disk image. The tool operates on a custom disk structure with a superblock, FAT (File Allocation Table), and directory entries. Users can interact with the disk image using functions that manipulate its structure.

   **Key Features:**
   - Inspect disk image structure
   - List files and directories within the disk image
   - Retrieve and insert files into the disk image

2. **CSC360 A2 - Train Crossing Simulation**  
   This project simulates a train crossing system using multithreading in C. It involves managing multiple trains with their respective loading and crossing times, while a dispatcher controls the order based on priority. This project showcases the use of multithreading to simulate the concurrent operations of train movements, with a focus on synchronization and resource management.

   **Key Features:**
   - Multithreading for concurrent train operations
   - Dispatcher thread managing train priorities and crossing order
   - Output logging of train events

3. **CSC360 A1 - Simple Shell Interpreter**  
   This project implements a simple command-line shell in C. The shell allows users to execute commands both in the foreground and background, navigate directories, and manage running background processes. It demonstrates the basic functionalities of a shell, including process management and directory traversal.

   **Key Features:**
   - Command execution in the foreground and background
   - Directory navigation
   - Listing and managing background processes

### Folder Contents

- **CSC360_A3_Disk_Image_Manager**: Contains the source code for managing a custom disk image and interacting with its file system.
- **CSC360_A2_Train_Crossing_Simulation**: Contains the source code for simulating a train crossing system using multithreading.
- **CSC360_A1_Simple_Shell_Interpreter**: Contains the source code for a simple shell interpreter supporting basic commands and process management.

### Usage

- For **CSC360 A3 - Disk Image Manager**, you can compile and run the code to manage a disk image. It allows for listing directory contents, inspecting the disk structure, retrieving, and inserting files.
  
- For **CSC360 A2 - Train Crossing Simulation**, compile and run the simulation to observe how multiple trains operate concurrently with proper synchronization. You can configure the system with custom input data for train properties.

- For **CSC360 A1 - Simple Shell Interpreter**, compile and run the shell to execute commands like `cd`, `bg`, `bglist`, and `exit`. The shell supports background process management.

Each project demonstrates critical aspects of operating systems, such as process synchronization, file system manipulation, and shell interaction. Make sure to follow the specific README instructions inside each subfolder for compiling and running the respective projects.
