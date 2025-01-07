#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

#define MAX_TRAINS 100


// trains objects
typedef struct {
    int numID;
    int crossingTime;
    int loadingTime;
    char direction;
    int priority;
} train;

// global variables
train trains[MAX_TRAINS];
int trainCount = 0;
int trainsdoneloading = 0;
int trainscrossed = 0; 

//mutexes
pthread_mutex_t track_mutex;
pthread_mutex_t queue_mutex;

//con vars
pthread_cond_t startcrossing[MAX_TRAINS];
pthread_cond_t donecrossing[MAX_TRAINS];
pthread_cond_t train_start_to_load;
pthread_cond_t train_done_load;

// thread
pthread_t train_threads[MAX_TRAINS]; 
pthread_t dispatcher_thread; 

// trainqueue struct
typedef struct {
    train trains[MAX_TRAINS];
    int size;
    char last_dispatched_direction[10];
} TrainQueue;

// timer struct
typedef struct {
    struct timeval start_time;
} Timer;


// function prototypes
void getTrainData(char *file);
void createTrainThreads();
void* trainthreadFunction(void* arg);
void createDispatcherThread(); 
void* dispatcher_function(void* arg);
void startLoadingtrains();
void destroyThreads();
void joinThreads();
void sleepfor(int time); 
void initializeQueues();
void addTrainToqueue(train* t);
void signalTrainReady(int trainID, char dir);
train crossingLogic();
void init_queue(TrainQueue* queue);
void add_train(TrainQueue* queue, train* train);
int compare_trains(const void* a, const void* b);
int countSamePriorityTrains(TrainQueue* queue, train dispatched_train);
train selectTrainFromCandidates(TrainQueue* queue, train dispatched_train, int count_same_priority);
void removeDispatchedTrain(TrainQueue* queue, int trainID);
void updateLastDispatchedDirection(TrainQueue* queue, train dispatched_train);
void timeformatting(int tenths, char *buffer, size_t bufferSize);
void startTimer(Timer *timer);
int getElapsedTenths(Timer *timer);


TrainQueue queue;
Timer timer;

int main(int argc, char* argv[]) {
    startTimer(&timer);

    // Redirect stdout to output.txt
    FILE *outputFile = freopen("output.txt", "w", stdout);

    if (argc < 2) {
        return EXIT_FAILURE;
    }

    init_queue(&queue);

    // Initialize mutexes
    pthread_mutex_init(&track_mutex, NULL);
    pthread_mutex_init(&queue_mutex, NULL);

    // Initialize condition variable
    pthread_cond_init(&train_start_to_load, NULL);
    pthread_cond_init(&train_done_load, NULL);

    // Load input
    getTrainData(argv[1]);


    // Create train threads
    createTrainThreads();

    // Create dispatcher thread
    createDispatcherThread(); 

    // Join threads
    joinThreads();

    // Destroy threads and resources
    destroyThreads();

    // Close the output file
    fclose(outputFile);
    
    return EXIT_SUCCESS;
}




void* trainthreadFunction(void* arg){
    // Implementation removed
    
    train* t = (train*)arg; // Cast argument to train pointer
    
	// printf("Train %d start of thread\n", t->numID);
    char timeString[20]; // Buffer to hold the formatted time string

	pthread_mutex_lock(&queue_mutex);
	pthread_cond_wait(&train_start_to_load, &queue_mutex); // wait on convar
	pthread_mutex_unlock(&queue_mutex);

	// waits for loading
	char* dir = (t->direction == 'W' || t->direction == 'w') ? "West" : "East";
    // timestamp when finished loading 
    sleepfor(t->loadingTime);
    int tenths = getElapsedTenths(&timer);
    timeformatting(tenths, timeString, sizeof(timeString));
    printf("%s Train %2d is ready to go %s\n", timeString, t->numID, dir);
	
	// add train to queue station
    add_train(&queue, t);
    trainsdoneloading++;

    pthread_cond_broadcast(&train_done_load);


    // wait for signal to cross
    pthread_mutex_lock(&track_mutex);
	pthread_cond_wait(&startcrossing[t->numID], &track_mutex );
    pthread_mutex_unlock(&track_mutex);
    

    // wait for crossing
    tenths = getElapsedTenths(&timer);
    timeformatting(tenths, timeString, sizeof(timeString));
    printf("%s Train %2d is ON the main track going %4s\n", timeString, t->numID, dir );
    sleepfor(t->crossingTime);
    tenths = getElapsedTenths(&timer);
    timeformatting(tenths, timeString, sizeof(timeString));
    printf("%s Train %2d is OFF the main track after going %4s\n",timeString, t->numID, dir );
    trainscrossed++;

    // signal track is free to dispatcher
    pthread_cond_broadcast(&donecrossing[t->numID]);

    // exit thread
    pthread_exit(NULL); 


}


void* dispatcher_function(void* arg){
    // Implementation removed

    // printf("start of dispatcher thread\n");
    startLoadingtrains();

	// printf("start waiting for loading\n");
	// Wait for all trains to finish loading
	pthread_mutex_lock(&track_mutex);
	pthread_cond_wait(&train_done_load, &track_mutex);
	pthread_mutex_unlock(&track_mutex);

    // crossinglogic needs to return train or numid

    while(trainscrossed < trainCount){
        if(queue.size == 0){
            pthread_mutex_lock(&track_mutex);
	        pthread_cond_wait(&train_done_load, &track_mutex);
	        pthread_mutex_unlock(&track_mutex);
        }

        // crossinglogic needs to return train or numid
        train traintodispatch = crossingLogic();
        pthread_cond_signal(&startcrossing[traintodispatch.numID]);

        pthread_mutex_lock(&track_mutex);
        pthread_cond_wait(&donecrossing[traintodispatch.numID], &track_mutex);
        pthread_mutex_unlock(&track_mutex);
    }


pthread_exit(NULL);

}



train crossingLogic() {
    pthread_mutex_lock(&queue_mutex);

    if (queue.size == 0) {
        // printf("No trains to dispatch.\n");
        pthread_mutex_unlock(&queue_mutex);
        return (train){0}; 
    }

    qsort(queue.trains, queue.size, sizeof(train), compare_trains);
    train dispatched_train = queue.trains[0];

    int count_same_priority = countSamePriorityTrains(&queue, dispatched_train);
    
    if (count_same_priority == 1) {
    } else {
        dispatched_train = selectTrainFromCandidates(&queue, dispatched_train, count_same_priority);
        
    }

    removeDispatchedTrain(&queue, dispatched_train.numID);
    updateLastDispatchedDirection(&queue, dispatched_train);
    
    pthread_mutex_unlock(&queue_mutex);
    return dispatched_train;
}

void updateLastDispatchedDirection(TrainQueue* queue, train dispatched_train) {
    char dir_str[2];
    dir_str[0] = dispatched_train.direction;
    dir_str[1] = '\0';
    strcpy(queue->last_dispatched_direction, dir_str);
}

void removeDispatchedTrain(TrainQueue* queue, int trainID) {
    int index = -1;
    for (int i = 0; i < queue->size; i++) {
        if (queue->trains[i].numID == trainID) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        for (int i = index; i < queue->size - 1; i++) {
            queue->trains[i] = queue->trains[i + 1];
        }
        queue->size--;
    }
}

train selectTrainFromCandidates(TrainQueue* queue, train dispatched_train, int count_same_priority) {
    train* selected_train = NULL;
    bool same_direction_found = false;

    for (int i = 0; i < count_same_priority; i++) {
        if (queue->trains[i].direction == dispatched_train.direction) {
            if (!same_direction_found || queue->trains[i].loadingTime < selected_train->loadingTime) {
                selected_train = &queue->trains[i];
                same_direction_found = true;
            }
        } else {
            if (!selected_train || selected_train->direction != dispatched_train.direction) {
                selected_train = &queue->trains[i];
            }
        }
    }

    return selected_train ? *selected_train : dispatched_train;
}

int countSamePriorityTrains(TrainQueue* queue, train dispatched_train) {
    int count = 1;
    for (int i = 1; i < queue->size; i++) {
        if (queue->trains[i].priority == dispatched_train.priority) {
            count++;
        } else {
            break; // Stop when priorities differ
        }
    }
    return count;
}


// Function to compare two trains (for sorting)
int compare_trains(const void* a, const void* b) {
    train* train_a = (train*)a;
    train* train_b = (train*)b;

    if (train_a->priority != train_b->priority) {
        return train_b->priority - train_a->priority; 
        // Higher priority first
    }
    return (train_a->loadingTime > train_b->loadingTime) ? 1 : -1; 
    // Earlier timestamp first
}



// gets train data from input file, creates train structs and put into trains array
void getTrainData(char *file) {
    FILE *fp = fopen(file, "r");

    int cross_t, load_t;
    char dir;

    while (fscanf(fp, " %c %d %d", &dir, &load_t, &cross_t) == 3) {
        train* train = &trains[trainCount];
        train->numID = trainCount;
        train->loadingTime = load_t;
        train->crossingTime = cross_t;
        train->direction = dir;
        
        int prior = 0;
        if(dir == 'W' || dir == 'E'){
            prior = 1;
        }
        train->priority = prior;
        trainCount++;
    }

    fclose(fp);
}


// initialize the train queue
void init_queue(TrainQueue* queue) {
    queue->size = 0;
    strcpy(queue->last_dispatched_direction, "");
}

// add a train to the queue
void add_train(TrainQueue* queue, train* train) {
    if (queue->size < MAX_TRAINS) {
        queue->trains[queue->size++] = *train;
    } else {
        printf("Queue is full!\n");
    }
}


void startLoadingtrains(){
    
    // printf("start of startloadingtrains\n");
	pthread_cond_broadcast(&train_start_to_load);
}

void createTrainThreads(){
    
    for(int i = 0; i < trainCount; i++) {
		pthread_cond_init(&donecrossing[i], NULL);
		pthread_cond_init(&startcrossing[i], NULL);
        pthread_create(&train_threads[i], NULL, trainthreadFunction, &trains[i]);
    }

    

}

void createDispatcherThread(){
    
    pthread_create(&dispatcher_thread, NULL, dispatcher_function, NULL);
}


void joinThreads(){

    for(int i = 0; i < trainCount; i++) {
        pthread_join(train_threads[i], NULL);
    }
	
	pthread_join(dispatcher_thread, NULL); // Join the dispatcher thread
}

void destroyThreads(){
    
    pthread_cond_destroy(&train_start_to_load);
    pthread_cond_destroy(&train_done_load);
    pthread_mutex_destroy(&track_mutex);
    pthread_mutex_destroy(&queue_mutex);
    for(int i = 0; i < trainCount; i++) {
		pthread_cond_destroy(&donecrossing[i]);
		pthread_cond_destroy(&startcrossing[i]);

    }

}


void sleepfor(int time) {
    struct timespec req, rem;

    if (time < 0) {
        return; // No sleep for negative values
    }

    req.tv_sec = time / 10;
    req.tv_nsec = (time % 10) * 100000000;

    if (req.tv_sec == 0 && req.tv_nsec == 0) {
        req.tv_nsec = 1;
    }

    
    while (nanosleep(&req, &rem) == -1) {
       
        req = rem;
    }
}


void timeformatting(int tenths, char *buffer, size_t bufferSize) {

    int seconds = tenths / 10;
    int tenths_remainder = tenths % 10;

    int hours = seconds / 3600;
    seconds = seconds % 3600;
    int minutes = seconds / 60;
    seconds = seconds % 60;

    // Format output
    snprintf(buffer, bufferSize, "%02d:%02d:%02d.%d", hours, minutes, seconds, tenths_remainder);
}

void startTimer(Timer *timer) {
    gettimeofday(&timer->start_time, NULL);
}

int getElapsedTenths(Timer *timer) {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    long seconds = current_time.tv_sec - timer->start_time.tv_sec;
    long microseconds = current_time.tv_usec - timer->start_time.tv_usec;
    long total_microseconds = (seconds * 1000000) + microseconds;

    return total_microseconds / 100000;
}