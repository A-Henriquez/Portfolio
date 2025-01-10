# Train Crossing Simulation

## Project Overview

The project simulates a train crossing system where multiple trains, each with its own loading and crossing times, are managed by a dispatcher thread. The dispatcher ensures trains cross the track in an orderly manner based on their priority and direction. The program reads train data from a file, processes the trains using threads, and outputs the results to a log file. The system demonstrates how multithreading and synchronization can be applied to model real-world concurrent systems. This project was developed for the CSC360 Operating Systems course. 

### Key Features:
- **Multithreading**: Each train operates in its own thread, allowing for concurrent loading and crossing.
- **Dispatcher Thread**: A dispatcher thread manages the order of trains crossing based on their priority and direction.
- **Priority Handling**: Trains are prioritized based on their defined properties, ensuring fair and efficient management of crossings.
- **Output Logging**: Logs every event during the simulation, including when trains enter, load, cross, and exit the system.

### Features and Code Structure:
- **Train Struct**: Represents a train with attributes such as ID, loading time, crossing time, direction, and priority.
- **TrainQueue Struct**: Manages the queue of trains waiting to cross the track.
- **Timer Struct**: Tracks elapsed time during the simulation to ensure proper timing of events.
- **Thread Functions**: 
  - `TrainThreadFunction`: Handles the operation of individual trains.
  - `DispatcherFunction`: Manages the sequence of train crossings based on priority.
- **Utility Functions**: Includes functions for initializing structures, adding trains to the queue, handling synchronization, and formatting time.

### Concurrency Control:
- **Mutexes**: Used to protect shared resources like the track and the train queue to prevent race conditions.
- **Condition Variables**: Used to signal events between threads, such as when a train is ready to load or has finished crossing.

### Technologies Used:
- **C Programming Language**: The simulation is written in C.
- **POSIX Threads (pthreads)**: Used to implement multithreading for concurrent train operations.
- **Mutexes and Condition Variables**: Employed for synchronization to manage race conditions between threads.
