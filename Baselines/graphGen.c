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


#include "graphGen.h"
#include <stdio.h>
#include <stdlib.h>

/********* Format of r *******************************************************
 * 	row1	
 * 	row2
 * 	...
 * 	row n-1  col0	col1 col2  ... col n-2
 * 	A is a lower triangular matrix - diagonal entries are excluded
 * 	*********************************************************************/

/******************************************************************************
** Function: 	genGraph
** Description:	Function generates a random graph of size n
** Inputs:    	size - number of vertices required in graph to be generated
**		d - the degree of the graph to be generated
** Return Value:graph - the graph generated (or NULL if unsuccessful)
** Side Effects:
** Programmed by:	Kerri Morgan	17 March, 2005
** Modified:
** Comments:		23 March, 2005 - no longer uses A, instead keeps a
**			single sum of items in r.  Attempting to reduce the
**			time taken for large graphs.
**			24 March, 2005 - changing data types to floats, as
**			larger range of values is required.  
**			(May possibly have graphs with n=10,000 and 
**			 m=9,999,999/2 edges and d = 9999 )
**			26th March, 2005.  Added the variable startVal.  This
**			variable iterates through starting at each row when
**			calculating cumulative probability.  This is to
**			avoid the last row in large graphs being largely 
**			excluded due to the entries all being > 0.999999
** Acknowledgements: Steger-Wormald Algorithm.  
**		     A.Steger and N.C.Wormald, Generating random regular graphs
**		     quickly, Combinatorics, Probability and Computing 8(1999)
**		     377-396.
******************************************************************************/
Vertex* genGraph(int size, int d)
{
	float aSize;
	float* r=NULL;
	double R;
	int i, j;
	Vertex* G=NULL;
	long int m=0; /* number of edges*/
	double p;
	int flag;
	float Asum; /* replaces A */
	double currentVal;

	if((d>size) || (d<0) || (size <1))
	{
		printf("WARNING 4.1: Cannot create graph with %d vertices and degree %d\n", size, d);
		return NULL;
	}

	if((d*size)%2 == 1)
	{
		printf("WARNING 4.5: Cannot create graph as nd is odd (%d vertices and degree %d)\n", size, d);
		return NULL;
	}
	/* graph has degree 0 - disjoint vertices */
	if(d==0)
	{
		G=initGraph(size);
		return G;
	}
	/* create enough space for a lower triangular matrix
	 * but not for the diagonal as we don't need edges that are
	 * loops */
	aSize=size*(size-1)/2;
	
	r=(float*) malloc(sizeof(float)*aSize);
	if(r==NULL)
	{
		fprintf(stderr, "ERROR 4.1: Insufficient memory to create r in genGraph\n");
		exit(1);

	}

	R=(double)aSize*d*d;
	
	/* initially all vertices have degree=0.
	 * so r(u,v)=d*d */
	for(i=1; i<size; i++)	/* each pair (i, j) s.t. i<j */
	{
		for(j=0; j<i; j++)
		{
			r[i*(i-1)/2 + j]=d*d;	/* row i*(i-1)/2, column j */
		}
	}


	G=initGraph(size);
	if(G==NULL)
	{
		free(r);
		return NULL;
	}

	/* set all the arrays of adjacent vertices to be size d */
	for(i=0; i<size;  i++)
	{
		G[i].adjVerts=(int*)malloc(sizeof(int)*d);
		if(G[i].adjVerts==NULL)
		{
			fprintf(stderr, "ERROR 4.2: Insufficient memory to create graph\n");
			free(r);
			killGraph(G, size);
			exit(1);
		}
	}

	while(R>0)
	{
		/* get a random value p */
		p=drand48();

/*		for(i=1; i<size; i++)
		{	for(j=0; j<i; j++)
			{
				printf("%f ",r[i*(i-1)/2+j]);
			}
			printf("\n"); 
		} */
		flag=0;

		/* Asum holds the sum of all entries up to r(i,j) */
		Asum = 0.0;

		for(i=1; i<size; i++)
		{
			/* only check rows where the degree of the vertex is
			** less than d **/
			if(G[i].degree < d)
			{
				for(j=0; j<i; j++)
				{
					Asum +=r[i*(i-1)/2+j];
					currentVal = (double)Asum/R;
					if(p<= currentVal)
					{
						if(addEdge2(G, size, i, j)==TRUE)
						{
//printf("%d %d\n", i, j);
							m++;
							R = update_r(G, size, d, r, i, j, R);
						}
						else
						{
							printf("EDGE Failed to add %d %d\n", i, j);
							exit(1);
						}
						flag=1;
						break;
					}
				}
				if(flag==1)
				{
					break;
				}
			}
		}
	}
	
	/* successfully completed */
	if(m == d*(size/2))
	{
		free(r);
		saveGraph(G, size, d*(size/2));
		return G;
	}
	else //if(((d*(size/2))-m) >= 1)
	{
	//	printGraph(G, size);
		printf("TESTING: Graph above is inadequate\n");
		saveGraph(G, size, m);
		killGraph(G, size);
		G=NULL;
		free(r);
		return genGraph(size, d);
	}
}

/******************************************************************************
** Function:	update_r
** Description:	Function updates the values in r to reflect changes due to 
**		an edge being added to G
** Inputs:	G - the graph
** 		size - size of graph
**		d - the degree we wish to the graph to be once it is fully
**		    created
**		r - each entry r(u,v)= (d-d(u))*(d-d(v))
**		v, w - edge recently added
**		sum - present summation of all entries in r
** Return Value:sum - summation of all entries in r
** Side Effects:updates contents of r
** Created:	Kerri Morgan	18th March, 2005
** Modified:	22 March, 2005 - to set r(u,v) to 0 if (u,v) is an edge
** Comments:	
** Acknowledgements:
******************************************************************************/
double update_r(Vertex* G, int size, int d, float* r, int v, int w, double sum)
{
	int i, j;
	double oldVal;


	/* r(u,v)=0 */
	if(v>w)
	{
		i=v;
		j=w;
	}
	else
	{
		i=w;
		j=v;
	}

	/* set r(i,j) to 0 */
	oldVal=(double)r[i*(i-1)/2+j];
	r[i*(i-1)/2+j]=0.0;
	sum-=oldVal;

	/* each row effected */
	if(v!=0)
	{
		i=v;
		for(j=0; j<i; j++)
		{
			oldVal=(double)r[i*(i-1)/2+j];
			if(oldVal >0)
			{
			/*r(u,v)= (d-d(u))*(d-d(v)) */
			r[i*(i-1)/2 + j]=(d - G[i].degree)*(d - G[j].degree);
			sum-=oldVal;
			sum+=(double)r[i*(i-1)/2 + j];
			}
		}
	}
	if(w!=0)
	{
		i=w;
		for(j=0; j<i; j++)
		{
			oldVal=(double)r[i*(i-1)/2+j];
			if(oldVal > 0)
			{
			/*r(u,v)= (d-d(u))*(d-d(v)) */
			r[i*(i-1)/2 + j]=(d - G[i].degree)*(d - G[j].degree);
			sum-=oldVal;
			sum+=(double)r[i*(i-1)/2 + j];
			}
		}
	}
	/* each column effected */
	if(v != size-1)
	{
		j=v;
		for(i=j+1; i<size; i++)
		{
			//printf("TEST %d %d\n", i, j);
			/* recalcs same value if i==j , but cost of test each
			 * of n iterations is small compared to one calculation*/
			oldVal=(double)r[i*(i-1)/2+j];
			if(oldVal > 0)
			{
			/*r(u,v)= (d-d(u))*(d-d(v)) */
			r[i*(i-1)/2 + j]=(d - G[i].degree)*(d - G[j].degree);
			sum-=oldVal;
			sum+=(double)r[i*(i-1)/2 + j];
			//printf("TST %f\n", sum);
			}
		}
	}
	if(w!=size-1)
	{
		j=w;
		for(i=j+1; i<size; i++)
		{
			//printf("TEST %d %d\n", i, j);
			oldVal=(double)r[i*(i-1)/2+j];
			if(oldVal >0)
			{
			/*r(u,v)= (d-d(u))*(d-d(v)) */
			r[i*(i-1)/2 + j]=(d - G[i].degree)*(d - G[j].degree);
			sum-=oldVal;
			sum+=(double)r[i*(i-1)/2 + j];
			//printf("TST %f\n", sum);
			}
		}
	}

	return sum;
}

/******************************************************************************
** Function:	update_A
** Description: Function updates A to reflect changes in r and R
** Inputs:	A - array of cumulative probability
**		r - array containing values - (r(u,v)=(d-(d(u))(d-d(v)) 
**		R - summation 
**		size - size of graph
** Return Value: none
** Side Effects: updates A
** Created:	Kerri Morgan	18 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void update_A(float* A, float* r, float R, int size)
{
	int i, j;
	float temp=0;

	for(i=1; i<size; i++)
	{
		for(j=0; j<i; j++)
		{
			temp+=r[i*(i-1)/2+j];
			A[i*(i-1)/2+j]= temp/R;
		}	
	}
}
/******************************************************************************
** Function:	get_index
** Description:	Function calculates the index in the array r which corresponds
**		to edge (i,j)
** Inputs:	i, j - the vertices incident to the edge
** Return Value: the index corresponding to this edge 
** Side Effects: none
** Created:	Kerri Morgan	26 March, 2005
** Modified:
** Comments:	Note: The first row in r is the row corresponding to
**		vertex 1 NOT vertex 0.
** Acknowledgements:
******************************************************************************/
int get_index(int i, int j)
{
	int index;
	int temp;

	if(i<j)
	{
		temp=i;
		i=j;
		j=temp;
	}
	
	index = i*(i-1)/2;
	index +=j;

	return index;
}
/******************************************************************************
** Function:	genGraph2
** Description:	Function genertes a random graph in a modified version of the
**		algorithm produced by Stegar and Wormald
** Inputs:    	size - number of vertices required in graph to be generated
**		d - the degree of the graph to be generated
** Return Value:graph - the graph generated (or NULL if unsuccessful)
** Side Effects:
** Programmed by:	Kerri Morgan	8 April, 2005
** Modified:
** Comments:		
** Acknowledgements: Steger-Wormald Algorithm.  
**		     A.Steger and N.C.Wormald, Generating random regular graphs
**		     quickly, Combinatorics, Probability and Computing 8(1999)
**		     377-396.
******************************************************************************/
Vertex* genGraph2(int size, int d)
{
	Vertex* G=NULL;
	Edge* edges=NULL;
	float aSize;
	float* r=NULL;
	double R;
	int i, j;
	long int m=0; /* number of edges*/
	long int numEdges;
	double p;
	int edgeNum;

	if((d>size) || (d<0) || (size <1))
	{
		printf("WARNING 4.1: Cannot create graph with %d vertices and degree %d\n", size, d);
		return NULL;
	}
	
	if((d*size)%2 == 1)
	{
		printf("WARNING 4.5: Cannot create graph as nd is odd (%d vertices and degree %d)\n", size, d);
		return NULL;
	}

	/* graph has degree 0 - disjoint vertices */
	if(d==0)
	{
		G=initGraph(size);
		return G;
	}

	/* create array of edges */
	edges = initEdges(size);
	if(edges == NULL)
	{
		fprintf(stderr, "ERROR 4.3: Unable to create array of edges - insufficient memory\n");
		exit(1);
	}
//printEdges(edges, size*(size-1)/2);
	/* create lower triangular matrix r */
	aSize=size*(size-1)/2;
	              
	r=(float*) malloc(sizeof(float)*aSize);
	if(r==NULL)
	{
		fprintf(stderr, "ERROR 4.1: Insufficient memory to create r\n");
		free(edges);
		exit(1);
	}

	R=(double)aSize*d*d;

	/* initially all vertices have degree 0.
	 * so r(u,v) =d*d *********************/
	for(i=1; i<size; i++)
	{
		for(j=0; j<i; j++)
		{
			r[i*(i-1)/2 + j]=d*d;
		}
	}

	G=initGraph(size);
    	if(G==NULL)
	{
		free(edges);
		free(r);
		return NULL;
	}

        /* set all the arrays of adjacent vertices to be size d */
        for(i=0; i<size;  i++)
	{
		G[i].adjVerts=(int*)malloc(sizeof(int)*d);
		if(G[i].adjVerts==NULL)
		{
			fprintf(stderr, "ERROR 4.2: Insufficient memory to create graph\n");
			free(edges);
			free(r);
			killGraph(G, size);
			exit(1);
		}
	}

	numEdges = (size *(size-1))/2;
	while((R>0) && (numEdges >0))
	{
		/* get an edge */
		edgeNum= (int)(drand48()*(numEdges-1));

		/* decide if I should add the edge */
		p = drand48();
		i= edges[edgeNum].v;
		j=edges[edgeNum].w;
//printf("TEST %d %d and %f %f\n", i, j, p, r[i*(i-1)/2+j]/R);
	
//printEdges(edges, numEdges);
//printf("****************\n");
		if((G[i].degree == d) || (G[j].degree ==d))
		{
//			printf("REMOVE\n");
			/* remove this edge from consideration */
			numEdges--;
			if(numEdges!=0)
			{
				edges[edgeNum].v=edges[numEdges].v;
				edges[edgeNum].w=edges[numEdges].w;
			}
		}
		else if(p<r[i*(i-1)/2+j]/R ) /* accept */
		{
//			printf("ACCEPT\n");
			if(addEdge2(G, size, i, j)==TRUE)
			{
				m++;
				R = update_r(G, size, d, r, i, j, R);
			}
			numEdges--;
			if(numEdges !=0)
			{
				edges[edgeNum].v=edges[numEdges].v;
				edges[edgeNum].w=edges[numEdges].w;
			}
		}
	}
	/* successfully completed */
	if(m == d*(size/2))
	{
		free(r);
		free(edges);
		saveGraph(G, size, d*(size/2));
		return G;
	}
	else //if(((d*(size/2))-m) >= 1)
	{
	//	printGraph(G, size);
		printf("TESTING: Graph above is inadequate\n");
		saveGraph(G, size, m);
		killGraph(G, size);
		G=NULL;
		free(r);
		free(edges);
		return genGraph2(size, d);
	}
	free(r);
	free(edges);
	return G;
}
/******************************************************************************
** Function:	initEdges
** Description:	Function creates an array of edges
** Inputs:	size - number of vertices
** Return Value:edges - an array of edges, or NULL if unsuccesful
** Side Effects: none
** Created:	Kerri Morgan	8th April, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
Edge* initEdges(int size)
{
	Edge* edges=NULL;
	int numEdges;
	int i, j;
	int cur=0;
	
	if(size > 0)
	{
		numEdges = (size*(size-1))/2;
		edges = (Edge*) malloc (sizeof(Edge)*(numEdges));
		if(edges != NULL)
		{
			for(i=0; i<size; i++)
			{
				for(j=0; j<i; j++)
				{
					edges[cur].v=i;
					edges[cur].w=j;
					cur++;
				}
			}
		}
	}
	return edges;
}
/******************************************************************************
** Function:	printEdges
** Description: function prints the list of edges
** Inputs:	edges - an array of edges
**		size - the number of edges
** Return Value:none
** Side Effects:outputs information to screen
** Created:	Kerri Morgan	8th April, 2005
** Modified:	
** Comments:	
** Acknowledgements:
******************************************************************************/
void printEdges(Edge* edges, int size)
{
	int i;
	
	for(i=0; i<size; i++)
	{
		printf("%d %d\n", edges[i].v, edges[i].w);
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
** Function:	classicalGen
** Description:	Function produces a random graph according to the classical 
**		graph generator by Erdos.
** Inputs:	size - number of vertices
**		d - average degree of required graph
** Return Value:G - the graph
** Side Effects:
** Created:	Kerri Morgan	17 May, 2005
** Modified:
** Comments:	
** Acknowledgements:	Erdos
******************************************************************************/
Vertex* classicalGen(int size, int d)
{
	Vertex* G=NULL;
	long int m=0; /* number of edges*/
	double p;
	int i, j;
	Node* cur=NULL;
	
	if((d>size) || (d<0) || (size <1))
	{
		printf("WARNING 4.4: Cannot create graph with %d vertices and degree %d\n", size, d);
		return NULL;
	}

	/* graph has degree 0 - disjoint vertices */
	if(d==0)
	{
		G=initGraph(size);
		return G;
	}

	G=initGraph(size);
	if(G==NULL)
	{
		return NULL;
	}


	for(i=0; i<size; i++)
	{
		for(j=i+1; j<size; j++)
		{
			p=drand48();
			if(p <= (double)d/(size-1))
			{
				if(addEdge(G, size, i, j)==TRUE)
					m++;
			}
		}
	}

	for(i=0; i<size; i++)
	{
		if(G[i].degree >0)
		{
			G[i].adjVerts=(int*)malloc(sizeof(int)*G[i].degree);
			if(G[i].adjVerts==NULL)
			{
				fprintf(stderr, "ERROR 4.2: Insufficient memory to create graph\n");
				killGraph(G, size);
				exit(1);
			}
			cur = G[i].adjVertices.startPtr;
			j=0;
			while(cur !=NULL)
			{
				G[i].adjVerts[j]=cur->vertex;
				j++;
				cur=cur->next;
			}
			killList(&G[i].adjVertices);
		}
	}
	saveGraph(G, size, m);
	return G;

}

