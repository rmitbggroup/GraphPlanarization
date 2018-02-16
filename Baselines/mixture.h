#ifndef	MIXTURE_H
#define MIXTURE_H

#include "K_Alg.h"
#include "subsetRem.h"

int mixture_Alg(Vertex* G, int size, int save);
int* initialiseR(Vertex* G, int size, int Rsize);
void createComponent(Vertex* G, K_VertInfo* info, int v,int comp, int* edges,int* vertices);
K_VertInfo* initialiseInfo(Vertex* G, int size, int* comp);
void createTreeInfo(Vertex* G, K_VertInfo* info,int v);
void createNonTreeInfo(Vertex* G, K_VertInfo* info,int v);

#endif

