#include <stdio.h>
#include <stdlib.h>

struct node
{
    char bit;
    struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;
struct node *prev = NULL;

// inserts node at end of list
void insert(char bit)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));

    temp->bit = bit;
    temp->next = NULL;

    // if no head, make this the start
    if (head == NULL)
    {
        head = temp;
        return;
    }
    current = head;
    // find end of list
    while (current->next != NULL)
        current = current->next;

    // insert new node at end
    current->next = temp;
}

void delete (struct node **head, char bit)
{
    // temporary node
    struct node *temp;

    // check if head node has the value, if it does,
    // make the next value the head node
    if ((*head)->bit == bit)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    else
    {
        struct node *current = *head;
        while (current->next != NULL)
        {
            // if the value of the next node matches
            if (current->next->bit == bit)
            {
                temp = current->next;
                // make the next node the one after the one to be deleted
                current->next = current->next->next;
                free(temp);
                break;
            }
            // if value doesn't match, keep iterating through list
            else
                current = current->next;
        }
    }
}
