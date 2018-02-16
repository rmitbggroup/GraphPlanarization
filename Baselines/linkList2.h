#ifndef LIST2_H
#define LIST2_H

#include "node2.h"
#include "bool.h"

/************* Structures *****************************************/

struct C_LinkedListRec
{
	int count; 
	C_Node* startPtr;
	C_Node* endPtr;
};

typedef struct C_LinkedListRec List2;

/************ Function Prototypes *********************************/

void initList2(List2* listPtr);
int emptyList2(List2* listPtr);
void insertInList2(List2* listPtr, int item);
void insertAtEnd2(List2* listPtr, int item);
void removeFromList2(List2* listPtr, int item);
void printList2(List2* listPtr);
void killList2(List2* listPtr);
int inList2(List2* listPtr, int item);

#endif
