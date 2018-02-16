/**
 * Copyright (C) 2005, 2009  Kerri Morgan
 *
 * This file is part of the planar program package.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * **/

#include "K_Alg.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
/******************************************************************************
** Function:	M_MIPS
** Description: Function implements my own algorithm to find a induced planar 
**		subgraph.
** Inputs:	G - the graph
**		size - number of vertices in graph
**		save - flag to indicate whether to save subgraph or not
** Return Value:size of P
** Side Effects:
** Created:	Kerri Morgan 	7th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int M_MIPS(Vertex* G, int size, int save)
{
	int P=0;
	K_VertInfo* info=NULL;
	int* R= NULL;
	int Rsize;
	R_Index* R_idx=NULL;	
	int idxSize;
	int componentNum =0;
	int i;
	
	info=initK_Info(G, size);
	R=initR(size);
	Rsize = size;

	/* sort R in order of increasing degree */
	sort(G, R, size);

	R_idx = createIndex(G, R, size, &idxSize);
/******************
for(i=0; i< size; i++)
	printf("%d ", R[i]);
printf("\n\n");
for(i=0; i<idxSize; i++)
	printf("(%d %d %d) ", R_idx[i].start, R_idx[i].finish, R_idx[i].degree);
printf("\n\n");	
********************/

	P=growKTrees(G, size, info, R,  &componentNum, R_idx, idxSize);
	
	#ifdef DEBUG
	printKInfo(info, size);
	printf("TEST TREE SIZE: %d is P\n", P);
	#endif

	P=growOuterplanar( G, size, info, R, R_idx, idxSize, P, &componentNum);
	#ifdef DEBUG
	printKInfo(info, size);
	printf("TEST: %d is P\n", P);
	#endif


	/* extend graph */
	P=extendGraph(G, size, info, R, R_idx, idxSize, P, &componentNum);

	for(i=0; i<size; i++)
	{
		killList2(&info[i].adjTrees);
		killList2(&info[i].adjNonTrees);
	} 
	free(info);
	free(R);
	free(R_idx);

	if(save ==1 )
	{
		writePlanarSubgraph(G,size);
	}
	return P;
}

/******************************************************************************
** Function:	CreateOPGraph
** Description: Function creates an outerplanar subgraph
** Inputs:	G - the graph
**		size - number of vertices in graph
**		save - flag to indicate whether to save subgraph or not
** Return Value:size of P
** Side Effects:
** Created:	Kerri Morgan 	27th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int CreateOPGraph(Vertex* G, int size, int save)
{
	int P=0;
	K_VertInfo* info=NULL;
	int* R= NULL;
	int Rsize;
	R_Index* R_idx=NULL;	
	int idxSize;
	int componentNum =0;
	int i;
	
	info=initK_Info(G, size);
	R=initR(size);
	Rsize = size;

	/* sort R in order of increasing degree */
	sort(G, R, size);

	R_idx = createIndex(G, R, size, &idxSize);

	P=growKTrees(G, size, info, R,  &componentNum, R_idx, idxSize);
	#ifdef DEBUG
	printKInfo(info, size);
	#endif


	P=growOuterplanar( G, size, info, R, R_idx, idxSize, P, &componentNum);

	#ifdef DEBUG
	printKInfo(info, size);
	#endif	

	if(save == 1)
	{
		writePlanarSubgraph(G, size);
	}
	
	for(i=0; i<size; i++)
	{
		killList2(&info[i].adjTrees);
		killList2(&info[i].adjNonTrees);
	} 
	free(info);
	free(R);
	free(R_idx);
	return P;
}

/******************************************************************************
** Function:	GrowCleverTrees
** Description: Function implements my own algorithm to find a induced forest 
**		subgraph.
** Inputs:	G - the graph
**		size - number of vertices in graph
**		save - flag to indicate whether to save subgraph or not
** Return Value:size of P
** Side Effects:
** Created:	Kerri Morgan 	23rd June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int GrowCleverTrees(Vertex* G, int size, int save)
{
	int P=0;
	K_VertInfo* info=NULL;
	int* R= NULL;
	int Rsize;
	R_Index* R_idx=NULL;	
	int idxSize;
	int componentNum =0;
	int i;
	
	info=initK_Info(G, size);
	R=initR(size);
	Rsize = size;

	/* sort R in order of increasing degree */
	sort(G, R, size);

	R_idx = createIndex(G, R, size, &idxSize);

	P=growKTrees(G, size, info, R,  &componentNum, R_idx, idxSize);

	#ifdef DEBUG
	printKInfo(info, size);
	#endif

	for(i=0; i<size; i++)
	{
		killList2(&info[i].adjTrees);
		killList2(&info[i].adjNonTrees);
	} 
	free(info);
	free(R);
	free(R_idx);
	if(save ==1)
	{
		writePlanarSubgraph(G, size);
	}

	return P;
}

/******************************************************************************
** Function:	initK_Info
** Description:	Initialises structure holding information about planar 
**		subgraph
** Inputs:	G - the graph
**		size - number of vertices in subgraph
** Return Value:pointer to info structure.
** Side Effects:
** Created:	Kerri Morgan	7th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
K_VertInfo* initK_Info(Vertex* G, int size)
{
	K_VertInfo* info=NULL;
	int i;

	info=(K_VertInfo*)malloc(sizeof(K_VertInfo)*size);
	if(info==NULL)
	{
		fprintf(stderr, "ERROR 10.1: Insufficient memory to create information storage\n");
		exit(1);
	}

	for(i=0; i<size; i++)
	{
		info[i].inP = FALSE;
		info[i].type = UNDECIDED;
		info[i].component =-1;
		info[i].cycle =FALSE;
		info[i].Nn=0;
		info[i].Nf=0;
		info[i].RG_Index=-1;
		initList2(&info[i].adjTrees);
		initList2(&info[i].adjNonTrees);
	}
	return info;
}

/******************************************************************************
** Function: 	growIndependentSet
** Description:	Function finds a maximal indpendent set in G
** Inputs:	G - the graph
**		info - holds information about the planar subgraph
**		R - set of vertices not in P.  R is sorted by increasing vertex
**		degree
**		Rsize - size of R
** Return Value:
** Side Effects:
** Created:	Kerri Morgan	7th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int growIndpendentSet(Vertex* G, int size, K_VertInfo* info, int* R, int * Rsize)
{
	int P=0;
	int index;
	int v, w;
	int j;
	int free =0;
	int tsize;

	tsize = *Rsize;



	index = pickVertex(info, R, tsize, 0, &free);
	//index=pickLowDegVert(G, info, R, *Rsize, low);
	while(index != -1)
	{
		//free = index;
		v = R[index];
		*Rsize=(*Rsize)-1;
		G[v].member = TRUE;
		info[v].type = FOREST;
		info[v].component =P;
		info[v].inP = TRUE;
		P++;
		/* increment the number of tree neighbours */
		for(j=0; j<G[v].degree; j++)
		{
			w = G[v].adjVerts[j];
			info[w].Nf++;
			insertInList2(&(info[w].adjTrees), info[v].component);
		}
		index = pickVertex(info, R, tsize, index+1, &free); 
	}
	/* move other elements along*/
	j=free;
	while((j != *Rsize)&&(*Rsize <tsize ))
	{
		R[j]=R[j+1];
		j++;
	}
	*Rsize = size -P;
//	for(j=0; j<*Rsize; j++)
//		printf("%d\n", R[j]);
	return P;
}

/******************************************************************************
** Function:	pickVertex
** Description:	Function finds the vertex of lowest degree in R that can
**		be added to an independent set.
** Inputs:	info - holds information about P
**		R - set of vertices not in P
**		Rsize - number of vertices in R
**		cur - index of next vertex to be considered
**		free - index of first place that contains a vertex that is
**		       in P and thus can be overwritten (or -1 if no such index)
** Return Value:index of vertex number in R (if none -1)
** Side Effects:updates R
** Created:	Kerri Morgan	7th June, 2005 (as pickLowDeg Vert)
** Modified:	Changed to remove "in P" vertices from R
**		Changed free to a pointer (14th June, 2005)
** Comments:	On entry to the function free is the last place that had a 
**		vertex removed.	
** Acknowledgements:
******************************************************************************/
int pickVertex( K_VertInfo* info, int* R, int Rsize, int cur, int* free)
{
	int i;
	int index;

	for(i=cur; i<Rsize; i++)
	{
		index =R[i];
		if((info[index].Nf == 0) && (info[index].inP == FALSE)) /* no neighbours*/
		{
			return i;
		}
		else
		{
			R[*free]=R[i];
			*free = (*free)+1;
		}
	}
	return -1;
}

/******************************************************************************
** Function:	pickLowDegVert
** Description:	Function finds the vertex of lowest degree in R
** Inputs:	G - the graph
**		info - holds information about P
**		R - set of vertices not in P
**		Rsize - number of vertices in R
**		low - previous lowest degree vertex 
** Return Value:index of vertex number in R (if none -1)
** Side Effects:
** Created:	Kerri Morgan	7th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int pickLowDegVert(Vertex* G, K_VertInfo* info, int* R, int Rsize, int low)
{
	int i;
	int index;
	int lowest=INT_MAX;
	int best =-1;

	for(i=0; i<Rsize; i++)
	{
		index =R[i];

		if((info[index].Nf == 0) && (info[index].inP == FALSE)) /* no neighbours*/
		{
			if(G[index].degree < lowest)
			{
				lowest = G[index].degree;
				best = i;
				if(lowest == low)
				{
					/* no possibility of any smaller*/
					return best;
				}
			}

		}
	}
	return best;
}
/******************************************************************************
** Function:	printKInfo
** Description:	Functon prints contents of info
** Inputs:	info - information about P
**		size - number of vertices in graph
** Return Value:none
** Side Effects:prints information to screen
** Created:	Kerri Morgan	7th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void printKInfo(K_VertInfo* info, int size)
{
	int i;

printf("Vx      In P    Type    Component       Nn      Nf	NonTAdj	TCompsAdj\n");
        for(i=0; i<size; i++)
	{
		printf("%d\t", i);
		if(info[i].inP==TRUE)
		{
			printf("T    \t");
		}
		else
		{
			printf("F    \t");
		}
		printf("%d\t", info[i].type);
		printf("%d\t", info[i].component);
		printf("%d\t", info[i].Nn);
		printf("%d\t", info[i].Nf);
		printList2(&info[i].adjNonTrees);
		printList2(&info[i].adjTrees);
	}
}
/******************************************************************************
** Function:	growCleverSingleTreeAlg
** Description:	Function grows an independent set (trees of single vertices)
** Inputs:
** Return Value:
** Side Effects:
** Created:	Kerri Morgan	8th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int growCleverSingleTreeAlg(Vertex* G, int size, int save)
{
	int P=0;
	K_VertInfo* info=NULL;
	int* R= NULL;
	int Rsize;
	int componentNum =0;
	
	info=initK_Info(G, size);
	R=initR(size);
	Rsize = size;
	
	/* sort R in order of increasing degree */
	sort(G, R, size);

	P=growIndpendentSet(G, size, info, R, &Rsize);
	componentNum =P;

	#ifdef DEBUG
	printf("TESTING***********************************************\n");
	printKInfo(info, size);
	printf("****************************************************\n\n");
	#endif

	free(info);
	free(R);
	return P;

}

/******************************************************************************
** Function:	sort
** Description:	Function sorts all elements in R in increasing order of degree
** Inputs:	G - the graph
**		R - list of vertices not in P 
**		size - number of vertices in R
** Return Value: none
** Side Effects: alters order of R
** Created:	Kerri Morgan	8th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void sort(Vertex* G, int* R, int size)
{
	int* R2=NULL;
	int i;

	R2=(int*)malloc(sizeof(int)*size);

	if(R2==NULL)
	{
		fprintf(stderr, "ERROR 10.2: Insufficient Memory to sort R\n");
		exit(1);
	}
	for(i=0; i<size; i++)
	{
		R2[i]=R[i];
	}

	mergeSort(G, R, size, R2);
	free(R2);
}

/******************************************************************************
** Function:	mergeSort
** Description:	Function sorts R using merge sort code based on that presented
**		in cse1303/2304.
** Inputs:	G - the graph
**		
** Return Value:
** Side Effects:
** Created:	Kerri Morgan
** Modified:
** Comments:	
** Acknowledgements:	cse1303 MergesortRec code adapted for this function
******************************************************************************/
void mergeSort(Vertex* G, int* R, int size, int*R2)
{
	int i;
	int mid;

	mid = size/2;

	if(size  > 1)
	{
		mergeSort(G, R, mid, R2);
		mergeSort(G, R+mid, size-mid, R2);
		merge(G,R, mid, R+mid, size-mid, R2);

		for(i=0; i<size; i++)
		{
			R[i]=R2[i];
		}
	}
}

/******************************************************************************
** Function:	merge
** Description:	Function merges two sorted arrays.
** Inputs:	G - the graph
**		R, R2 - arrays
**		Rsize, R2size - size of R and R2 respectively
** Return Value:none
** Side Effects:R contains vertices sorted by degree
** Created:	Kerri Morgan
** Modified:
** Comments:	
** Acknowledgements: Adapted from cse1303 Code
******************************************************************************/
void merge(Vertex* G, int* R, int Rsize, int* R2, int R2size, int* temp)
{
	int i,j,k;

	i=0;
	j=0;

	for(k=0; k<Rsize+R2size; k++)
	{
		if(i==Rsize)
		{
			temp[k]=R2[j];
			j++;
		}
		else if(j== R2size)
		{
			temp[k]=R[i];
			i++;
		}
		else if (G[R[i]].degree <= G[R2[j]].degree) /* sort by degree order*/
		{
			temp[k]=R[i];
			i++;	
		}
		else
		{
			temp[k]=R2[j];
			j++;
		}
	}
}

/******************************************************************************
** Function:	growKTrees
** Description:	Function finds a forest subgraph of a graph, G.
** Inputs:	G - the graph
**		size - number of vertices in graph
**		info - information about planar subgraph
**		R - list of vertices not in planar subgraph P.
**		compNum - next free component number
**		idx - index into R
**		idxSize - size of idx
** Return Value: size of planar subgraph (the forest)
** Side Effects:updates R, Rsize and info.
** Created:	Kerri Morgan	14th June, 2005
** Modified:
** Comments:	Modified - structures changed for efficiency 16th June, 2005	
** Acknowledgements:
******************************************************************************/
int growKTrees(Vertex* G, int size, K_VertInfo* info, int* R, int* component, R_Index* idx, int idxSize)
{
	int i;
	int v, w,x;
	int index;
	int low = 0;  
	int set=0;
	int P=0;
	
	/* check each set of vertices */
	for(set = 0; set < idxSize; set++)
	{
		if(idx[set].start <= idx[set].finish)
		{
			index = idx[set].start;

			/* start with no adjacent vertices option*/
			low=-1;
			/* add all possible vertices in this set to P */
			index = pickTreeVert1(info, R, size, index, idx, idxSize, set, low);
			while(index !=-1)
			{
				v = R[index];
				low = info[v].Nf;  /* At this stage P has trees only */
				G[v].member = TRUE;
				info[v].type = FOREST;
				info[v].inP = TRUE;

				w=getAdjTreeVertex(G, size, info, v);
				if(w==-1) /* new singleton tree */
				{	
					info[v].component = *component;
					*component = (*component)+1;
				}
				else
				{
					info[v].component = info[w].component;
					/* update all parts of tree to have same component number */
					/* at same time update all their neighbours adjTrees list*/
					updateTree(G, size, info,  v, info[v].component);
				}
				/* for each of this vertex's neighbours update their component
		 		* list of adjacent trees */	
				for(i=0; i< G[v].degree; i++)
				{
					x = G[v].adjVerts[i];
					if(info[x].inP == FALSE)
					{
						insertInList2(&(info[x].adjTrees), info[v].component);
						/* increment number of forest neighbours */
						info[x].Nf++;
					}
				}
				P++;
				R[index]=R[idx[set].finish];
				if(index == idx[set].finish)
				{
					index = idx[set].start;
				}

				idx[set].finish--;
//printf("***TESTING: Adding vertex %d\n", v);
	//printKInfo(info, size);
/*printf("START: %d FINISH: %d\n", idx[set].start, idx[set].finish);
for(k=0; k<size; k++)
{
	printf("%d ", R[k]);
}	
printf("\n\n"); */
				/* empty set */
				if(idx[set].start > idx[set].finish)
				{
					break;
				}

				index = pickTreeVert1(info, R, size, index, idx, idxSize, set, low);
			}
		}
	}
	return P;
}

/******************************************************************************
** Function:	getAdjVertex
** Description:	Function finds a vertex w that is adjacent to vertex v.  
** Inputs:	G - the graph
**		size - number of vertices in graph
**		info - contains information about P
**		v - vertex
** Return Value:w - the required vertex, or -1 if no such vertex exists
** Side Effects: none
** Created:	Kerri Morgan	22 June, 2005
** Modified:	from getAdjTreeVertex function
** Comments:	
** Acknowledgements:
******************************************************************************/
int getAdjVertex(Vertex* G, int size, K_VertInfo* info, int v)
{
	int w;
	int i;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if(info[w].inP == TRUE) 
		{
			return w;
		}
	}
	return -1;

}
/******************************************************************************
** Function:	getAdjTreeVertex
** Description:	Function finds a vertex w that is adjacent to vertex v.  Vertex
**		w is a member of a tree
** Inputs:	G - the graph
**		size - number of vertices in graph
**		info - contains information about P
**		v - vertex
** Return Value:w - the required vertex, or -1 if no such vertex exists
** Side Effects: none
** Created:	Kerri Morgan	14th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int getAdjTreeVertex(Vertex* G, int size, K_VertInfo* info, int v)
{
	int w;
	int i;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if((info[w].inP == TRUE) && (info[w].type == FOREST))
		{
			return w;
		}
	}
	return -1;

}
/******************************************************************************
** Function:	updateTree
** Description:	Function updates tree to be considered part of component compNum
**		(the component number of vertex v)
** Inputs:	G - the graph
**		size - number of vertices in graph
**		info - contains information about P
**		v - current vertex under consideration
**		compNum - component number of v
** Return Value:none
** Side Effects:updates list of adjacent tree components
** Created:	Kerri Morgan	14th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void updateTree(Vertex* G, int size, K_VertInfo* info, int v, int compNum)
{
	int i;
	int j;
	int w, x;
	int old;

	for(i=0; i<G[v].degree; i++)
	{
		w = G[v].adjVerts[i];
		if((info[w].inP == TRUE) && (info[w].component != compNum))
		{
			/* reset component number*/
			old = info[w].component;
			info[w].component= compNum;
			
			/* for each neighbour of this vertex
			   replace old component number with new
			   component number  adjacency list*/
			for(j=0; j<G[w].degree; j++)
			{
				x=G[w].adjVerts[j];
				removeFromList2(&info[x].adjTrees, old);
				insertInList2(&info[x].adjTrees, compNum);
			}
			updateTree( G, size, info, w, compNum);
		}	
	}
}

/******************************************************************************
** Function:	pickTreeVert1
** Description:	Function picks a vertex to add to the planar subgraph.  The 
**		vertex will become part of a tree component
** Inputs:	info - information about P
**		R - set of vertices not in P
**		size - number of vertices in graph
**		cur - current index in R
**		idx - index into sections of R
**		idxSize - size of idx
**		set - current set (that cur belongs to)
**		low - the number of forest neighbours of previous vertex added
**		      to P from this set.  If no such vertex was added, low =-1
** Return Value:index of selected element (-1 if none)
** Side Effects:
** Created:	Kerri Morgan	16th June, 2005
** Modified:	
** Comments:	
** Acknowledgements:
******************************************************************************/
int pickTreeVert1( K_VertInfo* info, int* R, int size, int cur, R_Index* idx, int idxSize, int set, int low)
{
	int i;
	int best =-1;
	int numNf = INT_MAX;
	int flag =-1; 
	int v;

	for(i=cur; i<=idx[set].finish; i++)
	{
		v=R[i];
		/* only create trees */
		if((info[v].inP==FALSE) && (info[v].Nf == info[v].adjTrees.count))
		{
			if(info[v].Nf < numNf)
			{
				/* same number of forest neighbours as previous 
				   selection, so we know it must be best */
				if(info[v].Nf == low)
				{

					return i;
				}
				else /* may be the best */
				{
					best =i;
					numNf = info[v].Nf;
				}
			}
		}
	}

	/** if still in function the best value so far is greater than low.
	 ** check first part of the array to see if earlier index has 
	 ** at least as low value of forest neighbours */
	for(i=idx[set].start; i<cur; i++)
	{
		v=R[i];
		/* only create trees */
		if((info[v].Nf == info[v].adjTrees.count) && (info[v].inP == FALSE))
		{
			if(info[v].Nf < numNf)
			{
				best =i;
				numNf = info[v].Nf;
				if(flag !=1)
				{
					flag =1;
				}
			}
			else if ((info[v].Nf == numNf) && (flag !=1))
			{
				best = i;
				numNf = info[v].Nf;
				flag =1;
			}
		}
	}
	return best;
}   


/******************************************************************************
** Function:	pickTreeVert
** Description:	Function picks a vertex to add to the planar subgraph.  The 
**		vertex will become part of a tree component
** Inputs:	info - information about P
**		R - set of vertices not in P
**		Rsize - number of vertices in R
**		cur - current index in R
**		free - last free index in R
**		low1 - number of trees previous choice was adjacent to	
** Return Value:index of selected element (-1 if none)
** Side Effects:
** Created:	Kerri Morgan	14th June, 2005
** Modified:	low1 - is now a pointer, so value is updated in function calling
**		pickTreeVert
** Comments:	
** Acknowledgements:
******************************************************************************/
int pickTreeVert( K_VertInfo* info, int* R, int Rsize, int cur, int* free, int* low1)
{
	int i;
	int index;
	int best=-1;
	int lowVal=INT_MAX;
	int low;

	low=*low1;
	for(i=cur; i<Rsize; i++)
	{
		index = R[i];
		/* this vertex is only adjacent to one vertex in each tree
		** component */
		if((info[index].Nf == info[index].adjTrees.count) && (info[index].inP == FALSE))
		{
			if(info[index].Nf == low)
			{
				return i;
			}
			else if(info[index].Nf < lowVal)
			{
				best =i;
				lowVal =info[index].Nf;
			}
		}
		R[*free]=R[i];
		*free=(*free)+1;
		if(*free > Rsize) 
		{
			fprintf(stderr, "ERROR 10.3: %d (value for free) exceeds %d (current value)\n", *free, i);
			exit(1);	
		}
	}
	*free=0;
	/* wrap around */
	for(i=0; i<cur; i++)
	{
		index = R[i];
		/* this vertex is only adjacent to one vertex in each tree
		** component */
		if((info[index].Nf == info[index].adjTrees.count) && (info[index].inP == FALSE))
		{
			if(info[index].Nf == low)
			{
				return i;
			}
			else if(info[index].Nf < lowVal)
			{
				best =i;
				lowVal =info[index].Nf;
			}
		}
		R[*free]=R[i];
		*free=(*free)+1;
		if(*free > cur) 
		{
			fprintf(stderr, "ERROR 10.4: %d (value for free) exceeds %d (current value)\n", *free, i);
			exit(1);	
		}
	}
	*low1=lowVal;
	*free =0;
	return best;
}   

/******************************************************************************
** Function:	createIndex
** Description:	Function creates an index into the sorted list R
** Inputs:	G - the graph
**		R - list of vertices ordered by increasing order of degree
**		size - number of vertices in G
**		idxSize - number of different degrees in graph
** Return Value: pointer to index into R
** Side Effects: creates index structure
** Created:	Kerri Morgan	16th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
R_Index* createIndex(Vertex* G, int* R, int size, int* idxSize)
{
	R_Index* index=NULL;
	int i;
	int num =0;
	int curDegree = -1;
	int max;


	/* if the entire graph is d-regular */
	if(G[R[0]].degree ==G[R[size-1]].degree)
	{
		index = (R_Index*)malloc(sizeof(R_Index));
		if(index == NULL)
		{
			fprintf(stderr, "ERROR 10.5: Insufficient memory to create index into R\n");
			exit(1);
		}
		index[0].start=0;
		index[0].finish=size-1;
		index[0].degree = G[0].degree;
		*idxSize =1;
		return index;
	}

	/* how many entries do I need*/
	max = G[R[size-1]].degree;
	for(i=0; i<size; i++)
	{
		if(curDegree < G[R[i]].degree)
		{
			curDegree = G[R[i]].degree;
			num++;
			if(curDegree == max)
			{
				break;
			}
		}
	}
	
	*idxSize = num;

	/* create index*/
	index = (R_Index*)malloc(num*sizeof(R_Index));
	if(index == NULL)
	{
		fprintf(stderr, "ERROR 10.6: Insufficient memory to create index into R\n");
		exit(1);
	}
	
	/* at this point i holds the start of the final section,
	 * and size-1 is the finish of the final section ****/
	index[num-1].start = i;
	index[num-1].degree = max;
	index[num-1].finish = size -1;
	index[num-2].finish = i-1;
	index[0].start =0;
	index[0].degree = G[R[0]].degree;

	/* second last partition */
	i--;
	num = num -2;
	curDegree = G[R[i]].degree;
	if(num == 0)
	{
		i=0;
	}
	while(i>0)
	{
		if(curDegree > G[R[i]].degree)
		{
			index[num].degree = curDegree;
			curDegree = G[R[i]].degree;
			index[num].start = i +1;
			num--; 
			index[num].finish =i;
			if(num==0)
			{
				break;
			}
		}
		i--;
	}
	if(G[R[1]].degree > G[R[0]].degree)
	{
		index[0].finish =0;
		index[1].start=1;
		index[1].degree = G[R[1]].degree;
	}	
	index[0].degree = G[R[0]].degree;
	return index;	
}
/******************************************************************************
** Function:	growOuterplanar
** Description:	Function takes a induced forest subgraph and attempts to 
**		extend it to an outerplanar graph
**	
** Inputs:	G - the graph
** 		size - number of vertices in graph
**		info - information on P
**		R - list of vertices not in P
**		idx - index into R
**		idxSize - size of idx
**		P -size of current planar subgraph
**		component - current next component number
** Return Value:P -size of P
** Side Effects:updates the structures - info, R, idx.
** Created:	Kerri Morgan	22 June 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int growOuterplanar(Vertex* G, int size, K_VertInfo* info, int* R, R_Index* idx, int idxSize, int P, int* component)
{
	int i;
	int v, w;
	int index;
	int low = 0;  
	int set=0;

	/* check each set of vertices */
	for(set = 0; set < idxSize; set++)
	{
		if(idx[set].start <= idx[set].finish)
		{
			index = idx[set].start;
			/* start with no adjacent vertices option*/
			low=-1;
			/* add all possible vertices in this set to P */
			index = pickOPVertex(G,info, R,  index, idx, idxSize, set, &low);
			while(index !=-1)
			{
				v = R[index];
				G[v].member = TRUE;
				info[v].type = NON_TREE;
				info[v].inP = TRUE;

				info[v].component = *component;
				*component = (*component)+1;

				/* mark circuits */
				markAsCircuits(G, info, v);
				for(i=0; i<G[v].degree; i++)
				{
					w=G[v].adjVerts[i];
					/* merging components */
					if((info[w].inP == TRUE) && (info[w].component != info[v].component))
					{
						addToComponent(G, w, info, info[v].component);
					}
					else if(info[w].inP == FALSE)
					{
						insertInList2(&(info[w].adjNonTrees), info[v].component);
						info[w].Nn++;
					}
				}
	
				P++;
				R[index]=R[idx[set].finish];
				if(index == idx[set].finish)
				{
					index = idx[set].start;
				}

				idx[set].finish--;
//printf("***TESTING: Adding vertex %d\n", v);
	//printKInfo(info, size);
/*printf("START: %d FINISH: %d\n", idx[set].start, idx[set].finish);
for(k=0; k<size; k++)
{
	printf("%d ", R[k]);
}	
printf("\n\n"); */
				/* empty set */
				if(idx[set].start > idx[set].finish)
				{
					break;
				}

				index = pickOPVertex(G, info, R,  index, idx, idxSize, set, &low);
			}
		}
	}
	return P;
}

/******************************************************************************
** Function:	addToComponent
** Description:	Function adds vertex w to this component number (removing the
** 		existing component number) and updating the adjacent vertices
**		information
** Inputs:	G - the graph
**		v - vertex to be added to component
**		info - holds information about P
**		component - the component number to which v is being added
** Return Value:none
** Side Effects:updates info
** Created:	Kerri Morgan 	23 June, 2005
** Modified:
** Comments:	The new component is a non-tree component
** Acknowledgements:
******************************************************************************/
void addToComponent(Vertex* G, int v, K_VertInfo* info, int component)
{
	int i;
	int w;
	int old;
	int flagT=0;

	if(info[v].type == FOREST)
	{
		info[v].type = NON_TREE;
		flagT=1;
	}
	old = info[v].component;
	info[v].component = component;
		
	for(i=0; i<G[v].degree; i++)
	{
		w = G[v].adjVerts[i];
		if((info[w].inP == TRUE) && (info[w].component != component))
		{
			addToComponent(G, w, info, component);
		}
		else if(info[w].inP == FALSE)
		{
			if(flagT==1) /* if it was a tree */
			{
				removeFromList2(&info[w].adjTrees, old);
				insertInList2(&info[w].adjNonTrees, component);
				info[w].Nn++;
				info[w].Nf--;
			}
			else
			{
				removeFromList2(&info[w].adjNonTrees, old);
				insertInList2(&info[w].adjNonTrees, component);
			}
		}
	}
}

/******************************************************************************
** Function:	pickOPVertex
** Description:	Function selects a vertex which can be added to P whilst 
**		maintaining outerplanarity
** Inputs:	G - the graph
**		info - contains information about P
**		R - list of vertices not in P
**		index - position in R of the last selection
**		idx - sets of indexes into R
**		idxSize - number of sets in idx
**		set - current set number
**		low - the heuristic value of the previous choice
** Return Value:index into R of selected vertex (-1 if no selection possible)
** Side Effects:
** Created:	Kerri Morgan	23 June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int pickOPVertex(Vertex* G, K_VertInfo* info,int* R, int cur, R_Index* idx,int idxSize,int set,int* low)
{
	int i;
	int best =-1;
	int num = INT_MAX;
	int flag =-1; 
	int v;
	int temp;

	for(i=cur; i<=idx[set].finish; i++)
	{
		v=R[i];
		if(info[v].inP==FALSE) 
		{
			if(checkAdjComp(info, v) == TRUE)
			{
				temp= legalOP_Add( G, info, v);
				if(temp < num)
				{
					if(temp == (*low))
					{
						return i;
					}
					else /* may be the best */
					{
						best =i;
						num = temp;
					}
				} 
				
			}
		}
	}

	/** if still in function the best value so far is greater than low.
	 ** check first part of the array to see if earlier index has 
	 ** at least as low value of forest neighbours */
	for(i=idx[set].start; i<cur; i++)
	{
		v=R[i];


		if(info[v].inP == FALSE)
		{
			if(checkAdjComp(info, v) == TRUE)
			{

				temp= legalOP_Add( G, info, v);
				if(info[v].Nf < num)
				{
					best =i;
					num = temp;
					if(flag !=1)
					{
						flag =1;
					}
				}
				else if ((temp == num) && (flag !=1))
				{
					best = i;
					num = temp;
					flag =1;
				}
			}
		}
	}
	*low = num;
	return best;
}   

/******************************************************************************
** Function:	checkAdjComp
** Description:	Function checks for a given vertex that each component 
**		has at most two vertices adjacent to this vertex
** Inputs:	info - information about P
**		v - the vertex concerned
** Return Value:  TRUE - if the above statement is true, otherwise FALSE
** Side Effects:
** Created:	Kerri Morgan	23 June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int checkAdjComp(K_VertInfo* info, int v)
{
	C_Node* cur = NULL;

	/* if we have at most 1 more neighbours than adjacent components
	 * then we know that one component may have 2 neighbours, 
	 * but the rest have only 1 neighbour */
	if((info[v].Nn <= info[v].adjNonTrees.count +1) && (info[v].Nf <= info[v].adjTrees.count+1))
	{
		return TRUE;
	}	
	cur = info[v].adjNonTrees.startPtr;
	while(cur != NULL)
	{
		if(cur->numAdjacent > 2)
		{
			return FALSE;
		}
		cur = cur->next;
	}

	cur = info[v].adjTrees.startPtr;
	while(cur != NULL)
	{
		if(cur->numAdjacent > 2)
		{
			return FALSE;
		}
		cur = cur->next;
	}
	return FALSE;
}

/******************************************************************************
** Function:	numCycleNeighbours
** Description:	Function counts the number of cycle neighbours of vertex v
** Inputs:	G - the graph
**		info - contains information about P
**		v - vertex
** Return Value: the number of neighbours in P that are part of a cycle
**
** Side Effects: none
** Created: 	Kerri Morgan 20th April, 2006
** Modified:
******************************************************************************/
int numCycleNeighbours(Vertex* G, K_VertInfo* info, int v)
{
	int i;
	int num=0;
	int w;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if((info[w].inP==TRUE) && (info[w].cycle == TRUE))
		{
			num++;
		}
	}
	return num;
}

/******************************************************************************
** Function:	pathFinder
** Description:	Function finds a path from vertex x to vertex y using only 
**		vertices in the current component and at most one vertex 
**		that is a member of a cycle (within component).
** Inputs:	G - the graph
**		info - contains information about P
**		x, y - vertices defining start and finish of remaining path
**		c - flag inicating if the current path contains a vertex that
**		is a member of a cycle
**		curPath - list of vertices currently in path
** Return Value: TRUE - if path found, otherwise FALSE.
** Side Effects:updates curPath
** Created:	Kerri Morgan	23 June, 2005
** Modified:	Kerri Morgan    20 April, 2006
**		Now checks that there are no additional cycles other than the 
**		current one.
** Comments:	
** Acknowledgements:
******************************************************************************/
int pathFinder(Vertex* G, K_VertInfo* info, List* Path, int x, int y, int c)
{
  	int i;
        int w;
	int j, wx;

        if(info[x].component != info[y].component)
        {
                fprintf(stderr, "ERROR 10.7: Attempting to find a path between vertices in different components (%d -- %d)\n", x, y);
		exit(1);
        }

        /* same component and adjacent vertices */
        if(adjacentVertices(G, x,y))
        {
		/* changes here */
		if(info[y].cycle == FALSE) /* not a cycle vertex */
		{
                	insertAtEnd(Path, y);
			return TRUE;
		}
		else if(c==0) /* no previous cycle vertex */
		{
			insertAtEnd(Path, y);
			return TRUE;
		}
		else 
		{
			if(numCycleNeighbours(G, info, y)<=2) /* only one cycle */
			{
				insertAtEnd(Path, y);
				return TRUE;
			}
			else /* multiple cycles */
			{
				insertAtEnd(Path, y);
				return FALSE;
			}
		}
        }

        for(i=0; i<G[x].degree; i++)
        {                      
         	w=G[x].adjVerts[i];
        	/* w is not already in path AND is in same component */
        	if((info[x].component==info[w].component) && (inList(Path, w)==FALSE))
                {
			if((info[w].cycle == TRUE) && (c== 0)) /* no cycle previously found */
			{ 
                        	insertAtEnd(Path, w);
                        	if(pathFinder( G,  info, Path, w, y, 1)==TRUE)
                        	{
                               	 	return TRUE;
                        	}
				else  if(numCycleNeighbours(G, info, w)<=2) /* only one cycle involved here -try to
									       involve the next vertex in path */
				/* two cycle vertices in a row may help */
				{
					
					for(j=0; j<G[w].degree; j++)
					{
						wx = G[w].adjVerts[j];
						if((info[w].component == info[wx].component) && (inList(Path, wx)==FALSE))
						{
							if(info[wx].cycle == TRUE)
							{
								/* check if this is the other end of the cycle*/
								if(numCycleNeighbours(G, info, wx)<=2)
								{
									insertAtEnd(Path, wx);
									if(pathFinder(G, info, Path, wx, y, 1)== TRUE)
									{
										return TRUE;
									}
									removeFromList(Path, wx);
								}
							}
						}
					}
					removeFromList(Path, w);
					
				}
					/* old else	
                        	else
                        	{
                               	 	removeFromList(Path, w);
                        	}*/
                	}
			else if (info[w].cycle == FALSE)
			{
                        	insertAtEnd(Path, w);
                        	if(pathFinder( G,  info, Path, w, y, c)==TRUE)
                        	{
                               	 	return TRUE;
                        	}
                        	else
                        	{
                               	 	removeFromList(Path, w);
                        	}
			}
		}
        }
        return FALSE;        
}

/******************************************************************************
** Function:	legalOP_Add
** Description: Function evaluates if a vertex can be added to a component 
** 		whilst maintaining outerplanarity
** Inputs:	G - the graph
** 		size - number of vertices in graph
**		info - contains information about P
**		v - vertex 	
**
** Return Value:TRUE - if vertex can be added, else FALSE
** Side Effects:
** Created:	Kerri Morgan	23rd June, 2005
** Modified:
** Comments:	Function ensures that any two vertices in a component that
**		are adjacent to v, are connected by a path with a most 1 cycle
**		vertex in it	
** Acknowledgements:
******************************************************************************/
int legalOP_Add(Vertex* G,  K_VertInfo* info, int v)
{
	List L;
	C_Node* cur = NULL;
	int numCycles = 0;	
	int x, y;
	int lengthCycles=0;
	int comp;

	cur = info[v].adjNonTrees.startPtr;
	while(cur != NULL)
	{
		if(cur->numAdjacent == 2)
		{
			comp = cur->component;
			initList(&L);
			getAdjacentPair(G,info,  v, &x, &y, comp);	
			if((x==-1) || (y==-1))
			{
				fprintf(stderr, "ERROR 10.8: Unable to find two vertices adjacent to %d in component %d\n", v, comp);
				exit(1);
			}	
			if(pathFinder(G, info, &L, x, y, 0)== TRUE)
			{
				numCycles++;
				lengthCycles += L.degree;
				killList(&L);
			}
			else
			{
				killList(&L);
				return -1;
			}
		}
		cur = cur->next;
	}

	cur = info[v].adjTrees.startPtr;
	while(cur != NULL)
	{
		if(cur->numAdjacent == 2)
		{
			initList(&L);
			comp = cur->component;
			getAdjacentPair(G,info, v, &x, &y, comp);	
			if((x==-1) || (y==-1))
			{
				fprintf(stderr, "ERROR 10.8: Unable to find two vertices adjacent to %d in component %d\n", v, comp);
				exit(1);
			}	
			if(pathFinder(G,  info, &L, x, y, 0)== TRUE)
			{
				numCycles++;
				lengthCycles += L.degree;
				killList(&L);
			}
			else
			{
				killList(&L);
				return -1;
			}
		}
		cur = cur->next;
	}
	return numCycles; //*10 + lengthCycles;
//	return lengthCycles;
}
/******************************************************************************
** Function:	getAdjacentPair
** Description:	Function finds two vertices (x and y) in component c
**		adjacent to vertex v
** Inputs:	G - the graph
**		info - information about P
**		v,x,y - vertices
**		comp - component number
** Return Value: none
** Side Effects:The vertex numbers of the found vertices are assigned to x and y
**		(-1 if none found)
** Created:	Kerri Morgan	23 June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void getAdjacentPair(Vertex* G, K_VertInfo* info, int v, int* x, int* y, int comp)
{
	int i;
	int j=0;
	int w;
	int found[2];

	found[0] =-1;
	found[1] =-1;
	
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		
		if(info[w].component == comp)
		{
			found[j]=w;
			j++;
			if(j == 2)
			{
				break;
			}
		}
	}
	*x=found[0];
	*y=found[1];
}

/******************************************************************************
** Function:	markAsCiruits
** Description:	Function notes all vertices involved in a circuit once vertex
**		v is added
** Inputs:	G - graph
**		info - information about P
**		v - the vertex whose addition forms the circuits. 
** Return Value:none
** Side Effects:updates information in info
** Created:	Kerri Morgan	24 June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void markAsCircuits(Vertex* G, K_VertInfo* info, int v)
{
	int x, y;
	List L;
	C_Node* cur = NULL;
	Node* ptr = NULL;
	int comp;
	int w;

	cur = info[v].adjNonTrees.startPtr;
	/* all non tree components adjacent to v*/
	while(cur != NULL)
	{
		/* new circuits */
		if(cur->numAdjacent == 2)
		{
			initList(&L);
			comp = cur->component;
			getAdjacentPair(G,info,  v, &x, &y, comp);	
			if((x==-1) || (y==-1))
			{
				fprintf(stderr, "ERROR 10.9: Unable to find two vertices adjacent to %d in component %d\n", v, comp);
				exit(1);
			}	
			if(pathFinder(G, info, &L, x, y, comp)== TRUE)
			{
				ptr=L.startPtr;
				while(ptr!=NULL)
				{
					w = ptr->vertex;
					info[w].cycle = TRUE;
					ptr=ptr->next;	
				}		
				killList(&L);
			}
			else	/* adding v does not introduce a circuit*/
			{
				killList(&L);
			}
		}
		cur = cur->next;
	}

	cur = info[v].adjTrees.startPtr;
	/* all tree components adjacent to v*/
	while(cur != NULL)
	{
		/* cycles introduced */
		if(cur->numAdjacent == 2)
		{
			initList(&L);
			comp = cur->component;
			getAdjacentPair(G,info, v, &x, &y, comp);	
			if((x==-1) || (y==-1))
			{
				fprintf(stderr, "ERROR 10.9: Unable to find two vertices adjacent to %d in component %d\n", v, comp);
				exit(1);
			}	
			if(pathFinder(G,  info, &L, x, y, comp)== TRUE)
			{
				ptr=L.startPtr;
				while(ptr!=NULL)
				{
					w = ptr->vertex;
					info[w].cycle = TRUE;
					ptr=ptr->next;	
				}		
				killList(&L);
			}
			else	/* adding v does not introduce a circuit*/
			{
				killList(&L);
			}
		}
		cur = cur->next;
	}
	info[v].cycle=TRUE;

}
/******************************************************************************
** Function:	createReducedGraph
** Description: Function locates the reduced graph of P.
**		Reductions are done by the following rules:
**		i) if vertex v is adjacent to only one vertex namely vertex w 
**		   in P,(or the partially reduced graph of P), 
**		   then remove vertex v and edge vw.
**		ii) if vertex v is adacent to exactly two vertices in P,
**		   namely w and x, and the edge wx is not in P (or partially 
**		   reduced graph of P)
** Inputs:	G - the graph
**		size - number of vertices in the graph
**		PSize - number of vertices in P
**		info - holds information about P
** Return Value:MegaVerts - an array of information about the megavertices (the
**		vertices of P that remain in the reduced graph).  Each 
**		megavertex contains a non-empty subset of vertices of P.
** Side Effects:
** Created:	Kerri Morgan	5th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
RGVert*  createReducedGraph(Vertex* G, int size, int Psize, K_VertInfo* info)
{
	RGVert* MegaVerts = NULL;
	/* create an array to store information about megavertices */
	MegaVerts = createMegaVerts(info, size, Psize);
	
	/* create a graph that is induced by vertices in P */
	initReducedGraph(G, size);

	/* reduce it */
	reduceGraph(G, size, info, MegaVerts);

	//unInitReducedGraph(G,size);
	

	return MegaVerts;

}

/******************************************************************************
** Function:	createMegaVerts
** Description:	Function creates an array containing information for each
**		vertex in P concerning their megavertex (or megavertices) in 
**		the reduced graph.
**		Each vertex is initially a megavertex.
** Inputs:	info - information about P
**		size - number of vertices in original graph
**		Psize - number of vertices in P
** Return Value:MegaVerts - array of information about megavertices
** Side Effects:initialises MegaVerts
** Created:	Kerri Morgan	5th July, 2005
** Modified:
** Comments:	The index j is the location in the array of megavertices
**		of vertex i
** Acknowledgements:
******************************************************************************/
RGVert* createMegaVerts(K_VertInfo* info, int size, int Psize)
{
	int i,j;
	RGVert* MegaVerts=NULL;

	MegaVerts=(RGVert*)malloc(sizeof(RGVert)*Psize);
	if(MegaVerts==NULL)
	{
		fprintf(stderr, "ERROR 10.10: Unable to create array of megavertices\n");
		exit(1);
	}

	j=0;
	/* find the vertices in P */
	for(i=0; i<size; i++)
	{
		if(info[i].inP == TRUE)
		{
			if(j == Psize)
			{
				printKInfo(info, size);
				fprintf(stderr, "ERROR 10.11: Incorrect size given for P (%d)\n", Psize);
				exit(1);
			}
			MegaVerts[j].type = MEGAVERT;
			MegaVerts[j].vertex = i;
			info[i].RG_Index = j;
			initList(&(MegaVerts[j].vertList));
			insertInList(&(MegaVerts[j].vertList), i);
			j++;
		}
	}
	if(j!=Psize)
	{
		fprintf(stderr, "ERROR 10.12: Incorrect size given for P (%d compared to %d)\n", j, Psize);
		exit(1);

	}
	return MegaVerts;
}

/******************************************************************************
** Function:	extendGraph
** Description:	Function takes an outerplanar graph and attempts to extend it
**	
** Inputs:	G - the graph
** 		size - number of vertices in graph
**		info - information on P
**		R - list of vertices not in P
**		idx - index into R
**		idxSize - size of idx
**		P -size of current planar subgraph
**		component - current next component number
** Return Value:P -size of P
** Side Effects:updates the structures - info, R, idx.
** Created:	Kerri Morgan	5 July 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int extendGraph(Vertex* G, int size, K_VertInfo* info, int* R, R_Index* idx, int idxSize, int P, int* component)
{
	RGVert* megaVerts=NULL;
	int i;
	int index;
	int low = 0;  
	int set=0;
	int v,w;
	
	/* create initial groups of megavertices*/
	megaVerts=createReducedGraph(G, size, P, info);

	#ifdef DEBUG
	printMegaList(megaVerts, P);
	#endif 

	/* check each set of vertices */
	for(set = 0; set < idxSize; set++)
	{
		if(idx[set].start <= idx[set].finish)
		{
			index = idx[set].start;
			
			/* start with no adjacent vertices option*/
			low=-1;
		
			/* add all possible vertices in this set to P */
			index = findVert2Add(G,info, R,  index, idx, idxSize, set, &low, megaVerts, P);
			while(index !=-1)
			{
/* review this section to merge several components to one  and update information of nonP neighbours */
				#ifdef DEBUG
				printf("TESTING:::: found %d to add to graph \n", R[index]);
				#endif
			
				v = R[index];
				G[v].member = TRUE;
				info[v].type = NON_TREE;
				info[v].inP = TRUE;

				info[v].component = *component;
				*component = (*component)+1;

				/* for all vertices adjacent to v */
				for(i=0; i<G[v].degree; i++)
				{
					w=G[v].adjVerts[i];
					/* neighbour is already in P - merge components */
					if((info[w].inP == TRUE) && (info[w].component != info[v].component))
					{
						addToComponent(G, w, info, info[v].component);
					}
					else if(info[w].inP == FALSE) /* neighbour is not in P - increment nonTree neighbours */
					{
						insertInList2(&(info[w].adjNonTrees), info[v].component);
						info[w].Nn++;
					}
				}
	
				/* remove old reduced graph */
				for(i=0; i<P; i++)
				{
					killList(&(megaVerts[i].vertList));
				}
				for(i=0; i<size; i++)
				{
					killList(&(G[i].adjVertices));
				}
				P++; 
				free(megaVerts);
				megaVerts = NULL;

				/* destroy old reduced graph */
				unInitReducedGraph(G,size);

				/* create new groups of megavertices*/
				megaVerts=createReducedGraph(G, size, P, info);
			
				#ifdef DEBUG
				printMegaList(megaVerts, P);
				printGraph(G, size);
				#endif

				/* remove the vertex that has been added to P */
				R[index]=R[idx[set].finish];
				if(index == idx[set].finish)
				{
					index = idx[set].start;
				}
				idx[set].finish--;
				
				/* empty set */
				if(idx[set].start > idx[set].finish)
				{
					break;
				}


				index = findVert2Add(G,info, R,  index, idx, idxSize, set, &low, megaVerts, P);
			}
		}
	}

	/***TESTING - attempt to remove a vertex here and then recursively
	 * call extendGraph (put removed vertex in after found another to add
	 * if exists another vertex to add, else
	 * put this vertex back and continue */
	/* only do if megaVerts !=NULL */
	
	/* remove old reduced graph */
	for(i=0; i<size; i++)
	{
		killList(&(G[i].adjVertices));
	}
	if(megaVerts !=NULL)
	{
		#ifdef DEBUG
			printMegaList(megaVerts, P);
		#endif
		for(i=0; i<P; i++)
		{
			killList(&(megaVerts[i].vertList));
		}
	
		free(megaVerts);
	}
	return P;
}


/******************************************************************************
** Function:	initReducedGraph
** Description:	Function creates an induced subgaph <P> to be used in the 
**		reduction process
** Inputs:	G - the graph
**		size - number of vertices in graph
** Return Value:none
** Side Effects:updates linked list structure in G
** Created:	Kerri Morgan	5th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void initReducedGraph(Vertex* G, int size)
{
	int i,j;
	int w;

	for(i=0; i<size; i++)
	{
		if(G[i].member == KNOWN)
		{
			for(j=0; j<G[i].degree; j++)
			{
				w=G[i].adjVerts[j];
				if(G[w].member == KNOWN)
				{
					insertInList(&G[i].adjVertices, w);
				}
			}
		}
		else /* vertex is not in reduced graph (we show this by giving it degree 0 */
		{
			G[i].adjVertices.degree=0;
		}
	}
}

/******************************************************************************
** Function:	reduceGraph
** Description: Functon reduces the graph by the following operations:
**		i) if vertex v is adjacent to only one vertex namely vertex w 
**		   in P,(or the partially reduced graph of P), 
**		   then remove vertex v and edge vw.
**		ii) if vertex v is adacent to exactly two vertices in P,
**		   namely w and x, and the edge wx is not in P (or partially 
**		   reduced graph of P)
** Inputs:	G - the graph (contains the subgaph <P>)
**		size - number of vertices in G
**		info - contains information about P
**		MegaVerts - contains information about megavertices in reduced
**		graph
** Return Value:none
** Side Effects:updates the linked list structure in G, megaVerts structure
**		and info's index into megaVerts structure
** Created:	Kerri Morgan	5th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void reduceGraph(Vertex* G, int size, K_VertInfo* info, RGVert* MegaVerts)
{
	int i;
	int v,w;
	Node* cur=NULL;

	for(i=0; i<size; i++)
	{
		if(G[i].adjVertices.degree == 1)
		{
			cur = G[i].adjVertices.startPtr;
			if(cur == NULL)
			{
				fprintf(stderr, "ERROR: No vertices in list but %d has degree 1\n", i);
				exit(1);
			}
			v=cur->vertex;
			reduceOp1(G, size,info, MegaVerts, i, v);
		}
		else if(G[i].adjVertices.degree ==2)
		{
			cur = G[i].adjVertices.startPtr;
			v=cur->vertex;
			cur=cur->next;
			if(cur == NULL)
			{
				fprintf(stderr, "ERROR : Only one vertex in list when %d has degree 2\n", i);
				exit(1);
			}
			w=cur->vertex;
			/* check no edge between v and w */
			if(inList(&(G[w].adjVertices), v) == FALSE)
			{
				reduceOp2(G, size,info, MegaVerts, i, v, w);
			}
		}
	}
}
/******************************************************************************
** Function:	reduceOp1
** Description:	Function reduces graph by removing vertex v and edge vw
** Inputs:	G - the graph
**		size - number of vertices in original graph G
**		info - contains information about P
**		MegaVerts - contains information about megavertices
**		v - vertex being removed
**		w - vertex adjacent to v
** Return Value:none
** Side Effects:updates linked lists in G, info's RG_Index and MegaVerts' 
** information
** Created:	Kerri Morgan	5th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void reduceOp1(Vertex* G, int size, K_VertInfo* info, RGVert* MegaVerts, int v,int w)
{
	int vx,wx;
	Node* cur=NULL;
	int idx1, idx2, idx3;

	//printf("TESTING operation 1 on vertex %d adjacent to %d\n", v, w);

	idx1 = info[v].RG_Index;	
	idx2 = info[w].RG_Index;	

	if(MegaVerts[idx1].type != MEGAVERT)
	{
		fprintf(stderr, "ERROR 10.13: Function is attempting to reduce a vertex (%d) that is not a megavertex\n", v);
		exit(1);
	}
	
	cur = MegaVerts[idx1].vertList.startPtr;
	while(cur!=NULL)
	{
		insertInList(&(MegaVerts[idx2].vertList), cur->vertex);
		idx3=info[cur->vertex].RG_Index;
		if(idx3 != idx1)
		{
			/* this vertex has a new megavertex
			 * - replace it in list*/
			removeFromList(&(MegaVerts[idx3].vertList), v);
			insertInList(&(MegaVerts[idx3].vertList), w);
		}
		cur = cur->next;
	}
	/* no edge vw */
	removeFromList(&G[v].adjVertices, w);
	removeFromList(&G[w].adjVertices, v);
	killList(&G[v].adjVertices);	
	/* v now has vertex w as its megavertex */
	killList(&(MegaVerts[idx1].vertList));		
	insertInList(&(MegaVerts[idx1].vertList), w);
	/* vertex v is no longer a megavertex */
	MegaVerts[idx1].type =NON_MEGAVERT;


	/* has this reduction made vertex w reducible? */
	if(G[w].adjVertices.degree == 1)
	{
		cur = G[w].adjVertices.startPtr;
		vx=cur->vertex;

		reduceOp1(G, size, info, MegaVerts, w,vx);
	}
	else if(G[w].adjVertices.degree == 2)
	{
		cur = G[w].adjVertices.startPtr;
		vx=cur->vertex;
		cur = cur->next;
		wx=cur->vertex;
		/* if no edge between vx and wx */
		if(inList(&(G[vx].adjVertices), wx)==FALSE)
		{
			reduceOp2(G, size,info, MegaVerts, w, vx, wx);
		}
	}
}

/******************************************************************************
** Function:	reduceOp2
** Description:	Function reduces graph by removing vertex v and edges vw and vx
** Inputs:	G - the graph
**		size - number of vertices in original graph G
**		info - contains information about P
**		MegaVerts - contains information about megavertices
**		v - vertex being removed
**		w,x - vertex adjacent to v
** Return Value:none
** Side Effects:updates linked lists in G, info's RG_Index and MegaVerts' 
** information
** Created:	Kerri Morgan	5th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void reduceOp2(Vertex* G, int size, K_VertInfo* info, RGVert* MegaVerts, int v,int w, int x)
{
	int idx1, idx2, idx3, idx4;
	Node* cur=NULL;
	int vx, wx;

	//printf("TESTING operation 2 on vertex %d adjacent to %d\n", v, w);
	/* v is no longer adjacent to w and x */
	removeFromList(&G[w].adjVertices, v);
       removeFromList(&G[x].adjVertices, v);
	
	/* w and x are no longer adjacent to v */
	removeFromList(&G[v].adjVertices, w);
	removeFromList(&G[v].adjVertices, x);
	
	/* x is now adjacent to w*/
	insertInList(&G[x].adjVertices, w);
	/* w is now adjacent to x */
	insertInList(&G[w].adjVertices, x);
	
	idx1=info[v].RG_Index;
	idx2 = info[w].RG_Index;	
	idx3 = info[x].RG_Index;	
	
	if(MegaVerts[idx1].type != MEGAVERT)
	{
		fprintf(stderr, "ERROR 10.14: Function is attempting to reduce a vertex (%d) that is not a megavertex\n", v);
		exit(1);
	}
	
	cur = MegaVerts[idx1].vertList.startPtr;
	while(cur!=NULL)
	{
		insertInList(&(MegaVerts[idx2].vertList), cur->vertex);
		insertInList(&(MegaVerts[idx3].vertList), cur->vertex);
		idx4=info[cur->vertex].RG_Index;
		if(idx4 != idx1) /* don't alter idx1 until completed changeover*/
		{
			/* this vertex has two new megavertices
			 * instead of vertex v as a megavertex */
			removeFromList(&(MegaVerts[idx4].vertList), v);
			insertInList(&(MegaVerts[idx4].vertList), w);
			insertInList(&(MegaVerts[idx4].vertList), x);
		}
		cur = cur->next;
	}

	/* v now has vertex w as its megavertex */
	killList(&(MegaVerts[idx1].vertList));		
	insertInList(&(MegaVerts[idx1].vertList), w);
	insertInList(&(MegaVerts[idx1].vertList), x);
	/* vertex v is no longer a megavertex */
	MegaVerts[idx1].type =NON_MEGAVERT;

	/* has this reduction made vertex w reducible? */
	if(G[w].adjVertices.degree == 1)
	{
		cur = G[w].adjVertices.startPtr;
		vx=cur->vertex;

		reduceOp1(G, size, info, MegaVerts, w,vx);
	}
	else if(G[w].adjVertices.degree == 2)
	{
		cur = G[w].adjVertices.startPtr;
		vx=cur->vertex;
		cur = cur->next;
		wx=cur->vertex;
		/* if no edge between vx and wx */
		if(inList(&(G[vx].adjVertices), wx)==FALSE)
		{
			reduceOp2(G, size,info, MegaVerts, w, vx, wx);
		}
	}
	/* has this reduction made vertex x reducible? */
	if(G[x].adjVertices.degree == 1)
	{
		cur = G[x].adjVertices.startPtr;
		vx=cur->vertex;

		reduceOp1(G, size, info, MegaVerts, x,vx);
	}
	else if(G[x].adjVertices.degree == 2)
	{
		cur = G[x].adjVertices.startPtr;
		vx=cur->vertex;
		cur = cur->next;
		wx=cur->vertex;
		/* if no edge between vx and wx */
		if(inList(&(G[vx].adjVertices), wx)==FALSE)
	{
			reduceOp2(G, size,info, MegaVerts, x, vx, wx);
		}
	}

}

/******************************************************************************
** Function:	unInitReducedGraph
** Description: Function frees all memory of linked list structure in G
** Inputs:	G - the graph
**		size - number of vertices in graph
** Return Value:none
** Side Effects:frees memory in G
** Created:	Kerri Morgan	5th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void unInitReducedGraph(Vertex* G,int size)
{
	int i;

	for(i=0; i<size; i++)
	{
		killList(&G[i].adjVertices);
	}
}

/******************************************************************************
** Function:	printMegaList
** Description: Function prints list of megavertice information
** Inputs:	megaVert - list of information about megavertices
**		size - number of elments in the list
** Return Value:none
** Side Effects:
** Created:	Kerri Morgan	6th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void printMegaList(RGVert* megaVert, int size)
{
	int i;

	for(i=0; i<size; i++)
	{
		if(megaVert[i].type == MEGAVERT)
		{
			printf("Megavertex %d containing: ", megaVert[i].vertex);
			printList(&(megaVert[i].vertList));
		}
		else
		{
			printf("Non-megavertex %d belongs to following megavertices: ", megaVert[i].vertex);
			printList(&(megaVert[i].vertList));
		}
	}
}

/******************************************************************************
** Function:	locateMegaVertices
** Description:	Function locates all the megavertices adjacent to a vertex v in
**		a given component in the reduced graph.
** Inputs:	v - the vertex
**		G - the graph
**		info - holds information about P
**		megaVerts - list of information about megavertices
**		L - list to hold the required megavertices
**		compNum - number of the component
**		E - array to hold mandatory edges
** Return Value:numEdges
** Side Effects:updates L
** Created:	Kerri Morgan	6th July, 2005
** Modified:	9th September, 2005 - Modified to also produce a list of
**		edges between the megavertices.  
**		In the case where a vertex in P is not a megavertex and
**		is in the set of two megavertices (M1 and M2),
**		any path found in the
**		M_MIPS algorithm must include {M1,M2} as an edge.
**		An additional parameter E is included to keep a list of
**		such vertices.
** Comments:	
** Acknowledgements:
******************************************************************************/
int locateMegaVertices(Vertex* G, K_VertInfo* info,  int v, RGVert* megaVert, List* L, int compNum, MV_Edge* E)
{
	int i,j;
	int w, v1, v2;
	int idx;
	MV_Edge mvEdge;
	Node* cur = NULL;
	Node* cur2 = NULL;
	int numEdges=0;


	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if(info[w].component == compNum)
		{
			idx = info[w].RG_Index;
			/* if node is a megavertex add it to the list */
			if(megaVert[idx].type == MEGAVERT)
			{
				insertInList(L, megaVert[idx].vertex);
			}
			else /* add all vertices in it's megavertex list */
			{
				addList(L, &(megaVert[idx].vertList));
				
				/* a vertex may have belong to two or one
				 * megavertex set.  
				 * If it belongs to two, get the edge between
				 * these two vertices*/
				if(getMVEdge(&(megaVert[idx].vertList), &mvEdge)==1)
				{ 
					/* if the edge is not already in E,
					 * add it */
					j=MVEdge_exists(E, mvEdge, G[v].degree);
					if(j>=G[v].degree) /* overrun our array size */
					{
						fprintf(stderr, "ERROR 10.23: Function has found more edges than degree of vertex\n");
						exit(1);
					}
					else if(E[j].v1== -1) /* no vertex stored here*/
					{
						E[j].v1 = mvEdge.v1;
						E[j].v2=mvEdge.v2;
						numEdges++;
					}

				}
			}	
		}
	}
#ifdef DEBUG
	printf("TEST***\n");
printList(L);
printMVEdges(E, G[v].degree);
#endif
	/* weed out any little triangles
	 * If there exists a subgraph v1,v2,v3 that is completely connected,
	 * and two of these vertices say v1 and v2 have degree 2 in the 
	 * reduced graph, then it suffices to have our path include vertex
	 * v3, the vertex of higher degree */
	cur = L->startPtr;
	while(cur != NULL)
	{
		w = cur->vertex;
		/* does this vertex have degree 2 in reduced graph */
		if(G[w].adjVertices.degree == 2)
		{
			cur2 = G[w].adjVertices.startPtr;
			if(cur2!=NULL)
			{
				v1 =cur2->vertex;
				cur2=cur2->next;
				if(cur2 !=NULL)
				{
					v2 = cur2->vertex;
				}
				else
				{
					fprintf(stderr, "Error 10.24: Vertex %d is adjacent to two vertices in reduced graph, but list contains less than this number\n", w);
					exit(1);	
				}
			}
			else
			{
				fprintf(stderr, "Error 10.24: Vertex %d is adjacent to two vertices in reduced graph, but list contains less than this number\n", w);
				exit(1);	
			}
			/* avoid deleting all three vertices from list */
			if((G[v1].adjVertices.degree == 2) && (G[v2].adjVertices.degree == 2))
			{
				if(checkAdjacent(G, v1,v2)==TRUE)
				{
					/* leave w in list - get rid of any others */

					/** w----v1 **/
					#ifdef DEBUG
					printf("*LITTLE TRIANGLE %d %d %d\n", w, v1, v2);
					#endif
					cur = cur->next;
					/* may need to step over one or two vertices in list*/
					if(cur != NULL)
					{
						if((cur->vertex == v1) || (cur->vertex == v2))
						{
							cur = cur->next;
							if(cur != NULL)
							{
								if((cur->vertex == v1) || (cur->vertex == v2))
								{
									cur = cur->next;
								}
							}
						}
					}
				
					/* remove from list v1 and v2 and leave w */
					removeFromList(L, v1);
					removeFromList(L, v2);
					/* remove edges v1,v2; v1,w; v2,w */
					numEdges-=removeEdges(E, G[v].degree,v1,v2, w, numEdges);
					
				}
				else
				{
					cur = cur->next;
				}

			}	
			else if(G[v1].adjVertices.degree == 2)
			{
				/** w----v1 **/
				if(checkAdjacent(G, v1,v2)==TRUE)
				{
					#ifdef DEBUG
					printf("**LITTLE TRIANGLE %d %d %d\n", w, v1, v2);
					#endif
					cur = cur->next;
					if((cur != NULL) && (cur->vertex == v1))
					{
						cur = cur->next;
					}
					/* remove w and v1 from list and insert v2 */
					removeFromList(L, v1);
					removeFromList(L, w);
					insertInList(L, v2);
					/* remove edges v1,v2; v1,w; v2,w */
					numEdges-=removeEdges(E, G[v].degree,v1,v2, w, numEdges);
					
				}
				else
				{
					cur = cur->next;
				}
			}
			else if(G[v2].adjVertices.degree == 2)
			{
				/** w----v2 **/
				if(checkAdjacent(G, v1,v2)==TRUE)
				{
					#ifdef DEBUG
					printf("***LITTLE TRIANGLE %d %d %d\n", w, v1, v2);
					#endif
					cur = cur->next;
					if((cur != NULL) && (cur->vertex == v2))
					{
						cur = cur->next;
					}
					/* remove w and v2 from list and insert v1 */
					removeFromList(L, v2);
					removeFromList(L, w);
					/* insert v1 */
					insertInList(L, v1);
					/* remove edges v1,v2; v1,w; v2,w */
					numEdges-=removeEdges(E, G[v].degree,v1,v2, w, numEdges);
					
				}
				else
				{
					cur = cur->next;
				}
			}
			else
			{
				cur=cur->next;
			}	
		}
		else
		{	
			cur = cur->next;
		}
	}
#ifdef DEBUG
printf("TEST\n");
printMVEdges(E, G[v].degree);
printList(L);
#endif
	return numEdges;
}

/******************************************************************************
** Function:	findVert2Add
** Description:	Function selects a vertex that can be added to P whilst 
**		maintaining planarity
** Inputs:	G - the graph
**		info - contains information about P
**		R - list of vertices not in P
**		index - position in R of the last selection
**		idx - sets of indexes into R
**		idxSize - number of sets in idx
**		set - current set number
**		low - the heuristic value of the previous choice
**		megaVerts - contains information about megavertices
**		mSize - size of megaVerts
** Return Value:index into R of selected vertex (-1 if no selection possible)
** Side Effects:
** Created:	Kerri Morgan	6th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int findVert2Add(Vertex* G, K_VertInfo* info,int* R, int cur, R_Index* idx,int idxSize,int set,int* low, RGVert* megaVerts, int mSize)
{
	int i;
	int best =-1;	
	int num = INT_MAX;
	int flag =-1; 
	int v;

	/* add vertices in current set in order of number of edges that will be added to P
	 * when they are added */
	for(i=cur; i<=idx[set].finish; i++)
	{
		v=R[i];
		if(info[v].inP==FALSE) 
		{
			#ifdef DEBUG
			printf("TEST: consider vertex %d with %d neighbours, but %d is best\n", v, info[v].Nn+info[v].Nf, num);
			#endif
			/* does this vertex have less neighbours in P, than
			 * a previous best */
			if((info[v].Nn + info[v].Nf) < num)
			{
				#ifdef DEBUG
				printf("TEST: vertex has lower neighbours\n");
				#endif
				if(checkAdd(G, info, megaVerts, mSize, v)==TRUE)
				{
					#ifdef DEBUG
					printf("Passed checkAdd process\n");
					#endif
					num = info[v].Nn+info[v].Nf;
					best =i;
					/* the check method is too strict - in some
					 * cases it prevents a vertex being added that
					 * may be addable.  This may change as other
					 * vertices are added and the reduced graph alters*/
					if(num <= *low)
					{
						return best;
					}
				}
			}
		}
	}
	/* did not find a vertex of same value */
	for(i=idx[set].start; i<cur; i++)
	{
		v=R[i];
		if(info[v].inP == FALSE)
		{
			if((info[v].Nn+info[v].Nf) <= num)
			{
				if(checkAdd(G, info, megaVerts, mSize, v)==TRUE)
				{
					if((info[v].Nn+info[v].Nf) < num)
					{
						best =i;
						num = info[v].Nn+info[v].Nf;
						if(flag !=1)
						{
							flag =1;
						}
					}
					else if(((info[v].Nn+info[v].Nf) == num) && (flag !=1))
					{
						best = i;
						num = info[v].Nn+info[v].Nf;
						flag =1;
					}
				}
			}
		}
	}
	*low = num;
	return best;
}   

/******************************************************************************
** Function:	checkAdd
** Description:	Function checks if vertex v can be added to <P> whilst 
**		maintaining planarity
** Inputs:	G - the graph
**		info - information about P
**		megaVerts - contains information about the reduced graph
**		mSize - number of elements in megaVerts
**		v  - vertex to be added
** Return Value:TRUE if can be added, otherwise FALSE
** Side Effects:
** Created:	Kerri Morgan	6th July, 2005
** Modified:
** Comments:	1. There are no edges between components so adding a vertex may
**		merge components but not cause violations in planarity in
**		vertices between two existing components.
**		2. Components that are trees can always have a vertex added.
**		From 1 and 2, if for each non-Tree component the vertex can
**		be added without violating the planarity property, then
**		the vertex can be added to the subgraph and planarity maintained
** Acknowledgements:
******************************************************************************/
int checkAdd(Vertex* G, K_VertInfo* info, RGVert* megaVerts, int  mSize, int v)
{
	int component;
	C_Node* cur = NULL;	

	cur = info[v].adjNonTrees.startPtr;

	/* A vertex can always be added to a tree component without causing
	 * nonplanarity.  If a vertex has only a single neighbour in any
	 * component it can also be added.  
	 * We need only check those vertices that have multiple neighbours in
	 * a single component*/
	#ifdef DEBUG
	printf("TESTING in checkAdd vertex %d\n", v);	
	#endif

	/*for each component that is a nontree */
	while(cur != NULL)
	{
		/* if v has more than one neighbour in this component*/
		if(cur->numAdjacent >=2)
		{
			component = cur->component;
			
			#ifdef DEBUG
			printf("TESTING component %d\n", component);
			#endif

			//check that v can be added to this component whilst maintaining planarity
			if(checkCompAdd(G, info, megaVerts, mSize, v, component) == FALSE)
			{
				return FALSE;
			}
		}
		else 
		{ 
			#ifdef DEBUG
			printf("Componet has less than 2 neighbours (%d)\n", cur->numAdjacent); 
			#endif
		}
		cur = cur->next;
	}
	return TRUE;

}
/******************************************************************************
** Function:	checkCompAdd
** Description:Function checks that a vertex can be added to this component 
**		and planarity maintained.
** Inputs:	G - the graph
**		info - information about P
**		megaVerts - information about megavertices
**		mSize - size of megaVerts
**		v - the vertex
**		comp - the component
** Return Value:TRUE - if the vertex can be added, otherwise FALSE
** Side Effects:none
** Created:	Kerri Morgan	10th July, 2005
** Modified:	9th September, 2005.  Modified so that the path obtained 
** 		must contain the edges in E. 
** Comments:	
** Acknowledgements:
******************************************************************************/
int checkCompAdd(Vertex* G, K_VertInfo* info, RGVert* megaVerts, int  mSize, int v, int comp)
{
	List L;
	MV_Edge* E;
	int numEdges;

	/* create space for edges */
	E = (MV_Edge*)malloc(sizeof(MV_Edge)*G[v].degree);
	if(E == NULL)
	{
		fprintf(stderr, "ERROR 10.20:Unable to create space to store megavertex edges\n");
		exit(1);
	}

	initEdgeList(E, G[v].degree);

	initList(&L);
	numEdges=locateMegaVertices(G, info, v, megaVerts, &L, comp, E);
	/* check a path exists that includes all the megavertices and satsifies
	 * numeric constraints */
	#ifdef DEBUG
	printf("TEST vertex is %d and component %d and megaverts::\n", v, comp);
	printList(&L);
	printMVEdges(E, G[v].degree);
	printf("TEST %d is number of edges\n", numEdges);
	#endif 

	if(L.degree == 1)
	{
		free(E);
		killList(&L);
		return TRUE;
	}
	else if(L.degree <=0)
	{
		fprintf(stderr, "ERROR 10.15: No megavertices adjacent to vertex %d in this component\n", v);
		exit(1);
	}
	else if(findValidPathII(G, megaVerts, &L, E, numEdges) == TRUE)
	//else if(findValidPath(G, megaVerts, &L) == TRUE)
	{
		free(E);
		killList(&L);
		return TRUE;
	}
	else
	{
		free(E);
		killList(&L);
		return FALSE;
	}
}

/******************************************************************************
** Function:	findValidPath
** Description:	Function attempts to find a path that includes all megavertices
**		in the list.  This path must also satisfy the following:
**		i) Path is of maximum length two
**		OR
**		ii) Path is of length three.  Let the path P = x,y,z.
**		    degree(y) <=3
**		OR
**		iii) Path is longer than three, but can be reduced to either
**		     case (i) or (ii)
** Inputs:	G - the graph
**		megaVerts -information about the megavertices
**		L - list of megavertices that must be in path
** Return Value:TRUE if a valid path exists otherwise FALSE
** Side Effects:
** Created:	Kerri Morgan	10th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int findValidPath(Vertex* G, RGVert* megaVerts, List* L)
{
	int v;
	List* Paths; 
	int i;
	int numPaths;
	int result;

	Paths = (List*)malloc(sizeof(List)*L->degree);
	if(Paths == NULL)
	{
		fprintf(stderr, "ERROR 10.16: Unable to create subpath lists - insufficient memory\n");
		exit(1);
	}

	for(i=0; i<L->degree; i++)
	{
		initList(&Paths[i]);	
	}
	i=0;
	while(emptyList(L) == FALSE)
	{
		v= findVertWithLeastNeighbours(L, G);
		if(v == -1)
		{
			fprintf(stderr, "Error 10.17: Unable to find vertex in non-empty list\n");
			exit(1);
		}
		/* from this vertex grow a path */
		makePath(G, L, v, &Paths[i]);
		i++;
	}
	numPaths =i;
	#ifdef DEBUG
	for(i=0; i<numPaths; i++)
	{
		printf("%d: ", i);
		printList(&Paths[i]);
	}
	#endif
	if(mergeSubPaths( Paths,  G, numPaths)== TRUE)
	{
		#ifdef DEBUG
		printf("SUCCESS\n");
		printList(&Paths[0]);
		#endif
	}
	else
	{
		#ifdef DEBUG
		printf("FAILURE\n");
		for(i=0; i<numPaths; i++)
		{
			printList(&Paths[i]);
		}
		#endif
		for(i=0; i<numPaths; i++)
		{
			killList(&Paths[i]);
		}
		return FALSE;
	}

	result = evaluatePath(G, &Paths[0]);
	for(i=0; i<numPaths; i++)
	{
		killList(&Paths[i]);
	}

	
	return result;
}


/******************************************************************************
** Function:	findValidPathII
** Description:	Function attempts to find a path that includes all megavertices
**		in the list.  This path must also satisfy the following:
**		i) Path is of maximum length two
**		OR
**		ii) Path is of length three.  Let the path P = x,y,z.
**		    degree(y) <=3
**		OR
**		iii) Path is longer than three, but can be reduced to either
**		     case (i) or (ii)
** Inputs:	G - the graph
**		megaVerts -information about the megavertices
**		L - list of megavertices that must be in path
**		E - set of edges that must be in path
** 		size - number of edges
** Return Value:TRUE if a valid path exists otherwise FALSE
** Side Effects:
** Created:	Kerri Morgan	10th September, 2005
** Modified:	This is a modification of findValidPath.  In the previous
**		function I neglected to make it mandatory that certain
**		edges be included in the path.  
**		For example,  if vertex v is in the sets represented by
**		two vertices in the reduced graph say m1 and m2, then
**		the edge m1m2 must be in the path if it is to valid
** Comments:	
** Acknowledgements:
******************************************************************************/
int findValidPathII(Vertex* G, RGVert* megaVerts, List* L, MV_Edge* E, int size)
{
	int v;
	int v1, v2;
	List* Paths; 
	int i;
	int numPaths;
	int result;
	int success;

	Paths = (List*)malloc(sizeof(List)*L->degree);
	if(Paths == NULL)
	{
		fprintf(stderr, "ERROR 10.16: Unable to create subpath lists - insufficient memory\n");
		exit(1);
	}

	for(i=0; i<L->degree; i++)
	{
		initList(&Paths[i]);	
	}
	i=0;
	while(emptyList(L) == FALSE)  
	{
		/* if there are still edges to add
		 * to path, add one */
		if(size > 0)
		{
			size--;
			v1 = E[size].v1;
			v2 = E[size].v2;
			/* check that at least one of these vertices has not
			 * been used in a previous path */
			if((inList(L, v1)== FALSE) && (inList(L, v2)==FALSE))
			{
				/* cannot use this edge as both it's vertices 
				 * are already in paths, but not adjacent*/
				numPaths =i;
				for(i=0; i<numPaths; i++)
				{
					killList(&Paths[i]);
				}
				return FALSE;
			}
			E[size].v1 =-1;
			E[size].v2 = -1;
			insertInList(&Paths[i], v1);
			insertAtEnd(&Paths[i], v2);
			removeFromList(L, v1);
			removeFromList(L, v2);

		}
		else /* add a single vertex */
		{
			v= findVertWithLeastNeighbours(L, G);
			removeFromList(L, v);
			insertInList(&Paths[i], v);
		}
		if(v == -1)
		{
			fprintf(stderr, "Error 10.17: Unable to find vertex in non-empty list\n");
			exit(1);
		}

		/* grow a path from this vertex (or pair of vertices */
		success = makePathII(G, L, &Paths[i], E, &size);
		/* an edge exists that cannot be added */
		if(success == -1)
		{
			numPaths =i;
			for(i=0; i<numPaths; i++)
			{
				killList(&Paths[i]);
			}
			return FALSE;

		}
		else if(success == 1) /* path was a cycle */
		{
			/* cannot have a valid path that consists of
			 * a cycle plus something else */
			if(i >=1)
			{
				numPaths =i;
				for(i=0; i<numPaths; i++)
				{
					killList(&Paths[i]);
				}
				return FALSE;
			}
			else if(emptyList(L) == TRUE)
			{
				if(size == 0)
				{
					break;
				}
				else /* still edges */
				{
					numPaths =i;
					for(i=0; i<numPaths; i++)
					{
						killList(&Paths[i]);
					}
					return FALSE;
				}
			}
			else
			{
				numPaths =i;
				for(i=0; i<numPaths; i++)
				{
					killList(&Paths[i]);
				}
				return FALSE;
			}
		}
		/* If there are still edges, but
		 * the list is empty; re-fill list*/
		/*if(emptyList(L) == TRUE)
		{
			if(size != 0)
			{
				for(j=0; j<size; j++)
				{
					insertInList(L, E[j].v1);
					insertInList(L, E[j].v2);
				}
			}
		} */
		i++;
	}

	numPaths =i;
	#ifdef DEBUG
	for(i=0; i<numPaths; i++)
	{
		printf("%d: ", i);
		printList(&Paths[i]);
	}
	printf("TEST\n");
	#endif

	if(mergeSubPaths( Paths,  G, numPaths)== TRUE)
	{
		#ifdef DEBUG
		printf("SUCCESS\n");
		printList(&Paths[0]);
		#endif
	}
	else
	{
		#ifdef DEBUG
		printf("FAILURE\n");
		for(i=0; i<numPaths; i++)
		{
			printList(&Paths[i]);
		}
		#endif
		for(i=0; i<numPaths; i++)
		{
			killList(&Paths[i]);
		}
		return FALSE;
	}
	
	 /* Given a path (w1,w2, ..., wk) exists containing 
	 * all the above edges and megavertices,
	 * for any two vertices wi, wi+1 in path that
	 * are both adjacent to some vertex v not in path
	 * and v has degree 2, insert v in the path */
	insertDeg2VertInPath(G, &Paths[0]);
	#ifdef DEBUG
		printList(&Paths[0]);
	#endif 
	result = evaluatePath(G, &Paths[0]);

	for(i=0; i<numPaths; i++)
	{
		killList(&Paths[i]);
	}

	
	return result;
}

/******************************************************************************
** Function:	growPath
** Description:	Function grows a path from vertex v using only vertices in L
** Inputs:	L - a list of megavertices
**		L1 - a list containing vertex v.  This list will hold the
**		path once the function is completed.
**		G - the graph
**		v - the vertex
** Return Value:TRUE if a path was successfully found, otherwise FALSE
** Side Effects:updates L1 to hold the path found
** Created:	Kerri Morgan	12th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int growPath(List* L, List* L1,Vertex* G,  int v)
{
	int cur;
	int w,wx;
	int prev =-1;

	cur =v;
	w = findAdjVert(G, L, cur);

	while(w!=-1)
	{
		#ifdef DEBUG
		printf("TTEST add %d\n", w);
		#endif
		removeFromList(L, w);
		/* check if w is also adjacent to prev.
		 * prev ---> cur ----> w
		 * |-------------------^
		 * If this is so and cur is degree 2, then insert w at end
		 * otherwise insert w between prev and cur */
		if(prev != -1)
		{
			if(inList(&(G[w].adjVertices), prev) == TRUE)
			{
				if(G[w].adjVertices.degree == 2)
				{
		/* w is only adjacent to prev and cur
		 * prev ---> w ----> cur
		 * |-------------------^   ***/
					removeFromList(L1, cur);
					insertAtEnd(L1, w);
					insertAtEnd(L1, cur);
					wx=cur;
					cur =w;
					w=wx;
				}
				else
				{
					insertAtEnd(L1, w);	
				}
			}
			else
			{
				insertAtEnd(L1, w);	
			}
		}
		else
		{
			insertAtEnd(L1, w);	
		}
		prev = cur;
		cur =w;
		w=findAdjVert(G, L, cur);
	}

	return TRUE;	
}
/******************************************************************************
** Function:	findAdjVert
** Description:	Function finds a vertex adjacent to v in L.
** Inputs:	G - the graph
**		L - list of megavertices
**		v - the vertex
** Return Value:A vertex adjacent to v in L, or -1 if no such vertex exists
** Side Effects:
** Created:	Kerri Morgan	12th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int  findAdjVert(Vertex* G, List* L, int v)
{
	Node* cur = NULL; 

	if(G[v].adjVertices.degree == 0)
	{
		return -1;
	}
	cur = G[v].adjVertices.startPtr;
	
	while(cur!=NULL)
	{
		if(inList(L, cur->vertex)==TRUE)
		{
			return cur->vertex;
		}
		cur = cur->next;
	}
	return -1;
}
/******************************************************************************
** Function:	mergeSubPaths
** Description:	Function merges subpaths into a single path.
** Inputs:	Paths - series of subpaths
**		size - number of paths
**		G - the graph
** Return Value:TRUE - if the paths are merged into one path
**		otherwise FALSE
** Side Effects:Updates Paths to contain a single path
** Created:	Kerri Morgan	12th July, 2005
** Modified:
** Comments:	Currently a greedy short-sighted merge strategy
** Acknowledgements:
******************************************************************************/
int mergeSubPaths(List* Paths, Vertex* G, int size)
{
	int j;
	int v,w;
	int v1;
	int commonNeighbour;
	int merges=0;
	int found =0;
	Node* cur=NULL;

	if(size <=1)
	{
		if(size ==1)
		{
	/* check if end vertices have degree 2 - if so and no wrap around then 
	 * add the other vertex to the list */
			cur = Paths[0].startPtr;
			v= cur->vertex;
			if(G[v].adjVertices.degree == 2)
			{
				cur = G[v].adjVertices.startPtr;
				while(cur != NULL)
				{
					w=cur->vertex;
					if(inList(&Paths[0], w) == FALSE)
					{
						insertAtStart(&Paths[0], w);
						break;
					}
					cur=cur->next;
				}
			}

			cur = Paths[0].endPtr;
			v= cur->vertex;
			if(G[v].adjVertices.degree == 2)
			{
				cur = G[v].adjVertices.startPtr;
				while(cur != NULL)
				{
					w=cur->vertex;
					if(inList(&Paths[0], w) == FALSE)
					{
						insertAtEnd(&Paths[0], w);
						break;
					}
					cur=cur->next;
				}
			}

		}
		return TRUE;
	}

	/* if we need to merge x lists then we need to do (x-1) merges*/
	while(merges < size-1)
	{
		v=Paths[0].startPtr->vertex;
		w=Paths[0].endPtr->vertex;
		found =0;

		/* for each of the other paths*/
		for(j=1; j<size; j++)
		{
			/* skip any paths that no longer exist, as they have
			 * been merged into Path[0] */
			if(Paths[j].degree != 0)
			{
			/* check if the start vertex of this list has a common
			 * neighbour with either v or w */
				v1=Paths[j].startPtr->vertex;
				commonNeighbour = getCommonNeighbour(G, w, v1, Paths, size);	

				#ifdef DEBUG
				printf("TEST: %d is common neighbour\n", commonNeighbour);
				#endif
				if(commonNeighbour !=-1)
				{
				/* append Path[j] to end of Path[i] via 
				 * commonNeighbour */
					//insertAtEnd(&Paths[i], commonNeighbour);
					//append(&Paths[i], &Paths[j]);
					insertAtEnd(&Paths[0], commonNeighbour);
					append(&Paths[0], &Paths[j]);
					found =1;
					Paths[j].startPtr=NULL;
					Paths[j].endPtr=NULL;
					Paths[j].degree =0;
					break;
				}
				commonNeighbour = getCommonNeighbour(G, v, v1, Paths, size);	
				#ifdef DEBUG
				printf("TEST: %d is common neighbour\n", commonNeighbour);
				#endif
				if(commonNeighbour !=-1)
				{
				/* append Path[i] to end of reversed Path[j] 
				 * via commonNeigbhour */
					//insertAtStart(&Paths[i], commonNeighbour);
					//revPrePend(&Paths[i], &Paths[j]);
					insertAtStart(&Paths[0], commonNeighbour);
					revPrePend(&Paths[0], &Paths[j]);
					found =1;
					Paths[j].startPtr=NULL;
					Paths[j].degree =0;
					break;
				}
				v1=Paths[j].endPtr->vertex;
				commonNeighbour = getCommonNeighbour(G, v, v1, Paths, size);	
				if(commonNeighbour !=-1)
				{
				/* prepend Path[j] to start of Path[i] 
				 * via commonNeigbhour */
					insertAtStart(&Paths[0], commonNeighbour);
					prepend(&Paths[0], &Paths[j]);
					//insertAtStart(&Paths[i], commonNeighbour);
					//prepend(&Paths[i], &Paths[j]);
					found =1;
					Paths[j].startPtr=NULL;
					Paths[j].degree =0;
					break;
				}
				commonNeighbour = getCommonNeighbour(G, w, v1, Paths, size);	
				#ifdef DEBUG
				printf("TEST: %d is common neighbour\n", commonNeighbour);
				#endif
				if(commonNeighbour !=-1)
				{
				/* append Path[j] to end of reversed Path[i] 
				 * via commonNeigbhour */
					//insertAtEnd(&Paths[i], commonNeighbour);
					//revAppend(&Paths[i], &Paths[j]);
					insertAtEnd(&Paths[0], commonNeighbour);
					revAppend(&Paths[0], &Paths[j]);
					found =1;
					Paths[j].startPtr=NULL;
					Paths[j].degree =0;
					break;
				}
			}
		}
		if(found == 1)
		{
			merges++;
		}
		else
		{
			return FALSE;
		}
		
	}

	/* check if end vertices have degree 2 - if so and no wrap around then 
	 * add the other vertex to the list */
	cur = Paths[0].startPtr;
	v= cur->vertex;
	if(G[v].adjVertices.degree == 2)
	{
		cur = G[v].adjVertices.startPtr;
		while(cur != NULL)
		{
			w=cur->vertex;
			if(inList(&Paths[0], w) == FALSE)
			{
				insertAtStart(&Paths[0], w);
				break;
			}
			cur=cur->next;
		}
	}


	cur = Paths[0].endPtr;
	v= cur->vertex;
	if(G[v].adjVertices.degree == 2)
	{
		cur = G[v].adjVertices.startPtr;
		while(cur != NULL)
		{
			w=cur->vertex;
			if(inList(&Paths[0], w) == FALSE)
			{
				insertAtEnd(&Paths[0], w);
				break;
			}
			cur=cur->next;
		}
	}
	
	return TRUE;
}
/******************************************************************************
** Function:	getCommonNeighbour
** Description:	Function finds a vertex that is a neigbour of both vertex v and
**		vertex w
** Inputs:	G - the graph
**		v, w - vertices
**		Paths - lists of vertices currently in paths
**		size - number of paths
** Return Value:Number of vertex adjacent to v and w (if such a vertex exists).
**		Otherwise -1.
** Side Effects:None
** Created:	Kerri Morgan	12th July, 2005
** Modified:	To find a neighbour that is not in any of paths
** Comments:	
** Acknowledgements:
******************************************************************************/
int getCommonNeighbour(Vertex* G, int v, int w, List* Paths, int size)
{
	Node* cur=NULL;
	int vx;
	int i;
	int flag =0;

	cur = G[v].adjVertices.startPtr;
	while(cur != NULL)
	{
		vx=cur->vertex;
		/* if vx is a neighbour of w and v1*/
		if(inList(&G[w].adjVertices, vx)==TRUE)
		{
			flag =0;
			/* check vx is not in a path already */
			for(i=0; i<size; i++)
			{
				if(inList(&Paths[i], vx)==TRUE)
				//if(inList(&Paths[i], vx)!=TRUE)
				{
					flag =1;
					break;
				}
			}
			/* vertex is not in any path */
			if(flag == 0)
			{
				return vx;
			}
		}
		cur= cur->next;
	}
	return -1;
}

/******************************************************************************
** Function:	findVertWithLeastNeigbours
** Description:	Function finds the vertex in L that has the least number of 
**		neighbours in L
** Inputs:	L - list of megavertices
**		G - the graph
** Return Value:v - the megavertex with the least neighbours
**		(-1 if no such vertex exists)
** Side Effects:removes v from List
** Created:	Kerri Morgan	15th July, 2005
** Modified:
** Comments:	If there are two vertices of the same degree, selection 
**		is biased against vertices of degree 2
** Acknowledgements:
******************************************************************************/
int findVertWithLeastNeighbours(List* L, Vertex* G)
{
	Node* cur1 =NULL;
	Node* cur2 = NULL;
	int v=-1;
	int num = INT_MAX;
	int temp;
	int x,w;
	int count=0;

	if(L->degree == 0) /* no vertices in list */
	{
		return -1;
	}
	else if(L->degree ==1) /* one vertex in list*/
	{
		v=L->startPtr->vertex;
		removeFromList(L, v);
		initList(L);
		return v;
	}
	else if(L->degree == 2) /* two vertices in list */
	{
		v=L->startPtr->vertex;
		if(G[v].adjVertices.degree == 2)
		{
			v = L->endPtr->vertex;
			removeFromList(L, v);
			return v;
		}
		else 
		{
			removeFromList(L, v);
			return v;	
		}
	}
	cur1 = L->startPtr;
	/* for each vertex in list check how many neighbours they have in the list
	 * At each iteration, vertex v is the vertex with least neighbours in the
	 * section of the list examined */
	while(cur1 !=NULL)
	{
		x=cur1->vertex;
		temp = G[x].adjVertices.degree;
		cur2 = G[x].adjVertices.startPtr;
		count =0;
		/* count number of vertex x's neighbours in the list */
		while(cur2 != NULL)
		{
			w=cur2->vertex;		
			/* adjacent to v and in component*/
			if(G[w].member == KNOWN)
			{
				if(inList(L, w)== TRUE)
				{
					count++;
					/* all vertices adjacent to vertex x are in list */
					if(count == temp)
					{
						break;
					}
					else if(count > num) /* this vertex has already more neighbours than vertex v*/
					{
						break;
					}
				}
			}
			cur2 = cur2->next;
		}
		if(count < num)
		{
			v =x;
			num = count;
		}
		else if ((count == num) && (temp != 2)) /* bias against vertices of degree 2 */
		{
			v=x;
			num = count;
		}
		cur1=cur1->next;
	}
	removeFromList(L, v);
	return v;
}
/******************************************************************************
** Function:	makePath
** Description: Function takes a list of vertices and a vertex v and creates a
**		path of vertices that contains vertex v and a subset of vertices
**		from the list.  
**		In some cases the path may consist solely of vertex v
** Inputs:	G - the graph
**		L - list of megavertices
**		v - vertex
**		Path - list to store path in
** Return Value:none
** Side Effects: updates contents of Path
** Created:	Kerri Morgan	27th July, 2005
** Modified:
** Comments:	vertex v has already been removed from the list L, but has not
**		been added to Path when this function is called
** Acknowledgements:
******************************************************************************/
void makePath(Vertex* G, List* L, int v, List* Path)
{
	int w1, w2;
	Node* cur=NULL;

	/* if path is empty and vertex v has degree 2 then check if both
	 * vertices adjacent to v are in L.
	 * If w1 and w2 adjacent to v are in L, place w1,v, w2 in list 
	 * then extend path from end 
	 * else put v in list and extend path from start */

	if(G[v].adjVertices.degree == 2)
	{
		/* check if both the neighbours of v are in list */
		cur = G[v].adjVertices.startPtr;
		if(cur!=NULL)
		{
			w1 =cur->vertex;
			cur = cur->next;
			if(cur!=NULL)
			{
				w2=cur->vertex;
			}
			else
			{
				fprintf(stderr, "ERROR 10.18: Vertex %d is listed as adjacent to tow vertices, but unable to located these vertices\n", v);
				exit(1);
			}
		}
		else
		{
			fprintf(stderr, "ERROR 10.18: Vertex %d is listed as adjacent to two vertices, but unable to located these vertices\n", v);
			exit(1);
		}
		#ifdef DEBUG
		printf("%d %d are adjacent to %d\n", w1, w2, v);
		#endif
		if(inList(L, w1) == FALSE)
		{
			if(inList(L, w2) == TRUE)
			{
				/* v has one neighbour w2 in list*/
				removeFromList(L, w2);
				insertAtEnd(Path, v);
				insertAtEnd(Path, w2);
				extendFromEnd(G, L, w2, Path);
			}
			else 
			{
				insertAtEnd(Path, v);
				extendFromEnd(G, L, v, Path);
			}
		}
		else if(inList(L, w2)== FALSE) /* w1 is in L and w2 is not in L*/
		{
			removeFromList(L, w1);
			insertAtEnd(Path, v);
			insertAtEnd(Path, w1);
			extendFromEnd(G, L, w1, Path);
		}
		else /* w1 and w2 are in L */
		{
			removeFromList(L, w1);
			removeFromList(L, w2);
			insertAtEnd(Path, w1);
			insertAtEnd(Path, v);
			insertAtEnd(Path, w2);
			extendFromEnd(G, L, w2, Path);
		}
	}
	else /* v does not have degree 2 */
	{
		insertAtEnd(Path, v);
		extendFromEnd(G, L, v, Path);
	}
	/* now extend the front of the path */
	w1 = Path->startPtr->vertex;
	extendFromStart(G, L, w1, Path);

}

/******************************************************************************
** Function:	extendFromEnd
** Description: Function extends a path from the end using vertices in list L
** Inputs:	G - the graph
**		L - the list of vertices available
**		v - the last vertex in path
**		Path - the path of vertices
** Return Value:none
** Side Effects:A vertex is added to Path and removed from L
** Created:	Kerri Morgan	27th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void extendFromEnd(Vertex* G, List* L, int v, List* Path)
{
	Node* cur = NULL;
	int w1 =-1;
	int w2 = -1;

	cur = G[v].adjVertices.startPtr;
	while(cur != NULL)
	{
		w2 = cur->vertex;
		if(inList(L, w2) == TRUE)
		{
			w1 = w2;
			/* biased towards vertices of degree 2 */
			if(G[w2].adjVertices.degree == 2)
			{
				break;
			}
		}
		cur = cur->next;
	}

	if(w1 != -1)
	{
		removeFromList(L, w1);
		insertAtEnd(Path, w1);
		extendFromEnd(G, L, w1, Path);
	}

}

/******************************************************************************
** Function:	extendFromStart
** Description:	Function extends a list from the beginning using vertices 
**		from L
** Inputs:	G - the graph
**		L - the list of vertices available
**		v - the last vertex in path
**		Path - the path of vertices
** Return Value: none
** Side Effects: A vertex is added to Path and removed from L
** Created:	Kerri Morgan
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void extendFromStart(Vertex* G, List* L, int v, List* Path)
{
	Node* cur = NULL;
	int w1 =-1;
	int w2 = -1;

	cur = G[v].adjVertices.startPtr;
	while(cur != NULL)
	{
		w2 = cur->vertex;
		if(inList(L, w2) == TRUE)
		{
			w1 = w2;
			if(G[w2].adjVertices.degree == 2)
			{
				break;
			}
		}
		cur = cur->next;
	}

	if(w1 != -1)
	{
		removeFromList(L, w1);
		insertAtStart(Path, w1);
		extendFromStart(G, L, w1, Path);
	}
}

/******************************************************************************
** Function:	evaluatePath
** Description: Function takes a path of vertices and converts it to a path of
**		degrees of those vertices in the reduced graph of P
** Inputs:	G - the graph
**		Path - the path of vertices
** Return Value: TRUE if the path evaluates to a series of values that 
**		correspond to the vertices lying on a single face.  Otherwise
**		FALSE
** Side Effects:
** Created:	Kerri Morgan	28th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int evaluatePath(Vertex* G, List* Path)
{
	Node* cur=NULL;
	int vx, wx;
	int i;
	int* temp = NULL;
	int size;

	cur = Path->startPtr;
	if(cur == NULL)
	{
		return FALSE;
	}
	else
	{
		size = Path->degree;
		/* any path of size two is suitable*/
		if(size <=2)
		{
			return TRUE;
		}
		temp = (int*) malloc(sizeof(int)*size);

		if(temp == NULL)
		{
			fprintf(stderr, "Error 10.19: Insufficent memory to create space to evaluate path\n");
			exit(1);
		}
		/* create an array of the degrees in the path */
		i=0;
		while(cur != NULL)
		{
			vx = cur->vertex;
			temp[i] =G[vx].adjVertices.degree;
				
			#ifdef DEBUG
			printf("TESTING: EVALUATE\n");
			#endif
			/* if the degree of the first of last is 2 then add an extra vertex to begin and end*/
			i++;
			cur = cur->next;
		}
	}
	#ifdef DEBUG
	for(i=0; i<size; i++)
	{
		printf("%d ", temp[i]);
	}
	printf("\n");
	#endif
	/* any path of length three with the internal vertex degree 3 or less
	 * is acceptable */
	if(size == 3)
	{
		if(temp[1] <=3)
		{
			free(temp);
			return TRUE;
		}
	}

	/* for each internal vertex in path of degree 2, decrement
	 * the degree of the vertex on either side by 1 and set the 
	 * vertex itself to 0 */
	for(i=1; i<size-1; i++)
	{
		if(temp[i] == 2)
		{
			temp[i-1]--;
			temp[i+1]--;
			temp[i]=0;
			
			i++;
		}
	}
	/* check if path is a cycle */
	cur= Path->startPtr;
	vx = cur->vertex;

	cur = Path->endPtr;
	wx = cur->vertex;
	#ifdef DEBUG
	printList(Path);
	#endif
	
	/* this should be if wx and vx are adjacent and one is degree 2 */	
	//if(vx != wx) 
	if(checkAdjacent(G, vx,wx)==TRUE)
	{
		if(G[vx].adjVertices.degree == 2)
		{
			if(inList(&G[vx].adjVertices, wx)==TRUE)
			{
				temp[size-1]--;
				temp[1]--;
				temp[0]=0;
			}
		}
		else if(G[wx].adjVertices.degree == 2)
		{
			if(inList(&G[wx].adjVertices, vx)== TRUE)
			{
				temp[size-2]--;
				temp[size-1]=0;
				temp[0]--;
			}
		}
	}
	#ifdef DEBUG	
	for(i=0; i<size; i++)
	{
		printf("%d ", temp[i]);
	}
	printf("\n");
	#endif

	/* check if this is a path that corresponds to a single face*/
	if(validSequence(temp, size) == TRUE)
	{
		free(temp);
		return TRUE;
	}
	else
	{
		free(temp);
		return FALSE;  
	}
}

/******************************************************************************
** Function:	validSequence
** Description:	Function checks if the sequence is valid.  A sequence is valid
**		if it matches the following regular expression:
**		[num]?2*3?2*[num]? (where num is any positive integer).
**		Any 0's in the sequence are ignored
** Inputs:	sequence - a sequence of degrees in r(P).  The sequence has
**		already been further reduced to take into consideration any
**		triangles traversed.
** Return Value:TRUE if the sequence is valid, otherwise FALSE
** Side Effects: none
** Created:	Kerri Morgan	29th July, 2005
** Modified:
** Comments:	Here is my attempt to provide the NFA I drew for this task.
It is not complete.  In state_0 there is a transition into state_0 for the 
value 0.  In state_1 there is a transition into state_1 for numbers less than
or equal to 2.  In state_2 there is a transition into state_2 for numbers
less than or equal to 2.  

	state_0----------> state_1-------------->state_2
		  num>0	     |		num=3	   |
		  	     |			   |
			      |			   |
			       |		   |num>2
			        |      num>3	   |
				|----------------> \/
						  state_3---------> REJECT
							   Any number
** Acknowledgements:
******************************************************************************/
int validSequence(int sequence[], int size)
{
	int i=0;
	int state =0;

	while(i<size)
	{
		switch (state)
		{
			case 0: /* any leading 0's ignored*/
				if(sequence[i] > 0)
				{
					state = 1;
				}
				break;
			case 1: 
				if(sequence[i] == 3)
				{
					state = 2;
				}
				else if (sequence[i]>3)
				{
					state = 3;
				}
				break;
			case 2:	
				if(sequence[i]>2)
				{
					state = 3;
				}
				break;
			case 3:
				return FALSE;
				break;
		}
		i++;
	}
	return TRUE;
	
}

/******************************************************************************
** Function:	selectMegaRemoval
** Description:	find a vertex that can be removed so the graph will reduce
**		further
** Inputs:	G - the graph
**		size - size of the graph
** Return Value: vertex number of -1 if no such vertex exists
** Side Effects: none
** Created:	Kerri Morgan	10th August, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int selectMegaRemoval(Vertex* G, int size)
{
	int best = -1;
	int numNeighbours = -1;
	int d3_neighbours = -1;
	int count;
	Node* cur = NULL;
	int i;
	int v;

	for(i=0; i<size; i++)
	{
		count =0;
		if(G[i].adjVertices.degree > 0)
		{
			cur= G[i].adjVertices.startPtr;
			while(cur!=NULL)
			{
				v=cur->vertex;
				if(G[v].adjVertices.degree == 3)
				{
					count++;
				}
				cur=cur->next;
			}
			if(count > d3_neighbours)
			{
				d3_neighbours = count;
				best =i;
				numNeighbours = G[i].adjVertices.degree;
			}
			else if(count == d3_neighbours)
			{
				if(G[i].adjVertices.degree > numNeighbours)
				{
					numNeighbours = G[i].adjVertices.degree;
					best=i;
				}
			}
		}
	}
	return best;

}

/******************************************************************************
** Function:	checkAdjacent
** Description: Function checks if two vertices are adjacent in reduced graph
** Inputs:	G - the graph
**		v,w - two vertices
** Return Value: TRUE - the vertices are adjacent, otherwise FALSE
** Side Effects:
** Created:	Kerri Morgan	9 September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int checkAdjacent(Vertex* G, int w, int v)
{
	Node* cur = NULL;
	
	cur = G[v].adjVertices.startPtr;

	while(cur!=NULL)
	{
		if(cur->vertex == w)
		{
			return TRUE;
		}
		cur = cur->next;	
	}
	return FALSE;
}

/******************************************************************************
** Function:	initEdgeList
** Description: Function initialises an array of edges to (-1,-1)
** Inputs:	E - the array of edges
**		size - size of array
** Return Value: none
** Side Effects: initialises all edges to (-1, -1) - not an edge
** Created:	Kerri Morgan	9 September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void initEdgeList(MV_Edge* E, int size)
{
	int i;
	
	for(i=0; i<size; i++)
	{
		E[i].v1=-1;
		E[i].v2=-1;
	}
}

/******************************************************************************
** Function:	getMVEdge
** Description: Function extracts an edge between megavertices
** Inputs:	L - list of megavertices
**		E - structure to hold the edge
** Return Value: 1 - if successful otherwise 0.
** Side Effects: Updates contents of E
** Created:	Kerri Morgan	9th September, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int getMVEdge(List* L, MV_Edge* E)
{
	Node* cur = NULL;

	/* list should only have at most two megavertices */
	if(L->degree > 2)
	{
		fprintf(stderr, "Error 10.21:List contains more than two megavertices\n");
		exit(1);
	}
	else if(L->degree == 2)
	{
		cur = L->startPtr;
		if(cur!=NULL)
		{
			E->v1 = cur->vertex;
			cur = cur->next;
			if(cur!=NULL)
			{
				E->v2 = cur->vertex;
				return 1;
			}

			else
			{
				fprintf(stderr, "Error 10.22: List does not contain sufficient vertices\n");
				exit(1);
			
			}
		}
		else
		{
			fprintf(stderr, "Error 10.22: List does not contain sufficient vertices\n");
			exit(1);
			
		}
	}
	else
	{
		return 0;
	}	

}

/******************************************************************************
** Function:	MVEdge_exists
** Description: Function checks if E contains a specified edge.
** Inputs:	E - array of edges 
** 		edge - the specified edge
**		size - maximum size of E
** Return Value: the index of the position in the array that the edge is
**		stored at.  If the edge is not in array, function returns the
**		first free index
** Side Effects: none
** Created:	Kerri Morgan	9th September,2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int MVEdge_exists( MV_Edge* E, MV_Edge edge, int size)
{
	int i;
	for(i=0; i<size; i++)
	{
		/* no more edges to consider */
		if(E[i].v1 == -1)
		{
			return i;
		}	
		else if((E[i].v1 == edge.v1) && (E[i].v2 == edge.v2))
		{
			return i;
		}
		else if((E[i].v2 == edge.v1) && (E[i].v1 == edge.v2))
		{
			return i;
		}
	}
	return i;
}

/******************************************************************************
** Function:	printMVEdges
** Description: Function prints the contents of an array of edges between
**		megavertices.
** Inputs:	E - edges
**		size - maximum number of edges
** Return Value: none
** Side Effects:
** Created:	Kerri Morgan	9th September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void printMVEdges(MV_Edge* E, int size)
{
	int i;

	printf("Edges are as follows:\n");
	for(i=0; i<size; i++)
	{
		if(E[i].v1 == -1)
		{
			break;
		}
		printf("%d %d\n", E[i].v1, E[i].v2);
	}
	printf("\n\n");
}

/******************************************************************************
** Function:	removeEdges	
** Description: Function removes all edges with both endpoints in the set
**		{v1,v2, v3}
** Inputs:	E -set of edges
**		size - maximum size of E
**		v1, v2, v3 - vertices
**		num - number of edges
** Return Value: count - number of edges removed
** Side Effects: updates E
** Created:	Kerri Morgan	9th  September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int removeEdges(MV_Edge* E, int size, int v1, int v2, int v3, int num)
{
	int i;
	int count =0;

	for(i=0; i<size; i++)
	{
		if(E[i].v1 == -1)
		{
			return count;
		}
		if(E[i].v1 == v1)
		{
			if((E[i].v2 == v2) || (E[i].v2 == v3))
			{
				num--;
				E[i].v1 = E[num].v1;
				E[i].v2 = E[num].v2;

				E[num].v1 = -1;
				E[num].v2 = -1;

				count++;
				i--;
			}
		}
		else if(E[i].v1 == v2)
		{
			if((E[i].v2 == v1) || (E[i].v2 == v3))
			{
				num--;
				E[i].v1 = E[num].v1;
				E[i].v2 = E[num].v2;

				E[num].v1 = -1;
				E[num].v2 = -1;

				count++;
				i--;
			}
		}
		else if(E[i].v1 == v3)
		{
			if((E[i].v2 == v1) || (E[i].v2 == v2))
			{
				num--;
				E[i].v1 = E[num].v1;
				E[i].v2 = E[num].v2;

				E[num].v1 = -1;
				E[num].v2 = -1;

				count++;
				i--;
			}

		}	
	}
	return count;

}


/******************************************************************************
** Function:	makePathII
** Description: Function takes a list of vertices and a Path containing one
**		or two vertices  and creates a
**		path of vertices that contain a subset of the vertices in L
**		and/or those in E
**		In some cases the path may consist solely of vertex v
** Inputs:	G - the graph
**		L - list of megavertices
**		Path - list to store path in
**		E -set of edges
**		size - number of edges in E
** Return Value:1 if the path found is a cycle otherwise 0 if a successful
**		path is found otherwise -1, if some edge is found that
**		cannot be merged with existing paths.
** Side Effects: updates contents of Path, L and E.  If edges are removed
**		from E, then size is updated
** Created:	Kerri Morgan	10th September, 2005
** Modified:  	This function  is a re-write of makePath.  It gives 
**		precedence to edges over vertices in the list
** Comments:	
** Acknowledgements:
******************************************************************************/
int makePathII(Vertex* G, List* L,  List* Path, MV_Edge* E, int* size)
{
	int start, finish;
	int i;
	int v;

	start = Path->startPtr->vertex;
	finish = Path->endPtr->vertex;

	#ifdef DEBUG
		printf("ENTRY WITH THIS LIST\n");
		printList(Path);
	#endif
	/* still edges in the list */
	if(*size > 0)
	{
		/* check if path can be extended via an edge */
		for(i=0; i < *size; i++)
		{
			/* if there exists an edge with both ends in path,
			 * but these are not both at end of the path, then
			 * this is an invalid path */
			if(inList(Path, E[i].v1) && inList(Path, E[i].v2))
			{
				/* v1 is an internal vertex */
				if((E[i].v1 != start) && (E[i].v1 != finish)) 
				{
					return -1;
				}
				else if((E[i].v2 != start) && (E[i].v2 != finish))
				{
					return -1;
				}
			}
			/* I can extend the path by this edge at the
			 * beginning of the path */
			if((E[i].v1 == start) || (E[i].v2 == start))
			{
				#ifdef DEBUG
				printf("FOUND %d %d\n", E[i].v1, E[i].v2);
				#endif
				if(E[i].v1 == start)
				{
					/* check list is not size 1 */
					if(start != finish)
					{
						if(E[i].v2 == finish)
						{
							#ifdef DEBUG
							printf("remove edge %d %d form list\n", start, finish);
							#endif
							/* no need to add the edge
							 * start, ...., finish is path
							 * and edge adding is (start, finish) 
							 */
							
							removeFromList(L, E[i].v1);
							removeFromList(L, E[i].v2);
			
							/* remove the edge */
							(*size) = *(size)-1;
							E[i].v1 = E[*size].v1;
							E[i].v2 = E[*size].v2;
							
							return 1; /* path is a cycle */
						}
						else
						{
							#ifdef DEBUG
							printf("add edge %d %d at start list\n", E[i].v1, E[i].v2);
							#endif
							insertAtStart(Path, E[i].v2);
				
							removeFromList(L, E[i].v1);
							removeFromList(L, E[i].v2);
			
							/* remove the edge */
							(*size) = *(size)-1;
							E[i].v1 = E[*size].v1;
							E[i].v2 = E[*size].v2;

							return makePathII(G, L, Path, E, size);
						}
					}
					else /* path consists of single vertex */
					{
							#ifdef DEBUG
							printf("add edge %d %d at end list\n", E[i].v1, E[i].v2);
							#endif
						insertAtEnd(Path, E[i].v2);
						removeFromList(L, E[i].v2);
				
						return makePathII(G, L, Path, E, size);
						
						/* remove the edge */
						(*size) = *(size)-1;
						E[i].v1 = E[*size].v1;
						E[i].v2 = E[*size].v2;
					
						removeFromList(L, E[i].v1);
					}
				}
				else /* E[i].v2 == start */
				{
					/* check list is not size 1 */
					if(start != finish)
					{
						if(E[i].v1 == finish)
						{
							#ifdef DEBUG
							printf("REMMOVE edge %d %d from list\n", start, finish);
							#endif
							/* no need to add the edge
							 * start, ...., finish is path
							 * and edge adding is (start, finish) 
							 */
				
							removeFromList(L, E[i].v1);
							removeFromList(L, E[i].v2);
							
							/* remove the edge */
							(*size) = *(size)-1;
							E[i].v1 = E[*size].v1;
							E[i].v2 = E[*size].v2;
					
							return 1; /* path is a cycle */
						}
						else
						{
							#ifdef DEBUG
							printf("add edge %d %d at start list\n", E[i].v1, E[i].v2);
							#endif
							insertAtStart(Path, E[i].v1);
				
							removeFromList(L, E[i].v1);
							removeFromList(L, E[i].v2);
			
							/* remove the edge */
							(*size) = *(size)-1;
							E[i].v1 = E[*size].v1;
							E[i].v2 = E[*size].v2;
				
							return makePathII(G, L, Path, E, size);
						}
					}
					else /* path consists of single vertex */
					{
							#ifdef DEBUG
							printf("add edge %d %d at start list\n", E[i].v1, E[i].v2);
							#endif
						insertAtEnd(Path, E[i].v1);
						
						removeFromList(L, E[i].v1);
						removeFromList(L, E[i].v2);
				
						/* remove the edge */
						(*size) = *(size)-1;
						E[i].v1 = E[*size].v1;
						E[i].v2 = E[*size].v2;
					
						return makePathII(G, L, Path, E, size);
					}
				}	

			}
			else if((E[i].v1 == finish) || (E[i].v2 ==finish))
			{
				#ifdef DEBUG
				printf("finish FOUND %d %d\n", E[i].v1, E[i].v2);
				#endif

				if(E[i].v1 == finish)
				{
					/* check list is not size 1 */
					if(start != finish)
					{
						if(E[i].v2 == start)
						{
							#ifdef DEBUG
							printf("REMOVE from list %d %d\n", start, finish);
							#endif
							/* no need to add the edge
							 * start, ...., finish is path
							 * and edge adding is (start, finish) 
							 */
				
							removeFromList(L, E[i].v1);
							removeFromList(L, E[i].v2);
							
							/* remove the edge */
							(*size) = *(size)-1;
							E[i].v1 = E[*size].v1;
							E[i].v2 = E[*size].v2;
					
							return 1; /* path is a cycle */
						}
						else
						{
							#ifdef DEBUG
							printf("add edge %d %d at end list\n", E[i].v1, E[i].v2);
							#endif
							insertAtEnd(Path, E[i].v2);
							
							removeFromList(L, E[i].v1);
							removeFromList(L, E[i].v2);
				
							/* remove the edge */
							(*size) = *(size)-1;
							E[i].v1 = E[*size].v1;
							E[i].v2 = E[*size].v2;
				
							return makePathII(G, L, Path, E, size);
						}
					}
					else /* path consists of single vertex */
					{
							#ifdef DEBUG
							printf("add edge %d %d at end list\n", E[i].v1, E[i].v2);
							#endif
						insertAtEnd(Path, E[i].v2);
						
						removeFromList(L, E[i].v1);
						removeFromList(L, E[i].v2);
						
						/* remove the edge */
						(*size) = *(size)-1;
						E[i].v1 = E[*size].v1;
						E[i].v2 = E[*size].v2;
				
						return makePathII(G, L, Path, E, size);
					}
				}
				else /* E[i].v2 == finish */
				{
					/* check list is not size 1 */
					if(start != finish)
					{
						if(E[i].v1 == start)
						{
							#ifdef DEBUG
							printf("TEST remove %d %d\n", start, finish);
							#endif
							/* no need to add the edge
							 * start, ...., finish is path
							 * and edge adding is (start, finish) 
							 */
							
							removeFromList(L, E[i].v1);
							removeFromList(L, E[i].v2);
							
							/* remove the edge */
							(*size) = *(size)-1;
							E[i].v1 = E[*size].v1;
							E[i].v2 = E[*size].v2;
					
							return 1; /* path is a cycle */
						}
						else
						{
							#ifdef DEBUG
							printf("**add edge %d %d at end list\n", E[i].v1, E[i].v2);
							#endif
							insertAtEnd(Path, E[i].v1);
							
							removeFromList(L, E[i].v1);
							removeFromList(L, E[i].v2);
				
							/* remove the edge */
							(*size) = *(size)-1;
							E[i].v1 = E[*size].v1;
							E[i].v2 = E[*size].v2;
				
							return makePathII(G, L, Path, E, size);
						}
					}
					else /* path consists of single vertex */
					{
						#ifdef DEBUG
						printf("add edge %d %d at end list\n", E[i].v1, E[i].v2);
						#endif
						insertAtEnd(Path, E[i].v1);
					
						removeFromList(L, E[i].v1);
						removeFromList(L, E[i].v2);
						
						/* remove the edge */
						(*size) = *(size)-1;
						E[i].v1 = E[*size].v1;
						E[i].v2 = E[*size].v2;
				
						return makePathII(G, L, Path, E, size);
					}
				}	
			}
		}
		
	}

	/* A vertex v may occur in more than one edge.  
	 * Removing an edge from E, causes the removal 
	 * of a vertex from L.  However, another edge
	 * may still contain this vertex, so we re-add 
	 * them her */

/*	WRONG - we don't want to reuse vertices in the path 
 * 	for(i=0; i< *size; i++)
	{
		insertInList(L, E[i].v1);
		insertInList(L, E[i].v2);
	}
	*/
	
	/* If still in this function, no edge exists that contains either
	 * start of finish vertices of path */
	
	v = findLowestDegreeNeighbourInList(G, Path->endPtr->vertex, L);	
	if(v == -1)
	{
		v = findLowestDegreeNeighbourInList(G, Path->startPtr->vertex, L);	
		if(v == -1)
		{
			/* Path cannot be further extended */
			return 0;
		}
		else /* extend path from the start */
		{
			/* insert this vertex at the start of the list */
			insertAtStart(Path, v);

			#ifdef DEBUG
				printf("add vert1 %d at start list\n", v);
			#endif
			/* remove from the list*/
			removeFromList(L, v);
			return makePathII(G, L, Path, E, size);
		}
	}
	else /* extend path from end */
	{
		/* insert this vertex at the start of the list */
		insertAtEnd(Path, v);

		#ifdef DEBUG
			printf("add vert %d  at end list\n", v);
		#endif
		/* remove from the list*/
		removeFromList(L, v);

		return makePathII(G, L, Path, E, size);
	}
	
	return 0; 
}

/******************************************************************************
** Function:	findLowestDegreeNeighbourInList
** Description: Function finds the vertex in list L  of lowest degree 
**		adjacent to vertex v in the reduced graph of G.
** Inputs:	G - the graph
**		v - vertex
**		L - list of vertices
** Return Value: w - the vertex in L of lowest degree adjacent to v.
**		 (-1 if no such vertex exists)
** Side Effects: none
** Created:	Kerri Morgan	11th September, 2005
** Modified:
** Comments:	The vert
** Acknowledgements:
******************************************************************************/
int findLowestDegreeNeighbourInList(Vertex* G, int v, List* L)
{
	Node* cur = NULL;
	int w;
	int bestVert=-1;
	int neighbours = INT_MAX;

	cur = G[v].adjVertices.startPtr;
	while(cur != NULL)
	{
		w = cur->vertex;
		if(inList(L, w) == TRUE)
		{
			
			if(G[w].adjVertices.degree == 2)
			{
				return w;
			}
			if(G[w].adjVertices.degree < neighbours)
			{
				bestVert=w;
				neighbours =G[w].adjVertices.degree;
			}
		}
		cur = cur->next;
	}
	return bestVert;

}

/******************************************************************************
** Function:	insertDeg2VertInPath
** Description: Function examines a path.  If two vertices adjacent to each
**		other in the path are both adjacent to some vertex v not in
**		the path and the degree of v is 2, then the function adds
**		v to the path
** Inputs:	G - graph 
**		Path - a list of vertices 
** Return Value: none
** Side Effects: updates path
** Created:	Kerri Morgan 	14th September, 2005	
** Modified:
** Comments:	This function is called once a "valid" path is found. 
**		That is a path that contains all the megavertices and
**		any edges between megavertices as dictated by the 
**		neighbours of the vertex in R we are considering adding
**		to P.
**		Once such a path is known to exist, we can add these vertices
**		to the path to help the process of calculating the number of
**		faces traversed by this path to be more accurately
**		ascertained.  
** Acknowledgements:
******************************************************************************/
void insertDeg2VertInPath(Vertex* G, List* Path)
{
	Node* cur=NULL;
	Node* next=NULL;
	Node* new;
	int v,w;
	int vert;

	cur=Path->startPtr;
	if(cur != NULL)
	{
		next = cur->next;
		while((cur != NULL) && (next !=NULL))
		{
			v=cur->vertex;
			w=next->vertex;
		
			vert = sharedNeighbour(G, v,w);
			/* if there exists a vertex of degree 2
			 * adjacent to v and w but not in path,
			 * add it */
			if(vert !=-1)
			{
				if(inList(Path, vert) == FALSE)
				{
					new = makeNode(vert);
					new->prev =cur;
					new->next = next;
					cur->next = new;
					next->prev =new;
					Path->degree++;
					#ifdef DEBUG
					printf("HERE***************\n");
					#endif
				}
			}			
			cur =next;
			next = cur->next;
				
		}
	}	
}
/******************************************************************************
** Function:	sharedNeighbour
** Description: Function checks if two vertices have a shared neighbour of
**		degree 2
** Inputs:	G - the graph
**		v,w - vertices
** Return Value:vert - vertex number of neighbouring vertex of
**		degree 2 
**		If no such vertex exists, -1 is returned
** Side Effects:
** Created:	Kerri Morgan	14th September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int sharedNeighbour(Vertex* G, int v, int w)
{
	Node* cur=NULL;
	int vert;

	cur = G[v].adjVertices.startPtr;
	while(cur !=NULL)
	{
		vert = cur->vertex;
		if((G[vert].adjVertices.degree == 2) && (vert != w))
		{
			if(inList(&(G[w].adjVertices), vert) == TRUE)
			{
				return vert;
					
			}
		}
		cur = cur->next;
	}
	return  -1;
}

/******************************************************************************
** Function:	
** Description: 
** Inputs:	
** Return Value:
** Side Effects:
** Created:	Kerri Morgan	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
