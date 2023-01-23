#include <stdio.h>

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
void Execute();

// Variable declarations
struct LinkedListNode *head_p = NULL;
int noOfOperations;
int noOfMemberOperations;
int noOfInsertOperations;
int noOfDeleteOperations;

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
    else    // Value already exists in the linked list
    {
        return 0;
    }
}

// Deletes an element from the linked list
int Delete(int value) {
    struct LinkedListNode* curr_p = head_p;
    struct LinkedListNode* pred_p = NULL;

    /* Find value */
    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {   // Delete first node in the list
            head_p = curr_p->next;
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    } else {    // Value doesn't exist in the list
        return 0;
    }
}

int main()
{
    printf("Hello World!");
    return 0;
}