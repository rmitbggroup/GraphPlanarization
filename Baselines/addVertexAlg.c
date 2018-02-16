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
#include <stdlib.h>
#include <time.h>

/******************************************************************************
** Function: 	addVertAlg
** Description:	Function planarises graph, by incrementally adding vertices
** Inputs:	G - graph
**		size - number of vertices
**		save - flag to indicate whether to save subgraph or not
** Return Value:size of P
** Side Effects:
** Created:	Kerri Morgan	6 March 2005
** Modified:	18th April, 2005 To rectify my poor method of  selection of  x 
**		I added R - and step through it in a similar manner to a queue.
**		This is not ideal, but works well with the existing code
**		20 April, 2005 - to give option to save subgraph.  Note: the
**		subgraph is not actually saved, rather the graph with all 
**		vertices in R represented as isolated vertices
** Comments:	
** Acknowledgements: Algorithm based on "An Algorithm for finding large induced
**		     planar subgraphs", Keith Edwards and Graham Farr
******************************************************************************/
int addVertAlg(Vertex* G, int size, int save)
{
	int P;
	int v=0;
	int y,z;
	VertInfo* info=NULL;	
	int* R=NULL;	
	int Rsize;
	int removed;
	int index =0;
	double timeForPaths=0.0;
	/* component counter */
	int componentNum=0;
	initMembers(G, size);

	info= initInfo(size);
	R = initR(size);
	Rsize = size;

	index=selectVertexForRemoval(info,Rsize, R, index);
	if(index >=0)
	{
		v=R[index];
	}
	else
	{
		v=-1;
	}
	while(v>=0)
	{
#ifdef DEBUG
printf("TESTING***********************************************\n");
printInfo(info, size);
printf("****************************************************\n\n");
#endif
//scanf("%d", &z);

		if(info[v].Nn <=1)
		{
			componentNum=action1(G, size, info, v, componentNum);
			/*update R */
			Rsize--;
			R[index]=R[Rsize];
		}
		else /* Nn =2 */
		{
			if(info[v].Nf >1)
			{
				fprintf(stderr, "ERROR 3.6: Select function returned vertex %d with Nn=2 and Nf>1\n", v);
				exit(1);
			}
			getNFNeighbours(G, size, info, v, &y, &z);
			if(z==-1 || y==-1)
			{
				fprintf(stderr, "ERROR 3.2: Error in retrieving adjacent non-forest neighours (%d is adjacent to %d and %d)\n", v, y,z);
				exit(1);
			}
			if(info[z].component != info[y].component)
			{
				#ifdef DEBUG
				printf("Action 2 with %d\n", v);
				#endif

				componentNum=action2(G, size, info, v,componentNum);
				/*update R */
				Rsize--;
				R[index]=R[Rsize];
			}
			else
			{
				/* does 3.3 and 3.4 - may as well when looking ofr path */
				#ifdef DEBUG
				printf("Action 3 with %d\n", v);
				#endif	
				removed =-1;
				componentNum=action3(G,size, info, v, componentNum, y, z, &removed,&timeForPaths);
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

		index=selectVertexForRemoval(info,Rsize, R, index);
		if(index <0)
		{
			v=-1;
		}
		else
		{
			v=R[index];
		}

	}
#ifdef	DEBUG
printf("TESTING***********************************************\n");
printInfo(info, size);
printf("****************************************************\n\n");
#endif
/**	P=0;	
	for(v=0; v<size; v++)
	{
		if(info[v].inP == TRUE)
		{
			P++;
		}
	} **/
	P = size - Rsize;
	/*
	if(save ==1)
	{
		writePlanarSubgraph(G, size);
	}
	*/
	free(info);
	free(R);
	printf("the total time for finding alternative paths is %f secs\n",timeForPaths/ CLOCKS_PER_SEC);
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
**		cur - index of last vertex selected
** Return Value: The index of R that contains the 
** 		vertex number if such a vertex exists, otherwise -1.
** Side Effects: none
** Created:	Kerri Morgan	7 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int selectVertexForRemoval(VertInfo* I, int size, int* R, int cur)
{
	int i;
	int index;

	for(i=0; i<size; i++)
	{
		index=R[(i+cur)%size];
		if(I[index].inP == FALSE)
		{
			if(I[index].Nn<=1)
			{
				return (i+cur)%size; //index;
			}
			else if((I[index].Nn==2) && (I[index].Nf <=1))
			{
				return (i+cur)%size; //index;
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
	int i;
	int count=0;
	int w;

	*x=-1;
	*y=-1;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if(info[w].type==NON_TREE)
		{
			if(count==0)
			{
				*x=w;
				count++;
			}
			else if(count ==1)
			{
				*y=w;
				break;
			}
		}
	}

}

/******************************************************************************
** Function:	action1
** Description:	Function implements action 3.1 in the algorithm by Edwards and
**		Farr
** Inputs:	G -graph
**		info - contains information about components of <P>
**		size - number of vertices in graph
**		v - vertex being added to P
**		compNum - next free compNum
** Return Value:none
** Side Effects:updates info structure
** Created:	Kerri Morgan	7 March, 2005
** Modified:	11 March, 2005:  Only  one component number - not one for each
**		type
** Comments:	
** Acknowledgements:
******************************************************************************/
int  action1(Vertex* G, int size, VertInfo* info, int v, int compNum)
{
	int count;
	int temp1, temp2;

	#ifdef DEBUG
	printf("TESTING action1 with %d\n", v);
	#endif
	if(info[v].Nn==0) 
	{
		if(info[v].Nf==0) /* isolated vertex */
		{
			#ifdef DEBUG
			printf("TESTING %d - adding isolated vertex \n", v);
			#endif
			info[v].inP=TRUE;
			info[v].type=FOREST;
			info[v].component= compNum;
			compNum++;	
			G[v].member =KNOWN;
			incNF(G, info, v);
		}
		else
		{
			count = countAdjTrees(G, info, v);
			if(count == info[v].Nf) /*Merge all the trees to one tree*/
			{
				#ifdef DEBUG
				printf("TESTING %d - merging trees\n", v);
				#endif
				info[v].inP = TRUE;
				info[v].type=FOREST;
				markAsTree(G, info, v, compNum);
				compNum++;	
				incNF(G, info, v);
			}
			else /* becomes a non_Tree */
			{
				#ifdef DEBUG	
				printf("TESTING %d is merging into a non-T\n", v);
				#endif	
				info[v].inP=TRUE;
				markAsNonTree(G, info, v, compNum);
				compNum++;	
			}
			G[v].member = KNOWN;
		}
	}
	else if(info[v].Nn == 1)
	{
		/* no other neighbours - I can just extend the non-tree component*/
		if(info[v].Nf == 0)
		{
			G[v].member = KNOWN;
			info[v].inP=TRUE;
			info[v].type=NON_TREE;
			/* get the non-tree neighbour in P */
			getNFNeighbours(G, size, info, v, &temp1, &temp2);	
			if(temp1==-1)
			{
				fprintf(stderr, "ERROR 3.5: Function called with Nn set at %d, but no non-forest neighbour located\n", info[v].Nn);
				exit(1);
			}	
			/* assign same component number ot this vertex */
			info[v].component=info[temp1].component;
			/* incrment v's neighbours Nn components */
			incNn(G, info, v);
		}
		else /* we have some tree neighbours */
		{
			G[v].member = KNOWN;
			info[v].inP=TRUE;
			//info[v].type = NON_TREE; is done in markasNonTree

			/* get non-tree neighbour */
			getNFNeighbours(G,size,info, v, &temp1, &temp2);
			if(temp1==-1)
			{
				fprintf(stderr, "ERROR 3.5: Function called with Nn set at %d, but no non-forest neighbour located\n", info[v].Nn);
				exit(1);
			}
	
			/* merge v with non-tree neighbour */
			markAsNonTree(G, info, v, info[temp1].component);
		}
	}
	else
	{
		fprintf(stderr, "ERROR 3.4: Function action1 called with incorrect value for Nn (%d) with vertex %d\n", info[v].Nn, v);
		exit(1);
	}
	return compNum;
}
/******************************************************************************
** Function:	action2
** Description: Function performs step 3.2 of addition of vertices algorithm
** Inputs:	G -the graph
**		info - contains info about P
**		x - vertex being added to P
**		compNum - next available component number
** Return Value:none
** Side Effects:updates values in info
** Created:	Kerri Morgan	11 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int action2(Vertex* G, int size, VertInfo* info, int x, int compNum)
{
	info[x].inP=TRUE;
	markAsNonTree(G, info, x, compNum);
	compNum++;	

	G[x].member = KNOWN;

	return compNum;
}

/******************************************************************************
** Function:	action3
** Description:	Implements action 3.3 and 3.4 in the addition of vertices 
**		algorithm
** Inputs:	G - the graph
**		size - number of vertices
**		info - holds information about P
**		v - current vertex being added to P
**		compNum - next available component number
**		removed - holds index of vertex removed (or -1 if none removed)
** Return Value:updated value for compNum
** Side Effects:Updates info
** Created:	Kerri Morgan	11 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int action3(Vertex* G, int size, VertInfo* info, int v, int compNum, int y, int z, int* removed,double* timeForPaths)
{
	int x;
	int oldComponent;
	int i;
	int w;
	clock_t start, finish;
	int numEdges=0;
	int numVertices=0;
	
	*removed =-1;
	start=clock();
	/* is there a vertex along the path that allows a choice of paths? */
	x=crossroads(G, size, info, y, z);
	finish=clock();
	*timeForPaths+=(double)(finish-start);
	/* a single y-z path allows us to simply add vertex v to this component */
	if(x == -1)
	{
		info[v].inP=TRUE;
		markAsNonTree(G, info, v, compNum);
		compNum++;
		G[v].member=TRUE;
		return compNum;
	}
	else
	{
		/* add vertex v */
		  info[v].inP=TRUE;
		  G[v].member=KNOWN;
		  markAsNonTree(G, info, v, compNum);
		  compNum++;

		/* remove vertex x */
		info[x].inP=FALSE;
		info[x].type = UNDECIDED;
		oldComponent = info[x].component;
		info[x].component=-1;
		G[x].member = UNKNOWN;
		*removed =x;	
		/* all x's neighbours now have one less nonTree neighbour */
		decNn(G, info, x);		
		
		/* update information about the component(s) created by
		 * the removal of x */
		for(i=0; i<G[x].degree; i++)
		{
			w=G[x].adjVerts[i];
			/* if this vertex is still in the old component */
			if(info[w].component == oldComponent)
			{
				numEdges=0;
				numVertices=1;
				updateInfo(G, info, w, compNum, &numEdges, &numVertices);
				compNum++;
			
			/* this subcomponent is no longer a nonT, so must update
			 * information for all vertices that are neighbours */
				if(numEdges+1 == numVertices)
				{
					changeToTree(G, info, w);
					#ifdef DEBUG	
					printf("Changing to tree\n");
					#endif	
				}
			}
		}

		
		/* check if old x component is still connected and if still nonT */
		/* for each vertex adjacent to x
		 * if it still has the old component number
		 * give it a new one - and all its neighbours
		 * if this new component is now a tree
		 * for every vertex in this component
		 * decrement Nn anbd increment Nf of all neighbours */


		#ifdef DEBUG	
		printf("REMOVE %d\n", x);	
		#endif	
	}
	return compNum;
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
	int w,i,j, flag;
	int* temp=NULL;

	temp=(int*)malloc(sizeof(int)*G[v].degree);
	if(temp == NULL)
	{
		fprintf(stderr, "Error 3.3: Unable to allocate memory in countAdjTrees function\n");
		exit(1);
	}

	for(j=0; j<G[v].degree; j++) /* for each vertex adjacent to v */	
	{
		w=G[v].adjVerts[j];
		if(info[w].type == FOREST)
		{
			flag=0;
			/* have we already counted this tree? */
			for(i=0; i<count; i++)
			{
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
	
	info[v].component = num;
	info[v].type = FOREST;

	/* for each vertex adjacent to v */	
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if((info[w].type == FOREST) && (info[w].component != num))
		{
			markAsTree(G, info, w, num);	
		}
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
	int i;
	int w;


	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		info[w].Nf++;
	}
}
/******************************************************************************
** Function:	printInfo	
** Description:	Function prints information about neighbours of vertices in P
		This function is used in testing
** Inputs:	info - information about vertices
**		size - number of vertices in graph
** Return Value:none
** Side Effects:Outputs information to screen 
** Created:	Kerri Morgan	11 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void printInfo(VertInfo* info, int size)
{
	int i;

	printf("Vx	In P	Type	Component	Nn	Nf\n");
	for(i=0; i<size; i++)
	{
		printf("%d\t", i);
		if(info[i].inP==TRUE)
		{
			printf("T    \t");
		}
		else
		{
			printf("F    \t");
		}
		printf("%d\t", info[i].type);
		printf("%d\t\t", info[i].component);
		printf("%d\t", info[i].Nn);
		printf("%d\n", info[i].Nf);
	}
}
/******************************************************************************
** Function:	markAsNonTree
** Description:	Function merges several components in P into a non-tree 
**		component.  It also updates the number of Nn and Nf neighbours
**		for vertices adjacent to vertices in this component.
** Inputs:	G - the graph
**		info - holds information about P
**		v - the vertex whose addition causes the merge
**		num - next free component number
** Return Value:none
** Side Effects:updates info structure
** Created:	Kerri Morgan	11 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void markAsNonTree(Vertex* G, VertInfo* info, int v, int num) 
{
	int i;
	int w;

        info[v].component = num;

	/* component was a tree, but now is a non-tree */
	if(info[v].type == FOREST) 
	{
		/* for each of it's neighbours decrement their number of
		** forest neighbours and increment their number of non-forest 
		** neighbours */
		decNF(G, info, v);
		incNn(G, info, v);
	}
	else if(info[v].type == UNDECIDED)
	{
		/* for each of it's neighbours increment their number of 
		** non-forest  neighbours */
		incNn(G, info, v);
	}

        info[v].type = NON_TREE;

	/* for each vertex adjacent to v */
	for(i=0; i<G[v].degree; i++)
	{
                w=G[v].adjVerts[i];
		if((info[w].inP== TRUE)&& (info[w].component !=num))
		{
			markAsNonTree(G,info, w, num);			
		}
	}
} 

/******************************************************************************
** Function:    incNn
** Description: Function increments the number of non-forest neighbours for all
**              the neighbours of v.
** Inputs:      G - graph
**              info - information concerning P
**              v - a vertex
** Return Value:none
** Side Effects:updates info
** Created:     Kerri Morgan    11 March, 2005
** Modified:
** Comments:
** Acknowledgements:
******************************************************************************/
void incNn(Vertex* G, VertInfo* info, int v)
{
        int i;
        int w;


        for(i=0; i<G[v].degree; i++)
        {
                w=G[v].adjVerts[i];
                info[w].Nn++;
        }
} 

/******************************************************************************
** Function:    decNn
** Description: Function decrements the number of non-forest neighbours for all
**              the neighbours of v.
** Inputs:      G - graph
**              info - information concerning P
**              v - a vertex
** Return Value:none
** Side Effects:updates info
** Created:     Kerri Morgan    15 March, 2005
** Modified:
** Comments:
** Acknowledgements:
******************************************************************************/
void decNn(Vertex* G, VertInfo* info, int v)
{
        int i;
        int w;

        for(i=0; i<G[v].degree; i++)
        {
                w=G[v].adjVerts[i];
                info[w].Nn--;
        }
} 

/******************************************************************************
** Function:    decNF
** Description: Function decrements the number of forest neighbours for all
**              the neighbours of v.
** Inputs:      G - graph
**              info - information concerning P
**              v - a vertex 
** Return Value:none
** Side Effects:updates info
** Created:     Kerri Morgan    11 March, 2005
** Modified:
** Comments:
** Acknowledgements:
******************************************************************************/
void decNF(Vertex* G, VertInfo* info, int v)
{
        int i;
        int w;


        for(i=0; i<G[v].degree; i++)
        {
                w=G[v].adjVerts[i];
                info[w].Nf--;
        }
}          

/******************************************************************************
** Function:	crossroads

** Description:	Function finds the first point on y-z path where an alternative
**		path (say y...x....z) can be taken.
** Inputs:	G -graph
**		size - number of vertices in graph
**		info - information about P
**		y, z - vertices defining y-z path
** Return Value:x - if path y-z is not unique, otherwise -1
** Side Effects:
** Created:	Kerri Morgan		11 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int crossroads(Vertex* G, int size, VertInfo* info, int y, int z)
{
	int i;
	List Path;
	List Path2;
	Node* cur=NULL;
	Node* next=NULL;
	int v, w, xi;

	initList(&Path);
	initList(&Path2);

	insertAtEnd(&Path, y);	
	getInitPath(G,size, info,&Path,  y, z);
	//printList(&Path);

	cur = Path.startPtr;
	next=cur->next;

	/* there is a path(y ... x ...z) from y to z, 
	** but y is also adjacent to z */

	if((adjacentVertices(G, y,z)==TRUE) && (next->vertex !=z))
	{
		#ifdef DEBUG	
		printf("TEST:Direct yz path and not used\n");
		#endif	
		killList(&Path);
		killList(&Path2);
		return y;
	}

	/* check each vertex in path to ascertain if there is an alternative
	** path to z from it */
	while((cur !=NULL) && (next!=NULL))
	{
		v=cur->vertex;
		w =next->vertex;
		insertAtEnd(&Path2, v);
		for(i=0; i<G[v].degree; i++)
		{
			xi=G[v].adjVerts[i];
			/* don't use the same path */
			if((xi != w) && (info[xi].component==info[y].component)) 
			{
				insertAtEnd(&Path2, xi);
				if(getInitPath(G,size, info,&Path2,  xi, z)==TRUE)
				{
					//printList(&Path2);
					killList(&Path);
					killList(&Path2);
					return v;
				}
				removeFromList(&Path2, xi);
			}
		}
		cur=next;
		next=cur->next;
	}

/* need to return the vertex in the path */
	
	killList(&Path2);	
	killList(&Path);
	return -1;
}
/******************************************************************************
** Function:	getInitPath
** Description:	Function decides if there exists a path between x and y in <P>
** Inputs:	G - graph
**		size - number of vertices in graph
**		info - information about P
**		Path - to hold path from x to y
**		x, y - vertices in graph
** Return Value: TRUE - path is found, otherwise FALSE
** Side Effects:updates path
** Created:	Kerri Morgan	11 March, 2005
** Modified:
** Comments:	Note: Path initially holds the first vertex in path	
** Acknowledgements:
******************************************************************************/
int getInitPath(Vertex* G, int size, VertInfo* info, List* Path, int x, int y)
{
	int i;
	int w;

	/* must be path in same component */
	if(info[x].component != info[y].component)
	{
		return FALSE;
	}
	
	/* same component and adjacent vertices */
	if(adjacentVertices(G, x,y))
	{
		insertAtEnd(Path, y);
		return TRUE;
	}

	for(i=0; i<G[x].degree; i++)
	{
		w=G[x].adjVerts[i];
	/* w is not already in path AND is in same component */
	if((info[x].component==info[w].component) && (inList(Path, w)==FALSE))
		{
			insertAtEnd(Path, w);
			if(getInitPath( G, size, info, Path, w, y)==TRUE)
			{
				return TRUE;
			}
			else
			{
				removeFromList(Path, w);
			}
		}
	}
	return FALSE;
}

/******************************************************************************
** Function:	adjacentVertices
** Description:	Function decides if there exists an edge vw in G
** Inputs:	G - graph
**		v, w - vertices in graph
** Return Value:TRUE - if the edge exists otherwise FALSE
** Side Effects:
** Created:	Kerri Morgan	11 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int adjacentVertices(Vertex* G, int v, int w)
{
	int i;

	for(i=0; i<G[v].degree; i++)
	{
		if(G[v].adjVerts[i]==w)
		{
			return TRUE;
		}
	}
	return FALSE;
}


/******************************************************************************
** Function:	updateInfo
** Description:	Function updates information about P, when a vertex adjacent to
**		v is removed from P.
**		Function counts number of vertices and edges in this component
** Inputs:	G - the graph
**		info - structure holding information about P
**		v - vertex removed from P
**		num - new component number
**		edges - counter for number of edges in component
**		vertices - counter for number of vertices in component
** Return Value:none
** Side Effects:updates the component number for this (sub)component created
** Created:	Kerri Morgan	16th March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void updateInfo(Vertex* G, VertInfo* info, int v, int num, int* edges, int* vertices)
{
	int i;
	int w;
	
	info[v].component = num;
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		/* if there is a vertex in P that is connected to v, but
		 * not currently listed as in component num */
		if((info[w].inP == TRUE) && (info[w].component !=num))
		{
			*vertices=(*vertices)+1;
			updateInfo(G, info, w, num, edges, vertices);
		}
		/* count edges */
		/* v>w - don't count edges twice (vw and wv) */
		if((info[w].inP == TRUE) && (v >w))
		{
			*edges= (*edges)+1;
		}
	}
}



/******************************************************************************
** Function:	changeToTree
** Description: Function takes a component, which used to be a subcomponent of
**		a non-Tree component, but due to the removal of a vertex is now
**		a tree.  The function updates the structure to reflect the
**		change.
** Inputs:	G - the graph
**		info - holds information about P
**		v - vertex in component currently being considered
** Return Value:none
** Side Effects:updates info - changes component type to TREE
**			     - updates Nn and Nf for all neighbours of
**			       vertices in this component
** Created:	Kerri Morgan	16 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void changeToTree(Vertex* G, VertInfo* info, int v)
{
	int i;
	int w;
	/* component was a non-tree, but now is a tree */
	if(info[v].type == NON_TREE) 
	{
		/* for each of it's neighbours decrement their number of
		** non-tree neighbours and increment their number of tree
		** neighbours */
		incNF(G, info, v);
		decNn(G, info, v);
	}

        info[v].type = FOREST;

	/* for each vertex adjacent to v */
	for(i=0; i<G[v].degree; i++)
	{
                w=G[v].adjVerts[i];
/** I can use the nonT flag as a note that I have not visited this vertex */		if((info[w].inP== TRUE)&& (info[w].type != FOREST))
		{
			changeToTree( G,  info, w);
		}
	}
}

/******************************************************************************
** Function:	initR
** Description:	Creates and initialises an array listing vertices in R
** Inputs:	size - number of vertices in graph
** Return Value:R - the list of vertices in R
** Side Effects:
** Created:	Kerri Morgan	18th April, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int* initR(int size)
{
	int i;
	int* R=NULL;

	R=(int*) malloc(sizeof(int)*size);
	if(R==NULL)
	{
		fprintf(stderr, "Insufficient memory to create R\n");
		exit(1);
	}

	for(i=0; i<size; i++)
	{
		R[i]=i;
	}
	return R;
}
