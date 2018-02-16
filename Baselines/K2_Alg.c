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

#include "K2_Alg.h"
#include "stdio.h"
#include "stdlib.h"
/******************************************************************************
** Function:	M_MIPS_II	
** Description: Function computes the size of an induced planar 
**		subgraph.  Modified version of M_MIPS
**		The subgraph created is a Palm Tree
** Inputs:	G- the graph
**		size - number of vertices in graph
***		save - flag indicating whether to save the subgraph
** Return Value:P - the size of the subgraph
** Side Effects:updates structure G
** Created:	Kerri Morgan	25th August, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int M_MIPS_II(Vertex* G, int size, int save)
{
        int P=0;
	K_VertInfo* info=NULL;
	int* R= NULL;
	int Rsize;
	R_Index* R_idx=NULL;
	int idxSize;
	int componentNum =0;
	int i;
								       
       	info=initK_Info(G, size);
	R=initR(size);
	Rsize = size;

	/* sort R in order of increasing degree */
	sort(G, R, size);
												        
	R_idx = createIndex(G, R, size, &idxSize);

	/*grow a maximal tree */
	P = growKTrees(G, size, info, R, &componentNum, R_idx, idxSize);
	#ifdef DEBUG
	printKInfo(info, size);
	printf("TREE SIZE : %d\n", P);
	#endif

	P+= growPalmTrees(G, size, info, R, &componentNum, R_idx, idxSize);
	#ifdef DEBUG	
	printf("TREE SIZE : %d\n", P);
	printKInfo(info, size);
	#endif
	//P = extendGraph(G, size, info, R, R_idx, idxSize, P, &componentNum);
	for(i=0; i<size; i++)
	{
		killList2(&info[i].adjTrees);
		killList2(&info[i].adjNonTrees);
	}

	free(info);
	free(R);
	free(R_idx);
	if(save == 1)
	{
		writePlanarSubgraph(G, size);
	}
	return P;


}


/******************************************************************************
** Function:	PT_2006
** Description: Function computes the size of an induced planar 
**		subgraph.  Modified version of M_MIPS
**		The subgraph created is a Palm Tree
** Inputs:	G- the graph
**		size - number of vertices in graph
***		save - flag indicating whether to save the subgraph
** Return Value:P - the size of the subgraph
** Side Effects:updates structure G
** Created:	Kerri Morgan	13th March, 2006
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int PT_2006(Vertex* G, int size, int save)
{
        int P=0;
	K_VertInfo* info=NULL;
	int* R= NULL;
	int Rsize;
	R_Index* R_idx=NULL;
	int idxSize;
	int componentNum =0;
	int i;
								       
       	info=initK_Info(G, size);
	R=initR(size);
	Rsize = size;

	/* sort R in order of increasing degree */
	sort(G, R, size);
												        
	R_idx = createIndex(G, R, size, &idxSize);

	/*grow a maximal tree */
	P = growKTrees(G, size, info, R, &componentNum, R_idx, idxSize);
	#ifdef DEBUG
	printKInfo(info, size);
	printf("TREE SIZE : %d\n", P);
	#endif

	P+= growPalmTreesIII(G, size, info, R, &componentNum, R_idx, idxSize);
	//printKInfo(info, size);
	#ifdef DEBUG	
	printKInfo(info, size);
	printf("PALM TREE SIZE : %d\n", P);
	#endif
	//printf("PALM TREE SIZE : %d\n", P);
	//P = extendGraph(G, size, info, R, R_idx, idxSize, P, &componentNum);
	for(i=0; i<size; i++)
	{
		killList2(&info[i].adjTrees);
		killList2(&info[i].adjNonTrees);
	}

	free(info);
	free(R);
	free(R_idx);
	if(save == 1)
	{
		writePlanarSubgraph(G, size);
	}
	return P;


}


/******************************************************************************
** Function:	M_MIPS_III	
** Description: Function computes the size of an induced planar 
**		subgraph.  Modified version of M_MIPS.  Includes growing a 
**		Palm tree, then extending the subgraph with the extend 
**		algorithm
** Inputs:	G- the graph
**		size - number of vertices in graph
***		save - flag indicating whether to save the subgraph
** Return Value:P - the size of the subgraph
** Side Effects:updates structure G
** Created:	Kerri Morgan	25th August, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int M_MIPS_III(Vertex* G, int size, int save)
{
        int P=0;
	K_VertInfo* info=NULL;
	int* R= NULL;
	int Rsize;
	R_Index* R_idx=NULL;
	int idxSize;
	int componentNum =0;
	int i;
								       
       	info=initK_Info(G, size);
	R=initR(size);
	Rsize = size;

	/* sort R in order of increasing degree */
	sort(G, R, size);
												        
	R_idx = createIndex(G, R, size, &idxSize);

	/*grow a maximal tree */
	P = growKTrees(G, size, info, R, &componentNum, R_idx, idxSize);
	#ifdef DEBUG
	printKInfo(info, size);
	printf("TREE SIZE : %d\n", P);
	#endif
	P+= growPalmTrees(G, size, info, R, &componentNum, R_idx, idxSize);

	#ifdef DEBUG	
	printf("TREE SIZE : %d\n", P);
	printKInfo(info, size);
	#endif
	P = extendGraph(G, size, info, R, R_idx, idxSize, P, &componentNum);
	if(save == 1)
	{
		writePlanarSubgraph(G, size);

	}
	for(i=0; i<size; i++)
	{
		killList2(&info[i].adjTrees);
		killList2(&info[i].adjNonTrees);
	}

	free(info);
	free(R);

	free(R_idx);
	if(save == 1)
	{
		writePlanarSubgraph(G, size);
	}
	return P;
}

/******************************************************************************
** Function:	growPalmTrees
** Description: Function extends a tree graph to a graph which can
**		contain "little triangles" and in the case of a 
**		component is a path with no triangles a cycle.
**		That is the resulting graph is a tree like 
**		structure with triangle appendages, plus in some
** 		cases a component(s) consisting of only a cycle.
** Inputs:	G - the graph
**              size - number of vertices in graph
**              info - information about planar subgraph
**              R - list of vertices not in planar subgraph P.
**              compNum - next free component number
**              idx - index into R
**              idxSize - size of idx
** Return Value: size of planar subgraph (the forest)
** Side Effects:updates R, Rsize and info.
** Created:	Kerri Morgan	25th August, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int growPalmTrees(Vertex* G, int size, K_VertInfo* info, int* R, int* component, R_Index* idx, int idxSize)
{
	int P=0;
	int num;
	int flag;
	List L;

	while(flag)
	{
		flag =0; /* no changes */
		/* add as many triangles as possible */
		num= selectTriangleVertex(G, info, R, idx, idxSize);
		P+=num;
		
		/* add as many large triangles as possible */
		num= selectLargeTriangleVertex(G, info,R, idx, idxSize, component);
		/* each of these type adds a vertex and removes a
		 * vertex, so does not change P */
		if(num >= 0) /* added some more d_P(v)=2 vertices */
		{
			P+=num;
			/* changes occured - may create possibility of 
			 * adding more little triangles */
			flag =1;
		}
		else /* no large triangles found */
		{
			/* close any paths into circuits at last resort */
			num=closePaths(G, info, R,  idx, idxSize,component);
			/* increases size of P */
			P+=num;
			if(num>0)
			{
			/* changes occured - may create possibility of 
			 * adding more little triangles */
				flag =1;	
			}
		}
	}

	initList(&L);
	getSmallCompList(&L, info, size);

	/* check if there exists any vertices adjacent to only components of
	 * size at most two.  If they do exists, add them to P.*/	
	while(L.degree >0)
	{
		num = mergeSmallComponents(G, size,  info,  R,  component,  idx, idxSize,  &L);
		if(num == 0)
		{
			break;
		}
		P+=num;
		killList(&L);
		initList(&L);
		getSmallCompList(&L, info, size);
	}
	killList(&L);

	/* ensure that all vertices in R are adjacent to at least three vertices in Q.
	 * That is the subset of P such that <Q> consists of components of at least size
	 * three */
	return P;
}
/******************************************************************************
** Function:	growPalmTreesIV
** Description: Function extends a tree graph to a graph which can
**		contain "little triangles" and in the case of a 
**		component is a path with no triangles a cycle.
**		That is the resulting graph is a tree like 
**		structure with triangle appendages, plus in some
** 		cases a component(s) consisting of only a cycle.
**		In addition there may be some components that 
**		contain a cycle but do not consist solely of 
**		a cycle.  In such cases, the component with
**		the cycle removed will consist of several
**		components of size at most two.
** Inputs:	G - the graph
**              size - number of vertices in graph
**              info - information about planar subgraph
**              R - list of vertices not in planar subgraph P.
**              compNum - next free component number
**              idx - index into R
**              idxSize - size of idx
** Return Value: size of planar subgraph (the forest)
** Side Effects:updates R, Rsize and info.
** Created:	Kerri Morgan	20th March, 2006
** Modified:
** Comments:	This is a modified version of growPalmTreesII.  
******************************************************************************/
int growPalmTreesIV(Vertex* G, int size, K_VertInfo* info, int* R, int* component, R_Index* idx, int idxSize)
{
	int P=0;
	int flag=1;
	int last=0;

	P+= addOPVertices(G, size, info, R, idx, idxSize, component);
	P+=obliterateSmallComponents2(G,size, info, component,&last, &flag);

	return P;
}


/******************************************************************************
** Function:	growPalmTreesIII
** Description: Function extends a tree graph to a graph which can
**		contain "little triangles" and in the case of a 
**		component is a path with no triangles a cycle.
**		That is the resulting graph is a tree like 
**		structure with triangle appendages, plus in some
** 		cases a component(s) consisting of only a cycle.
**		In addition there may be some components that 
**		contain a cycle but do not consist solely of 
**		a cycle.  In such cases, the component with
**		the cycle removed will consist of several
**		components of size at most two.
** Inputs:	G - the graph
**              size - number of vertices in graph
**              info - information about planar subgraph
**              R - list of vertices not in planar subgraph P.
**              compNum - next free component number
**              idx - index into R
**              idxSize - size of idx
** Return Value: size of planar subgraph (the forest)
** Side Effects:updates R, Rsize and info.
** Created:	Kerri Morgan	20th March, 2006
** Modified:
** Comments:	This is a modified version of growPalmTreesII.  
******************************************************************************/
int growPalmTreesIII(Vertex* G, int size, K_VertInfo* info, int* R, int* component, R_Index* idx, int idxSize)
{
	int P=0;
	int flag=1;
	int last=0;


	P+= addOPVertices(G, size, info, R, idx, idxSize, component);
	P+=obliterateSmallComponents(G,size, info, component,&last, &flag);

	return P;
}


/******************************************************************************
** Function:	growPalmTreesII
** Description: Function extends a tree graph to a graph which can
**		contain "little triangles" and in the case of a 
**		component is a path with no triangles a cycle.
**		That is the resulting graph is a tree like 
**		structure with triangle appendages, plus in some
** 		cases a component(s) consisting of only a cycle.
**		In addition there may be some components that 
**		contain a cycle but do not consist solely of 
**		a cycle.  In such cases, the component with
**		the cycle removed will consist of several
**		components of size at most two.
** Inputs:	G - the graph
**              size - number of vertices in graph
**              info - information about planar subgraph
**              R - list of vertices not in planar subgraph P.
**              compNum - next free component number
**              idx - index into R
**              idxSize - size of idx
** Return Value: size of planar subgraph (the forest)
** Side Effects:updates R, Rsize and info.
** Created:	Kerri Morgan	11th March, 2006
** Modified:
** Comments:	This is a modified version of growPalmTrees.  In this 
**		algorithm the Palm Tree structure allows cycles with
**		Palm Tree appendages.
**  		In this code I am not using R - the degree of the
**		vertex will not be considered.
** Acknowledgements:
******************************************************************************/
int growPalmTreesII(Vertex* G, int size, K_VertInfo* info, int* R, int* component, R_Index* idx, int idxSize)
{
	int P=0;
	int flag=1;
	int last=0;

	flag = addOPVertices(G, size, info, R, idx, idxSize, component);
	P+=flag;
	//printKInfo(info, size);	
	while(flag)
	{
		flag =0; /* no changes */
		/* Ensure that there does not exist a vertex that has at most two neighbours
		 * in components of size 3+.
		 * It should be noted that this 3+ component will be non-tree otherwise
		 * the vertex would have been added in addOPVertices */	
		//printKInfo(info, size);
	//	flag = obliterateSmallComponents(G, size, info, component, &last, &flag);

		flag=0;
		last =0;
		
		P+=obliterateSmallComponents(G,size, info, component,&last, &flag);
		// P is not incremented as this operation maintains the size of P */
		//printKInfo(info, size);

	//printKInfo(info, size);	
		if(flag ==0)
		{
			break;
		}	
		flag = addOPVertices(G, size, info, R, idx, idxSize, component);
		P+=flag;
	//printKInfo(info, size);	
	}

	return P;
}


/******************************************************************************
** Function:	Nf_Comp_Small		
** Description: Function checks if vertex v is only adjacent to small
**		components (those of size at most two)
** Inputs:	G - the graph
**		size - number of vertices in the graph
**		info - contains information about P
**		v - the vertex
**		L - list of small components
** Return Value: TRUE - if all forest neighbours are in size 1 and 2 components.
**		Otherwise FALSE
** Side Effects: none
** Created:	Kerri Morgan 20th March, 2006	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int Nf_Comp_Small(Vertex* G, int size, K_VertInfo* info, int v, List* L)
{
	int i;
	int w;

	/* for each of vertex v's neighbours */
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		/* w is a member of some tree component*/
		if((info[w].inP==TRUE) && (info[w].type == FOREST))
		{
			if(inList(L, info[w].component)==FALSE)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}


/******************************************************************************
** Function:	newCyclicComp	
** Description: Function does the work of forming a new non-forest component
**		as the result of adding vertex v
** Inputs:	G - the graph
**		size - number of vertices in the graph
**		info - contains information about P
**		component - next free component number
**		v -the vertex
** Return Value: 
** Side Effects: updates information in info.
** Created:	Kerri Morgan 22nd March, 2006	
** Modified:	This is a slice of code that I've found my self pasting in
**		everywhere - so I've made it into a function:)
** Comments:	
** Acknowledgements:
******************************************************************************/
void newCyclicComp (Vertex* G, int size, K_VertInfo* info, int* component, int v)
{
	int adjVert;
	int j;

	/* add new component */
	removeFromNonP(G, info, v, *component, NON_TREE);
	*component = (*component)+1;
	
	updateNonTree(G, size, info, v, info[v].component);
	for(j=0; j<G[v].degree; j++)
	{
		adjVert= G[v].adjVerts[j];
		if(info[adjVert].inP==FALSE)
		{
			insertInList2(&(info[adjVert].adjNonTrees), info[v].component);
			info[adjVert].Nn++;
		}
	}
}

/******************************************************************************
** Function:	newTreeComp	
** Description: Function does the work of forming a new non-forest component
**		as the result of adding vertex v
** Inputs:	G - the graph
**		size - number of vertices in the graph
**		info - contains information about P
**		component - next free component number
**		v -the vertex
** Return Value: none
** Side Effects: updates information in info and value of component
** Created:	Kerri Morgan 22nd March, 2006	
** Modified:	This is a slice of code that I've found my self pasting in
**		everywhere - so I've made it into a function:)
** Comments
******************************************************************************/
void newTreeComp(Vertex* G, int size, K_VertInfo* info, int* component, int v)
{
	int adjVert;
	int j;
	
	/* make a big tree by adding vertex v */
	adjVert = getAdjTreeVertex(G, size, info, v);
	if(adjVert==-1)
	{
		removeFromNonP(G, info, v, *component, FOREST);
		*component = (*component)+1;	
	}
	else
	{
		removeFromNonP(G, info, v, info[adjVert].component, FOREST);
		/* update all parts to belong to
		* same tree */
		updateTree(G, size, info, v, info[v].component);
	}
	for(j=0; j<G[v].degree; j++)
	{
		adjVert= G[v].adjVerts[j];
		if(info[adjVert].inP==FALSE)
		{
			insertInList2(&(info[adjVert].adjTrees), info[v].component);
			info[adjVert].Nf++;
		}
	}
}
/******************************************************************************
** Function:	obliterateSmallComponents2	
** Description: Function identifies (if any exists) a vertex v that has only
**		two neighbours in a component of size 3+.  The remaining
**		neighbours belong to components of size 1 or 2.
**		The vertex is added to P and another vertex is removed to
**		maintain the planarity of <P>
**		Furthermore if the graph <P> is outerplanar, this function
**		will not add any vertex that destroys the outerplanarity of
**		<P union {v}>
** Inputs:	G - the graph
**		size - number of vertices in the graph
**		info - contains information about P
**		component - next free component number
**		last - the last vertex swapped
**		changed - is set to 1 if changes occur
** Return Value: the number of vertices that have been added to P
** Side Effects: updates information in info.
** Created:	Kerri Morgan 20th March, 2006	
** Modified:	Function loops through all remaining vertices and adds
**		vertices that not only satisfy the above criteria, but 
**		also those that satisfy one of the following:
**		i) The vertex has at most two forest neigbhours in
**		   different components and no  non-forest neighbour.
**		ii) The vertex has at most one neighbour in any component
**		iii) The vertex has 
**		6th April to recognise K4's
**
** Comments:	
** Acknowledgements: 
******************************************************************************/
int obliterateSmallComponents2(Vertex* G, int size, K_VertInfo* info, int* component, int* last, int* changed)
{
	int P=0;
	int i,k;
	List L;
	int w;
//printf("TST obliterate\n");
	/* get a list of components of size at most two */	
	/* NOTE: only do this once, as if a vertex is added,
	 * the function returns*/
	initList(&L);
	getSmallCompList(& L, info, size);

	i=*last;
	for(k=0; k<size; k++)
	{
		i++;
		if(i>=size)
		{
			i=0;
		}
		if(info[i].inP == FALSE)
		{
			/* merge components if possible */
			if((info[i].Nn== info[i].adjNonTrees.count) && (info[i].Nf== info[i].adjTrees.count))
			{
				if(info[i].Nn == 0) /* new component is formed by merging several trees into  one*/
				{
					newTreeComp( G, size, info,  component, i);
					P++;
				}
				else /* new component contains a cycle */
				{
					newCyclicComp(G, size, info, component, i);
					P++;
			     	}
				*last = i;
				(*changed)=1;
				killList(&L);
				getSmallCompList(&L, info, size); 
			}
			/* vertex i has only two neighbours in non-forest components in <P>*/
			else if(info[i].Nn==2) 
			{
				/* either i has at most one tree neighbour, or
				* it has several small tree neighbours */
				if((info[i].Nf <=1) || (Nf_Comp_Small(G, size, info, i, &L)== TRUE))
				{

					if(info[i].Nn == info[i].adjNonTrees.count) /* different components for Nn neighbours*/
					{
						newCyclicComp(G,size, info, component, i);
						P++;
						*last = i;
						(*changed =1);
						killList(&L);
						getSmallCompList(&L, info, size);
					}
					else
					{
						//printf("VERTEX %d should be added\n", i);
						printf("( vertex has %d Nn and %d Nf\n", info[i].Nn, info[i].Nf);
						w = locateDivergentVertex2( G, size,  info, i);
						if(w !=-1)
						{
						printf("NEED TO REMOVE VERTEX %d\n", w);
						printKInfo(info,size);
						exit(1);
						}	
						if(w == -1) /* there is an unique path so I can just add the vertex */
						{
							newCyclicComp ( G, size, info,  component, i);

							*last = i;
							(*changed)=1;
							killList(&L);
							getSmallCompList(& L, info, size); 
							P++;
						//fprintf(stderr, "ERROR 14.10: Unable to locate vertex to remove from path\n");
						//exit(1);
						}
						else
						{	
							/* remove the vertex from the component */
							removeFromP(G, info, w,component);														//printKInfo(info, size);
							if((info[i].Nn==0) && (info[i].Nf == info[i].adjTrees.count))
							{
								newTreeComp( G, size, info,  component, i);
	
							}
							else /* contains a cycle */
							{
								newCyclicComp(G, size, info, component, i);
							}	
							*last = i;
							(*changed)=1;
							/* this change may effect any of the other vertices in R -
							* they may now be able to be added to P */
							k=i;
							killList(&L);
							getSmallCompList(& L, info, size); 
							//killList(&L);
							//return P;
						}
					}
				}
			}
			else if((info[i].Nn <=1) && (info[i].Nf <=2))
			{
				if(info[i].Nn ==0) 
				{
					/* creates a tree component */
					if(info[i].Nf <=1)
					{
						newTreeComp(G, size, info, component, i);
						P++;
					}	
					else if(info[i].Nf == info[i].adjTrees.count)
					{
						newTreeComp(G, size, info, component, i);
						P++;
					}
					else /* more than one neighbour in same tree */
					{
						newCyclicComp(G, size, info, component, i);
						P++;
					}
				}
				else
				{
					newCyclicComp(G, size, info, component, i);
						P++;
				}
				*last = i;
				(*changed)=1;
				killList(&L);
				getSmallCompList(&L, info, size);
			}
		
		}

	}
	killList(&L);
	*last =i;
	return P;
}

/******************************************************************************
** Function:	obliterateSmallComponents	
** Description: Function identifies (if any exists) a vertex v that has only
**		two neighbours in a component of size 3+.  The remaining
**		neighbours belong to components of size 1 or 2.
**		The vertex is added to P and another vertex is removed to
**		maintain the planarity of <P>
**		Furthermore if the graph <P> is outerplanar, this function
**		will not add any vertex that destroys the outerplanarity of
**		<P union {v}>
** Inputs:	G - the graph
**		size - number of vertices in the graph
**		info - contains information about P
**		component - next free component number
**		last - the last vertex swapped
**		changed - is set to 1 if changes occur
** Return Value: the number of vertices that have been added to P
** Side Effects: updates information in info.
** Created:	Kerri Morgan 20th March, 2006	
** Modified:	Function loops through all remaining vertices and adds
**		vertices that not only satisfy the above criteria, but 
**		also those that satisfy one of the following:
**		i) The vertex has at most two forest neigbhours in
**		   different components and no  non-forest neighbour.
**		ii) The vertex has at most one neighbour in any component
**		iii) The vertex has 
**	
**
** Comments:	
** Acknowledgements:
******************************************************************************/
int obliterateSmallComponents(Vertex* G, int size, K_VertInfo* info, int* component, int* last, int* changed)
{
	int P=0;
	int i,k;
	List L;
	int w;
//printf("TST obliterate\n");
	/* get a list of components of size at most two */	
	/* NOTE: only do this once, as if a vertex is added,
	 * the function returns*/
	initList(&L);
	getSmallCompList(& L, info, size);

	i=*last;
	for(k=0; k<size; k++)
	{
		i++;
		if(i>=size)
		{
			i=0;
		}
		if(info[i].inP == FALSE)
		{
			/* merge components if possible */
			if((info[i].Nn== info[i].adjNonTrees.count) && (info[i].Nf== info[i].adjTrees.count))
			{
				if(info[i].Nn == 0) /* new component is formed by merging several trees into  one*/
				{
					newTreeComp( G, size, info,  component, i);
					P++;
				}
				else /* new component contains a cycle */
				{
					newCyclicComp(G, size, info, component, i);
					P++;
			     	}
				*last = i;
				(*changed)=1;
				killList(&L);
				getSmallCompList(&L, info, size); 
			}
			/* vertex i has only two neighbours in non-forest components in <P>*/
			else if(info[i].Nn==2) 
			{
				/* either i has at most one tree neighbour, or
				* it has several small tree neighbours */
				if((info[i].Nf <=1) || (Nf_Comp_Small(G, size, info, i, &L)== TRUE))
				{

					if(info[i].Nn == info[i].adjNonTrees.count) /* different components for Nn neighbours*/
					{
						newCyclicComp(G,size, info, component, i);
						P++;
						*last = i;
						(*changed =1);
						killList(&L);
						getSmallCompList(&L, info, size);
					}
					else
					{
						//printf("VERTEX %d should be added\n", i);
						//printf("( vertex has %d Nn and %d Nf\n", info[i].Nn, info[i].Nf);
						w = locateDivergentVertex( G, size,  info, i);
						//printf("NEED TO REMOVE VERTEX %d\n", w);
							
						if(w == -1) /* there is an unique path so I can just add the vertex */
						{
							newCyclicComp ( G, size, info,  component, i);

							*last = i;
							(*changed)=1;
							killList(&L);
							getSmallCompList(& L, info, size); 
							P++;
						//fprintf(stderr, "ERROR 14.10: Unable to locate vertex to remove from path\n");
						//exit(1);
						}
						else
						{	
							/* remove the vertex from the component */
							removeFromP(G, info, w,component);														//printKInfo(info, size);
							if((info[i].Nn==0) && (info[i].Nf == info[i].adjTrees.count))
							{
								newTreeComp( G, size, info,  component, i);
	
							}
							else /* contains a cycle */
							{
								newCyclicComp(G, size, info, component, i);
							}	
							*last = i;
							(*changed)=1;
							/* this change may effect any of the other vertices in R -
							* they may now be able to be added to P */
							k=i;
							killList(&L);
							getSmallCompList(& L, info, size); 
							//killList(&L);
							//return P;
						}
					}
				}
			}
			else if((info[i].Nn <=1) && (info[i].Nf <=2))
			{
				if(info[i].Nn ==0) 
				{
					/* creates a tree component */
					if(info[i].Nf <=1)
					{
						newTreeComp(G, size, info, component, i);
						P++;
					}	
					else if(info[i].Nf == info[i].adjTrees.count)
					{
						newTreeComp(G, size, info, component, i);
						P++;
					}
					else /* more than one neighbour in same tree */
					{
						newCyclicComp(G, size, info, component, i);
						P++;
					}
				}
				else
				{
					newCyclicComp(G, size, info, component, i);
						P++;
				}
				*last = i;
				(*changed)=1;
				killList(&L);
				getSmallCompList(&L, info, size);
			}
		
		}

	}
	killList(&L);
	*last =i;
	return P;
}

/***************************************************************************
** Function:	alterCompType	
** Description: Function changes all vertices in component to type FOREST
** Inputs:	G - the graph
**		info - contains information about P
**		v current vertex
** Return Value: none
** Side Effects: updates information in info.
** Created:	Kerri Morgan 20th March, 2006	
** Modified:
** Comments:	
** Acknowledgements:
*****************************************************************************/
void alterCompType(Vertex* G,  K_VertInfo* info,  int v)
{
	int  j;
	int w;
	int comp;

	if(info[v].type !=FOREST)
	{
		comp = info[v].component;
		/* change it to forest */
		if(info[v].inP == TRUE)
		{
			info[v].type = FOREST;
		
			/* check each vertex */
			for(j=0; j<G[v].degree; j++)
			{
				w=G[v].adjVerts[j];
				if((info[w].inP == TRUE) && (info[w].type != FOREST))
				{
					alterCompType(G,info, w);
				}
				else
				{
					removeFromList2(&(info[w].adjNonTrees),comp);
					insertInList2(&(info[w].adjTrees),comp);
					info[w].Nn--;
					info[w].Nf++;
				}
			}
		}
	}
}

/***************************************************************************
** Function:	countEdges	
** Description: Function counts the edges in a component
** Inputs:	G - the graph
**		size - number of vertices in the graph
**		info - contains information about P
**		comp - component number
**		num - number of vertices in component
** Return Value: the number of vertices that have been added to P
** Side Effects: updates information in info.
** Created:	Kerri Morgan 13th March, 2006	
** Modified:
** Comments:	
** Acknowledgements:
*****************************************************************************/
int countEdges(Vertex* G, int size, K_VertInfo* info, int comp, int* num)
{
	int i, j;
	int w;
	int edges =0;
	int vertices =0;


	/* check each vertex */
	for(j=0; j<size; j++)
	{
		if(info[j].component == comp)
		{
			vertices++;
			for(i=0; i<G[j].degree; i++)
			{
				w=G[j].adjVerts[i];
			
				if(info[w].component==comp) 
				{
					if(w<j)
					{
						//printf("TEST %d %d is edge\n", w,j);
						edges++;
					}
				}
			}
		}
	}
	*num =vertices;
	return edges;
}


/******************************************************************************
** Function:	addOPVertices	
** Description: Function adds vertices to  P whilst maintaining planarity.  
**		Furthermore if the graph <P> is outerplanar, this function
**		will not add any vertex that destroys the outerplanarity of
**		<P union {v}>
** Inputs:	G - the graph
**		size - number of vertices in the graph
**		info - contains information about P
**		component - next free component number
**		R - set of vertices not in P
**		idxSize -size of R
**		idx - set of indexes into R
** Return Value: the number of vertices that have been added to P
** Side Effects: updates information in info.
** Created:	Kerri Morgan 13th March, 2006	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int addOPVertices(Vertex* G, int size, K_VertInfo* info, int* R, R_Index* idx, int idxSize, int* component)
{
	int P=0;
	int i;
	int set;
	int vx;

//printf("TEST - enter OP\n ");	
	/* for each set */
	for(set=0; set < idxSize; set++)
	{
	     vx=idx[set].start;
	     while(vx<=idx[set].finish)
	     {
		     i=R[vx];
		     if(info[i].inP == FALSE)
		     {
		     /*printf("TESTING TESTING TESTING\n");
		     printKInfo(info, size);
		     printf("TEST %d = %d and %d = %d\n", info[i].Nn, info[i].adjNonTrees.count, info[i].Nf, info[i].adjTrees.count); */
			     /* at most one neighbour in any component */
			     if((info[i].Nn== info[i].adjNonTrees.count) && (info[i].Nf== info[i].adjTrees.count))
			     {
				     if(info[i].Nn == 0) /* new component is formed by merging several trees into  one*/
				     {
					newTreeComp( G, size,  info,  component, i);
					//printf("TEST add %d\n", i);
					P++;
					// successfully tested:
					//printf("FOUND %d is a vertex with all forest neighbours and no duplicates\n", i);
					//

					if(vx==idx[set].finish)
					{
						idx[set].finish--;
					}
					else
					{
						/* copy end vertex into this place */
						R[vx]=R[idx[set].finish];
						idx[set].finish--;
						/* need to recheck the new value at R[i]*/
						vx--;
					}
				     }
				     else /* new component contains a cycle */
				     {
					newCyclicComp ( G, size,  info,  component, i);
					P++;

					if(vx==idx[set].finish)
					{
						idx[set].finish--;
					}
					else
					{
					/* copy end vertex into this place */
						R[vx]=R[idx[set].finish];
						idx[set].finish--;
						/* need to recheck the new value at R[i]*/
						vx--;
					}
			 	     }
			     }
			     /* duplicate entries in components but 
			      * all neighbours of vertex i in any component 
			      * lie on a unique path */
			     else if(checkUniquePaths(G, size, info, i)==TRUE)
			     {
					newCyclicComp ( G, size,  info,  component, i);

					//printf("TEST add %d\n", i);
					P++;

					if(vx==idx[set].finish)
					{
						idx[set].finish--;
					}
					else
					{
					/* copy end vertex into this place */
						R[vx]=R[idx[set].finish];
						idx[set].finish--;
						/* need to recheck the new value at R[i]*/
						vx--;
					}
			     }

			}

		     	vx++;
		     }
	     }
//printf("EXIT OP\n");
	return P;
}

/******************************************************************************
** Function:	addOneNeighbourInEachCompVertices	
** Description: Function adds vertices who have at most one neighbour in
**		any component of size greater than two.
** Inputs:	G - the graph
**		size - number of vertices in the graph
**		info - contains information about P
**		component - next free component number
**		R - set of vertices not in P
**		idxSize -size of R
**		idx - set of indexes into R
** Return Value: the number of vertices that have been added to P
** Side Effects: updates information in info.
** Created:	Kerri Morgan 13th March, 2006	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int addOneNeighbourInEachCompVertices(Vertex* G, int size, K_VertInfo* info, int* R, R_Index* idx, int idxSize, int* component)
//int addOneNeighbourInEachCompVertices(Vertex* G, int size, K_VertInfo* info, int* component)
{
	int P=0;
	int i,j;
	int adjVert;
	List L;
	int set;
	int vx;

	initList(&L);
	getSize2CompList(&L, info, size);
	
	/* for each set */
	for(set=0; set < idxSize; set++)
	{
	     vx=idx[set].start;
	     while(vx<=idx[set].finish)
	     {
		i = R[vx];
		if(info[i].inP == FALSE)
		{
			if(info[i].Nn==0)
			{
				if(info[i].Nf == info[i].adjTrees.count)
				{
					/* make a big tree by adding vertex i */
					adjVert = getAdjTreeVertex(G, size, info, i);
					if(adjVert==-1)
					{
						removeFromNonP(G, info, i, *component, FOREST);
						*component = (*component)+1;	
					}
					else
					{
						removeFromNonP(G, info, i, info[adjVert].component, FOREST);
						/* update all parts to belong to
						 * same tree */
						updateTree(G, size, info, i, info[i].component);
					}
					for(j=0; j<G[i].degree; j++)
					{
						adjVert= G[i].adjVerts[j];
						if(info[adjVert].inP==FALSE)
						{
							insertInList2(&(info[adjVert].adjTrees), info[i].component);
							info[adjVert].Nf++;
						}
					}
					killList(&L);
					getSize2CompList(&L, info, size);
					P++;
					// successfully tested:
					//printf("FOUND %d is a vertex with all forest neighbours and no duplicates\n", i);
					//

					if(vx==idx[set].finish)
					{
						idx[set].finish--;
					}
					else
					{
						/* copy end vertex into this place */
						R[vx]=R[idx[set].finish];
						idx[set].finish--;
						/* need to recheck the new value at R[i]*/
						vx--;
					}
				}
				else if (L.degree > 0) /* multiple neighbours in same component all belong to
				           components of size at most 2 */
			        {
					if(allMultiNeighboursInSmallComponents(G, i, &L, info)==TRUE)
					{
					/* if all the vertices adjacent to more than one vertex are adjacent
					 * to this component - add them */
					 /* make a big non-tree by adding them  */
					// successfully tested 
						//printf("FOUND %d is a vertex with all forest neighbours and some duplicates\n", i);
						removeFromNonP(G, info, i, *component, NON_TREE);
						*component = (*component)+1;

						for(j=0; j<G[i].degree; j++)
						{
							adjVert=G[i].adjVerts[j];
							if(info[adjVert].inP==FALSE)
							{
								insertInList2(&(info[adjVert].adjNonTrees), info[i].component);
								info[adjVert].Nn++;
							}
							else /* vertex is in P */
							{
								/* not already updated */
								if(info[adjVert].component != info[i].component)
								{
									/* components are all trees in this section */
									addTree2NonTree(G, size, info, adjVert, info[i].component);
								}
							}
						}	
						P++;
						killList(&L);
						getSize2CompList(&L, info, size);


						if(vx==idx[set].finish)
						{
							idx[set].finish--;
						}
						else
						{
						/* copy end vertex into this place */
							R[vx]=R[idx[set].finish];
							idx[set].finish--;
							/* need to recheck the new value at R[i]*/
							vx--;
						}
					}
				}	
			}
			else if(info[i].Nn == info[i].adjNonTrees.count)  
			{
				if(info[i].Nf == info[i].adjTrees.count)
				{
					removeFromNonP(G, info, i, *component, NON_TREE);
					*component = (*component)+1;
				//	printf("FOUND %d is a verrtex with some non-forest neighbours and some forest neighbours but no duplciates\n", i);

					updateNonTree(G, size, info, i, info[i].component);
					for(j=0; j<G[i].degree; j++)
					{
						adjVert= G[i].adjVerts[j];
						if(info[adjVert].inP==FALSE)
						{
							insertInList2(&(info[adjVert].adjNonTrees), info[i].component);
							info[adjVert].Nn++;
						}
					}

					/* make a big non-tree by adding them */	
					killList(&L);
					getSize2CompList(&L, info, size);
					P++;

					if(vx==idx[set].finish)
					{
						idx[set].finish--;
					}
					else
					{
					/* copy end vertex into this place */
						R[vx]=R[idx[set].finish];
						idx[set].finish--;
						/* need to recheck the new value at R[i]*/
						vx--;
					}

				}
				else if (L.degree > 0) /* multiple neighbours in same component all
					   belong to components of size at most 2 */
				{
					/* make a big non-tree by adding them */	
					if(allMultiNeighboursInSmallComponents(G, i, &L, info)==TRUE)
					{
				//	printf("FOUND %d is a verrtex with some non-forest neighbours and some forest neighbours but some duplciates\n", i);
					/* if all the vertices adjacent to more than one vertex are adjacent
					 * to this component - add them */
					 /* make a big non-tree by adding them  */
						removeFromNonP(G, info, i, *component, NON_TREE);
						*component = (*component)+1;
						updateNonTree(G, size, info, i, info[i].component);
						for(j=0; j<G[i].degree; j++)
						{
							adjVert= G[i].adjVerts[j];
							if(info[adjVert].inP==FALSE)
							{
								insertInList2(&(info[adjVert].adjNonTrees), info[i].component);
								info[adjVert].Nn++;
							}
						}
					 	P++;
						killList(&L);
					 	getSize2CompList(&L, info, size);
					
						if(vx==idx[set].finish)
						{
							idx[set].finish--;
						}
						else
						{
					/* copy end vertex into this place */
							R[vx]=R[idx[set].finish];
							idx[set].finish--;
							/* need to recheck the new value at R[i]*/
							vx--;
						}
					}
				}
			}
		}
		vx++;
	   }
	}
	killList(&L);
	
	return P;

}


/******************************************************************************
** Function: 	allMultiNeighboursInSmallComponents	
** Description: Function checks if any neighbours of vertex v in
**		the same component are in a component of size two.
** Inputs:	v - the vertex
**		L - list of size 2 components
**		info - contains information about P
** Return Value: true - if all components containing more than one neighbour
**		 to vertex v are size two, otherwise false
** Side Effects: none
** Created:	Kerri Morgan 13th March, 2006	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int allMultiNeighboursInSmallComponents(Vertex* G, int v, List* L, K_VertInfo* info)
{
	C_Node* curNodePtr=NULL;

	curNodePtr = info[v].adjTrees.startPtr;

	while(curNodePtr !=NULL)
	{
		if(curNodePtr->numAdjacent ==2)
		{
			/* 2 neighbours and component is not size 2 */
			if(inList(L, curNodePtr->component) != 1)
			{
				return FALSE;
			}
		}
		else if(curNodePtr->numAdjacent > 2)
		{
			/* more than two neighbours */
			return FALSE;
		}
		curNodePtr = curNodePtr->next;
	}
	return TRUE;
}


/******************************************************************************
** Function:	selectLargeTriangleVertex	
** Description: Function finds vertices with exactly two 
**		neighbours, w1 and w2, in P.  There does not exist
**		an edge (w1,w2) in <P>.  The component that the
**		vertices w1 and w2 is in cannot consist only
**		of a single path if w1 and w2 both have degree
**		one in <P>
**		Any such vertex is then added and another vertex 
**		is removed.  If either w1 or w2 have degree 2 or 
**		more in P, then remove that wi.
**		Otherwise, remove one of the vertices on the path
**		between these two vertices.  Such a vertex exists,
**		or the component consist of a single path.
** Inputs:	G - the graph
**		info - information about P
**		R - set of vertices not in P
**		idx - index into section of R
**		idxSize - size of idx
** Return Value: num1 - number of vertices added
** Side Effects: updates info, G, R and idx to reflect the movement
**		of vertices from R to P
** Created:	Kerri Morgan	25th August, 2005	
** Modified:    Kerri Morgan    21st April, 2006 - checks if any vertices have at most 
**		neighbour as the result of a removal
** Comments:	
** Acknowledgements:
******************************************************************************/
int selectLargeTriangleVertex(Vertex* G, K_VertInfo* info, int* R, R_Index* idx, int idxSize, int* component)
{
	int num1 =0;
	int flag =0;
	int set;
	int i;
	int v;
	int w1, w2;
	int num;
	int toDelete;
	int kx, ky;

	for(set=0; set<idxSize; set++)
	{
		i=idx[set].start;
		while(i<=idx[set].finish)
		{
			v=R[i];
			if(info[v].inP == FALSE)
			{

				toDelete =-1;
			
			/* vertex has two forest neighbours in P */
			if((info[v].Nn == 0) && (info[v].Nf==2))
			{ 
				findTwinNeighbours(G, v, &w1, &w2);
				if(w1==-1 || w2==-1)
				{
					fprintf(stderr, "ERROR 14.2: Unable to locate two vertices adjacent to vertex %d with two neighbours in P\n", v);
					exit(1);				
				}
				else if( info[w1].component != info[w2].component)
				{
					/* removing a vertex from a previous large triangle has 
					 * broken a component into two pieces
					 * Now vertex v is adjacent to two tree components
					 * and can be added to merge them */
					 add2_KTree(G, info,component, v);
					 num1++;
					 flag=1;
					if(i==idx[set].finish)
					{
						idx[set].finish--;
					}
					else
					{
						/* copy end vertex into this place */
						R[i]=R[idx[set].finish];
						idx[set].finish--;
						/* need to recheck the new value at R[i]*/
						i--;
					}
				}
				else /* both w1 and w2 are in same component */
				{
					num=numberOfNeighbours(G, info, w1);
					if(num > 1)
					{
						/* w1 has at least two neighbours */
						toDelete =w1;	
					}
					else
					{
						num = numberOfNeighbours(G, info, w2);
						/* w2 has at least two neighbours */
						if(num >1)
						{
							toDelete =w2;			
						}
						else
						{
							toDelete =vertDegreeGreaterThanTwo(G,w1,w2);
		
						}
					}
					if(toDelete != -1) /* this component is not a path */
					{
						flag =1;
						/* remove vertex toDelete */
						removeFromKTree(G, info, component, toDelete);
						/* add vertex v 
						 * The result is still a tree, 
						 * but may merge two trees */
						 add2_KTree(G, info, component, v);
						 /* overwrite v with deleted vertex in R */


		 				 R[i]=toDelete;				 
						/* check if any neighbour of toDelete has only one neighbour - if so add it */
						 num1+=addable_Vert(G,  info, toDelete,  component, R, idx, idxSize);
					}
					/* else cycle - left to closePaths function */
				}
			}
			else if((info[v].Nf == 0) && (info[v].Nn==2))
			{
				findTwinNeighbours(G, v, &w1, &w2);

				if(w1==-1 || w2==-1)
				{
					fprintf(stderr, "ERROR 14.2: Unable to locate two vertices adjacent to vertex %d with two neighbours in P\n", v);
					exit(1);				
				}
				else if( info[w1].component != info[w2].component)
				{
					/* removing a vertex in a previous large triangle 
					 * has created a vertex adjacent to two separate
					 * components */
					 add2_Non_KTree(G, info, component, v);
					 num1++;
					 flag =1;

					if(i==idx[set].finish)
					{
						idx[set].finish--;
					}
					else
					{
						/* copy end vertex into this place */
						R[i]=R[idx[set].finish];
						idx[set].finish--;
						/* need to recheck the new value at R[i]*/
						i--;
					}
				}
				else /* same component for w1 and w2 */
				{

					num=numberOfNeighbours(G, info, w1);
					if(num > 1)
					{
						/* w1 has at least two neighbours */
						toDelete =w1;	
					}
					else
					{
						num = numberOfNeighbours(G, info, w2);
						/* w2 has at least two neighbours */
						if(num >1)
						{
							toDelete =w2;			
						}
						else
						{
							toDelete = vertDegreeGreaterThanTwo_NonTree(G, info, w1, w2);
	
						}
					}
					if(toDelete != -1)
					{
						flag =1;
						/* remove vertex toDelete */
						remove_KVertex(G, info, component, toDelete);
						/* add vertex v */
						add_KVertex(G, info, component, v);
	
						/* overwrite v with deleted vertex */
		 				 R[i]=toDelete;				 
						
						/* check if any neighbour of toDelete has only one neighbour - if so add it */
						 num1+=addable_Vert(G,  info, toDelete,  component, R, idx, idxSize);
					}
				}
				/* else component is a path - dealt with in closePath function */
			}
			else if((info[v].Nf == 1) && (info[v].Nn==1)) 
			{
				/* one forest neighbour and one non-forest neighbour */
				add2_Non_KTree(G, info, component, v);
				flag =1;
				num1++;
				if(i==idx[set].finish)
				{
					idx[set].finish--;
				}
				else
				{
					/* copy end vertex into this place */
					R[i]=R[idx[set].finish];
					idx[set].finish--;
					/* need to recheck the new value at R[i]*/
					i--;
				}
			}
			else if((info[v].Nf ==1) && (info[v].Nn==0))
			{
				add2_KTree(G, info,component, v);
				num1++;
				flag=1;
				if(i==idx[set].finish)
				{
					idx[set].finish--;
				}
				else
				{
					/* copy end vertex into this place */
					R[i]=R[idx[set].finish];
					idx[set].finish--;
				/* need to recheck the new value at R[i]*/
					i--;
				}
			}
			else if((info[v].Nf==0)&& (info[v].Nn==1))
			{
				add2_Non_KTree(G, info, component, v);
				flag =1;
				num1++;
				if(i==idx[set].finish)
				{
					idx[set].finish--;
				}
				else
				{
					/* copy end vertex into this place */
					R[i]=R[idx[set].finish];
					idx[set].finish--;
					/* need to recheck the new value at R[i]*/
					i--;
				}
			}
			else if((info[v].Nf==0)&&(info[v].Nn==0))
			{
				info[v].component = *component;
				*component = (*component)+1;
				G[v].member = TRUE;
				info[v].inP=TRUE;
				info[v].type = FOREST;
				for(kx=0;kx<G[v].degree; kx++)
				{
					ky=G[v].adjVerts[kx];
					if(info[ky].inP==FALSE)
					{
						insertInList2(&(info[ky].adjTrees), info[v].component);
						info[ky].Nf++;
					}
				}
				flag =1;
				num1++;
				if(i==idx[set].finish)
				{
					idx[set].finish--;
				}
				else
				{
					/* copy end vertex into this place */
					R[i]=R[idx[set].finish];
					idx[set].finish--;
					/* need to recheck the new value at R[i]*/
					i--;
				}

			}
			}
			i++;
		}
	}
	if(flag ==1)
	{
		return num1;
	}
	else
	{
		return -1;
	}
}	

/******************************************************************************
** Function:	selectTriangleVertex	
** Description: Function finds vertices with exactly two 
**		neighbours, w1 and w2, in P.  There exists an
**		edge (w1,w2) in <P>.
**		These vertices are added to P
** Inputs:	G - the graph
**		info - information about P
**		R - set of vertices not in P
**		idx - index into section of R
**		idxSize - size of idx
** Return Value: num - number of vertices added
** Side Effects: updates info, G, R and idx to reflect the movement
**		of vertices from R to P
** Created:	Kerri Morgan	25th August, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int selectTriangleVertex(Vertex* G, K_VertInfo* info, int* R, R_Index* idx, int idxSize)
{
	int set;
	int i,j;
	int v, w;
	int w1, w2;
	int count =0;

	/* for each set */
	for(set=0; set < idxSize; set++)
	{
		i=idx[set].start;
		while(i<=idx[set].finish)
		{
			v = R[i];
			/* not in P */
			if(info[v].inP == FALSE)
			{
				
				/* two neighbours */
				if((info[v].Nn + info[v].Nf)==2)
				{

					findTwinNeighbours(G, v, &w1, &w2);
					if(w1==-1 || w2==-1)
					{
						fprintf(stderr, "ERROR 14.1: Unable to locate two vertices adjacent to vertex %d with two neighbours in P\n", v);
						exit(1);				
					}
					/* <w1,w2> is an edge */
					if(neighbours(G, w1, w2)==TRUE)
					{
						#ifdef DEBUG
						printf("TESTING adding %d\n", v);
						#endif
						/* add vertex v to P */
						G[v].member=TRUE;
						info[v].type =NON_TREE;
						info[v].inP=TRUE;

						info[v].component=info[w1].component;

						/* all vertices adjacent to vertex v are now
						 * adjacent to this component */
						for(j=0; j<G[v].degree; j++)
						{
							w=G[v].adjVerts[j];
							if(info[w].inP==FALSE)
							{
								info[w].Nn++;
								insertInList2(&(info[w].adjNonTrees), info[v].component);
							}
						}
						
						if(info[w1].type == FOREST) /* as w2 is in same component, it too is in the forest*/
						{
							/* mark all vertices in component as nontrees
							 * and update the information for their nonP neighbours*/
							markAsNonTree_K(G, info, w1);
						}
						count++;

						if(i==idx[set].finish)
						{
							idx[set].finish--;
						}
						else
						{
							/* copy end vertex into this place */
							R[i]=R[idx[set].finish];
							idx[set].finish--;
							/* need to recheck the new value at R[i]*/
							i--;
						}
						
					}
				}
			}

			i++;
		}

	}
	return count;

}


/******************************************************************************
** Function:	closePaths
** Description: Function finds vertices with exactly two 
**		neighbours, w1 and w2, in P, where both w1 and
**		w2 are end vertices in a path.  The component
**		these vertices are in consists solely of a path.
**		The vertex is added to P.	
** Inputs:	G - the graph
**		info - information about P
**		R - set of vertices not in P
**		idx - index into section of R
**		idxSize - size of idx
**		component - next free component number
** Return Value: count - number of vertices added 
** Side Effects: updates info, G, R and idx to reflect the movement
**		of vertices from R to P.  Also may update
**		component
** Created:	Kerri Morgan	25th August, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int closePaths(Vertex* G, K_VertInfo* info, int* R, R_Index* idx, int idxSize, int* component)
{
	int num1, num2 =0;
	int set;
	int i;
	int v,w;
	int w1, w2;

	for(set=0; set<idxSize; set++)
	{
		i=idx[set].start;
		while(i<=idx[set].finish)
		{
			v=R[i];
			if(info[v].inP == FALSE)
			{

				/* vertex has two forest neighbours in P */
				if((info[v].Nn == 0) && (info[v].Nf==2))
				{
					findTwinNeighbours(G, v, &w1, &w2);
					if(w1==-1 || w2==-1)
					{
						fprintf(stderr, "ERROR 14.6: Unable to locate two vertices adjacent to vertex %d with two neighbours in P\n", v);
						exit(1);				
					}
					else if( info[w1].component != info[w2].component)
					{
						fprintf(stderr, "ERROR 14.7: Vertex %d is adjacent to two vertices (%d %d) in different components - contradicts maximal tree\n", v, w1, w2);
						exit(1);				
					}
					num1=numberOfNeighbours(G, info, w1);
					num2=numberOfNeighbours(G, info, w2);
	
					if((num1== 1) && (num2 == 1))
					{
						w =vertDegreeGreaterThanTwo(G,w1,w2);
						/* component consists of only a path*/
						if(w == -1) 
						{
							/* add vertex v */
					 		add2_Non_KTree(G, info, component, v);

							if(i==idx[set].finish)
							{
								idx[set].finish--;
							}
							else
							{
								/* copy end vertex into this place */
								R[i]=R[idx[set].finish];
								idx[set].finish--;
							}

							#ifdef DEBUG
							printf("TEST::: %d is added in close paths\n", v);
							#endif
							return 1;
							
						}
					}
				}
			}
			i++;
		}
	}
	return 0;
}

/******************************************************************************
** Function:	findTwinNeighbours
** Description: Function finds two neighbours of v in P.
** Inputs:	G - the graph
**		v - the vertex
**		w1,w2 - to store the neighbours
** Return Value:none 
** Side Effects:w1 and w2 hold the neighbours of v, if no such neighbours exist
**		they hold -1.
** Created:	Kerri Morgan	25th August, 2005	
** Modified:
** Comments:  w1 and w2 need not be adjacent	
** Acknowledgements:
******************************************************************************/
void findTwinNeighbours(Vertex* G, int v, int* w1, int* w2)
{
	int i;
	int w;

	*w1=-1;
	*w2=-1;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if(G[w].member == TRUE)
		{
			if(*w1==-1)
			{
				*w1 = w;
			}
			else
			{
				*w2=w;
				break;
			}

		}
	}
}

/******************************************************************************
** Function:	neighbours	
** Description: Function determines if two vertices are adjacent
** Inputs:	G - the graph
**		w1,w2 - the vertices
** Return Value:TRUE - if they are adjacent otherwise FALSE 
** Created:	Kerri Morgan	25th August, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int  neighbours(Vertex* G, int w1, int w2)
{
	int i;

	for(i=0; i<G[w1].degree; i++)
	{
		if(G[w1].adjVerts[i]==w2)
		{
			return TRUE;
		}
	}

	return FALSE;

}

/******************************************************************************
** Function:	markAsNonTree_K
** Description: Function marks the vertex v as belonging to a non-tree 
**		component.  Any vertices adjacnet to v that still are
**		marked as belonging to a nontree are updated in one of two
**		ways:
**		i) Vertex w is adjacent to vertex v and belongs to P
**		   If w is not already marked as a non-tree, mark it as one.
**		ii) Vertex w belongs to R - update it's lists of adjacent
**		    components, and its number of forest and non-forest
**		    neighbours
**		
** Inputs:	G - the graph
**		info - information about P
**		v - the vertex
** Return Value:none
** Side Effects:updates info
** Created:	Kerri Morgan	27th August, 2005	
** Modified:	
** Comments:	This function is similar to one used in addVertAlg.h, but
**		works with a structure of type K_VertInfo
** Acknowledgements:
******************************************************************************/
void markAsNonTree_K(Vertex* G, K_VertInfo* info, int v)
{
	int w;
	int i;
	int component;

	info[v].type = NON_TREE;
	component = info[v].component;
	
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		
		if(info[w].inP == TRUE)
		{
			if(info[w].type == FOREST)
			{
				markAsNonTree_K(G, info, w);
			}
		}
		else /* not in P */
		{
			/* remove this component from the list of tree
			 * neighbours and add to list of non-tree
			 * neighbours */
			removeFromList2(&(info[w].adjTrees), component);
			insertInList2(&(info[w].adjNonTrees), component);
			info[w].Nn++;
			info[w].Nf--;
		}
	}
}

/******************************************************************************
** Function:	numberOfNeighbours	
** Description: Function counts the number of neighbours in P of vertex v
** Inputs:	G - the graph
**		info  - information about P
**		v - the vertex
** Return Value:num - number of neighbours in P of vertex v
** Side Effects: none
** Created:	Kerri Morgan	27th August, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int numberOfNeighbours(Vertex* G, K_VertInfo* info, int v)
{
	int num=0;
	int i;
	
	for(i=0; i<G[v].degree; i++)
	{
		if(info[G[v].adjVerts[i]].inP==TRUE)
		{
			num++;
		}
	}
	return num;
}

/******************************************************************************
** Function:	vertDegreeGreaterThanTwo
** Description: Function finds a vertx in a path connecting w1 to w2 with
**		degree in <P> of at least three.
** Inputs:	G - the graph
**		w1, w2 - vertices
** Return Value:the number of a vertex of at least degree three in the path.
**		If no such vertex exists, the function returns -1
** Side Effects:
** Created:	Kerri Morgan	30th August, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int vertDegreeGreaterThanTwo(Vertex* G, int w1, int w2)
{
	int i;
	int v;
	int prev =-1;
	int cur, next;
	int count;

	cur =w1;
	while(cur!=w2)
	{
		count =0;
		for(i=0; i<G[cur].degree; i++)
		{
			v = G[cur].adjVerts[i];
			if(G[v].member ==KNOWN)
			{
				count++;
				if(G[cur].adjVerts[i] !=prev)
				{
					next =v;
				}
				if(count > 2)
				{
					return cur;
				}
			}	
		}
		prev =cur;
		cur = next;
	}
	return -1;
	
}

/******************************************************************************
** Function:	remove_KVertex	
** Description: Function removes a vertex from P
** Inputs:	G - the graph
**		info - information about P
**		comp - the first free component number
**		v - the vertex
** Return Value:none
** Side Effects: updates info, G and comp
** Created:	Kerri Morgan	2 September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void remove_KVertex(Vertex* G, K_VertInfo* info, int* comp, int v)
{
	int i;
	int old;
	int type;
	int w;
	int edges, vertices;

	type = info[v].type;
	old = info[v].component;
	G[v].member = UNKNOWN;
	info[v].inP = FALSE;
	info[v].component = -1;

	killList2(&(info[v].adjTrees));
	killList2(&(info[v].adjNonTrees));

	initList2(&(info[v].adjTrees));
	initList2(&(info[v].adjNonTrees));

	/* for all neighbours of v */
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		/* non-P vertex*/
		if(info[w].inP == FALSE)
		{
			if(type == FOREST)
			{
				/* decrement forest neighbours */
				info[w].Nf--;
				removeFromList2(&(info[w].adjTrees), old);
			}
			else if(type == NON_TREE)
			{
				info[w].Nn--;
				removeFromList2(&(info[w].adjNonTrees), old);
			}
		}
		else if(info[w].component == old) /* P neighbour still in old component */
		{
			edges =0;
			vertices =1;

			infoUpdate(G, info, w, *comp, &edges, &vertices);
			*comp= (*comp)+1;
			/* changes have made a component become a tree */
			if((type == NON_TREE) && (vertices - edges == 1))
			{
				alterToTree(G, info, w);

			}
		}
	}
	info[v].Nn=0;
	info[v].Nf=0;
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if(info[w].inP == TRUE)
		{
			if(info[w].type == FOREST)
			{
				info[v].Nf++;	
			}
			else 
			{
				info[v].Nn++;
			}
		}
	}	
	info[v].type = UNDECIDED;

}

/******************************************************************************
** Function:	RemoveFromKTree
** Description: Function removes a vertex from a tree
** Inputs:	G - the graph
**		info - information about P
**		comp - next free component number
**		v - the vertex to remove
** Return Value:
** Side Effects:
** Created:	Kerri Morgan	30th August, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void removeFromKTree(Vertex* G, K_VertInfo* info, int* comp, int v)
{
	int i; 
	int w;
	int old_Comp;

	/* leaf */
	if(numberOfNeighbours(G, info, v)== 1)
	{
		old_Comp=info[v].component;
		info[v].inP = FALSE;
		info[v].component =-1;
		info[v].type = UNDECIDED;
		G[v].member = UNKNOWN;
		killList2(&(info[v].adjTrees));
		killList2(&(info[v].adjNonTrees));
		initList2(&(info[v].adjTrees));
		initList2(&(info[v].adjNonTrees));

		/* v has a single tree neighbour */
		insertInList2(&(info[v].adjTrees), old_Comp);
		info[v].Nn=1;
		info[v].Nf=0;

		/* update information for v's neighbours */
		for(i=0; i<G[v].degree; i++)
		{
			w=G[v].adjVerts[i];
			/* neighbour is not in P */
			if(info[w].inP == FALSE)
			{
				removeFromList2(&(info[w].adjTrees), old_Comp);
				info[w].Nf--;
			}	
		}
	}
	else /* non-leaf */
	{
		old_Comp=info[v].component;
		info[v].inP = FALSE;
		info[v].component =-1;
		info[v].type = UNDECIDED;
		G[v].member = UNKNOWN;
		killList2(&(info[v].adjTrees));
		killList2(&(info[v].adjNonTrees));
		initList2(&(info[v].adjTrees));
		initList2(&(info[v].adjNonTrees));
		
		/* component may have broken into two or more 
		 * pieces */
		for(i=0; i<G[v].degree; i++)
		{
		 	w= G[v].adjVerts[i];

			if((info[w].inP == TRUE) && (info[w].component == old_Comp))
			{
				updateCompDetails(G, info, w, *comp);
				*comp = (*comp)+1;
			}
			else /* not in P */
			{
				/* no longer adjacent to component via vertex v*/
				removeFromList2(&(info[w].adjTrees),old_Comp); 
				info[w].Nf--;	
			}
		
		}

	}

	/* update numbers of neighbours */
	info[v].Nn=0;
	info[v].Nf=0;
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if(info[w].inP == TRUE)
		{
			if(info[w].type == FOREST)
			{
				info[v].Nf++;	
			}
			else 
			{
				info[v].Nn++;
			}
		}
	}	
}



/******************************************************************************
** Function:	add2_KTree
** Description: Function adds a vertex to a tree component.  
**		This may merge several trees.
**		This function  is only to be called when the vertex
**		is adjacent to components that are trees.
** Inputs:	G - the graph	
**		info - information about P
**	      	component - the component number of the next free component
**		v - the vertex
** Return Value:none
** Side Effects:updates the info structure and the value in component
** Created:	Kerri Morgan	2 September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void add2_KTree(Vertex  *G, K_VertInfo* info, int* component, int v)
{
	int i;
	int w;

	info[v].inP = TRUE;
	info[v].component = *component;
	G[v].member = KNOWN;
	info[v].type=FOREST;

	/* clear non-P list */
	killList2(&(info[v].adjTrees));
	killList2(&(info[v].adjNonTrees));

	initList2(&(info[v].adjTrees));
	initList2(&(info[v].adjNonTrees));

	info[v].Nf=0;
	info[v].Nn=0;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];

		if(info[w].inP ==FALSE)
		{
			/* now adjacent to component */
			insertInList2(&(info[w].adjTrees), *component);
			info[w].Nf++;
		}
		else if((info[w].inP == TRUE) && (info[w].component != *component))
		{
			updateCompDetails(G, info, w, *component);
		}
	}
	*component = (*component) +1;
}
/******************************************************************************
** Function: updateCompDetails
** Description: Function replaces the old component number of vertex v
**		with the new component number(comp).  Any vertices 
**		belonging to P adjacent to v are then updated also.
**		The component is a tree component.
** Inputs:	G - the graph
**		info - information about P
** 		v - the vertex in P 
**		comp
** Return Value: none
** Side Effects:updates the component and adjTree members of the structure
** Created:	Kerri Morgan	2 September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void updateCompDetails(Vertex* G, K_VertInfo*  info, int v, int comp)
{
	int oldComp;
	int i;
	int w;

	oldComp = info[v].component;
	info[v].component = comp;
	
	for(i=0; i<G[v].degree; i++)
	{
		w = G[v].adjVerts[i];
	
		/* if w is also part of this component,
		 * but has not had it's component number 
		 * updated, then update it */	
		if((info[w].inP == TRUE) && (info[w].component !=comp))
		{
			updateCompDetails(G, info, w, comp);
		}
		else /* not in P */
		{
			/* remove old component number
			 * and add new component number*/
			removeFromList2(&(info[w].adjTrees), oldComp);
			insertInList2(&(info[w].adjTrees), comp);
		}

	}
}

/******************************************************************************
** Function:	addable_Vert	
** Description: Function checks if any neighbour of a removed vertex has now
**		at most 1 neighbour in each component.  If this is the
**		case, the vertex is added to P
** Inputs:	G - the graph
**		info - information about P
**		v - the vertex removed from P
**		component - next free component number
**		R - list of vertices in R
**		idx - indices into R
**		idxSize - size of idx
** Return Value: num - the number of vertices added
** Side Effects: updates info and component
** Created:	Kerri Morgan	2 September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int  addable_Vert(Vertex* G, K_VertInfo*  info, int v, int* component,int* R, R_Index* idx, int idxSize)
{
	int i;
	int v1;
	int w1, w2;
	int num=0;

        for(i=0; i< G[v].degree; i++)
	{
		v1 =G[v].adjVerts[i];

		/* if neighbour not in P */
		if(info[v1].inP == FALSE)
		{
			/* check if had two neighbours in 1 component 
			 * but now has one neighbour in 1 component */
			if(info[v1].Nf + info[v1].Nn ==1)
			{
				if(info[v1].Nf == 1) /* add to tree */
				{
					 add2_KTree(G,  info,  component, v1);
					 updateR(R, idx, idxSize, v1);
					 num++;
				}
				else if(info[v1].Nn == 1)
				{
					add2_Non_KTree(G, info, component, v1);
					updateR(R, idx, idxSize, v1);
					num++;
				}
			}
			else if (info[v1].Nf + info[v1].Nn == 2)
			/* check if had three neighbours but now
			 * has two neighbours, and those neighbours
			 * are in two different components.
			 * (NOTE: if now two neighbours in same componet,
			 * it will be picked up by triangle code )*/
			{
				/* check that they are not in same components */
				w1=-1;
				w2=-1;
				/* changed to v1 from v - mistake in code 24/9 */
				findTwinNeighbours(G, v1, &w1, &w2);
				if(w1 == -1 || w2 == -1)
				{
					fprintf(stderr, "ERROR 14.4: Unable to locate two neighbours adjacent to vertex %d with two neighbours in P\n", v1);
					exit(1);
					
				}
				if(info[w1].component != info[w2].component)
				{
					/* adjacent to two tree components*/
					if(info[v1].Nf == 2)  /* mistake here as well - changed v to v1 24/9 */
					{
						add2_KTree(G, info, component, v1);	
						updateR(R, idx, idxSize, v1);
					 	num++;
					}
					else
					{
					    	add2_Non_KTree(G, info, component, v1);
						updateR(R, idx, idxSize, v1);
						num++;
					}
				}
			}
																																				      }
	}
	return num;
}

/******************************************************************************
** Function:	add2_Non_KTree
** Description:  Function adds a vertex to P.  This vertex is adjacent to at
**		 least one component that is a non-tree.
** Inputs:	G - the graph
**		info - structure holding information about P
**		component - next free component
** 		v - the vertgex to be added
** Return Value:none
** Side Effects: updates P and component
** Created:	Kerri Morgan	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void add2_Non_KTree(Vertex  *G, K_VertInfo* info, int* component, int v)
{
	int w;
	int i;

	G[v].member = TRUE;
        info[v].type = NON_TREE;
	info[v].inP = TRUE;

	/* clear non-P list */
	killList2(&(info[v].adjTrees));
	killList2(&(info[v].adjNonTrees));

	initList2(&(info[v].adjTrees));
	initList2(&(info[v].adjNonTrees));

	info[v].Nf=0;
	info[v].Nn=0;

	info[v].component = *component;
	*component = (*component)+1;

	/* for all vertices adjacent to v */
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
	
		/* neighbour is already in P - merge components */
		if((info[w].inP == TRUE) && (info[w].component != info[v].component))
		{
			addToComponent(G, w, info, info[v].component);
		}
		else if(info[w].inP == FALSE) /* neighbour is not in P - increment nonTree neighbours */
		{
			insertInList2(&(info[w].adjNonTrees), info[v].component);
			info[w].Nn++;
		}
	}
}

/******************************************************************************
** Function:	vertDegreeGreaterThanTwo_NonTree	
** Description: Function finds the first vertex of degree greater than two
**		in a path from vertex w1 to w2 in a nonTree component
** Inputs:	G - the graph
**		w1, w2 - the vertices
** Return Value: The vertex to be deleted, else -1 if no such vertex exists
** Side Effects:
** Created:	Kerri Morgan	2 September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int vertDegreeGreaterThanTwo_NonTree(Vertex* G, K_VertInfo* info,  int w1, int w2)
{
	List Path;
	int w=-1;
	Node* cur = NULL;

	initList(&Path);

	insertAtEnd(&Path, w1);
	if(find_KPath(G, &Path, info, w1, w2) == FALSE)
	{
		fprintf(stderr, "Unable to find path between vertices (%d %d) in single component\n", w1, w2);
		exit(1);
	}

	/* find the first vertex in path that has more than two neighbours */
	cur = Path.startPtr;
	while(cur !=NULL)
	{
		w=cur->vertex;

		if(numberOfNeighbours(G, info, w) >=3)
		{
			killList(&Path);	
			return w;
		}
		cur = cur->next;

	}	
	killList(&Path);	
	return -1;
}

/******************************************************************************
** Function:	find_KPath
** Description: Function finds a path between vertices w1 and w2
** Inputs:	G - the graph
**		Path - the partially formed path
**		info - information about P
**		w1 - last vertex in partially formed path
**		w2 - target end vertex in path
** Return Value:TRUE - if such a path exist, otherwise FALSE
** Side Effects:Path contains a list of vertices in the path
** Created:	Kerri Morgan	2 September, 2005	2 September, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int find_KPath(Vertex* G, List* Path, K_VertInfo* info, int w1, int w2)
{
	int i;
	int v;

	if(neighbours(G, w1, w2) == TRUE)
	{
		insertAtEnd(Path, w2);
		return TRUE;
	}
	
	for(i=0; i<G[w1].degree; i++)
	{
		v = G[w1].adjVerts[i];
		/* vertex v is not already in path, but
		 * is in P */
		if((info[v].inP == TRUE) && (inList(Path, v)==FALSE))
		{
			insertAtEnd(Path, v);
			if(find_KPath(G, Path, info, v, w2)==TRUE)
			{
				/* path w1 to v and v to w2 */
				return TRUE;
			}
			else
			{
				removeFromList(Path, v);
			}
		}
	}
	return FALSE;
}

/******************************************************************************
** Function:	infoUpdate	
** Description: Function updates information about P when a vertex adjacent
**		to v is removed from P
** Inputs:	G - the graph
** 		info - structure holding information about P
**		v - vertex in component containing vertex
**		removed from P
**		comp - new component number
**		edges - counter for edges in component
**		vertices - counter for vertices in component
** Return Value: none
** Side Effects: updates the component numbe
** Created:	Kerri Morgan	2 September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void infoUpdate(Vertex* G, K_VertInfo* info, int v, int comp, int* edges, int* vertices)
{
	int i;
	int w;
	int old;
	
	old = info[v].component;
	info[v].component = comp;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		/* In P but not yet added to new component */
		if((info[w].inP == TRUE) && (info[w].component != comp))
		{
			*vertices = (*vertices)+1;
			infoUpdate(G, info, w, comp, edges, vertices);
		}
		if((info[w].inP == TRUE) && (v>w))
		{
			*edges = (*edges)+1;
		}
		if(info[w].inP == FALSE)
		{
			if(info[v].type == NON_TREE)
			{
				removeFromList2(&(info[w].adjNonTrees), old);
				insertInList2(&(info[w].adjNonTrees), comp);
			}
			else if(info[v].type == FOREST)
			{
				removeFromList2(&(info[w].adjTrees), old);
				insertInList2(&(info[w].adjTrees), comp);
			}
		}

	}

}


/******************************************************************************
** Function:	alterToTree	
** Description:  Function re-labels a component from NON_TREE to FOREST
** Inputs:	G - the graph
**		info - information about P
**		v - a vertex in the component
** Return Value:none
** Side Effects:updates info
** Created:	Kerri Morgan 	2 September, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void alterToTree(Vertex* G, K_VertInfo* info, int v)
{
	int i;
	int w;
	int comp;

	/* convert to tree */
	if(info[v].type != FOREST)
	{
		info[v].type =FOREST;
		comp = info[v].component;
		for(i=0; i<G[v].degree; i++)
		{
			w=G[v].adjVerts[i];

			if(info[w].inP == FALSE)
			{
				info[w].Nn--;
				info[w].Nf++;
				removeFromList2(&(info[w].adjNonTrees), comp);
				insertInList2(&(info[w].adjTrees), comp);
			}
			else if(info[w].type != FOREST) /* in P an not marked as forest */
			{
				alterToTree(G, info, w);
			}
		}
	}

}

/******************************************************************************
** Function:	add_KVertex
** Description: Function add vertex v to P
** Inputs:	G - the graph
**		info -  information about P
**		comp - next available component number
**		v - vertex to add
** Return Value: none
** Side Effects:
** Created:	Kerri Morgan	2 September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void add_KVertex(Vertex* G, K_VertInfo* info, int* comp, int v)
{
	if(info[v].Nn > 0) /* new component is a non-tree component*/
	{
		add2_Non_KTree(G,  info,  comp, v);
	}
	else /* tree components only */
	{
		/* vertex is adjacent to only one vertex in each component */
		if(info[v].Nf== info[v].adjTrees.count) 
		{
			add2_KTree(G,  info,  comp, v);
		}
		else /* adjacent to two vertices in some component, so
			new component is a non-tree */
		{
			add2_Non_KTree(G,  info,  comp, v);
		}

	}


}


/******************************************************************************
** Function:	updateR
** Description: Function updates R and Ridx after a vertex v is added to 
**		the graph
** Inputs:	R - list of vertices in R
**		R_idx list of indices into R
**		idxSize - size of R
**		v - the vertex
** Return Value: none
** Side Effects: updates R
** Created:	Kerri Morgan	3 September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void updateR(int* R, R_Index* idx, int idxSize, int v)
{
	int set;
	int i;

	for(set = 0; set <idxSize; set++)
	{
		i=idx[set].start;
		while(i<=idx[set].finish)
		{
			/* found the vertex */
			if(R[i] == v)
			{
				if(i==idx[set].finish)
				{
					idx[set].finish--;
				}
				else
				{
					R[i]=R[idx[set].finish];
					idx[set].finish--;
				}
				return;
			}
			i++;
		}

	}
}


/******************************************************************************
** Function:	getSize2CompList
** Description: Function creates a list of components of size two in
**		<P>.
** Inputs:	L - empty list
**		info - holds information about P
**		size - number of vertices in original graph
** Return Value:
** Side Effects:Updates the contents of list to hold numbers of components
**		of size at most two.
** Created:	Kerri Morgan	13th March, 2006
** Modified:	This is a modified version of getSmallCompList.  In this 
**		modification only components of size two are noted.
** Comments:	
** Acknowledgements:
******************************************************************************/
void getSize2CompList(List* L, K_VertInfo* info, int size)
{
	int i;
	List2 L1;
	C_Node* cur = NULL;

	initList2(&L1);

	for(i=0; i<size; i++)
	{
		if(info[i].inP == TRUE)
		{
			insertInList2(&L1, info[i].component);
		}
	}

	cur = L1.startPtr;
	while(cur != NULL)
	{
		if(cur->numAdjacent ==2)
		{
			insertInList(L, cur->component);
		}
		cur = cur->next;
	}
	killList2(&L1);

	#ifdef DEBUG
	printList(L);
	#endif
}

/******************************************************************************
** Function:	getSmallCompList
** Description: Function creates a list of components of size at most two in
**		<P>.
** Inputs:	L - empty list
**		info - holds information about P
**		size - number of vertices in original graph
** Return Value:
** Side Effects:Updates the contents of list to hold numbers of components
**		of size at most two.
** Created:	Kerri Morgan	21st September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void getSmallCompList(List* L, K_VertInfo* info, int size)
{
	int i;
	List2 L1;
	C_Node* cur = NULL;

	initList2(&L1);

	for(i=0; i<size; i++)
	{
		if(info[i].inP == TRUE)
		{
			insertInList2(&L1, info[i].component);
		}
	}

	cur = L1.startPtr;
	while(cur != NULL)
	{
		if(cur->numAdjacent <=2)
		{
			insertInList(L, cur->component);
		}
		cur = cur->next;
	}
	killList2(&L1);

	#ifdef DEBUG
	printList(L);
	#endif
}

/******************************************************************************
** Function:	mergeSmallComponents
** Description: Function finds any vertices in R that are only adjacent to
**		components of size at most two.  These are then added to P
** Inputs:	G - the graph
**		size - number of vertices in  the graph
**		info - information about P
**		R - list of vertices not in P
**		comp - next free component number
**		idx - indices into R
**		idxSize - size of idx
**		L - list of components of size at most 2
** Return Value: 1 if added a vertex to P, otherwise 0
** Side Effects:
** Created:	Kerri Morgan	21st September, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int mergeSmallComponents(Vertex* G, int size, K_VertInfo* info, int* R, int* comp, R_Index* idx, int idxSize, List* L)
{
	int set;
	int i,j;
	C_Node* cur= NULL;
	int flag=0;
	int v,w;

	for(set=0; set<idxSize; set++)
	{
		i=idx[set].start;
		while(i<=idx[set].finish)
		{
			v=R[i];
			if(info[v].inP == FALSE)
			{
				cur = info[v].adjNonTrees.startPtr;
				if(cur == NULL) /* else exists components
						   of size > 2 */
				{
					flag =1;
					cur =info[v].adjTrees.startPtr;
					while(cur != NULL)
					{
						if(inList(L, cur->component)== FALSE)
						{
							flag = 0;
							break;
						}	
						cur=cur->next;
					}
					if(flag ==1) /* all components of size <=2 */
					{
						G[v].member = TRUE;
						info[v].type = NON_TREE;
						info[v].inP = TRUE;
					
						info[v].component = *comp;
						*comp = (*comp)+1;
						for(j=0; j<G[v].degree; j++)
						{
							w=G[v].adjVerts[j];
							if((info[w].inP == TRUE) &&(info[w].component != info[v].component))
							{
								addToComponent(G,w, info, info[v].component);
							}
							else if(info[w].inP == FALSE)
							{
								/* v was not inP before so nothing to take away */
								insertInList2(&(info[w].adjNonTrees), info[v].component);
								info[w].Nn++;
							}
						}
						R[i]=R[idx[set].finish];
						idx[set].finish--;
						return 1;

					}

				}
			}
			i++;
		}
	}
	return 0;
}

/******************************************************************************
** Function:	addTree2NonTree
** Description: Function adds vertices that once belong to a tree to a
**		non-tree component.
** Inputs:	G - the graph
**		size - number of vertices in the graph
**		info - information about P
**		v - the vertex in tree component currently being added
**		compNum - the component number of the non-tree component
** Return Value:none
** Side Effects:updates info
** Created:	Kerri Morgan	13th March, 2006	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void addTree2NonTree(Vertex* G, int size, K_VertInfo* info, int v, int compNum)
{
	int i;
	int w;
	int oldNum;

	oldNum = info[v].component;

	if(info[v].component !=compNum)
	{
		info[v].type = NON_TREE;
		info[v].component = compNum;

		for(i=0; i<G[v].degree; i++)
		{
			w = G[v].adjVerts[i];
			if((info[w].inP== TRUE) && (info[w].component != compNum))
			{
				addTree2NonTree(G, size, info, w, compNum);
			}
			else if(info[w].inP == FALSE)
			{
				removeFromList2(&(info[w].adjTrees), oldNum);
				info[w].Nf--;
				insertInList2(&(info[w].adjNonTrees), compNum);
				info[w].Nn++;
			}
		}
	}
}


/******************************************************************************
** Function:	removeFromNonP
** Description: Function removes vertex from non-P and clears all lists of
**		adjacent vertices from info.
** Inputs:	info - information about P
**		v - the vertex 
**		compNum - the component number of the non-tree component
**		type - FOREST or NON_TREE
** Return Value:none
** Side Effects:updates info
** Created:	Kerri Morgan	14th March, 2006	
** Modified:
** Comments:	
*******************************************************************************/
void removeFromNonP(Vertex* G, K_VertInfo* info, int v, int compNum, int type)
{
	G[v].member = TRUE;
	info[v].type =type;
	info[v].inP = TRUE;
	info[v].component = compNum;

	/* clear non-P list */
	killList2(&(info[v].adjTrees));
	killList2(&(info[v].adjNonTrees));

	initList2(&(info[v].adjTrees));
	initList2(&(info[v].adjNonTrees));

	info[v].Nf=0;
	info[v].Nn=0;
}

/******************************************************************************
** Function:	removeFromP
** Description: Function removes vertex from P and clears all lists of
**		adjacent vertices from info.
** Inputs:	info - information about P
**		v - the vertex 
** Return Value:none
** Side Effects:updates info
** Created:	Kerri Morgan	14th March, 2006	
** Modified:
** Comments:	
*******************************************************************************/
void removeFromP(Vertex* G, K_VertInfo* info, int v, int* comp )
{
	int i;
	int w;
	int oldComponent;
	int edges, vertices;

	oldComponent = info[v].component;

	/* remove v from P */
	G[v].member = FALSE;
	info[v].inP = FALSE;
	info[v].component = -1;

	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		//printf("w is %d\n", w);
		/* nonP neighbours all now have one less P neighbour */
		if(info[w].inP==FALSE)
		{
			if(info[v].type == FOREST)
			{
				//printf("DECREMENT Nf and remove %d comp from tree\n", oldComponent);
				removeFromList2(&(info[w].adjTrees), oldComponent);
				info[w].Nf--;
			}
			else /* non tree */
			{
				//printf("DECREMENT Nn and remove %d comp from nontrees\n", oldComponent);
				removeFromList2(&(info[w].adjNonTrees), oldComponent); 
				info[w].Nn--;
			}
		}
		else /* this is a neighbour in the old component*/
		{
			/* if this vertex is still in the old component */
			if(info[w].component == oldComponent)
			{
				edges=0;
				vertices=1;
				updateRemoval( G,   info, w, *comp, &edges, &vertices);
				//insertInList2(&(info[v].adjNonTrees), *comp);
				//info[v].Nn++;
				/* this subcomponent is no longer a nonT, so must update
				** information for all vertices that are neighbours */
				if(edges+1 == vertices) 
				{
					alterCompType( G, info,  w);
				}
				*comp = (*comp)+1;
			} /* already assigned to the new component */
		}
	}

	/* clear non-P list */
	killList2(&(info[v].adjTrees));
	killList2(&(info[v].adjNonTrees));

	initList2(&(info[v].adjTrees));
	initList2(&(info[v].adjNonTrees));

	info[v].Nf=0;
	info[v].Nn=0;

	for(i=0; i<G[v].degree; i++)
	{
		w = G[v].adjVerts[i];
		if(info[w].inP==TRUE)
		{
			if(info[w].type == FOREST)
			{
				insertInList2(&(info[v].adjTrees), info[w].component);
				info[v].Nf++;
			}
			else
			{
				insertInList2(&(info[v].adjNonTrees), info[w].component);
				info[v].Nn++;
			}
		}
	}
	info[v].type =UNKNOWN;
}

/****************************************************************************
** Function:    updateRemoval
** Description: Function updates component(s) created when a vertex is
**		removed from a component.
** Inputs:      G - the graph
**              info - contains information about P
**              v - current vertex under consideration
**              num - new component number 
** Return Value:none
** Side Effects:updates lists of adjacent  components, updates lists of 
**		components
** Created:     Kerri Morgan    20th March, 2006
** Modified:
** Comments:
** Acknowledgements:
****************************************************************************/
void updateRemoval(Vertex* G,  K_VertInfo* info, int v, int num, int* edges, int* vertices)
{
	int i;
	int w;
	int old;

	old = info[v].component;
	info[v].component = num;
	for(i=0; i<G[v].degree; i++)
	{
		w=G[v].adjVerts[i];
		if((info[w].inP==TRUE) && (info[w].component != num))
		{
			*vertices = (*vertices)+1;
			updateRemoval(G, info, w, num, edges, vertices);
		}
		if((info[w].inP == TRUE) && (v>w))
		{
			*edges = (*edges+1);
		}
		if(info[w].inP !=TRUE)
		{
			insertInList2(&(info[w].adjNonTrees),num);
			removeFromList2(&(info[w].adjNonTrees), old);
		}
	}

}

/****************************************************************************
** Function:    updateNonTree
** Description: Function updates component to be considered part of 
**		component compNum.  The new component is a non-tree.
** Inputs:      G - the graph
**              size - number of vertices in graph
**              info - contains information about P
**              v - current vertex under consideration
**              compNum - component number of v
** Return Value:none
** Side Effects:updates lists of adjacent  components
** Created:     Kerri Morgan    14th March, 2006
** Modified:
** Comments:
** Acknowledgements:
****************************************************************************/
void updateNonTree(Vertex* G, int size, K_VertInfo* info, int v, int compNum)
{
	int i;
	int j;
	int w, x;
	int old;

	for(i=0; i<G[v].degree; i++)
        {
		w = G[v].adjVerts[i];
		if((info[w].inP == TRUE) && (info[w].component != compNum))
		{
			/* reset component number*/
			old = info[w].component;
			info[w].component= compNum;

			/* if the old component was a tree */
			if(info[w].type == FOREST)
			{
				info[w].type = NON_TREE;
				/* for each neighbour of this vertex
				* replace old component number with new
				* component number in  adjacency list*/
				for(j=0; j<G[w].degree; j++)
				{
					x=G[w].adjVerts[j];
					if(info[x].inP == FALSE)
					{
						removeFromList2(&info[x].adjTrees, old);
						insertInList2(&info[x].adjNonTrees, compNum);
						info[x].Nf--;
						info[x].Nn++;
					}
				}
			}
			else
			{
				/* for each neighbour of this vertex
				* replace old component number with new
				* component number in  adjacency list*/
				for(j=0; j<G[w].degree; j++)
				{
					x=G[w].adjVerts[j];
					if(info[x].inP==FALSE)
					{
						removeFromList2(&info[x].adjNonTrees, old);
						insertInList2(&info[x].adjNonTrees, compNum);
					}
				}
			}
			updateNonTree( G, size, info, w, compNum);
		}
       }
}

/****************************************************************************
** Function:  	checkUniquePaths 
** Description: Function checks each component containing neighbours of
**		vertex v.  If the neighbours of v in each component lie
**		on a unique path the function returns true.
** Inputs:      G - the graph
**              size - number of vertices in graph
**              info - contains information about P
**              v - current vertex under consideration
** Return Value:TRUE if all conditions are met, otherwise FALSE
** Side Effects:
** Created:     Kerri Morgan    15th March, 2006
** Modified: 
****************************************************************************/
int checkUniquePaths(Vertex* G, int size, K_VertInfo* info, int v)
{
	int compNum;
	C_Node* curNode=NULL;

	/*checking non-tree components */
	if(info[v].Nn != info[v].adjNonTrees.count)
	{
		curNode = info[v].adjNonTrees.startPtr;
		/* for each component*/
		while(curNode !=NULL)
		{
			/* if this component has more than two neighbours*/
			if(curNode->numAdjacent > 1)
			{
		compNum=curNode->component;
				if(checkUniquePathComp(G, size, info, v, compNum)== FALSE)
				{
					return FALSE;
				}
			}
			curNode = curNode->next;
		}
	}

	/* checking tree components */
	if(info[v].Nf != info[v].adjTrees.count)
	{
		curNode= info[v].adjTrees.startPtr;
		/* for each component*/
		while(curNode!=NULL)
		{
			/* if this component has more than three neighbours
			 * (note: a tree with two neighbours is always acceptable*/
			if(curNode->numAdjacent > 2)
			{
				compNum=curNode->component;
				if(checkUniquePathComp(G, size, info, v, compNum)== FALSE)
				{
					return FALSE;
				}
			}
			curNode = curNode->next;
		}
	}
	return TRUE;
}


/****************************************************************************
** Function:  	checkUniquePathComp 
** Description: Function checks a component containing neighbours of
**		vertex v.  If the neighbours of v in this component lie
**		on a unique path the function returns true.
** Inputs:      G - the graph
**              size - number of vertices in graph
**              info - contains information about P
**              v - current vertex under consideration
**		compNum - the component number
** Return Value:TRUE if all conditions are met, otherwise FALSE
** Side Effects:
** Created:     Kerri Morgan    15th March, 2006
** Modified: 
****************************************************************************/
int checkUniquePathComp(Vertex* G, int size, K_VertInfo* info, int v, int compNum)
{
	int i;
	int w1, w2, w;
	List L;

	initList(&L);
	w1=-1;
	w2=-1;
	
	for(i=0; i<G[v].degree; i++)
	{
		w= G[v].adjVerts[i];
		if((info[w].inP == TRUE) && (info[w].component== compNum))
		{
			if(w1 == -1)
			{
				insertInList(&L, w);
				w1 =w;
			}
			else if (w2 == -1)
			{
				w2=w;
				if(findKPath(G, size, info, w1, w2, &L) == FALSE)
				{
					killList(&L);
					return FALSE;
				}
			}
			else
			{
				if(inList(&L, w) == FALSE)
				{
					if(findKPath(G, size, info, w2, w, &L) == FALSE)
					{
						/* try extending the path from the other end */
						reverseList(&L);	
						/* no path either way */
						if(findKPath(G, size, info, w1, w, &L)== FALSE)
						{
							killList(&L);
							return FALSE;	
						}
						else
						{
							/* first element in the list is w2 and
							 * last is w*/
							w1=w2;
							w2=w;

						}
					}
					else
					{
						/* first element in path is w1 and last is w */
						w2=w;	
					}
				}
			}
		}
	}

	/* path is empty or consists of at most one vertex*/
	if(L.degree <=1 )
	{
		killList(&L);
		return TRUE;
	}

	if(info[w1].type == FOREST)
	{
		/* path must be unique */
		killList(&L);
		return TRUE;
	}
	else
	{
		/* check that the list is unique */
		/*  if it is return true else return false */
		if(uniqueKPath(G, size,info, w1, w2, &L)== TRUE)
		{
			killList(&L);
			return TRUE;	
		}
		else
		{
			killList(&L);
			return FALSE;
		}

	}

	killList(&L);
	return FALSE;
}
/****************************************************************************
** Function:  	locateDivergentVertex2
** Description: Function locates the first vertex w in a path w1, ..., w2
**		such that there exists a path w,..., w2
**		and w, ..., w2 is not a subsequence of w1, ..., w2.
** Inputs:      G - the graph
**		size - number of vertices in graph
**              info - contains information about P
**		v - the vertex we wish to add to component
** Return Value:w - if such a vertex exists, otherwise -1
** Side Effects:none
** Created:     Kerri Morgan    20th March, 2006
** Modified:    Modified version to recognise paths with only detours from
**		the endpoints
****************************************************************************/
int locateDivergentVertex2(Vertex* G, int size, K_VertInfo* info, int v)
{
	int w1=-1;
        int w2=-1;
	int w;
	int i;
	List path;

	if(info[v].Nn <2)
	{
		fprintf(stderr, "ERROR 14.8: Vertex %d is adjacent to insufficient vertices\n", v);
		exit(1);
	}

	for(i=0; i<G[v].degree; i++)
	{
		w = G[v].adjVerts[i];
		if((info[w].inP==TRUE) && (info[w].type == NON_TREE))
		{
			if(w1==-1)
			{
				w1 = w;
			}
			else
			{
				w2 =w;
				break;
			}
		
		
		}
	}
	if(w2==-1)
	{
		fprintf(stderr, "ERROR 14.8: Vertex %d is adjacent to insufficient vertices\n", v);
		exit(1);

	}
	initList(&path);
	insertAtEnd(&path, w1);
	if(findKPath( G, size,  info, w1, w2,  &path)==FALSE)
	{
		fprintf(stderr, "ERROR 14.9: Unable to find a path between %d and %d, neighbours of %din same component\n", w1, w2, v);
		exit(1);
	}
	printf("FOUND PATH\n");
	printList(&path);
	/* find the point at which the path diverges */
	w=locateDivergentPath2(G, size, info, w1, w2, &path);
	killList(&path);
	return w;
	
}
/****************************************************************************
** Function:  	locateDivergentPath2
** Description: Function locates the first vertex w in a path w1, ..., w2
**		such that there exists a path w,..., w2
**		and w, ..., w2 is not a subsequence of w1, ..., w2.
** Inputs:      G - the graph
**		size - number of vertices in graph
**              info - contains information about P
**              w1, w2 - start and end vertices in path
**		path  - path w1, ..., w2
** Return Value:w - if such a vertex exists, otherwise -1
** Side Effects:none
** Created:     Kerri Morgan    20th March, 2006
** Modified: 	locateDivergentPath2 to search only for paths that do not
**		begin and end at w1 and w2 respectively.
****************************************************************************/
int locateDivergentPath2(Vertex* G, int size, K_VertInfo* info, int w1, int w2, List* Path)
{
	Node* cur=NULL;
	int v;
	int w;
	int i;
	List tmpPath;
	int prev, next;
	int flag =0; /* indicates a second path was found */
	int start=0;
        int end=0;
	int endPath;


	//printList(Path);
	if(Path->degree <=2)
	{
		return -1;
	}
	
	prev =-1;
	cur=Path->startPtr;

	/* check each vertex in path to confirm that there is not another path */
	while(cur!=NULL)
	{
		if(cur->next !=NULL)
		{
			next = cur->next->vertex;
		}
		else
		{
			next =-1;
		}
	
		v=cur->vertex;

		if((v!=w1) && (v!=w2))
		{
			initList(&tmpPath);
			/* ignore paths that are little triangles */
			insertAtEnd(&tmpPath, prev);
			insertAtEnd(&tmpPath, next);

			insertAtEnd(&tmpPath, v);	
			for(i=0; i<G[v].degree; i++)
			{
				w=G[v].adjVerts[i];
				printf("CHECKING neighbour %d of vertex %d\n", w, v);
			
				/* check this vertex is not a predecessor or successor in
				 * path */
				if((w!=next) && (w!=prev))
				{
					printf("CHECKING VERTEX %d\n", w);
					/* once i have code to find a path to another vertex in
					 * rest of the path i'll replace this call for that*/
					insertAtEnd(&tmpPath, w);
					//printList(&tmpPath);
					endPath=findAPath(G,size,info,w, &tmpPath, Path);
					if(endPath > -1)
					{
						printf("FOUND ALT PATH to %d ... %d\n", v, endPath);
						if(flag == 0)
						{
							start = v;
							end =endPath;
							flag =1;
							/* can this vertex go anywhere else ? */
							killList(&tmpPath);
							insertAtEnd(&tmpPath, prev);
							insertAtEnd(&tmpPath, next);
							insertAtEnd(&tmpPath, v);
							insertAtEnd(&tmpPath, endPath);
							insertAtEnd(&tmpPath, w);
							if(findAPath(G,size, info, w, &tmpPath, Path) >-1)
							{
								printf("AND ANOTHER\n");
								killList(&tmpPath);
								return v;
							}
							else
							{
								removeFromList(&tmpPath, endPath);
								removeFromList(&tmpPath, w);
							}
						}
						else /* flag ==1 */
						{
							printf("PATH %d %d\n", v, endPath);
							printf("PATH %d %d\n", start, end);
							if((start !=v) && (start!=endPath))
							{
								killList(&tmpPath);
								return v;
							}
							else if((endPath!=end) && (endPath != start))
							{
								killList(&tmpPath);
								return v;
							}
						}
					}
					else
					{
						printf("FOUND NONE\n");
						/* error was here*/
						removeFromList(&tmpPath, w);
					}
				}
				
			}	
			killList(&tmpPath);
		}
		prev = v;
		cur = cur->next;	
	}
	/* no other paths are found */
	return -1;

}


/****************************************************************************
** Function:  	locateDivergentVertex
** Description: Function locates the first vertex w in a path w1, ..., w2
**		such that there exists a path w,..., w2
**		and w, ..., w2 is not a subsequence of w1, ..., w2.
** Inputs:      G - the graph
**		size - number of vertices in graph
**              info - contains information about P
**		v - the vertex we wish to add to component
** Return Value:w - if such a vertex exists, otherwise -1
** Side Effects:none
** Created:     Kerri Morgan    20th March, 2006
** Modified: 
****************************************************************************/
int locateDivergentVertex(Vertex* G, int size, K_VertInfo* info, int v)
{
	int w1=-1;
        int w2=-1;
	int w;
	int i;
	List path;

	if(info[v].Nn <2)
	{
		fprintf(stderr, "ERROR 14.8: Vertex %d is adjacent to insufficient vertices\n", v);
		exit(1);
	}

	for(i=0; i<G[v].degree; i++)
	{
		w = G[v].adjVerts[i];
		if((info[w].inP==TRUE) && (info[w].type == NON_TREE))
		{
			if(w1==-1)
			{
				w1 = w;
			}
			else
			{
				w2 =w;
				break;
			}
		
		
		}
	}
	if(w2==-1)
	{
		fprintf(stderr, "ERROR 14.8: Vertex %d is adjacent to insufficient vertices\n", v);
		exit(1);

	}
	initList(&path);
	insertAtEnd(&path, w1);
	if(findKPath( G, size,  info, w1, w2,  &path)==FALSE)
	{
		fprintf(stderr, "ERROR 14.9: Unable to find a path between %d and %d, neighbours of %din same component\n", w1, w2, v);
		exit(1);
	}
	/* find the point at which the path diverges */
	w=locateDivergentPath(G, size, info, w1, w2, &path);
	killList(&path);
	return w;
	
}
/****************************************************************************
** Function:  	locateDivergentPath
** Description: Function locates the first vertex w in a path w1, ..., w2
**		such that there exists a path w,..., w2
**		and w, ..., w2 is not a subsequence of w1, ..., w2.
** Inputs:      G - the graph
**		size - number of vertices in graph
**              info - contains information about P
**              w1, w2 - start and end vertices in path
**		path  - path w1, ..., w2
** Return Value:w - if such a vertex exists, otherwise -1
** Side Effects:none
** Created:     Kerri Morgan    20th March, 2006
** Modified: 
****************************************************************************/
int locateDivergentPath(Vertex* G, int size, K_VertInfo* info, int w1, int w2, List* Path)
{
	Node* cur=NULL;
	int v;
	int w;
	int i;
	List tmpPath;
	int prev, next;


	//printList(Path);
	if(Path->degree < 2)
	{
		return -1;
	}
	
	prev =-1;
	cur=Path->startPtr;

	/* check each vertex in path to confirm that there is not another path */
	while(cur!=NULL)
	{
		if(cur->next !=NULL)
		{
			next = cur->next->vertex;
		}
		else
		{
			next =-1;
		}
	
		v=cur->vertex;
		initList(&tmpPath);
		insertAtEnd(&tmpPath, v);	
		for(i=0; i<G[v].degree; i++)
		{
			w=G[v].adjVerts[i];
			
			/* check this vertex is not a predecessor or successor in
			 * path */
			if((w!=next) && (w!=prev))
			{
				/* another path exists */
				/* once i have code to find a path to another vertex in
				 * rest of the path i'll replace this call for that*/
				insertAtEnd(&tmpPath, w);
				//printList(&tmpPath);
				if(findKPath(G, size, info, w, w2, &tmpPath)==TRUE)
				{
					killList(&tmpPath);
					return v;
				}
				else
				{
					/* error was here */
					removeFromList(&tmpPath, w);
				}
			}
			
		}	
		killList(&tmpPath);
		prev = v;
		cur = cur->next;	
	
	}
	/* no other paths are found */
	return -1;

}



/****************************************************************************
** Function:  	uniqueKPath 
** Description: Function confirms if vertex w1 and w2 are connected via
**		only one path.
** Inputs:      G - the graph
**		size - number of vertices in graph
**              info - contains information about P
**              w1, w2 - start and end vertices in path
**		
**		path - (initially holds only w1) holds list of vertices on path
** Return Value:TRUE if path is the only path found between these vertices
**		else false
** Side Effects:none
** Created:     Kerri Morgan    18th March, 2006
** Modified: 
****************************************************************************/
int uniqueKPath(Vertex* G, int size, K_VertInfo* info, int w1, int w2, List* Path)
{
	Node* cur=NULL;
	int v;
	int w;
	int i;
	List tmpPath;
	int prev, next;


	//printList(Path);
	if(Path->degree < 2)
	{
		return TRUE;
	}
	
	prev =-1;
	cur=Path->startPtr;

	/* check each vertex in path to confirm that there is not another path */
	while(cur!=NULL)
	{
		if(cur->next !=NULL)
		{
			next = cur->next->vertex;
		}
		else
		{
			next =-1;
		}
	
		v=cur->vertex;
	
		initList(&tmpPath);
		insertAtEnd(&tmpPath, v);	
		for(i=0; i<G[v].degree; i++)
		{
			w=G[v].adjVerts[i];
			
			/* check this vertex is not a predecessor or successor in
			 * path */
			if((w!=next) && (w!=prev))
			{
				/* another path exists */
				/* once i have code to find a path to another vertex in
				 * rest of the path i'll replace this call for that*/
				insertAtEnd(&tmpPath, w);
				//printList(&tmpPath);
				if(findKPath(G, size, info, w, w2, &tmpPath)==TRUE)
				{
					killList(&tmpPath);
					return FALSE;
				}
				else
				{
					/* error was here */
					removeFromList(&tmpPath, w);
				}
			}
			
		}	
		killList(&tmpPath);
		prev = v;
		cur = cur->next;	
	}
	/* no other paths are found */
	return TRUE;

}

/****************************************************************************
** Function:  	findKPath 
** Description: Function finds a path from vertex w1 to vertex w2
** Inputs:      G - the graph
**              size - number of vertices in graph
**              info - contains information about P
**              w1, w2 - start and end vertices in path
**		path - (initially holds only w1) holds list of vertices on path
** Return Value:TRUE if a path is found otherwise false
** Side Effects:updates path
** Created:     Kerri Morgan    16th March, 2006
** Modified: 
****************************************************************************/
int findKPath(Vertex* G, int size, K_VertInfo* info, int w1, int w2, List* Path)
{
	int i;
	int w;

	/* check both vertices are in the same component */
	if(info[w1].component != info[w2].component)
	{
		return FALSE;
	}

	/* save component and adjacent vertices */
	if(adjacentVertices(G, w1, w2))
	{
		insertAtEnd(Path, w2);
		return TRUE;
	}

	for(i=0; i<G[w1].degree; i++)
	{
		w=G[w1].adjVerts[i];
		/* check not already in Path and is in same component*/
		if(info[w].inP == TRUE)
		{
			if((info[w1].component == info[w].component) && (inList(Path, w)==FALSE))
			{
				insertAtEnd(Path, w);
				if (findKPath( G, size,  info, w, w2,  Path)== TRUE)
				{
					return TRUE;
				}
				else
				{
					removeFromList(Path, w);
				}
			}
		}
	}
	return FALSE;
}


/****************************************************************************
** Function:  	findAPath 
** Description: Function finds a path from vertex w1 to a vertex in Path2
** Inputs:      G - the graph
**              size - number of vertices in graph
**              info - contains information about P
**              w1 - start and end vertices in path
**		path - (initially holds only w1) holds list of vertices on path
** Return Value:the first vertex on the Path2 that this Path meets.  If no such
**		vertex exists return -1
** Side Effects:updates path
** Created:     Kerri Morgan    12 April, 2006
** Modified: 
****************************************************************************/
int findAPath(Vertex* G, int size, K_VertInfo* info, int w1,  List* Path, List* Path2)
{
	int i;
	int w;
	int x;


	for(i=0; i<G[w1].degree; i++)
	{
		w=G[w1].adjVerts[i];
		
		/* check not already in Path and is in same component*/
		if(info[w].inP == TRUE)
		{
			if((info[w1].component == info[w].component) && (inList(Path, w)==FALSE))
			{
				if(inList(Path2, w)==TRUE)
				{
					/* found a way back to the initial path*/
					return w;
				}
				else
				{
					insertAtEnd(Path, w);
					x=findAPath(G,size, info,w, Path, Path2);
					if (x!=-1)
					{
						
						return x;
					}
					else
					{
						removeFromList(Path, w);
					}
				}
			}
		}
	}
	return -1;
}
