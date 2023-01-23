#include <stdio.h>
#include <stdlib.h>
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

int GenerateRandomNumber();
void Execute();
void InitializeLinkedList(int noOfNodes);
long GetCurrentTimestamp();

// Variable declarations
struct LinkedListNode *head_p = NULL;
int noOfOperations;
int noOfInsertOperations;
int noOfDeleteOperations;
int noOfMemberOperations;

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
        if (pred_p == NULL) // New first node
            head_p = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    }
    else // Value already exists in the linked list
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
        { // Delete first node in the list
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
    { // Value doesn't exist in the list
        return 0;
    }
}

// Returns a random number between 0 and (2^16) – 1
int GenerateRandomNumber()
{
    return rand() % MAX_VALUE;
}

// Executes Menber(), Insert() and Delete()
void Execute()
{
    long i;
    for (i = 0; i < noOfOperations; i++)
    {
        if (i < noOfInsertOperations)
        {
            Insert(GenerateRandomNumber());
        }
        else if (i < (noOfInsertOperations + noOfDeleteOperations))
        {
            Delete(GenerateRandomNumber());
        }
        else
        {
            Member(GenerateRandomNumber());
        }
    }
}

// Initializes the linked list
void InitializeLinkedList(int noOfNodes)
{
    srand(time(NULL));
    int Inserted = 0;
    int i;
    for (i = 0; i < noOfNodes; i++)
    {
        Inserted = Insert(GenerateRandomNumber());
        if (!Inserted)
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
    // Validate arguments
    if (argc != 6)
    {
        printf("Invalid # Arguments : %d\n", argc);
        return -1;
    }
    // Variables
    long startedTime, finishedTime, elapsedTime;

    // Extract and interpret the arguments
    int noOfNodes = atoi(argv[1]);
    noOfOperations = atoi(argv[2]);
    noOfMemberOperations = strtod(argv[3], NULL) * noOfOperations;
    noOfInsertOperations = strtod(argv[4], NULL) * noOfOperations;
    noOfDeleteOperations = strtod(argv[5], NULL) * noOfOperations;

    InitializeLinkedList(noOfNodes);      // Initialize the linkedlist
    startedTime = GetCurrentTimestamp();  // Get the starting time
    Execute();                            // Execute operations
    finishedTime = GetCurrentTimestamp(); // Get the finishing time
    elapsedTime = finishedTime - startedTime;

    printf("Elapsed Time : %ld", elapsedTime);
    return 0;
}