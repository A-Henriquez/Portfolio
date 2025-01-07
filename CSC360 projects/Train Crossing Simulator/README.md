# CSC360 A2

# Train Crossing Simulation

### Overview
This project simulates a train crossing system using multithreading in C. It manages multiple trains, each with its own loading and crossing times, and a dispatcher that ensures trains cross the track in an orderly manner based on priority. The program reads train data from a file, processes the trains using threads, and outputs the results to a file.

### Features
Multithreading: Each train operates in its own thread, allowing concurrent loading and crossing.
Dispatcher: A dispatcher thread manages the order of trains crossing based on priority and direction.
Priority Handling: Trains are prioritized based on their defined properties, ensuring a fair crossing system.
Output Logging: All events are logged to an output file for analysis.
#### Estimated Execution Time
The estimated execution time for the provided test input.txt is approximately 4.2 seconds.

### Code Structure
train struct: Represents a train with attributes such as ID, loading time, crossing time, direction, and priority.
TrainQueue struct: Manages the queue of trains waiting to cross.
Timer struct: Used to track elapsed time during the simulation.
Thread Functions: trainthreadFunction handles train operations, while dispatcher_function manages crossing logic.
Utility Functions: Functions for initializing structures, adding trains to the queue, handling synchronization, and formatting time.
Concurrency Control
Mutexes: Protect shared resources (track_mutex and queue_mutex) to prevent race conditions.
Condition Variables: Used to signal events between threads, such as when a train is ready to load or has finished crossing.
