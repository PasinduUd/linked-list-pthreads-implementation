#include <stdio.h>
#include <stdbool.h>

#define MAX_VALUE 65536

struct LinkedListNode
{
    int data;
    struct LinkedListNode *next;
};

// Method declarations
bool Member(int value);
bool Insert(int value);
bool Delete(int value);

void InitializeLinkedList(int noOfNodes);
int GenerateRandomValue();
long GetCurrentTimestamp();
void Execute();

// Variable declarations
struct LinkedListNode *head_p = NULL;
int noOfOperations;
int noOfMemberOperations;
int noOfInsertOperations;
int noOfDeleteOperations;

// Checks whether a given value exists in the linked list
bool Member(int value)
{
    struct LinkedListNode *curr_p;

    curr_p = head_p;
    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int main()
{
    printf("Hello World!");
    return 0;
}