#ifndef NODE_H
#define NODE_H

/************ Structures ******************************************/

struct NodeRec
{
	int vertex;
	struct NodeRec* prev;
	struct NodeRec* next;
};
typedef struct NodeRec Node;

/*********** Function Prototypes *********************************/
Node* makeNode(int item);

#endif
