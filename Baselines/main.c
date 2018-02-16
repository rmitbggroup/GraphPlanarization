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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "runTests.h"
#include "linkList.h"
#include "rome.h"
#include "graph.h"
#include "menu.h"
#include "addVertexAlg.h"
#include "removeVertexAlg.h"
#include "graphGen.h"
#include "genRandGraph.h"
#include "bollobas.h"
#include "hybridAlg.h"
#include "tree.h"
#include "H_Lau.h"
#include "K_Alg.h"
#include "K2_Alg.h"
#include "linkList2.h"
#include "subsetRem.h"
#include "mixture.h"
#include "mixture2.h"

#define MAX 80
Vertex* G=NULL;
int main()
{
	char temp[MAX];
	int choice=1;
	int size;
	int degree;
	int P=0;
	int type;
	clock_t start, finish;
	srand48(time(NULL));
	printf("Welcome to Version I of Kerri's Project\n");

	while(choice != 0)
        {

		displayMenu();
		choice = getCommand();
		P=0;
		switch(choice)
		{
			case READ_GRAPH:
				/* if we have already something read in G */
				start=clock();
				if(G!=NULL)
				{
					killGraph(G, size);
					size=0;
				}
				G=readGraph(&size, &degree);
				if(G == NULL)
				{
					printf("Read from file failed.\n\n");
				}
				else
				{	printf("successful read!\n");
					//printGraph(G, size);
				}
				break;

				case	 REMOVE_VERT_PLANARISE:
				start=clock();
				if(G!=NULL)
				{
					initMembers(G, size);
					initAdjLists(G, size);
					P=removeVertAlg(G,size, 1);
					double percentage=(double)P/size;
					printf("the percentage is %.2f%% \n",percentage*100);
				}
				else
				{
					printf("No graph currently read in system to planarise\n");
				}
				break;
			
			case	SUBSET_REMOVE:
				start=clock();
				if(G!=NULL)
				{
					initMembers(G, size);
					initAdjLists(G, size);
					P=subsetRemove(G,size, 1);
					double percentage=(double)P/size;
					printf("the percentage is %.2f%% \n",percentage*100);
				}
				else
				{
					printf("No graph currently read in system to planarise\n");
				}
				break;

			case ADD_VERT_PLANARISE:
				start=clock();
				if(G!=NULL)
				{
					initMembers(G,size);
					/* save graph for testing*/	
					P=addVertAlg(G,size, 1);
					double percentage=(double)P/size;
					printf("the percentage is %.2f%% \n",percentage*100);
				}
				else
				{
					printf("No graph currently read in system to planarise\n");
				}
				break;

			case H_LAU:
				start=clock();
				if(G!=NULL)
				{
					initMembers(G, size);
					P=H_L_alg(G,size, 1);
					double percentage=(double)P/size;
					printf("the percentage is %.2f%% \n",percentage*100);
				}
				else
				{
					printf("No graph currently read in system to planarise\n");
				}

				break;


			default:
				break;
		}
			finish=clock();

			double time=(double)(finish-start) / CLOCKS_PER_SEC;

			printf("the running time is %f secs\n",time);

        }
	killGraph(G, size);
	return 0;
}
