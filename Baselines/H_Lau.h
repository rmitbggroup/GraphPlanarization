#ifndef H_LAU_H
#define H_LAU_H
#include "graph.h"

/*********** structures ***************************************/
struct partitionRec {
	int part;
	int* neighbours;
};
typedef struct partitionRec Partition;

int H_L_alg(Vertex* G, int size, int save);
Partition* randPartition(Vertex* G, int size, int numClasses, int classes[]);
void printPartInfo(Partition* info, int size, int classes);
void killPartition(Partition* info, int size, int classes);
int selectClass(Partition* info, int numClasses, int size, int v);
int selectClassI(Partition* info, int numClasses, int size, int v);
void switchPart(Partition* info, int classNum[], Vertex* G, int size, int new, int v, int numClasses);

#endif
