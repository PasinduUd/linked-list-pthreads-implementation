#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_VALUE 65536

struct LinkedListNode
{
    int data;
    struct LinkedListNode *next;
};

// Method declarations
int Member(int value);
int Insert(int value);
int Delete(int value);

void InitializeLinkedList(int noOfNodes);
int GenerateRandomValue(void);
long GetCurrentTimestamp(void);
void* Execute(void*);

// Variable declarations
struct LinkedListNode *head_p = NULL;
pthread_mutex_t mutex;
int noOfOperationsPerThread;
int noOfMemberOperationsPerThread;
int noOfInsertOperationsPerThread;
int noOfDeleteOperationsPerThread;

// Checks whether a given value exists in the linked list
int Member(int value)
{
    struct LinkedListNode *curr_p;

    curr_p = head_p;
    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Inserts an element into the linked list
int Insert(int value)
{
    struct LinkedListNode *curr_p = head_p;
    struct LinkedListNode *pred_p = NULL;
    struct LinkedListNode *temp_p;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct LinkedListNode));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL)
            head_p = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    }
    else
    {
        return 0;
    }
}

// Deletes an element from the linked list
int Delete(int value)
{
    struct LinkedListNode *curr_p = head_p;
    struct LinkedListNode *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value)
    {
        if (pred_p == NULL)
        {
            head_p = curr_p->next;
            free(curr_p);
        }
        else
        {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

// Returns a random number between 0 and (2^16) – 1
int GenerateRandomNumber()
{
    return rand() % MAX_VALUE;
}

// Executes Menber(), Insert() and Delete() mutex
void* Execute(void* rank) {
    long start = ((long) rank) * noOfOperationsPerThread;
    long end = start + noOfOperationsPerThread;

    long i;
    for (i = start; i < end; i++) {
        if (i < start + noOfInsertOperationsPerThread) {
            pthread_mutex_lock(&mutex);
            Insert(GenerateRandomNumber());
            pthread_mutex_unlock(&mutex);
        } else if (i < start + noOfInsertOperationsPerThread + noOfDeleteOperationsPerThread) {
            pthread_mutex_lock(&mutex);
            Delete(GenerateRandomNumber());
            pthread_mutex_unlock(&mutex);
        } else {
            pthread_mutex_lock(&mutex);
            Member(GenerateRandomNumber());
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}

// Returns the current time
long GetCurrentTimestamp()
{
    struct timeval te;
    gettimeofday(&te, NULL);                                    // Get current time
    long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // Caculate milliseconds
    // printf("Milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

// Initializes the linked list
void InitializeLinkedList(int noOfNodes)
{
    int i;
    for (i = 0; i < noOfNodes; i++)
    {
        int value = GenerateRandomNumber();
        if (Insert(value) == 0)
        {
            i--;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        printf("Invalid # Arguments : %d\n", argc);
        return -1;
    };
    long startedTime, finishedTime, elapsedTime;
    pthread_t* threadHandles;

    int noOfThreads = atoi(argv[6]);
    int noOfNodes = atoi(argv[1]);
    noOfOperationsPerThread = atoi(argv[2]) / noOfThreads;

    noOfMemberOperationsPerThread = (int)(atof(argv[3]) * noOfOperationsPerThread);
    noOfInsertOperationsPerThread = (int)(atof(argv[4]) * noOfOperationsPerThread);
    noOfDeleteOperationsPerThread = (int)(atof(argv[5]) * noOfOperationsPerThread);

    threadHandles = (pthread_t*) malloc (noOfThreads * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    InitializeLinkedList(noOfNodes);

    long thread;
    startedTime = GetCurrentTimestamp();
    for (thread = 0; thread < noOfThreads; thread++)
    {
        pthread_create(&threadHandles[thread], NULL, Execute, (void*) thread);
    }

    for (thread = 0; thread < noOfThreads; thread++)
    {
        pthread_join(threadHandles[thread], NULL);
    }

    finishedTime = GetCurrentTimestamp();
    elapsedTime = finishedTime - startedTime;
    printf("Elapsed time: %ld milliseconds", elapsedTime);
    return 0;
}