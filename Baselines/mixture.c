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

#include "stdlib.h"
#include "stdio.h"
#include "mixture.h"
/******************************************************************************
** Function:	Function runs the subset removal algorithm on a graph, then
**		creates an information structure,so that further algorithms
**		can be performed on this planar subgraph
** Description: 
** Inputs:	
** Return Value:
** Side Effects:
** Created:	Kerri Morgan	14th September, 2005
** Modified:
** Comments:	
** Acknowledgements: The vertex removal algorithm is based on some notes by Keith
**		     Edwards.
******************************************************************************/
int mixture_Alg(Vertex* G, int size, int save)
{
	int P=0;
	K_VertInfo* info=NULL;
	int* R= NULL;
	int Rsize;
	R_Index R_idx[1];
	int idxSize=1;
	int component =0;
	int i;


	/* first run the vertex subset removal algorithm */
	P=subsetRemove(G, size, save);
	/* initialise information structure */
	info=initK_Info(G, size);

	/* there are size - P vertices in R */
	Rsize = size-P;

	if(Rsize <=0)
	{
		return P;
	}
	/* create enough space to store an array of vertices to consider */

	R = initialiseR(G, size, Rsize);
	R_idx[0].start = 0;
	R_idx[0].finish = Rsize-1;

	info =initialiseInfo(G, size, &component);

	P=extendGraph(G,size, info, R, R_idx, idxSize, P, &component);

	for(i=0; i<size; i++)
	{
		killList2(&info[i].adjTrees);
		killList2(&info[i].adjNonTrees);
	}
	free(R);
	free(info);
	return P;
}

/******************************************************************************
** Function:	initialiseR	
** Description: Function creates an array containing all  vertices not in P
** Inputs:	G - the graph
**		size - number of vertices in graph
**		Rsize - size of R
** Return Value: R - array of vertices in R
** Side Effects: Each element in R is a vertex in R
** Created:	Kerri Morgan	14th September, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int* initialiseR(Vertex* G, int size, int Rsize)
{
	int i,j;
	int* R=NULL;

	R=(int*) malloc(sizeof(int)*Rsize);
	if(R==NULL)
	{
		fprintf(stderr, "ERROR 15.1: Insufficient memory to create R\n");
		exit(1);
	}

	j=0;
	for(i=0; i<size; i++)
	{
		if(G[i].member == UNKNOWN)
		{
			R[j]=i;
			j++;
			if(j>Rsize)
			{
				fprintf(stderr, "Error 15.2: Number of vertices in R (%d) exceeds given size (%d)\n", j, Rsize);
				exit(1);
			}
		}
	}

	return R;
}

/******************************************************************************
** Function:	initialiseInfo	
** Description: Function initialise info to hold information about a planar 
**		subgraph of G
** Inputs:	G - the graph
**		size - number of vertices in G
**		comp - first free component number
** Return Value:info - holds information about P
** Side Effects: updates comp
** Created:	Kerri Morgan	14th September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
K_VertInfo* initialiseInfo(Vertex* G, int size, int* comp)
{
	K_VertInfo* info = NULL;
	int i;
	int edges =0;
	int vertices =0;

	info=initK_Info(G,size);
	for(i=0; i<size; i++)
	{
		if(G[i].member == KNOWN)
		{
			info[i].inP = TRUE;
		}
	}
	/* create components*/
	for(i=0; i<size; i++)	
	{
		if(info[i].inP == TRUE)
		{
			if(info[i].component == -1)
			{
				edges =0;
				vertices =1;
				info[i].component= *comp;
			createComponent(G,  info, i, *comp, &edges, &vertices);

				/* is this a tree or non-tree */
				if(edges == (vertices -1))
				{
					info[i].type=FOREST;
					createTreeInfo(G, info, i);
				}
				else
				{
					info[i].type=NON_TREE;
					createNonTreeInfo(G, info, i);
				}
				*comp = (*comp)+1;
			}	
		}	
	}
	return info;
}

/******************************************************************************
** Function:	createComponent	
** Description: Function finds a component consisting of vertex v and all
**		vertices in P that are connected to this vertex and it's
**		component
** Inputs:	G - the graph
**		info - information about P
**		v - the vertex
**		comp - component number of vertex v
**		edges - holds the number of edges in the component
**		vertices - holds the number of vertices in the component
** Return Value: none
** Side Effects: Updates info, edges and vertices
** Created:	Kerri Morgan	14th September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void createComponent(Vertex* G, K_VertInfo* info, int v, int comp, int* edges,int* vertices)
{
			
	int i;
	int w;

	for(i=0; i<G[v].degree; i++)
	{
		w = G[v].adjVerts[i];
		if(info[w].inP == TRUE) 
		{
			if (info[w].component != comp)
			{
				*vertices = (*vertices)+1;
				info[w].component = comp;
				createComponent(G, info, w, comp, edges, vertices);
			}
			if(w>v)
			{
				*edges = (*edges)+1;
			}
		}
	}
}

/******************************************************************************
** Function:	createTreeInfo
** Description: Function marks all vertices in this component as belonging to
**		a tree.  Any vertex not in the component but adjacent to the
**		current vertex v has it's tree neighbour information updated
** Inputs:	G - the graph
**		v - current vertex
** Return Value:none
** Side Effects:Updates info
** Created:	Kerri Morgan	14th September, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void createTreeInfo(Vertex* G, K_VertInfo* info,int v)
{
	int i;
	int w;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if(info[w].inP == TRUE)
		{
			/* if this vertex has not been dealt with */
			if(info[w].type != FOREST)
			{
				info[w].type = FOREST;
				createTreeInfo(G, info, w);
			}

		}
		else /* w is not in P */
		{
			info[w].Nf++;
			/* insert this component as a tree neighbour */
			insertInList2(&(info[w].adjTrees), info[v].component);
		}
	}
}


/******************************************************************************
** Function:	createNonTreeInfo
** Description: Function marks all vertices in this component as belonging to
**		a non-tree.  Any vertex not in the component but adjacent 
**		to the current vertex v has it's tree neighbour 
**		information updated
**		
** Inputs:	G - the graph
**		v - current vertex
** Return Value:none
** Side Effects:Updates info
** Created:	Kerri Morgan	14th September, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void createNonTreeInfo(Vertex* G, K_VertInfo* info,int v)
{
	int i;
	int w;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if(info[w].inP == TRUE)
		{
			/* if this vertex has not been dealt with */
			if(info[w].type != NON_TREE)
			{
				info[w].type = NON_TREE;
				createNonTreeInfo(G, info, w);
			}

		}
		else /* w is not in P */
		{
			info[w].Nn++;
			/* insert this component as a tree neighbour */
			insertInList2(&(info[w].adjNonTrees), info[v].component);
		}
	}
}

