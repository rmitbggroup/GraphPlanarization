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


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "subsetRem.h"
#include "removeVertexAlg.h"

/******************************************************************************
** Function:	subsetRemove
** Description:	Function implements the vertex subset removal algorithm
**		(as provided by Graham Farr and Keith Edwards)
** Inputs:	G - the graph
**		size - number of vertices
**		save - flag - save subgraph if 1
** Return Value:P - size of planar subgraph
** Side Effects:
** Created:	Kerri Morgan	10th August, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int subsetRemove(Vertex* G, int size, int save)
{
        int i;
	int P;
	int rho=0;
	int R=0;
	int w, v;
	int max_d = 0;
	Node* cur=NULL;
	List temp;

	/* mark all in P */
	for(i=0; i<size; i++)
	{
		G[i].member = KNOWN;
	}


/* use the linked list structure as a temporary structure to hold Rg */
	duplicateGraph(G,size);
	getReducedGraph(G, size);

	/* find the maximum value for d */
	for(i=0; i<size; i++)
	{
		if(G[i].adjVertices.degree > max_d)
		{
			max_d = G[i].adjVertices.degree;
		}
	}

	rho=floor(calcRho(G,size));

	while(R < rho)
	{
		w = selectVertForRemoval(G, size, max_d);

		if(w==-1)
		{
			break; /* no vertices to remove */
		}
		max_d = G[w].adjVertices.degree;
		G[w].member = UNKNOWN;
		/* remove w from all it's neighbours' adjacency lists*/
		cur = G[w].adjVertices.startPtr;
		while(cur != NULL)
		{
			v=cur->vertex;
			removeFromList(&G[v].adjVertices, w);
			cur = cur->next;
		}

		/* kill list after the recursive call, but
		 * disaccoiate with G ************/

		/* move the list to temp storage */
		temp.degree = G[w].adjVertices.degree;
		temp.startPtr = G[w].adjVertices.startPtr;
		temp.endPtr = G[w].adjVertices.endPtr;

		/* disassociate this list from G[w] */
		G[w].adjVertices.degree = 0;
		G[w].adjVertices.startPtr=NULL;
		G[w].adjVertices.endPtr = NULL;

		R++;

		furtherReduceGraph(G, size, &temp);
		killList(&temp);
	}
	P=size -R;

	if(save ==1)
	{
		writePlanarSubgraph(G, size);
	}
	unDuplicateGraph(G, size);

	return P;
}

/******************************************************************************
** Function:	selectVertForRemoval
** Description: Function selects a vertex for removal
** Inputs:	G - the graph
**		size - number of vertices in graph
**		d - maximum degree in the graph
** Return Value:index - number of vertex to be removed, otherwise -1
** Side Effects:
** Created:	Kerri Morgan	10 August 2005	
** Modified:	7 September, 2005 - to select the vertex with the most
**		neighbours of lower degree rather than the most neighbours
**		of degree d-1.
** Comments:	
** Acknowledgements: Based Keith Edwards ideas
******************************************************************************/
int selectVertForRemoval(Vertex* G, int size, int d)
{
	int val = 0;
	int index =-1;
	int numNeighbours = -1;
	int i;
	Node* cur;
	int v;
	int count;

	/* can't find any 2 neighboured vertices */
	if(d <=3)
	{
		/* pick vertex of highest degree ->which will be
		 * the first vertex of degree 3 */
		for(i=0; i<size; i++)
		{
			if(G[i].adjVertices.degree > val)
			{
				index = i;
				val = G[i].adjVertices.degree;

				/* won't find better than degree d */
				if(val == d)
				{
					return index;
				}
			}

		}
		return index;
	}

	for(i=0; i<size; i++)
	{
		/* vertex i is in reduced graph */
		if(G[i].adjVertices.degree == d)
		{
			/* count the neighbours of degree (d-1) */
			cur = G[i].adjVertices.startPtr;
			count =0;

			while(cur != NULL)
			{
				v = cur->vertex;
				if(G[v].adjVertices. degree <= (d-1))
				//if(G[v].adjVertices. degree == (d-1))
				{
					count++;
				}
				cur = cur->next;
			}
			if(count > numNeighbours)
			{
				numNeighbours = count;
				index = i;
			}
		}
	}
	if(index == -1)
	{
		return selectVertForRemoval( G, size, d-1);
	}
	else
	{
		return index;
	}
}


/******************************************************************************
** Function:	subsetRemoveProf
** Description:	Function implements the vertex subset removal algorithm
**		(as provided by Graham Farr and Keith Edwards)
** Inputs:	G - the graph
**		size - number of vertices
**		save - flag - save subgraph if 1
** Return Value:P - size of planar subgraph
** Side Effects:
** Created:	Kerri Morgan	10th August, 2005
** Modified:	29th September, 2005.  This function is same as subsetRemove
**		but I  needed a separate one for the purpose of profiling
**		the time required to run the algorithm.  Otherwise
**		the time taken by this function when called by Mixture is
**		included.
** Comments:	
** Acknowledgements:
******************************************************************************/
int subsetRemoveProf(Vertex* G, int size, int save)
{
        int i;
	int P;
	int rho=0;
	int R=0;
	int w, v;
	int max_d = 0;
	Node* cur=NULL;
	List temp;

	/* mark all in P */
	for(i=0; i<size; i++)
	{
		G[i].member = KNOWN;
	}


/* use the linked list structure as a temporary structure to hold Rg */
	duplicateGraph(G,size);
	getReducedGraph(G, size);

	/* find the maximum value for d */
	for(i=0; i<size; i++)
	{
		if(G[i].adjVertices.degree > max_d)
		{
			max_d = G[i].adjVertices.degree;
		}
	}

	rho=floor(calcRho(G,size));

	while(R < rho)
	{
		w = selectVertForRemoval(G, size, max_d);

		if(w==-1)
		{
			break; /* no vertices to remove */
		}
		max_d = G[w].adjVertices.degree;
		G[w].member = UNKNOWN;
		/* remove w from all it's neighbours' adjacency lists*/
		cur = G[w].adjVertices.startPtr;
		while(cur != NULL)
		{
			v=cur->vertex;
			removeFromList(&G[v].adjVertices, w);
			cur = cur->next;
		}

		/* kill list after the recursive call, but
		 * disaccoiate with G ************/

		/* move the list to temp storage */
		temp.degree = G[w].adjVertices.degree;
		temp.startPtr = G[w].adjVertices.startPtr;
		temp.endPtr = G[w].adjVertices.endPtr;

		/* disassociate this list from G[w] */
		G[w].adjVertices.degree = 0;
		G[w].adjVertices.startPtr=NULL;
		G[w].adjVertices.endPtr = NULL;

		R++;

		furtherReduceGraph(G, size, &temp);
		killList(&temp);
	}
	P=size -R;

	if(save ==1)
	{
		writePlanarSubgraph(G, size);
	}
	unDuplicateGraph(G, size);

	return P;
}

