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
#include "mixture2.h"
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
** Acknowledgements: The vertex algorithm is based on some notes by Keith
**		     Edwards.
******************************************************************************/
int mixture2_Alg(Vertex* G, int size, int save)
{
	int P=0;
	K_VertInfo* info=NULL;
	int* R= NULL;
	int Rsize;
	R_Index R_idx[1];
	int idxSize=1;
	int component =0;
	int i;

	/* do the hard work with the vertex subset removal algorithm */
	/* first run the vertex subset removal algorithm */
	P=subsetRemove(G, size, save);
	
	/* initialise information structure */
	free(info);
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

	//P=PT_2006_II(G, size, save, info, R, &component, R_idx, idxSize);
	//P+=growPalmTreesII(G,size, info, R, &component, R_idx, idxSize);
	P+=growPalmTreesIV(G,size, info, R, &component, R_idx, idxSize);
	//P=extendGraph(G,size, info, R, R_idx, idxSize, P, &component);
	for(i=0; i<size; i++)
	{
		killList2(&info[i].adjTrees);
		killList2(&info[i].adjNonTrees);
	}
	free(R);
	free(info);
	return P;
}

