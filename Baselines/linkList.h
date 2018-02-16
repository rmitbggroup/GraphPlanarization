#ifndef LIST_H
#define LIST_H

#include "node.h"
#include "bool.h"

/************* Structures *****************************************/

struct LinkedListRec
{
	int degree; /* how many vertices are adjacent to this vertex */
	Node* startPtr;
	Node* endPtr;
};

typedef struct LinkedListRec List;

/************ Function Prototypes *********************************/

void initList(List* listPtr);
int emptyList(List* listPtr);
void insertInList(List* listPtr, int adjacentVertex);
void insertAtEnd(List* listPtr, int adjacentVertex);
void insertAtFront(List* listPtr, int adjacentVertex);
void insertAtStart(List* listPtr, int adjacentVertex);
void removeFromList(List* listPtr, int vertex);
void printList(List* listPtr);
void killList(List* listPtr);
int inList(List* listPtr, int item);
void addList(List* L1, List* L2);
void append(List* L1, List* L2);
void prepend(List* L1, List* L2);
void revPrePend(List* L1, List* L2);
void revAppend(List* L1, List* L2);
void reverseList(List* L);
#endif
