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
int GenerateRandomValue();
long GetCurrentTimestamp();
void* Execute(void*);

// Variable declarations
struct LinkedListNode *head_p = NULL;
pthread_mutex_t mutex;
int noOfOperationsPerThread;
int noOfMemberOperationsPerThread;
int noOfInsertOperationsPerThread;
int noOfDeleteOperationsPerThread;

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        printf("Usage: ./linked_list_mutex <noOfNodes> <noOfOperations> <mMember> <mInsert> <mDelete> <noOfThreads>");
        return -1;
    };
    long start, finish, elapsed;
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
    start = GetCurrentTimestamp();
    for (thread = 0; thread < noOfThreads; thread++)
    {
        pthread_create(&threadHandles[thread], NULL, Execute, (void*) thread);
    }

    for (thread = 0; thread < noOfThreads; thread++)
    {
        pthread_join(threadHandles[thread], NULL);
    }

    finish = GetCurrentTimestamp();
    elapsed = finish - start;

    printf("Elapsed time: %ld milliseconds", elapsed);
    return 0;
}



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

//generate random value
int GenerateRandomValue()
{
    return rand() % MAX_VALUE;
}

//get current timestamp
long GetCurrentTimestamp()
{
    struct timeval te;
    gettimeofday(&te, NULL);
    long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
    return milliseconds;
}

//initialize linked list
void InitializeLinkedList(int noOfNodes)
{
    int i;
    for (i = 0; i < noOfNodes; i++)
    {
        int value = GenerateRandomValue();
        if (Insert(value) == 0)
        {
            i--;
        }
    }
}

//use mutex for member, insert and delete
void* Execute(void* rank) {
    long start = ((long) rank) * noOfOperationsPerThread;
    long end = start + noOfOperationsPerThread;

    long i;
    for (i = start; i < end; i++) {
        if (i < start + noOfInsertOperationsPerThread) {
            int value = generateRandom();
            pthread_mutex_lock(&mutex);
            Insert(value);
            pthread_mutex_unlock(&mutex);
        } else if (i < start + noOfInsertOperationsPerThread + noOfDeleteOperationsPerThread) {
            int value = generateRandom();
            pthread_mutex_lock(&mutex);
            Delete(value);
            pthread_mutex_unlock(&mutex);
        } else {
            int value = generateRandom();
            pthread_mutex_lock(&mutex);
            Member(value);
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}