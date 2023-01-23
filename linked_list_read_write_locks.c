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

int GenerateRandomNumber(void);
void *Execute(void *);
void InitializeLinkedList(int noOfNodes);
long GetCurrentTimestamp(void);

// Variable declarations
struct LinkedListNode *head_p = NULL;
pthread_rwlock_t rwlock;
int noOfOperationsPerThread;
int noOfInsertOperationsPerThread;
int noOfDeleteOperationsPerThread;
int noOfMemberOperationsPerThread;

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

    /* Find value */
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

// Executes Menber(), Insert() and Delete() using Read-Write Locks
void *Execute(void *rank)
{
    long start = ((long)rank) * noOfOperationsPerThread;
    long end = start + noOfOperationsPerThread;

    long i;
    for (i = start; i < end; i++)
    {
        if (i < start + noOfInsertOperationsPerThread)
        {
            pthread_rwlock_wrlock(&rwlock);
            Insert(GenerateRandomNumber());
            pthread_rwlock_unlock(&rwlock);
        }
        else if (i < start + noOfInsertOperationsPerThread + noOfDeleteOperationsPerThread)
        {
            pthread_rwlock_wrlock(&rwlock);
            Delete(GenerateRandomNumber());
            pthread_rwlock_unlock(&rwlock);
        }
        else
        {
            pthread_rwlock_rdlock(&rwlock);
            Member(GenerateRandomNumber());
            pthread_rwlock_unlock(&rwlock);
        }
    }

    return NULL;
}

// Initializes the linked list
void InitializeLinkedList(int noOfNodes)
{
    srand(time(NULL));
    int inserted = 0;
    int i;
    for (i = 0; i < noOfNodes; i++)
    {
        inserted = Insert(GenerateRandomNumber());
        if (!inserted)
        {
            i--;
        }
    }
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

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        printf("Invalid # Arguments : %d\n", argc);
        return -1;
    }
    long startedTime, finishedTime, elapsedTime;
    pthread_t *threadHandles;
    int noOfVariables = atoi(argv[1]);
    int noOfThreads = atoi(argv[6]);
    noOfOperationsPerThread = atoi(argv[2]) / noOfThreads;
    noOfMemberOperationsPerThread = strtod(argv[3], NULL) * noOfOperationsPerThread;
    noOfInsertOperationsPerThread = strtod(argv[4], NULL) * noOfOperationsPerThread;
    noOfDeleteOperationsPerThread = strtod(argv[5], NULL) * noOfOperationsPerThread;

    threadHandles = (pthread_t *)malloc(noOfThreads * sizeof(pthread_t));
    pthread_rwlock_init(&rwlock, NULL);
    InitializeLinkedList(noOfVariables);

    long thread;
    startedTime = GetCurrentTimestamp();
    for (thread = 0; thread < noOfThreads; thread++)
    {
        pthread_create(&threadHandles[thread], NULL, Execute, (void *)thread);
    }

    for (thread = 0; thread < noOfThreads; thread++)
    {
        pthread_join(threadHandles[thread], NULL);
    }

    finishedTime = GetCurrentTimestamp();
    elapsedTime = finishedTime - startedTime;
    printf("Elapsed Time : %ld", elapsedTime);
    return 0;
}