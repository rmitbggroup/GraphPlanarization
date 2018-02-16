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

#include "bollobas.h"
#include <stdlib.h>
#include <stdio.h>

/*****************************************************************************
****Function: createRandomGraph
****Description: Function creates a random graph of specified size with at most
****             degree d.
****Inputs:      size - number of vertices in required graph
****Outputs:     G - the graph if successful, otherwise NULL
****Comments:
****Created:     27 December, 2004       Kerri Morgan
****Modified:	 2nd May, 2005 	Additional parameter d, to allow graphs of 
****		 differing degrees to be created.
****Acknowledgement: 	Method by Dr. Bela Bollobas 
******************************************************************************/
Vertex* createRandomGraph(int size, int d)
{
	int* tempArray=NULL;
	unsigned int i,j;
	int v1, v2;     /* vertices */
	unsigned int num;
	Vertex* G=NULL;
	unsigned int numEdges = 0;
	int flag;

	if((d<0) || (d>=size))
	{
		printf("WARNING 8.1: Invalid value - cannot have a %d-regular graph\n", d);
		return G;

	}
	if((d*size)%2==1)
	{
		printf("WARNING 8.1: Invalid value - cannot have a %d-regualr graph with %d vertices\n", d, size);
		return G;
	}
	if(d==0)
	{
		G=initGraph(size);
		return G;
	}
	/* create space for dn 'pseudo-vertices' */
	tempArray = (int*)malloc(sizeof(int)*size*d);
	if(tempArray==NULL)
	{
		fprintf(stderr, "ERROR 8.1: Insufficient memory to randomly create graph\n");
												exit(1);
	}
	/* initialise tempArray */
	initArray(tempArray, size*d);

	/* each loop generates 2 of the d*size vertices*/
	num= (d*size)/2;
	for(i=0; i<num; i++)
	{
		v1=rand()%(size*d);
		v2=rand()%(size*d);
		/* each 'pseudo-vertex' can only be used once */
		if((tempArray[v1]!=-1) || (tempArray[v2]!=-1))
		{
				/* invalid edge*/
				i--;
		}
		else if(v1 == v2) /* loop in pseudo-graph => loop in graph */
		{
			i--;
		}
		else
		{
			tempArray[v1]=v2;
			tempArray[v2]=v1;
		}
	}
	/* create real graph */
	G = initGraph(size);
	for(i=0; i<size; i++)
	{
		 G[i].adjVerts=(int*)malloc(sizeof(int)*d);
	         if(G[i].adjVerts==NULL)
		{
		fprintf(stderr, "ERROR 8.2: Insufficient memory to create graph\n");
			killGraph(G, size);
			exit(1);
		}
			 
	}

	num = size*d;
	/* collapse pseudo-graph */
	for(i=0; i<num; i++)
	{
		v1=i/d;
		v2=tempArray[i]/d;
		if(v1 == v2)
		{
			break;
		//	printf("TESTING : loop\n");
		}
		else
		{
			/* check if edge exists */
			if(v1 > v2) /* any edge v1<v2 already added */
			{
				flag =0;
				for(j=0; j<G[v1].degree; j++)
				{
					if(G[v1].adjVerts[j]==v2)
					{
						flag =1;
						break;
					}
				}
				if(flag == 0)	/* edge does not exist at the present */
				{
					j=G[v1].degree;
					numEdges++;
					G[v1].adjVerts[j]=v2;
					G[v1].degree++;
					j=G[v2].degree;
					G[v2].adjVerts[j]=v1;
					G[v2].degree++;
				}
			}
		}
	}
	free(tempArray);
	if(numEdges !=  (d*size)/2)
	{
		killGraph(G, size);
		//printf("BAD GRAPH\n");
		G=NULL;
		G=createRandomGraph(size, d);
	}
	else
	{
		saveGraph(G,size, d*(size/2));
	}
	return G;
}
/******************************************************************************
** Function:	initArray
** Description:	Function initialises all elements in array to -1
** Inputs:	arr - the array
**		size - size of the array
** Return Value:none
** Side Effects:initialises the array
** Created:	Kerri Morgan	2nd May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void initArray(int* arr, int size)
{
	int i;

	for(i=0; i<size; i++)
	{
		arr[i]=-1;
	}
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
/******************************************************************************
** Function:	createB_graph	
** Description:	Function repetitively calls createRandomGraph2 until a 
**		successful graph is produced
** Inputs:	size - number of vertices required
**		d - degree
** Return Value:G - the graph
** Side Effects:
** Created:	Kerri Morgan	12 August, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
Vertex* createB_graph(int size, int d)
{
	Vertex* G = NULL;

	if((d<0) || (d>=size))
	{
		printf("WARNING 8.1: Invalid value - cannot have a %d-regular graph\n", d);
		return G;

	}
	if((d*size)%2==1)
	{
		printf("WARNING 8.1: Invalid value - cannot have a %d-regualr graph with %d vertices\n", d, size);
		return G;
	}

	while(G == NULL)
	{
		G = createRandomGraph2(size, d);

	}
	return G;
}

/*****************************************************************************
****Function: createRandomGraph2
****Description: Function creates a random graph of specified size with at most
****             degree d.
****Inputs:      size - number of vertices in required graph
****Outputs:     G - the graph if successful, otherwise NULL
****Comments:
****Created:     27 December, 2004       Kerri Morgan
****Modified:	 2nd May, 2005 	Additional parameter d, to allow graphs of 
****		 differing degrees to be created.
****		 Further modified to remove recursive call
****Acknowledgement: 	Method by Dr. Bela Bollobas 
******************************************************************************/
Vertex* createRandomGraph2(int size, int d)
{
	int* tempArray=NULL;
	unsigned int i,j;
	int v1, v2;     /* vertices */
	unsigned int num;
	Vertex* G=NULL;
	unsigned int numEdges = 0;
	int flag;

	if((d<0) || (d>=size))
	{
		printf("WARNING 8.1: Invalid value - cannot have a %d-regular graph\n", d);
		return G;

	}
	if((d*size)%2==1)
	{
		printf("WARNING 8.1: Invalid value - cannot have a %d-regualr graph with %d vertices\n", d, size);
		return G;
	}
	if(d==0)
	{
		G=initGraph(size);
		return G;
	}
	/* create space for dn 'pseudo-vertices' */
	tempArray = (int*)malloc(sizeof(int)*size*d);
	if(tempArray==NULL)
	{
		fprintf(stderr, "ERROR 8.1: Insufficient memory to randomly create graph\n");
												exit(1);
	}
	/* initialise tempArray */
	initArray(tempArray, size*d);

	/* each loop generates 2 of the d*size vertices*/
	num= (d*size)/2;
	for(i=0; i<num; i++)
	{
		v1=rand()%(size*d);
		v2=rand()%(size*d);
		/* each 'pseudo-vertex' can only be used once */
		if((tempArray[v1]!=-1) || (tempArray[v2]!=-1))
		{
				/* invalid edge*/
				i--;
		}
		else if(v1 == v2) /* loop in pseudo-graph => loop in graph */
		{
			i--;
		}
		else
		{
			tempArray[v1]=v2;
			tempArray[v2]=v1;
		}
	}
	/* create real graph */
	G = initGraph(size);
	for(i=0; i<size; i++)
	{
		 G[i].adjVerts=(int*)malloc(sizeof(int)*d);
	         if(G[i].adjVerts==NULL)
		{
		fprintf(stderr, "ERROR 8.2: Insufficient memory to create graph\n");
			killGraph(G, size);
			exit(1);
		}
			 
	}

	num = size*d;
	/* collapse pseudo-graph */
	for(i=0; i<num; i++)
	{
		v1=i/d;
		v2=tempArray[i]/d;
		if(v1 == v2)
		{
			break;
		//	printf("TESTING : loop\n");
		}
		else
		{
			/* check if edge exists */
			if(v1 > v2) /* any edge v1<v2 already added */
			{
				flag =0;
				for(j=0; j<G[v1].degree; j++)
				{
					if(G[v1].adjVerts[j]==v2)
					{
						flag =1;
						break;
					}
				}
				if(flag == 0)	/* edge does not exist at the present */
				{
					j=G[v1].degree;
					numEdges++;
					G[v1].adjVerts[j]=v2;
					G[v1].degree++;
					j=G[v2].degree;
					G[v2].adjVerts[j]=v1;
					G[v2].degree++;
				}
			}
		}
	}
	free(tempArray);
	if(numEdges !=  (d*size)/2)
	{
		killGraph(G, size);
		//printf("BAD GRAPH\n");
		G=NULL;
	}
	else
	{
		saveGraph(G,size, d*(size/2));
	}
	return G;
}
