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


#include "graph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 2000000

/******************************************************************************
** Function:	readGraph
** Description: Function reads graph from a user specified file.
** Inputs:	size - stores the size of the graph
** Return Value:G - the graph in which the information is stored
** Side Effects:stores information in G
** Created:	Kerri Morgan	4th March, 2005
** Modified:	30th May, 2005 - calculates maximum degree of graph
** Comments:	
** Acknowledgements:
******************************************************************************/
Vertex* readGraph(int* size, int* d)
{
	char temp[MAX];
	FILE* inFile=NULL;
	int v, w, edges, vertices;
	int count=0;
	Vertex* G=NULL;
	char* tmpPtr;
	char num[MAX];
	int i;
	int degree =0;

	printf("Filename: ");
	fgets(temp, MAX, stdin);
	/* chomp of newline*/
	temp[strlen(temp)-1]='\0';
	inFile=fopen(temp, "r");
	if(inFile == NULL)
	{
		printf("Warning 2.1: Unable to open file %s\n", temp);
		return G; 
	}
	/* read from the file*/

	/* number of vertices */
	if((fscanf(inFile, "%d", &vertices) ==1) && (vertices > 0))
	{
		if((fscanf(inFile, "%d", &edges) !=1) || (edges < 0))
		{
		printf("Warning 2.3: Incorrect content in file - second line does not contain the number of edges in graph\n");
		fclose(inFile);
		return G;
		}
		G=initGraph(vertices);
		/* chew up newline*/
		fgets(temp, MAX,inFile);	

		v=0; /* currentVertex */

		if(G!=NULL)
		{
			while(fgets(temp, MAX, inFile))
			{
			/* first number is the degree, rest is vertices */
				sscanf(temp, "%s", num);
				tmpPtr=temp+strlen(num)+1;
				sscanf(num, "%d", &G[v].degree);
				if(G[v].degree <0 || G[v].degree >= vertices)
				{
					printf("Warning 2.7: Incorrect content in file - invalid degree (%d) of vertex %d\n", G[v].degree, v);
					fclose(inFile);
					return NULL;
				}

				/* update maximum degree in graph */
				if(G[v].degree > degree)
				{
					degree = G[v].degree;
				}
				/* create space for the adjacency list */
				G[v].adjVerts = (int*)malloc(sizeof(int)*G[v].degree);
				if(G[v].adjVerts==NULL)
				{
					fprintf(stderr,"ERROR 2.1: Unable to allocate space for graph\n");
					exit(1);
				}
				i=0;
				while(sscanf(tmpPtr, "%s", num)==1)
				{
					if(i>= G[v].degree)
					{
						printf("WARNING 2.9: Vertex %d is adjacent to more than specified number of vertices\n", v);
						fclose(inFile);
						killGraph(G, vertices);
						return NULL;
					}
					tmpPtr=tmpPtr+strlen(num)+1;
					if(sscanf(num, "%d", &w)!=1)
					{
						printf("WARNING 2.10: Error in reading adjacent vertex from file - %s\n", num);
						fclose(inFile);
						killGraph(G,vertices);
						return NULL;
					}

					/* invalid file if multi-edge,loop or if v>w no v in w's adjacency list */
					if(validEdge(G,vertices, v,w)==FALSE)
					{
						fclose(inFile);
						killGraph(G,vertices);
						return NULL;
					}
					
					/* keep track of number of edges */
					if(w>v)
					{
						count++; /* increment the number of edges seen so far */
					}
					
					/* insert information */
					G[v].adjVerts[i]=w;
					i++;
				}
				if(i<G[v].degree)
				{
					printf("WARNING 2.15: Vertex %d is degree %d, but file only lists %d adjacent vertices\n", v, G[v].degree, i);
					fclose(inFile);
					killGraph(G, vertices);
					return NULL;
				}
				v++; /* next vertex */
			}
		}
		else
		{
			return G;
		}
	}
	else
	{
		printf("Warning 2.2: Incorrect content in file - first line does not contain the number of vertices in graph\n");
		fclose(inFile);
		return G; 
	}
	/*
	if(validateGraph(G, vertices) == FALSE)
	{
		killGraph(G, vertices);
		fclose(inFile);
		return NULL;
	}
	*/

	*size = vertices;
	*d = degree;
	fclose(inFile);	
	return G;
}

/******************************************************************************
** Function:	initGraph
** Description:	Function creates and initialise a graph
** Inputs:	size - the number of vertices in the graph
** Return Value:A pointer to the graph created
** Side Effects:Dynamically allocates the array of vertices.  Each of these 
**		vertices has an adjacency list initialised
** Created:	Kerri Morgan		3rd March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
Vertex* initGraph(int size)
{
	Vertex* G=NULL;
	int i;

	G = (Vertex*)malloc(sizeof(Vertex)*size);
	if(G==NULL)
	{
		fprintf(stderr, "ERROR 2.3: Insufficient memory to create graph\n");
		exit(1);
	}

	for(i=0; i<size; i++)
	{
		G[i].degree = 0;
		G[i].member = UNKNOWN;
		initList(&G[i].adjVertices);
		G[i].adjVerts=NULL;
	}	
	
	return G;
}

/******************************************************************************
** Function:	printGraph
** Description:	Function prints the information concerning the graph to screen.
**		Used for testing.
** Inputs:	G - points to graph
**		size - number of vertices in graph
** Return Value:none
** Side Effects:
** Created:	Kerri Morgan	4th March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void printGraph(Vertex* G, int size)
{
	int i,j;
	
	if(G!=NULL)
	{
		for(i=0; i<size; i++)
		{
			if(G[i].degree>0)
			{
				printf("Vertex %d is adjacent to the following vertices:\n", i);
				if(G[i].adjVerts !=NULL)
				{
					for(j=0; j<G[i].degree; j++)
					{
						printf("%d ", G[i].adjVerts[j]);
					}
					printf("\n");
				}
				else
				{
					printList(&G[i].adjVertices);
				}
			}
			else
			{
				printf("Vertex %d is not adjacent to any vertices\n",i);
			}
		}
	}
	else
	{
		printf("No graph currently read to print\n\n");
	}
}

/******************************************************************************
** Function:	killGraph
** Description: Function frees all memory associated with a graph G
** Inputs:	G - the graph
**		size - the number of vertices in graph;
** Return Value: none
** Side Effects:Graph G and adjacency lists have memory freed
** Created:	Kerri Morgan	6th March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void killGraph(Vertex* G, int size)
{
	int i;

	if(G!=NULL)
	{
		/* free memory from adjacency lists */
		for(i=0; i<size; i++)
		{
			if(G[i].adjVerts!=NULL)
			{
				free(G[i].adjVerts);
			}
			killList(&G[i].adjVertices);
		}
		free(G);
	}
}
/******************************************************************************
** Function:	addEdge
** Description:	Function adds an edge to the graph
** Inputs:	G - the graph
**		size - number of vertices in graph
**		v, w - two vertices that describe an edge
** Return Value: inserted - TRUE if inserted else FALSE
** Side Effects:If successful inserts an addtional edge in graph
** Created:	Kerri Morgan	6 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int addEdge(Vertex* G, int size, int v, int w)
{
	int inserted = FALSE;

	/* check for invalid input */
	if((v<0) || (w<0) || (v>= size) || (w>=size))
	{
		printf("WARNING 2.6: One or more vertices in edge (%d %d) out of range\n", v,w);
		return inserted;
	}

	/* no loops */
	if(v==w)
	{
		printf("WARNING 2.7: Edge (%d %d) is a loop - no loops permitted\n", v,w);
		return inserted;
	}

	/* no multi-edges */
	if(edgeExists(G, size, v,w)==TRUE)
	{
		printf("WARNING 2.8: Edge (%d %d) is already in graph - no multi-edges permitted\n", v, w);
		return inserted;
	}	

	insertInList(&G[v].adjVertices, w);
	insertInList(&G[w].adjVertices, v);
	G[v].degree++;
	G[w].degree++;
	inserted=TRUE;
	return inserted;
}

/******************************************************************************
** Function:	edgeExists
** Description:	Function checks if edge is already in graph
** Inputs:	G - graph
**		size - number of vertices
**		v,w - two vertices describing an edge
** Return Value:TRUE (if edge exists) otherwise FALSE
** Side Effects:
** Created:	Kerri Morgan	6 March, 2005
** Modified:
** Comments:	This function works with the linked list implementation of
**		adjacecny list
** Acknowledgements:
******************************************************************************/
int edgeExists(Vertex* G, int size, int v, int w)
{
	return inList(&(G[v].adjVertices), w);
}

/******************************************************************************
** Function:	initMembers	
** Description: Function sets all members as UNKNOWN
** Inputs:	G - graph
**		size - number of vertices in graph
** Return Value:none
** Side Effects:updates G[i].member to UNKNOWN for all i.
** Created:	Kerri Morgan		6 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void initMembers(Vertex* G, int size)
{
	int i;

	for(i=0; i<size; i++)
	{
		G[i].member=UNKNOWN;
	}
}
/******************************************************************************
** Function:	initAdjLists	
** Description: Function sets all lists to 0
** Inputs:	G - graph
**		size - number of vertices in graph
** Return Value:none
** Side Effects:updates G[i].adjList.degree to 0 for all i
** Created:	Kerri Morgan		6 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void initAdjLists(Vertex* G, int size)
{
	int i;

	for(i=0; i<size; i++)
	{
		killList(&G[i].adjVertices);
	}
}
/******************************************************************************
** Function:	validEdge
** Description:	Function tests if the edge (v,w) is a valid edge to be inserted
**		into the graph.
** Inputs:	G - the graph
**		size - size of the graph
**		v,w - two vertices that define the edge
**		      (NOTE: v is the current vertex, so G holds all adjacency
**		       list information for vertices 0..(v-1).
** Return Value:TRUE - inserting (v,w) would not create a multi-edge or loop
**		and (v,w) refers to a valid edge.  In all other cases FALSE
**		is returned
** Side Effects: A informative message is outputted to the screen
** Created:	Kerri Morgan	10 March, 2005
** Modified:
** Comments:	This function works on the basis of the file format being
**		as follows:
**		Number of vertices
**		Number of edges
**		For each vertex - degree followed by list of vertices adjacent
**		to the vertex
**		Thus if v<w we cannot check that w has v in its adjacency list,
**		but if v>w we can check if w has v in its adjacenty list
** Acknowledgements:
******************************************************************************/
int validEdge(Vertex* G, int size, int v, int w)
{
	int i;

	if(v==w)
	{
		printf("WARNING 2.11: File contains an edge that is a loop (%d %d)\n", v,w);
		return FALSE;
	}
	if((v<0) || (w<0) || (v>=size) || (w>=size))
	{
		printf("WARNING 2.12: File contains an invalid edge (%d %d)\n", v,w);
		return FALSE;
	}
	if(v>w)
	{
		for(i=0; i<G[w].degree; i++)
		{
			/* w is adjacent to v, so v can be adjacent to w */
			if(G[w].adjVerts[i]==v)
			{
				return TRUE;
			}
		}
		printf("WARNING 2.16: Vertex %d is listed as adjacent to vertex %d,\n but vertex %d is not listed as adjacent to vertex %d\n", w,v,v,w);
		return FALSE;
		
	}
	return TRUE; /* v is adjacent to w, and no adjaceny list exists for w*/


}
/******************************************************************************
** Function:	validateGraph
** Description:	Function checks graph is valid.  
**		When a graph is read in from a file, for any w listed as 
**		adjacent to v where w> v, we cannot check the file
**		has listed v as adjacent to w.
**		This function scans the adjacency lists and checks for errors
**		of this kind
** Inputs:	G - graph
**		size - number of vertices
** Return Value:TRUE - valid graph, otherwise FALSE
** Side Effects:
** Created:	Kerri Morgan	10 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int validateGraph(Vertex* G, int size)
{
	int v,w;
	int i, j;
	int flag;

	for(v=0; v<size; v++)
	{
		for(i=0; i<G[v].degree; i++)
		{
			w=G[v].adjVerts[i];
			if(w>v)
			{
				flag=0;
				for(j=0; j<G[w].degree; j++)
				{
					if(G[w].adjVerts[j]==v)
					{
						flag=1;
						break;
					}
				}
				if(flag ==0)
				{
					printf("WARNING 2.14: Vertex %d is listed as adjacent to vertex %d,\n but vertex %d is not listed as adjacent to vertex %d\n", v,w,w,v);
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

/******************************************************************************
** Function:	addEdge2
** Description:	Function adds an edge to the graph
** Inputs:	G - the graph
**		size - number of vertices in graph
**		v, w - two vertices that describe an edge
** Return Value: inserted - TRUE if inserted else FALSE
** Side Effects:If successful inserts an additional edge in graph
** Created:	Kerri Morgan	18 March, 2005
** Modified:
** Comments:	Functions inserts into array implementation of adj.list.
** Acknowledgements:
******************************************************************************/
int addEdge2(Vertex* G, int size, int v, int w)
{
	if((v<0) || (w<0) || (v>=size) || (w>=size))
	{
		fprintf(stderr, "ERROR 2.2: Attempting to insert invalid edge (%d %d) into graph\n", v, w);
		exit(1);
	}
	if(edgeExists2(G, size, v, w)==TRUE)
	{
		return FALSE; /* edge is already in graph */
	}

	G[v].adjVerts[G[v].degree]=w;
	G[v].degree++;
	G[w].adjVerts[G[w].degree]=v;
	G[w].degree++;
	return TRUE;
}

/******************************************************************************
** Function:	edgeExists2
** Description:	Function checks if edge is already in graph
** Inputs:	G - graph
**		size - number of vertices
**		v,w - two vertices describing an edge
** Return Value:TRUE (if edge exists) otherwise FALSE
** Side Effects:
** Created:	Kerri Morgan	18 March, 2005
** Modified:
** Comments:	This function works with the array implementation of
**		adjacency list
** Acknowledgements:
******************************************************************************/
int edgeExists2(Vertex* G, int size, int v, int w)
{
	int i;

	for(i=0; i<G[v].degree; i++)
	{
		if(G[v].adjVerts[i] == w)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/******************************************************************************
 * ** Function:    saveGraph
 * ** Description: Function saves a randomly generated graph to kerri_rand_graph.
 * **              Allows graph to be examined at a latter date
 * **              for testing purposes.
 * ** Inputs:      graph - the graph to be saved
 * **              size - number of vertices
 * **              edges - number of edges in graph
 * ** Return Value:
 * ** Side Effects:
 * ** Created:     Kerri Morgan    21 March, 2005
 * ** Modified:	   Moved this function to graph.c from graphGen.c
 * **		   11th April, 2005
 * ** Comments:
 * ** Acknowledgements:
 * ******************************************************************************/
void saveGraph(Vertex* G, int size, int edges)
{
	int i,j;
	FILE* infile=NULL;
	if(G==NULL)
	{
		printf("WARNING 4.2: Unable to save graph\n");
	}
	else
	{
		infile=fopen("kerri_rand_graph.txt", "w");
		if(infile == NULL)
		{
			printf("WARNING 4.3: Unable to open kerri_rand_graph.txt to save graph\n");
		}
		else
		{
			fprintf(infile, "%d\n", size);
			fprintf(infile, "%d\n", edges);
			for(i=0; i<size; i++)
			{
				fprintf(infile, "%d", G[i].degree);
				for(j=0; j<G[i].degree; j++)
				{
					fprintf(infile, " %d", G[i].adjVerts[j]);
				}
				fprintf(infile, "\n");
			}
		}
		fclose(infile);
	}
}

/******************************************************************************
** Function:	writePlanarSubgraph
** Description:	Functions writes the planar subgraph to file
** Inputs:	G - the graph
**		size - number of vertices in graph
** Return Value: none
** Side Effects:
** Created:	Kerri Morgan	19th April, 2005
** Modified:	19th April - to count number of edges and check it satisfies
**		Euler's characteristic NOTE: this assumes graph is connected
**		and it may not be.
** Comments:	Note: incorrect number of edges saved to file.
**		This function is really to allow subgraph to be examined
**		to confirm results are correct.  I don't consider cost of
**		additional computing of edge number justified
**		This is not actually the planar subgraph, instead the planar 
**		subgraph and a set of isolated vertices(which really belong in 
**		R)
** Acknowledgements:
******************************************************************************/
void writePlanarSubgraph(Vertex* G, int size)
{
	FILE* outfile=NULL;
	int i,j;
	int v;
	int count=0;
	int numV=0;
	int numE=0;

	outfile = fopen("SUBGRAPH.txt", "w");
	if(outfile == NULL)
	{
		fprintf(stderr, "WARNING 2.16: Unable to open SUBGRAPH.txt to store subgraph\n");
		exit(1);
	}

	/* n=size, m!= size, but estimated as such */
	fprintf(outfile, "%d\n%d\n", size, size); 

	/* lists of adjacent vertices */

	for(i=0; i<size; i++)
	{
		if(G[i].member == KNOWN)
		{
			numV++;
			count =0; 
			/* get new degree of vertex*/
			for(j=0; j<G[i].degree; j++)
			{
				v=G[i].adjVerts[j];
				if(G[v].member == KNOWN)
				{
					numE++;
					count++;
				}
			}
			fprintf(outfile, "%d ", count);
			for(j=0; j<G[i].degree; j++)
			{
				v=G[i].adjVerts[j];
				if(G[v].member == KNOWN)
				{
					fprintf(outfile, "%d ", v);
				}
			}
			
		}
		else
		{
			fprintf(outfile, "0");
		}
		fprintf(outfile, "\n");
	}
	numE/=2;
	if(numE > (3*numV-6))
	{
		if(numV > 2)
		{
			printf("TESTING: graph does not satisfy m<=3n-6\n");
		}
	}
	fclose(outfile); 
}

/******************************************************************************
** Function:	readGraphFromFile
** Description: Function reads graph from a user specified file.
** Inputs:	size - stores the size of the graph
**		d - degree of the graph
**		filename - the name of the file
** Return Value:G - the graph in which the information is stored
** Side Effects:stores information in G
** Created:	Kerri Morgan	4th August
** Modified:    modified version of readGraph
** Comments:	
** Acknowledgements:
******************************************************************************/
Vertex* readGraphFromFile(char* filename, int size, int d)
{
	FILE* inFile=NULL;
	int v, w, edges, vertices;
	int count=0;
	Vertex* G=NULL;
	char* tmpPtr;
	char num[MAX];
	char tmp[MAX];
	int i;

	inFile=fopen(filename, "r");
	if(inFile == NULL)
	{
		fprintf(stderr,"ERROR 2.4: Unable to open file %s\n", filename);
		exit(1);
	}
	/* read from the file*/

	/* number of vertices */
	if((fscanf(inFile, "%d", &vertices) ==1) && (vertices > 0))
	{
		if((fscanf(inFile, "%d", &edges) !=1) || (edges < 0))
		{
		fprintf(stderr, "ERROR 2.5: Incorrect content in file - second line does not contain the number of edges in graph\n");
		fclose(inFile);
		exit(1);
		}
		G=initGraph(vertices);
		/* chew up newline*/
		fgets(tmp, MAX,inFile);	

		v=0; /* currentVertex */

		if(G!=NULL)
		{
			while(fgets(tmp, MAX, inFile))
			{
			/* first number is the degree, rest is vertices */
				sscanf(tmp, "%s", num);
				tmpPtr=tmp+strlen(num)+1;
				sscanf(num, "%d", &G[v].degree);
				if(G[v].degree <0 || G[v].degree >= vertices)
				{
					fprintf(stderr,"Error 2.8: Incorrect content in file - invalid degree (%d) of vertex %d\n", G[v].degree, v);
					fclose(inFile);
					exit(1);
				}

				/* create space for the adjacency list */
				G[v].adjVerts = (int*)malloc(sizeof(int)*G[v].degree);
				if(G[v].adjVerts==NULL)
				{
					fprintf(stderr,"ERROR 2.9: Unable to allocate space for graph\n");
					exit(1);
				}
				i=0;
				while(sscanf(tmpPtr, "%s", num)==1)
				{
					if(i>= G[v].degree)
					{
						fprintf(stderr, "ERROR 2.10: Vertex %d is adjacent to more than specified number of vertices\n", v);
						fclose(inFile);
						killGraph(G, vertices);
						exit(1);
					}
					tmpPtr=tmpPtr+strlen(num)+1;
					if(sscanf(num, "%d", &w)!=1)
					{
						fprintf(stderr, "ERROR 2.11: Error in reading adjacent vertex from file - %s\n", num);
						fclose(inFile);
						killGraph(G,vertices);
						exit(1);
					}

					/* invalid file if multi-edge,loop or if v>w no v in w's adjacency list */
					if(validEdge(G,vertices, v,w)==FALSE)
					{
						fclose(inFile);
						killGraph(G,vertices);
						exit(1);
					}
					
					/* keep track of number of edges */
					if(w>v)
					{
						count++; /* increment the number of edges seen so far */
					}
					
					/* insert information */
					G[v].adjVerts[i]=w;
					i++;
				}
				if(i<G[v].degree)
				{
					printf("ERROR 2.12: Vertex %d is degree %d, but file only lists %d adjacent vertices\n", v, G[v].degree, i);
					fclose(inFile);
					killGraph(G, vertices);
					exit(1);
				}
				v++; /* next vertex */
			}
		}
		else
		{
			return G;
		}
	}
	else
	{
		fprintf(stderr, "ERROR 2.7: Incorrect content in file - first line does not contain the number of vertices in graph\n");
		fclose(inFile);
		exit(1);
	}
	if(validateGraph(G, vertices) == FALSE)
	{
		killGraph(G, vertices);
		fclose(inFile);
		fprintf(stderr, "ERROR 2.13: Graph provided was not a valid graph\n");
		return NULL;
	}
	fclose(inFile);	
	return G;
}
