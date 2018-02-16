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

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
/******************************************************************************
** Function:	growSingleTreeAlg
** Description:	Functions finds a planar graph.  The graph is a tree induced by
**		a subset of the vertices
** Inputs:	G - the graph
**		size - number of vertices in graph
**`		save - flag - indicates to save planar subgraph for testing
** Return Value:P - number of vertices in subgraph
** Side Effects:
** Created:	Kerri Morgan	26th May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int growSingleTreeAlg(Vertex* G, int size, int save)
{
	int P=0;
	VertInfo* info=NULL;
	int componentNum = 0;

	info=initInfo(size);

	/* grow a forest of singletons */
	P = growSingletons(G, size, info);
	componentNum =P; /* all components at this stage contain one vertex */

	if(save ==1 )
	{
		writePlanarSubgraph(G, size);
	}
	return P;
}

/******************************************************************************
** Function:	growTreeAlg
** Description:	Functions finds a planar graph.  The graph is a tree induced by
**		a subset of the vertices
** Inputs:	G - the graph
**		size - number of vertices in graph
**`		save - flag - indicates to save planar subgraph for testing
** Return Value:P - number of vertices in subgraph
** Side Effects:
** Created:	Kerri Morgan	26th May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int growTreeAlg(Vertex* G, int size, int save)
{
	int P=0;
	VertInfo* info=NULL;
	int componentNum = 0;

	info=initInfo(size);

	/* grow a forest of singletons */
	//P = growSingletons(G, size, info);
	//componentNum =P; /* all components at this stage contain one vertex */

	P = growTrees(G, size, info,  &componentNum);

	return P;
}
/******************************************************************************
** Function:	growSingletons
** Description:	Function finds a forest of single vertices - these form an 
**		induced subgraph of G
** PRECONDITION: The number of vertices in the planar subgraph is 0, that is
**		 the planar subgraph is the null graph
** Inputs:	G - the graph
**		size - number of vertices in Graph
**		info - information about graph
** Return Value: P - number of vertices in subgraph
** Side Effects: updates info
** Created:	Kerri Morgan	26th May, 2005
** Modified:
** Comments:		
** Acknowledgements:
******************************************************************************/
int growSingletons(Vertex* G, int size,  VertInfo* info)
{
	int i,j;
	int P=0;
	int w;

	for(i=0; i<size; i++)
	{
		if(info[i].Nf == 0)
		{
			info[i].inP = TRUE;
			G[i].member = TRUE;
			info[i].component=P;
			info[i].type=FOREST;
			P++;
		/* Every vertex adjacent to i is now adjacent to a tree*/
			for(j=0; j<G[i].degree; j++)
			{
				w=G[i].adjVerts[j];
				info[w].Nf++;
			}
		}
	}
	return P;
}
/******************************************************************************
** Function:	growTrees
** Description:	Function expands single vertex trees to larger trees by a 
**		process of adding more vertices to them, and merging trees
** Inputs:	G - the graph
**		size - number of vertices in graph
**		P - current number of vertices in P, the planar subgraph
** Return Value: updated size of P
** Side Effects:
** Created:	Kerri Morgan	26th May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int growTrees(Vertex* G, int size, VertInfo* info, int *num)
{
	int i,j;
	int P=0;
	int w;

	for(i=0; i<size; i++)
	{
		if((info[i].inP == FALSE) && (info[i].Nf ==0))
		{
			info[i].inP = TRUE;
			info[i].type = FOREST;
			info[i].component=*num;
			G[i].member = TRUE;
			for(j=0; j<G[i].degree; j++)
			{
				w=G[i].adjVerts[j];
				info[w].Nf++;
			}
			P++;
			P+=extendTree(G, size, i,  info, *num);
			*num = *num +1;
		}
	}
	#ifdef DEBUG
	printInfo(info, size);
	#endif

	return P;
}
/******************************************************************************
** Function:	extendTree
** Description:	Function extends a tree from vertex v
** Inputs:	G - the graph
**		size - number of vertices in graph
**		v - the vertex from which the tree grows
**		P - current number of vertices in P
**		info - contains information about P
**		num - component number of this tree
** Return Value:
** Side Effects:
** Created:	Kerri Morgan 26th May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int extendTree(Vertex* G, int size, int v, VertInfo* info, int num)
{
	int i, j;
	int w;
	int count =0;

	/* each vertex adjacent to v, and not adjacent to anything else in P */
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if((info[w].inP == FALSE) && (info[w].Nf ==1))
		{
			info[w].inP=TRUE;
			info[w].type = FOREST;
			info[w].component= num;
			G[w].member = TRUE;
			count++;
			for(j=0; j<G[w].degree; j++)
			{
				info[G[w].adjVerts[j]].Nf++;
			}
			count+=extendTree(G, size, w, info, num);	
		}
	}
	return count;
}
