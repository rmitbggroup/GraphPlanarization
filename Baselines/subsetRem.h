#ifndef SUBSET_REMOVE_H
#define SUBSET_REMOVE_H

#include "graph.h"
#include "bool.h"

int subsetRemove(Vertex* G, int size, int save);
int selectVertForRemoval(Vertex* G, int size, int d);
int subsetRemoveProf(Vertex* G, int size, int save);

#endif
