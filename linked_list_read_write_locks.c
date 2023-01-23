#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_VALUE 65536

struct LinkedListNode {
    int    data;
    struct LinkedListNode* next;
};

// Method declarations
int Member(int value);
int Insert(int value);
int Delete(int value);

int GenerateRandomNumber();
void Execute();
void InitializeLinkedList(int noOfNodes);
long GetCurrentTimestamp();

struct LinkedListNode* head_p = NULL;
pthread_rwlock_t rwlock;
int noOfOperationsPerThread;
int noOfInsertPerThread;
int noOfDeletePerThread;
int noOfMemberPerThread;


long current_timestamp(void);
void initialize(int);
int generateRandom(void);
void* doOperations(void*);

int main(int arc, char *argv[]) {
    if (arc != 7) {
        printf("Invalid number of arguments %d\n", arc);
        return -1;
    }
    long start, finish, elapsed;
    pthread_t* threadHandles;
    int noOfVariables = atoi(argv[1]);
    int noOfThreads = atoi(argv[6]);
    noOfOperationsPerThread = atoi(argv[2]) / noOfThreads;
    noOfMemberPerThread = strtod(argv[3], NULL) * noOfOperationsPerThread;
    noOfInsertPerThread = strtod(argv[4], NULL) * noOfOperationsPerThread;
    noOfDeletePerThread = strtod(argv[5], NULL) * noOfOperationsPerThread;

    threadHandles = (pthread_t*) malloc (noOfThreads * sizeof(pthread_t));
    pthread_rwlock_init(&rwlock, NULL);
    initialize(noOfVariables);

    long thread;
    start = current_timestamp();
    for (thread = 0; thread < noOfThreads; thread++) {
        pthread_create(&threadHandles[thread], NULL, doOperations, (void*)thread);
    }

    for (thread = 0; thread < noOfThreads; thread++) {
        pthread_join(threadHandles[thread], NULL);
    }
    finish = current_timestamp();
    // Calculate the elapsed time
    elapsed = finish - start;

    // Print the time to stdout
    printf("%ld", elapsed);
    return 0;
}

//get the current time
long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

//to get random data for initial data and test data
int generateRandom() {
    int value = rand() % MAX_VALUE;
    return value;
}

//initialize the linked list
void initialize(int noOfVariables) {
    srand (time(NULL));
    int inserted = 0;
    int i;
    for (i = 0; i < noOfVariables; i++) {
        inserted = Insert(generateRandom());
        if (!inserted) {
            i--;
        }
    }
}

//use rwlock for member, insert and delete.
void* doOperations(void* rank) {
    long start = ((long) rank) * noOfOperationsPerThread;
    long end = start + noOfOperationsPerThread;

    long i;
    for (i = start; i < end; i++) {
        if (i < start + noOfInsertPerThread) {
            int value = generateRandom();
            pthread_rwlock_wrlock(&rwlock);
            Insert(value);
            pthread_rwlock_unlock(&rwlock);
        } else if (i < start + noOfInsertPerThread + noOfDeletePerThread) {
            int value = generateRandom();
            pthread_rwlock_wrlock(&rwlock);
            Delete(value);
            pthread_rwlock_unlock(&rwlock);
        } else {
            int value = generateRandom();
            pthread_rwlock_rdlock(&rwlock);
            Member(value);
            pthread_rwlock_unlock(&rwlock);
        }
    }

    return NULL;
}

//delete an element from the linked list
int Delete(int value) {
    struct LinkedListNode* curr_p = head_p;
    struct LinkedListNode* pred_p = NULL;

    /* Find value */
    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            head_p = curr_p->next;
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    } else {
        return 0;
    }
}

////insert an element in a linked list
int Insert(int value) {
    struct LinkedListNode* curr_p = head_p;
    struct LinkedListNode* pred_p = NULL;
    struct LinkedListNode* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct LinkedListNode));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL)
            head_p = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    } else {
        return 0;
    }
}

//check for the existing element in a linked list
int  Member(int value) {
    struct LinkedListNode* curr_p;

    curr_p = head_p;
    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}