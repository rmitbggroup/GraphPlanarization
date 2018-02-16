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


#include "addVertexAlg.h"
#include <stdio.h>

/******************************************************************************
** Function: 	addVertAlg
** Description:	Function planarises graph, by incrementally adding vertices
** Inputs:	G - graph
**		size - number of vertices
** Return Value:P - set of vertices belonging to the planar graph
**		P is a subset of V(G)
** Side Effects:
** Created:	Kerri Morgan	6 March 2005
** Modified:
** Comments:	
** Acknowledgements: Algorithm based on "An Algorithm for finding large induced
**		     planar subgraphs", Keith Edwards and Graham Farr
******************************************************************************/
int* addVertAlg(Vertex* G, int size)
{
	int* P=NULL;
	int v;
	int y,z;
	VertInfo* info=NULL;	
	/* component counters */
	int countT=0;
	int countNonT=0;

	initMembers(G, size);

	info= initInfo(size);

	v=selectVertexForRemoval(info,size);
	while(v>=0)
	{
		if(info[v].Nn <=1)
		{
			action1(G, size, info, v, &countT, &countNonT);
		}
		else /* Nn =2 */
		{
			/* mark as seen - for TESTING
			 */
			info[v].inP = TRUE;
			getNFNeighbours(G, size, info, v, &y, &z);
			if(z==-1 || y==-1)
			{
				fprintf(stderr, "ERROR 3.2: Error in retrieving adjacent non-forest neighours (%d is adjacent to %d and %d)\n", v, y,z);
				exit(1);
			}
			if(info[z].component != info[y].component)
			{
				action2(G, size, info, v);
			}
			else
			{
				/* does 3.3 and 3.4 - may as well when looking ofr path */
				action3(G,size, info, v);
			}

		}
		printf("%d is found\n", v);
		v=selectVertexForRemoval(info,size);


	}
/*while(x>=0)
{
	info of Nn and Nf is held in graph structure under G[i]
	if(Nn <=1)
		do blah
	else if( x and y are in different components)
		do blah
	else if( uniq path)
		do blah
	else
		do blah
	x = someFunct(G, size)

	need to decide if I've become a forest or nonforest 
} */
	free(info);
	return P;
}

/******************************************************************************
** Function:	selectVertexForRemoval
** Description: Function selects a vertex for removal from G.  This vertex is
**		a candidate for addition to P (a subset of V(G) such that
**		<P> is an induced planar subgraph.
**		The selection criteria is as follows:
**		i) the vertex has 1 or no neighbours in the non-tree components
**		   of <P>.
**		ii) the vertex has 2 neighbours in the non-tree components of
**		    <P>, AND,  1 or no neighbours in the tree components of
**		    <P>
** Inputs:	info - contains information on the components formed by <P>
**		size - the number of vertices in G
** Return Value: The vertex number if such a vertex exists, otherwise -1.
** Side Effects: none
** Created:	Kerri Morgan	7 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int selectVertexForRemoval(VertInfo* I, int size)
{
	int i;

	for(i=0; i<size; i++)
	{
		if(I[i].inP == FALSE)
		{
			if(I[i].Nn<=1)
			{
				return i;
			}
			else if((I[i].Nn==2) && (I[i].Nf <=1))
			{
				return i;
			}
		}
	}
	return -1;
}
/******************************************************************************
** Function:	initInfo
** Description: Function initialises the information structure used in the
**		addVertAlg function
** Inputs:	size - number of vertices in graph
** Return Value:info - a pointer to the information structure
** Side Effects:none
** Created:	Kerri Morgan	7 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
VertInfo* initInfo(int size)
{
	VertInfo* info=NULL;
	int i;

	info = (VertInfo*)malloc(sizeof(VertInfo)*size);
	if(info==NULL)
	{
		fprintf(stderr, "ERROR 3.1: Insufficient memory to create information storage\n");
		exit(1);
	}
	
	for(i=0; i<size; i++)
	{
		info[i].inP=FALSE;
		info[i].type  = UNDECIDED;
		info[i].component = -1;
		info[i].Nn=0;
		info[i].Nf=0;
	}
	return info;
}

/******************************************************************************
** Function:	getNFNeighbours
** Description: Function retrieves at most two vertices adjacent to vertex v.
**		These vertices are members of non-forest components of <P>.
** Inputs:	G - graph
**		info - information about vertices in graph
**		size - number of vertices in grapyh
**		v - the vertex concerned
** Return Value:none
** Side Effects:updates x, y to appropriate vertex number (otherwise sets them
**		to -1
** Created:	Kerri Morgan	7 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void getNFNeighbours(Vertex* G, int size, VertInfo* info, int v, int* x, int* y)
{
	*x=-1;
	*y=-1;
	printf("NEED TO WRITE getNFNEIGhbours\n");
}
/******************************************************************************
** Function:	action1
** Description:	Function implements action 3.1 in the algorithm by Edwards and
**		Farr
** Inputs:	G -graph
**		info - contains information about components of <P>
**		size - number of vertices in graph
**		v - vertex being added to P
**		nTCount, TCount - counters of the component number of trees
**		and non-trees
** Return Value:none
** Side Effects:updates info structure
** Created:	Kerri Morgan	7 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void action1(Vertex* G, int size, VertInfo* info, int v, int* nTCount, int* TCount)
{
	int count;

	if(info[v].Nn==0) 
	{
		if(info[v].Nf==0) /* isolated vertex */
		{
			printf("TESTING %d - adding isolated vertex \n", v);
			info[v].inP=TRUE;
			info[v].type=FOREST;
			info[v].component=*TCount;
			*TCount = (*TCount)+1;
			G[v].member =KNOWN;
			incNF(G, info, v);
		}
		else
		{
			count = countAdjTrees(G, info, v);
			printf("TEST %d is count\n", count);
			if(count == info[v].Nf) /*Merge all the trees to one tree*/
			{
				printf("TESTING %d - merging trees\n", v);
				info[v].inP = TRUE;
				info[v].type=FOREST;
				markAsTree(G, info, v, *TCount);
				/* recFunc - mark as tree - num
				 * function will recFunc(G, info, v, num)
				 * marks v as info[v].component=Nfcount
				 * then each neighbour that is a tree and in P
				 * is marked as having this component and rec 
				 * call - don't call on things already in
				 * this component.*/
				*TCount = (*TCount+1);	
				incNF(G, info, v);
			}
			else /* becomes a non_Tree */
			{
				printf("TESTING %d is merging into a non-T\n");
				info[v].inP=TRUE;
				info[v].type = NON_TREE;
				/*  
				 *  1. mark all trees as non-T and give
				 *  new component number
				 *  2.  increment all their Nn neighbours
				 *  and decrement their Nf neighbours
				 *  3. do step 2 for v too
				 *  Best to do in same function - ie old compo
				 *  number acts as flag
				 *  THey also need their type changed to 
				 *  NON_TREE */


			}


		}
	}
}
/******************************************************************************
** Function:	action2
** Description:
** Inputs:
** Return Value:
** Side Effects:
** Created:	Kerri Morgan
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void action2(Vertex* G, int size, VertInfo* info, int v)
{
	printf("NEED TO WRITE action2\n");
}
/******************************************************************************
** Function:	action3
** Description:
** Inputs:
** Return Value:
** Side Effects:
** Created:	Kerri Morgan
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void action3(Vertex* G, int size, VertInfo* info,int v)
{
	printf("NEED TO WRITE action3\n");
}
/******************************************************************************
** Function:	countAdjTrees
** Description:	Function counts the number of different trees adjacent to vertex
**		v.
** Inputs:	G -the graph
**		info - information about <P>
** Return Value:number of different trees adjacent to v.
** Side Effects:none
** Created:	Kerri Morgan
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int countAdjTrees(Vertex* G, VertInfo* info, int v)
{
	int count=0;
	int* temp=NULL;
	int w,i, flag;
	Node* cur=NULL;

	temp=(int*)malloc(sizeof(int)*G[v].degree);
	if(temp == NULL)
	{
		fprintf(stderr, "Error 3.3: Unable to allocate memory in countAdjTrees function\n");
		exit(1);
	}

	cur = G[v].adjVertices.startPtr;
	while(cur!=NULL)	/* for each vertex adjacent to v */
	{
		w=cur->vertex;
		if(info[w].type == FOREST)
		{
			flag=0;
			for(i=0; i<count; i++)
			{
				/* already counted this tree? */
				if(temp[i] == info[w].component)
				{
					flag=1;
					break;
				}
			}
			if (flag ==0)
			{
				temp[count]=info[w].component;
				count++;
			}
		}
		cur = cur->next;
	}
	free(temp);
	return count;
}
/******************************************************************************
** Function:	markAsTree
** Description: Function updates all adjacent vertices in P to show they
**		belong to the same tree component
** Inputs:	G -graph
**		info - information about P
**`		v - current vertex
**		num - component number
** Return Value: none
** Side Effects: updates info
** Created:	Kerri Morgan	7 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void markAsTree(Vertex* G, VertInfo* info, int v, int num)
{
	int i;
	int w;
	Node* cur=NULL;
	
	info[v].component = num;
	info[v].type = FOREST;
	cur = G[v].adjVertices.startPtr;
	while(cur!=NULL)	/* for each vertex adjacent to v */
	{
		w=cur->vertex;
		if((info[w].type == FOREST) && (info[w].component != num))
		{
			markAsTree(G, info, w, num);	
		}
		cur=cur->next;
	}
}
/******************************************************************************
** Function:	incNF
** Description:	Function increments the number of forest neighbours for all 
**		the neighbours of v.
** Inputs:	G - graph
**		info - information concerning P
**		v - a vertex which has just been made or added to a tree
** Return Value:none
** Side Effects:updates info
** Created:	Kerri Morgan	7 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void incNF(Vertex* G, VertInfo* info, int v)
{
	Node* cur=NULL;
	int w;

	cur=G[v].adjVertices.startPtr;
	while(cur!=NULL)
	{
		w=cur->vertex;
		info[w].Nf++;
		cur=cur->next;
	}
}
