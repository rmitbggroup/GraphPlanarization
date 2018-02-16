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


#include "hybridAlg.h"
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
** Function:	hybrid
** Description:	Function planarises a graph G using a combination of the 
**		vertex addition algorithm and the concept of removing vertices
**		based very loosely on the vertex removal algorithm
** Inputs:	G - graph
**		size - number of vertices
**		save - flag to indicate whether to save subgraph or not
** Return Value:number of vertices in P
** Side Effects:
** Created:	Kerri Morgan	6 May, 2005
** Modified:
** Comments:	
** Acknowledgements:	Keith Edwards and Graham Farr (see addVertAlg function
**			in addVertAlg.c
******************************************************************************/
int hybrid(Vertex* G, int size, int save)
{
	int* age;
	int P;
	int v=0;
	int y,z;
	VertInfo* info=NULL;
	int* R=NULL;
	int Rsize;
	int removed;
	int index=0;
	int state = ADDING; 
	int lastchoice =-1;
	int flag =0;
	int start=0;
	int maxAge ;
	int componentNum=0;
	int i,k; 
	int max = 0;
	int j = -1;
	int m;
	int kx;
	int vx;
	
	/* set the aging variable - no vertex is 'kicked out' of P more than
	 * this number of times *******************************************/
	maxAge = G[0].degree;
	for(i=1; i< size; i++)
	{
		if(maxAge < G[i].degree)
		{
			maxAge +=G[i].degree;
		}
	}
	maxAge /=size;
	maxAge=maxAge/2;

	if((5>maxAge) && (size <=5000 )) 
	{
		maxAge =5;
	}
	else if ((size > 5000) && (maxAge == 0))
	{
		maxAge =2;
	}
	else if(maxAge > 10)
	{
		maxAge =10;
	}

	initMembers(G, size);

	info= initInfo(size);
	R = initR(size);
	Rsize = size;
	

	age = (int*) malloc(sizeof(int)*size);
	if(age==NULL)
	{
		fprintf(stderr, "ERROR 11.1: Unable to create array age\n");
		exit(1);
	}

	for(j=0; j<size; j++)
	{
		age[j]=0;
	}

	while(state !=FINISH)
	{
		index=selectVertexForRemoval(info,Rsize, R, index);
		if(index>=0)
		{
			v=R[index];
		}
		else
		{
			v=-1;
			state = FINISH;
		}

		while(state==ADDING) 	/* Add Vertex Algorithm */
		{
			if(info[v].Nn <=1)
			{
				componentNum = action1(G,size, info, v, componentNum);
				Rsize--;
				R[index]=R[Rsize];
			}
			else /* Nn=2 */
			{
				getNFNeighbours(G,size, info, v, &y, &z);
				if(z==-1 || y==-1)
				{
					fprintf(stderr, "ERROR 11.2: Error in retrieving adjacent non-forest neighours (%d is adjacent to %d and %d)\n", v, y,z);
					exit(1);
				}
				if(info[z].component != info[y].component)
				{
					componentNum=action2(G, size, info, v,componentNum);
					/*update R */
					Rsize--;
					R[index]=R[Rsize];
				}
				else
				{
					/* does 3.3 and 3.4 - may as well when looking ofr path */
					removed =-1;
					double time=0.0;
					componentNum=action3(G,size, info, v, componentNum, y, z, &removed,&time);
					info[v].inP=TRUE;
					/*update R */
					Rsize--;
					R[index]=R[Rsize];
					if(removed !=-1)
					{
						R[Rsize]=removed;
						Rsize++;
					}
				}

			}
		//	index=selectVertexForRemoval(info,Rsize, R, v);
			index=selectVertexForRemoval(info,Rsize, R, index);
			if(index <0)
			{
				state = REMOVING;
			}
			else
			{
				v=R[index];
			}

		}
		

		/* Select a vertex for removal.
		 * The vertex must have a neighbour that can replace it*/
		max=0;
		j=-1;
		for(m=0; m<size; m++)
		{
			i=(start+m)%size;
			if((G[i].member ==KNOWN) && (info[i].Nn >=3))
			{
				for(vx=0; vx<G[i].degree; vx++)
				{
					k=G[i].adjVerts[vx];
					if((G[k].member == UNKNOWN) && (info[k].Nn ==3) && (info[k].Nf <=1))
					{
						if((max < info[i].Nn) && (age[i] < maxAge))
						{
							if(i!=lastchoice)
							{
								max = info[i].Nn;
								j=i;
								kx=k;
								break;
							}
						}
					}
				}
			}
			/* i is not a valid candidate and as such the last choice cannot be repeated */
			else if(i==lastchoice) 
			{
				lastchoice =-1; /* The last choice was not not a valid choice */
			}

		}

		/* repeat a choice only once in a row */
		if((j==-1) && (lastchoice !=-1))
		{
			if(flag ==0)
			{
				flag=1;
				j=lastchoice;
			}
		}
		else 
		{
			lastchoice =j;
			flag=0;
		}


		if(j!=-1)
		{
			start =j+1;
			{
				componentNum= removeVertex(G, size, info, j, componentNum);
				age[j]++;
				R[Rsize]= j;
				Rsize++;
				state =ADDING;
					
				index=selectVertexForRemoval(info,Rsize, R, v);
				if(index <0)
				{
					state = FINISH;
				}
				else
				{
					v=R[index];
				}
			}
		}
		else
		{
			state = FINISH;
		
		}
	}
//printInfo(info, size);
	P= size-Rsize;
	if(save ==1)
	{
		writePlanarSubgraph(G, size);
	}
	free(info);
	free(R);
	free(age);
	return P;
	
}

/******************************************************************************
** Function:	removeVertex
** Description:	function removes a vertex from P.
** Inputs:	G - the graph
**`		size - number of vertices in G
**		info - holds information about P
**		v - vertex to be removed
**		component - next available component number
** Return Value: updated value for component number
** Side Effects: updates info
** Created:	Kerri Morgan	6 May, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int removeVertex(Vertex* G, int size, VertInfo* info, int v, int component)
{
	int old;
	int i;
	int w;
	int numEdges = 0;
	int numVertices =0;

	/* remove vertex v */
	info[v].inP=FALSE;
	info[v].type = UNDECIDED;
	old= info[v].component;
	info[v].component=-1;
	G[v].member=FALSE;

	/* all neighbours of v have one less non-Tree neighbour */
	decNn(G, info, v);
//printf("TEST: remove vertex %d\n", v);
	/* update information about */
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		/* if this vertex is still in old component */
		if(info[w].component == old)
		{
			numEdges =0;
			numVertices =1;
			updateInfo(G, info, w, component, &numEdges, &numVertices);
			component++;

			if(numEdges+1 == numVertices)
			{
				changeToTree(G, info, w);
			}

		}
	}
	return component;
}


