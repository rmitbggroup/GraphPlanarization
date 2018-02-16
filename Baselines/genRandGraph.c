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

#include "genRandGraph.h"
#include <stdlib.h>
#include <stdio.h>

/******************************************************************************
** Function: 	genGraph3
** Description: Function generates a random d-regular graph.
** Inputs:	size - number of vertices
**		d - degree of graph
** Return Value:G - the graph if successful, otherwise NULL
** Side Effects:
** Created:	Kerri Morgan	11 April, 2005
** Modified:
** Comments:	
** Acknowledgements:	The value (d-d(u)) were taken from Wormald and Steiger's
**			Algorithm.  (see graphGen.c)
**			Although presented differently, this algorithm is 
**			equivalent to the algorithm presented by Steger
**			and Wormald in their paper Generating Random Graph
**			Quickly. Combinatorics, Probability and Computing, 
**			8, 1999.  p. 377-396
******************************************************************************/
Vertex* genGraph3(int size, int d)
{
	Vertex* G=NULL;
	unsigned long int* r=NULL;
	int v, w;
	unsigned long int R;
	unsigned long int m=0;
	unsigned long int m2=0;
	unsigned long int used=0; /* number of vertices used up, that is
				     with degree = d ******************/
	unsigned long int numPossibleDestinations;
	double p, q, temp;
	double sum;
	int adjacent;
	int lostDestinations;

	if((d>size) || (d<0) || (size <1))
	{
		printf("WARNING 6.1: Cannot create graph with %d vertices and degree %d\n", size, d);
		return NULL;
	}
	else if((size*d)%2 ==1)
	{
		printf("WARNING 6.2: Cannot create graph as nd is odd (%d vertices and degree %d)\n", size, d);
		return NULL;
	}

	/* graph has degree 0 - disjoint vertices */
	if(d==0)
	{
		G=initGraph(size);
		return G;
	}

	/* create space for r */
	r= (unsigned long int*)malloc(sizeof(unsigned long int)*size);
	if(r == NULL)
	{
		fprintf(stderr, "ERROR 6.1: Insufficient memory to create r\n");
		exit(1);
	}

	/* initially all entries of r are d */
	for(v=0; v<size; v++)
	{
		r[v]=d;
	}

	/* R= sum of all entries in r*/
	R=size*d;

	/* From each vertex the number of possible destinations is initially
	 * size-1 *********************************************************/
	numPossibleDestinations=size *(size-1);


	G=initGraph(size);
	if(G==NULL)
	{
		free(r);
		return NULL;
	}
	/* set all the arrays of adjacent vertices to be size d */
	for(v=0; v<size;  v++)
	{
		G[v].adjVerts=(int*)malloc(sizeof(int)*d);
		if(G[v].adjVerts==NULL)
		{
			fprintf(stderr, "ERROR 6.2: Insufficient memory to create graph\n");
			free(r);
			killGraph(G, size);
			exit(1);
		}
	}

	while((numPossibleDestinations > 0) && (R>0))
	{
		p = drand48();
		q = drand48();
		/* ensure that p is less (or equal to) than q */
		if(p>q)
		{
			temp=p;
			p=q;
			q=temp;
		}
		sum = r[0];
		v=0;	
//printf("TEST %f %f\n", p , q);	
//printf("TEST %f is sum/R\n", sum/R);
		while ((p> sum/R) && (v < size))
		{
			v++;
			sum+=r[v];
//printf("TEST %f is sum/R\n", sum/R);
//printf("TEST %f is sum\n", sum);
		}
//		printf("FOUND %d is vert\n", v);

		w=v;
		while((q>sum/R) && (w<size))
		{
			w++;
			sum+=r[w];
		}
//		printf("%d %d is edge\n", v, w);
		/* avoid loops and multi-edges*/
		if((v!=w) && (v!=size) && (w!=size) && (edgeExists2(G, size, v,w)==FALSE))
		{
			if(addEdge2(G, size, v, w) == TRUE)
			{
		//		printf("TST:Added edge %d %d\n", v,w);
				/* update r */
				r[v]--;
				r[w]--;
				R-=2;
				m2++;

				if((G[v].degree == d) && (G[w].degree==d))
				{
					numPossibleDestinations-=2;  /* vw and wv */

					/* vertex v is no longer a possible destination */
					adjacent = numAdjVertOfTotalDegree(G,d, v);
					adjacent --; /* Don't count w*/

					/* Each vertex initially can go to size - 1 destinations.
					 * d of these have been actually used in edges from v, 
					 * The used vertices in the graph have already caused any possible
					 * destinations of v starting with these vertices to be decremented from 
					 * numPossibleDestinations, so we don't repeat them; on the
					 * other hand some of these may be already counted in the adjacent vertices
					 * so adjacent counteracts any double calculations */
					lostDestinations = 2*(size - d -1 -used+adjacent);
					numPossibleDestinations-=lostDestinations;
				
					/* vertex w is no longer a possible destination*/
					adjacent = numAdjVertOfTotalDegree(G,d, w);
					adjacent --; /* Don't count v*/
					lostDestinations = 2*(size - d -1 -used+adjacent);
					numPossibleDestinations-=lostDestinations;

					used+=2;
				}
				else if(G[v].degree==d)
				{
					numPossibleDestinations -=2;
					adjacent = numAdjVertOfTotalDegree(G,d, v);
					lostDestinations = 2*(size-d-1-used+adjacent);
					numPossibleDestinations-=lostDestinations;

					used +=1;
				}
				else if(G[w].degree == d)
				{
					numPossibleDestinations -=2;
					adjacent = numAdjVertOfTotalDegree(G,d, w);
					lostDestinations = 2*(size-d-1-used+adjacent);
					numPossibleDestinations-=lostDestinations;

					used+=1;
				}
				else
				{
					numPossibleDestinations-=2;
				}
//				printf("%ld\n", numPossibleDestinations);
			}
		}

	}
	if(m2 == (d*size)/2)
//	if(m< ((size*(size-1))/2))
	{
		saveGraph(G, size, m);
		free(r);
		return G;
	}
	else
	{
//		printf("TESTING: GRAPH IS INADEQUATE\n");
		killGraph(G, size);
		G=NULL;
		free(r);
		return genGraph3(size, d);
	}
}
/******************************************************************************
** Function:	numAdjVertOfTotalDegree
** Description:	Function calculates the number of vertices adjacent to vertex v  
**		of degree d
** Inputs:	G - the graph
**		d - the degree
**		v - the vertex
** Return Value:the number of vertices with degree d adjacent to v
** Side Effects: none
** Created:	Kerri Morgan	11 April, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int numAdjVertOfTotalDegree(Vertex* G, int d, int v)
{
	int i;
	int num=0;
	int w;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if(G[w].degree == d)
		{
			num++;
		}
	}
	return num;
}
