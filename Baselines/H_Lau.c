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

#include "H_Lau.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/******************************************************************************
** Function:	H_L_alg
** Description: Function finds a planar subgraph using Halldorsson and Lau's
**		partitioning approach
** Inputs:	G - the graph
**		size - number of vertices in graph
**		save - flag, indicates whether to save planar subgraph
**		d - largest degree in graph
** Return Value: P - number of vertices in planar subgraph
** Side Effects:
** Created:	Kerri Morgan
** Modified:	17th August, 2005
**		Removed the parameter d.  Calculate d within function now.
** Comments:	
** Acknowledgements:	Halldorsson, M. and Lau, H., 1997.  Low-degree graph
** partitioning via local search with applications to constraint 
** satisfaction, max cut and colouring.  Journal of Graph Algorithms and 
** Applications, 1(3), 1-13.
******************************************************************************/
int H_L_alg(Vertex* G, int size, int save)
{
	int P=0;
	int i;
	int numClasses;
	Partition* info=NULL;
	int* classNum=NULL;
	int index;
	int bestClass;
	int d=-1;

	/* find largest degree */
	for(i=0; i<size; i++)
	{
		if(G[i].degree > d)
		{
			d = G[i].degree;
		}
	}
	
	if(d <=2)
	{
		for(i=0; i<size; i++)
		{
			G[i].member = TRUE;
		}
		P = size;
		return P;
	}
	else
	{
		numClasses = ceil((d+1)/3.0);
		//numClasses = d-2;
		classNum = (int*) malloc(sizeof(int)*numClasses);
		if(classNum == NULL)
		{
			fprintf(stderr, "ERROR 9.3: Insufficient memory to record class size\n");
			exit(1);
		}
		for(i=0; i<numClasses; i++)
		{
			classNum[i]=0;
		}
		info =randPartition(G, size, numClasses, classNum);
		
		/* each vertex is assigned the least popular class amongst
		 * its neighbours */
		for(i=0; i<size; i++)
		{
			index = selectClass(info, numClasses, size, i);
			if((index !=-1) && (info[i].part !=index))
			{
				switchPart(info, classNum, G, size, index, i, numClasses);
			}
		}
	}
	P=0;
	bestClass =0;
	for(i=0; i<numClasses; i++)
	{
		if(P < classNum[i])
		{
			bestClass=i;
			P=classNum[i];
		}
	}
	//for(i=0; i<numClasses; i++)
	//	printf("%d\n", classNum[i]); 
	//printPartInfo(info, size, numClasses);
	if(save ==1)
	{
		for(i=0; i<size; i++)
		{
			if(info[i].part == bestClass)
			{
				G[i].member = TRUE;
			}
		}
		writePlanarSubgraph(G,size);
	}
	free(classNum);
	killPartition(info, size, numClasses);
	return P;
}
/******************************************************************************
** Function:	randPartition
** Description: Function randomly partitions a graph into d-2 parts. 
** Inputs:	G - the graph 
**		size - number of vertices in graph
**		numClasses - number of classes
** Return Value: info - contains information about partition
** Side Effects: 
** Created:	Kerri Morgan	30th May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
Partition* randPartition(Vertex* G, int size, int numClasses, int classNum[])
{
	Partition* info=NULL;
	int i, j;
	int v;
	int class;

	info = (Partition*) malloc (sizeof(Partition) * size);
	if(info == NULL)
	{
		fprintf(stderr, "ERROR 9.1: Insufficient memory to create partition\n");
		exit(1);
	}
	/* each vertex is adjacent to vertices belonging to these classes */
	for(i=0; i<size; i++)
	{
		info[i].neighbours = (int*)malloc(sizeof(int) * numClasses);
		if(info[i].neighbours == NULL)
		{
		fprintf(stderr, "ERROR 9.2: Insufficient memory to create partition\n");
		exit(1);
			i--;
			while(i>=0)
			{
				free(info[i].neighbours);
			}
			free(info);
		}
	}

	for(i=0; i<size; i++)
	{
		info[i].part = i%numClasses;
		classNum[i%numClasses]++;
		for(j=0; j<numClasses; j++)
		{
			info[i].neighbours[j]=0;
		}
	}
	for(i=0; i<size; i++)
	{
		/* for each neighbouring vertex, count the colour
		 * class it belongs to ****/
		for(j=0; j<G[i].degree; j++)
		{
			v=G[i].adjVerts[j];
			class = info[v].part;
			info[i].neighbours[class]++;	
		}
	}
	return info;
}
/******************************************************************************
** Function:	printPartInfo
** Description:	Function prints out information about the partition
** Inputs:	info - structure holding information about partition
**		size - number of vertices
**		classes - number of partitions
** Return Value:none
** Side Effects:prints information to screen
** Created:	Kerri Morgan	30th May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void printPartInfo(Partition* info, int size, int classes)
{
	int i, j;

	for(i=0; i<size; i++)
	{
		printf("%d is in %d and adjacent to: \n", i, info[i].part);
		for(j=0; j<classes; j++)
		{
			printf("\t%d of class %d\n", info[i].neighbours[j], j);
		}
		printf("\n");
	}
}
/******************************************************************************
** Function:	killPartition
** Description:	Function deallocates memory associated with partition 
**		structure
** Inputs:	info - structure holding information about partition
**		size - number of vertices
**		classes - number of partitions
** Return Value:none
** Side Effects:deallocates memory
** Created:	Kerri Morgan	30th May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void killPartition(Partition* info, int size, int classes)
{
	int i;

	for(i=0; i<size; i++)
	{
		free(info[i].neighbours);
	}
	free(info);
}
/******************************************************************************
** Function:   selectClassI
** Description: For a given vertex, the function selects the least popular
**		class assigned to its neighbours
** Inputs:	info - contains information about the current partition
**		numClasses - number of parts of partition
**		size - number of vertices in graph
**		v - vertex under consideration
** Return Value: index - number of selected class
** Side Effects:
** Created:	Kerri Morgan	2nd June, 2005
** Modified:
** Comments:	Alternative function that is biased towards selecting the
**		part that has at most 2 neighbours belonging to the same part
** Acknowledgements:
******************************************************************************/
int selectClassI(Partition* info, int numClasses, int size, int v)
{
	int index =-1;
	int best; 
	int j;

//	best = size+1;
//
	index = info[v].part;
	best = info[v].neighbours[index];

	if(best == 2)
		return index;
	for(j=0; j<numClasses; j++)
	{
		if(info[v].neighbours[j] < best) 
		{
			if(best > 2)
			{
				index = j;
				best=info[v].neighbours[j];
			}
		}
		/*else if((info[v].neighbours[j] > best) && (info[v].neighbours[j] <=2))
		{
			index =j;
			best = info[v].neighbours[j];
		}*/
		if(best == 2)
		{
			break;
		}
	}

	return index;
}
/******************************************************************************
** Function:   selectClass
** Description: For a given vertex, the function selects the least popular
**		class assigned to its neighbours
** Inputs:	info - contains information about the current partition
**		numClasses - number of parts of partition
**		size - number of vertices in graph
**		v - vertex under consideration
** Return Value: index - number of selected class
** Side Effects:
** Created:	Kerri Morgan	30th May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int selectClass(Partition* info, int numClasses, int size, int v)
{
	int index =-1;
	int best; 
	int j;

//	best = size+1;
//
	index = info[v].part;
	best = info[v].neighbours[index];
	for(j=0; j<numClasses; j++)
	{
		if(info[v].neighbours[j] < best)
		{
			index = j;
			best=info[v].neighbours[j];
		}
		if(best == 0)
		{
			break;
		}
	}

	return index;
}

/******************************************************************************
** Function:	switchPart	
** Description:	Function changes the class of a vertex v to the new class
**		and updates structures appropriately
** Inputs:	info - inforamtion on partititon
**		classNum - holds numbers in each class
**		G - the graph
**		new - the new class
**		v - the vertex concerned
**		size - number of vertices in graph
**		numClasses - number of classes
** Return Value:none
** Side Effects:updates partition information
** Created:	Kerri Morgan	30th May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void switchPart(Partition* info, int classNum[], Vertex* G,int size,  int new, int v, int numClasses)
{
	int old;
	int j;
	int w;
	int index;

	old=info[v].part;
	info[v].part = new;

	classNum[old]--;
	classNum[new]++;

	/* for each adjacent neighbour*/
	for(j=0; j<G[v].degree; j++)
	{
		w=G[v].adjVerts[j];
		/* update the number of neighbours belonging to parts old and index */
		info[w].neighbours[old]--;
		info[w].neighbours[new]++;
	}

	/* each of these neighbours may now need their part number altered */
	for(j=0; j<G[v].degree; j++)
	{
		w=G[v].adjVerts[j];
		index = selectClass(info, numClasses, size, w);
		//index = selectClassI(info, numClasses, size, w);
		if((index !=-1) && (info[w].part !=index))
		{
			switchPart(info, classNum, G, size, index, w, numClasses);
		}
	}
}

