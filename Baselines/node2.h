#ifndef NODE2_H
#define NODE2_H

/************ Structures ******************************************/

struct C_NodeRec
{
	int component;		/* component number */
	int numAdjacent;	/* number of times vertex is adjacnet to this
				   component */
	struct C_NodeRec* prev;
	struct C_NodeRec* next;
};
typedef struct C_NodeRec C_Node;

/*********** Function Prototypes *********************************/
C_Node* makeNode2(int item);

#endif
