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


#include "removeVertexAlg.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/******************************************************************************
** Function: removeVertAlg
** Description: Function planarises the graph by incrementally removing vertices
** Inputs:	G - the graph
**		size - number of vertices in the graph G.
**		save - flag - 1 to indicate if wish to save "subgraph" otherwise
**		       0
** Return Value: P - set of vertices belonging to the planar graph.
**		 P is a subset of V(G)
** Side Effects:
** Created:	Kerri Morgan	31 March, 2005
** Modified:	20 April, 2005 To give option to save the subgraph.  Note: the
**		saved graph is not exactly the subgraph - the removed vertices
**		are still there as isolated vertices.  However, good for testing
** Comments:	
** Acknowledgements: Algorithm based on the algorithm presented in 
**		     "Planarization and fragmentablility of some classes of
**		     graphs", Keith Edwards and Graham Farr.  2003.
******************************************************************************/
int removeVertAlg(Vertex* G,int size, int save)
{
	int i;
	int P;
	int rho=0;
	int R=0;
	int w, v;
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
	
	rho=floor(calcRho(G,size));

	while(R<rho)
	{
		w=getMaxDegreeVertex(G, size);
		/* Do not continue if reduced graph is empty */
		if(w ==-1)
		{
	//		printf( "ERROR 5.2: Unable to locate a vertex in P, yet R (%d) is smaller than rho (%d)\n", R, rho);
//			fprintf(stderr, "ERROR 5.2: Unable to locate a vertex in P, yet R (%d) is smaller than rho (%d)\n", R, rho);
			break;
		//	exit(1);
			/* should we stop here - means graph is already planar so why take more stuff away */
		}

		/* remove w from P */
		G[w].member = UNKNOWN;
		#ifdef DEBUG
		printf("%d is removed\n", w);
		#endif	
		/* remove neighbours of w from all their list */
		cur = G[w].adjVertices.startPtr;
		while(cur !=NULL)
		{
			v=cur->vertex;
			#ifdef DEBUG
			printf("Remove %d from adjList of %d\n", v, w);
			#endif
			removeFromList(&G[v].adjVertices, w);
			cur=cur->next;
		}
		/* killList after the recursive call, but disassociate the list with G */
		/* move the list to temp storage */
		temp.degree = G[w].adjVertices.degree;
		temp.startPtr = G[w].adjVertices.startPtr;
		temp.endPtr = G[w].adjVertices.endPtr;

		/* disassociate this list from G[w] */
		G[w].adjVertices.degree=0;
		G[w].adjVertices.startPtr=NULL;
		G[w].adjVertices.endPtr=NULL;

		/* kill list and set degree to 0 of w*/
		//killList(&G[w].adjVertices);
		R++;

		furtherReduceGraph(G, size, &temp);
		killList(&temp);
	}

	P= size-R;
	//printf("size is %d and R is %d so P is %d \n",size,R,P);
	/* write graph */
/*
	if(save == 1)
	{
		writePlanarSubgraph(G, size);
	}

	unDuplicateGraph(G, size);
*/
	return P;

}
/******************************************************************************
** Function:	calcRho
** Description: Function calculates the value for rho.
		Rho is calculated as follows:
		1. Let Rg be the graph that is formed by performing operations
		   3.1, 3.2, 3.3 and 3.4 (Edwards & Farr, 2003) on the graph G
		   until none of these operations are possible.
		2. Sum over the vertices in Rg
			(degree(Rg(v))-2)/(degree(Rg(v))+1)	
** Inputs:	G - the graph
***		size - number of vertices in the graph
** Return Value:rho - result of the calculation
** Side Effects:
** Created:	Kerri Morgan	31 March, 2005
** Modified:
** Comments:	
** Acknowledgements: see comment in removeVertAlg
******************************************************************************/
float calcRho(Vertex* G, int size)
{
	float rho=0;
	int i;



	for(i=0; i<size; i++)
	{
		if(G[i].adjVertices.degree > 0)
		{
			rho+= (float)(G[i].adjVertices.degree-2)/(float)(G[i].adjVertices.degree+1);
		}
		
	}
	return rho;
}
	
/******************************************************************************
** Function:	duplicateGraph
** Description:	Function creates a duplicate adjacency list for each vertex in
**		G.  The initial list is stored in the array, so the secondary
**		list is stored in the linked list structure.
** Inputs:	G - the graph
**		size - the number of vertices in G.
** Return Value:none
** Side Effects:updates G
** Created:	Kerri Morgan	31st March, 2005
** Modified:
** Comments:	The Linked list strutcture has a member degree, which can be
**		used as the degree of the secondary structure.
** Acknowledgements:
******************************************************************************/
void duplicateGraph(Vertex* G, int size)
{
	int i,j;

	for(i=0; i<size; i++)
	{
		for(j=0; j<G[i].degree; j++)
		{
			insertInList(&G[i].adjVertices, G[i].adjVerts[j]);
		}
	}
}


/******************************************************************************
** Function:	unDuplicateGraph
** Description:	Function destroys the duplication of the graph created by
**		duplicateGraph function.
**		G.  The initial list is stored in the array, so the secondary
**		list is stored in the linked list structure.
** Inputs:	G - the graph
**		size - the number of vertices in G.
** Return Value:none
** Side Effects:updates G
** Created:	Kerri Morgan	1st April, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void unDuplicateGraph(Vertex* G, int size)
{
	int i;

	for(i=0; i<size; i++)
	{
		killList(&G[i].adjVertices);
	}
}

/******************************************************************************
** Function:	operation3_2
** Description: Function performs 3.2 (Edwards & Farr, 2003)
** Inputs:	G - the graph
**		size - number of vertices in graph
**		v - the vertex to be removed
** Return Value:none
** Side Effects:updates linked list members of G
** Created:	Kerri Morgan	31 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void operation3_2(Vertex* G, int size, int v)
{
	int w;
	Node* cur=NULL;

	cur = G[v].adjVertices.startPtr;
	w=cur->vertex;
	
	removeFromList(&G[v].adjVertices, w);
	removeFromList(&G[w].adjVertices, v);

	#ifdef DEBUG
	printf("operation 3.2 on %d is adjacent to %d \n", v,  w);
	#endif	
	/* check if w is now in the position to be removed */
	if(G[w].adjVertices.degree == 1)
	{
		operation3_2(G, size, w);
	}
	else if(G[w].adjVertices.degree == 2)
	{
		operation3_3and4(G, size, w);
	}
}

/******************************************************************************
** Function:	operation3_3and4
** Description: Function performs 3.3 and 3.4 (Edwards & Farr, 2003)
** Inputs:	G - the graph
**		size - number of vertices in graph
**		v - the vertex to be removed
** Return Value:none
** Side Effects:updates linked list members of G
** Created:	Kerri Morgan	31 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void operation3_3and4(Vertex* G, int size, int cur)
{
	int v, w;

	getAdjacentVert(G, cur, &v, &w);
	if(v==-1 || w==-1)
	{
		fprintf(stderr, "ERROR 5.1: Vertex %d has degree 2 but unable to locate two adjacent vertices\n", cur);
		exit(1);
	}
	#ifdef DEBUG	
	printf("operation 3.3/4 on %d is adjacent to %d and %d\n", cur, v, w);
	#endif	

	/* is v adjacent to w */
	if(inList(&G[v].adjVertices, w)==TRUE)
	{
		/* delete vertex cur, and edges to v and w */
		removeFromList(&G[v].adjVertices, cur);
		removeFromList(&G[w].adjVertices, cur);
		removeFromList(&G[cur].adjVertices, w);
		removeFromList(&G[cur].adjVertices, v);
	}
	else /* v is not adjacent to w */
	{
		removeFromList(&G[v].adjVertices, cur);
		removeFromList(&G[w].adjVertices, cur);
		removeFromList(&G[cur].adjVertices, w);
		removeFromList(&G[cur].adjVertices, v);
		insertInList(&G[v].adjVertices, w);
		insertInList(&G[w].adjVertices, v);
	}
	
	/* check if v and w are now able to be removed */
	if(G[v].adjVertices.degree == 1)
	{
		operation3_2(G, size, v);
	}
	else if(G[v].adjVertices.degree == 2)
	{
		operation3_3and4(G, size, v);
	}
	if(G[w].adjVertices.degree == 1)
	{
		operation3_2(G, size, w);
	}
	else if(G[w].adjVertices.degree == 2)
	{
		operation3_3and4(G, size, w);
	}
}

/******************************************************************************
** Function:	getAdjacentVert
** Description:
** Inputs:	G - the graph'
**		cur - current vertex
**		v, w - variable to store 2 vertices adjacent to cur
** Return Value: none
** Side Effects:updates v and w to hold the vertices adjacent to cur (otherwise
**		-1)
** Created:	Kerri Morgan	31st March, 2005
** Modified:
** Comments:		
** Acknowledgements:
******************************************************************************/
void getAdjacentVert(Vertex* G, int cur, int* v, int* w)
{
	Node* start=NULL;

	*v=-1;
	*w=-1;

	start = G[cur].adjVertices.startPtr;
	if (start !=NULL)
	{
		*v = start->vertex;
		start = start->next;
		if(start!=NULL)
		{
			*w = start->vertex;
		}
	}
	

}
/******************************************************************************
** Function:	getReducedGraph
** Description: Function finds the reduced graph of all vertices in P.
** Inputs:	G - the graph (P is a subset of this graph)
**		size - number of vertices in this graph
** Return Value:none
** Side Effects:updates the linked list structure in G to reflect the structure **		of P.
** Created:	Kerri Morgan	1 April, 2005
** Modified:	
** Comments:	
** Acknowledgements:
******************************************************************************/
void getReducedGraph(Vertex* G, int size)
{
	int i; 

    /* for each vertex in G, if I can do one of operations do so
     ** then check if i can do any of these ops on the vertices I update */

	for(i=0; i<size; i++)
	{
		/* if(G[i].adjVertices.degree == 0)
 		 * 	
		 * {
		 * do nothing, but note that we don't count this as
 		 * a vertex that belongs to Rg.
  		 * } */
		if(G[i].adjVertices.degree == 1)
		{	//printf("degree 1, vertex: %d\n",i);
			operation3_2(G, size, i);
		}
		if(G[i].adjVertices.degree == 2)
		{	//printf("degree 2, vertex: %d\n",i);
			operation3_3and4(G, size, i);
		}
	}
}

/******************************************************************************
** Function:	getMaxDegreeVertex
** Description: Function finds the vertex of maximum degree in r(<P>)
** Inputs:	G - the graph
**		size - number of vertices in G
** Return Value: vert - the vertex of maximum degree
** Side Effects:
** Created:	Kerri Morgan	1 April, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int getMaxDegreeVertex(Vertex* G, int size)
{
	int i;
	int max=0;
	int vert =-1;

	for(i=0; i<size; i++)
	{
		if(G[i].adjVertices.degree > max)
		{
			max = G[i].adjVertices.degree;
			vert =i;
		}
	}
	return vert;
}



/******************************************************************************
** Function:	furtherReduceGraph
** Description: Function finds the reduced graph of all vertices in P.
** Inputs:	G - the graph (P is a subset of this graph)
**		size - number of vertices in this graph
**		adjList - List of vertices adjacent to the vertex that was last
**		removed from P
** Return Value:none
** Side Effects:updates the linked list structure in G to reflect the structure **		of P.
** Created:	Kerri Morgan	6 April, 2005
** Modified:	
** Comments:	Modified version of getReducedGraph function.  This is because
**		there is no need to check entire reduced graph, only those 
** 		vertices adjacent to the vertex just removed
** Acknowledgements:
******************************************************************************/
void furtherReduceGraph(Vertex* G, int size, List* adjList)
{
	int i ; 
	Node* cur;
    /* for each vertex in G, if I can do one of operations do so
     ** then check if i can do any of these ops on the vertices I update */

	cur=adjList->startPtr;
	while(cur!=NULL)
	{
		i=cur->vertex;		
		if(G[i].member == KNOWN)
		{
			/* if(G[i].adjVertices.degree == 0)
 		 	* 	
		 	* {
		 	* do nothing, but note that we don't count this as
 		 	* a vertex that belongs to Rg.
  		 	* } */
			if(G[i].adjVertices.degree == 1)
			{
				operation3_2(G, size, i);
			}
			if(G[i].adjVertices.degree == 2)
			{
				operation3_3and4(G, size, i);
			}
		}
		cur=cur->next;
	}
}
