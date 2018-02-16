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

#include "rome.h"
#include "K2_Alg.h"
#include "removeVertexAlg.h"
#include "addVertexAlg.h"
#include "genRandGraph.h"
#include "hybridAlg.h"
#include "graphGen.h"
#include "bollobas.h"
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 50

/******************************************************************************
** Function:	runRomeTests
** Description:	Functions coordinates the tests of approximation algorithms
**		on the Rome graph and records
**		the time taken and performance on the Rome graphs
** Inputs:	
** Return Value:none
** Side Effects:Outputs information to screen
** Created:	Kerri Morgan	12 January, 2007
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void runRomeTests(void)
{
	char filename[FILE_LEN];
	char num[FILE_LEN];
	int i;
	
	
	/* for each size of graphs */
//	for(i=10; i<101; i++)
	for(i=12; i<13; i++)
	{
		strcpy(filename, "ALF_CU/graphswith");
		sprintf(num, "%d", i);
		strcat(filename, num);
		strcat(filename, "nodes/TESTS/");
		printf("Test %s is filename\n", filename);
		runRomeI(filename, i);
		
	}
}

/******************************************************************************
** Function:	runRomeTestsI
** Description:	Functions coordinates the tests of approximation algorithms
**		on the Rome graph and records
**		the time taken and performance on the Rome graphs
**		based on average degree d
** Inputs:	
** Return Value:none
** Side Effects:Outputs information to screen
** Created:	Kerri Morgan	21 January, 2007
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void runRomeTestsI(void)
{
	char filename[FILE_LEN];
	char num[FILE_LEN];
	//int number;
	int i;
	int j;
	float k[23]={1.85, 1.95, 2.05, 2.15, 2.25, 2.35, 2.45, 2.55, 2.65, 2.75, 2.85, 2.95, 3.05, 3.15, 3.25, 3.35, 3.45, 3.55, 3.65, 3.75, 3.85, 3.95, 4.05};	
	
	/* for each size of graphs */
	i=0;
	while(i<23)
	{
		strcpy(filename, "ALF_CU/total_");
		sprintf(num, "%f", k[i]);
		j=0;
		while(num[j]!='0')
		{
			j++;
		}
		if(num[j+1]=='5')
		{
			j=j+2;
		}
		num[j]='\0';
		strcat(filename, num);
		printf("Test %s is filename\n", filename);
		runRomeII(filename, k[i]);
		i++;
	}
	
	
}
/******************************************************************************
** Function:	runRomeII
** Description:	Functions runs approximation algorithms on the graph and records
**		the time taken and performance on the Rome graphs stored in the
**		file (based on grouping of average degree d
** Inputs:	filename - name of file
**		d - all graphs have average degree less than d
** Return Value:none
** Side Effects:Outputs information to screen
** Created:	Kerri Morgan	21 January, 2007
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void runRomeII(char* filename, float d)
{
	int t;
	char str[10000];
	int s;
	int size;
	int n;
	//int num1=2000;
	int num; 
	char number[10];
	struct timeval startV, finishV;
	double start, finish;
	FILE* fileList = NULL;
	FILE* outfile = NULL;
	int i;
	int P=0;
	Vertex* G=NULL;
	char filename1[100];
	char filename2[100];
	char filename3[100];
	char filename4[100];
	char temp[10];
	double avgTime=0;
	double avgSize=0;
	double sizes[2000];
	double elapsed;
	double times[2000];
	
        double sum1=0.0;
	double sum2=0.0;

	FILE* rubbish;

	strcpy(filename2, "TEST_RESULTS_AVG_II/");
	sprintf(temp, "%f", d);
	s=0;
	while(temp[s]!='0')
	{
		s++;
	}
	if(temp[s+1]=='5')
	{
		s=s+2;
	}
	temp[s]='\0';
	strcat(filename2, temp);	
	/* CHANGE this line per algorithm */
	//strcat(filename2, "/INDEPENDENT_SET");
	//strcat(filename2, "/TREE");
	//strcat(filename2, "/HL");
	//strcat(filename2, "/OP1");
	//strcat(filename2, "/OP2");
	//strcat(filename2, "/PT");
	//strcat(filename2, "/VA");
	//strcat(filename2, "/VR");
	//strcat(filename2, "/VSR");
	//strcat(filename2, "/M_MIPS");
	//strcat(filename2, "/M_MIPS_III");
	strcat(filename2, "/Mixture");
        outfile= fopen(filename2, "w");
	if(outfile ==NULL)
	{
		fprintf(stderr, "ERROR 7.6: Unable to open %s\n", filename2);
		exit(1);
	}

	strcpy(filename1, filename);
	strcat(filename1, "test");
	fprintf(outfile, "TEST: Average Degree  is less than %f\n",d);
	/* CHANGE */
	//fprintf(outfile,  "Algorithm is IS Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is TREE Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is HL Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is OP1 Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is OP2 Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is PT Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is VA Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is VR Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is VSR Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is M_MIPS Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is M_MIPS_III Algorithm\n\n");
	fprintf(outfile,  "Algorithm is Mixture Algorithm\n\n");
	fprintf(outfile, "TIME(Microseconds):PERCENTAGE RETAINED:       SIZE OF P:\n");

	num=0;

	
	fileList = fopen(filename, "r");
	if(fileList == NULL)
	{
		fprintf(stderr, "Unable to open %s\n", filename);
		exit(1);
	}
	i=0;
	/* for each file in the list */
	while(fgets(str, 1000, fileList)!=NULL)
	{
		s=0;
		while(str[s]!=' ')
		{
			filename4[s]=str[s];
			s++;
		}
		filename4[s]='\0';
		strcpy(filename3, "ALF_CU/");
		strcat(filename3, filename4);
		rubbish =fopen(filename3, "r");
		if(rubbish==NULL)
		{
			fprintf(stderr, "Unable to open %s\n", filename3);
			exit(1);
		}
		else
		{
			num++;
			fclose(rubbish);
		}
		/* need to get the size of n*/
		s=0;
		while((str[s]<'0')|| (str[s]>'9'))
		{
			s++;
		}
		t=0;
		while(str[s]!='n')
		{
			number[t]=str[s];
			t++;
			number[t]='\0';
			s++;
		}
		sscanf(number, "%d", &n);
		size=n;
		G=initGraph(n);
		G = readGraphFromFile(filename3, n, 200);
		initMembers(G, n);
				               
	       	initAdjLists(G, n);
		start =0;
		finish=0;
		
		gettimeofday(&startV, NULL);
	        
	        /* CHANGE this liner per algorithm */	
		/* independent set */	    
	        //P=growCleverSingleTreeAlg(G, n, 0);
                //P=GrowCleverTrees(G, n, 0);
                //P=H_L_alg(G, n, 0);
		//P=CreateOPGraph(G,size, 0); /* OP1*/
		//P=PT_2006(G, size, 0); /* OP 2*/
	        //P=M_MIPS_II(G,size, 0); /* PT */
	        //P=addVertAlg(G,size, 0); /* VA */
	        //P=removeVertAlg(G,size, 0); /* VR */
	        //P=subsetRemove(G,size, 0); /* VSR */
	        //P=M_MIPS(G,size, 0); /* M_MIPS */
	        //P=M_MIPS_III(G,size, 0); /* M_MIPS_III */
	        P=mixture_Alg(G,size, 0); /* Mixture */

	   	gettimeofday(&finishV, NULL);
		start =(double)startV.tv_sec * 1000000 + startV.tv_usec;
		finish =(double)finishV.tv_sec * 1000000 + finishV.tv_usec;

                elapsed = finish -start;

		/* adding proportions */
		sizes[i]=(double)P/n;
		times[i]=elapsed/n;
		avgTime+=elapsed/n;
		avgSize+=(double)P/n;
		fprintf(outfile, "%.2f \t\t", elapsed);
		fprintf(outfile, "%.8f\t", (float)P*100.0/n);
		fprintf(outfile, "%d\n", P);
		killGraph(G, n);
		i++;
		
	}

	/* get mean and standard deviation*/
	fprintf(outfile, "Number of files is %d\n", num);
	avgTime=avgTime/num;
	fprintf(outfile, "Mean Time: %.2f microseconds per vertex\n", avgTime);
        avgSize = avgSize/num;
	fprintf(outfile, "Mean Size: %.2f\n", avgSize);
	printf("TEST %d is num\n", num);
		printf("TEST %f is avg Size and %f is avg elapsed\n", avgSize, avgTime);
	for(i=0; i<num; i++)
	{
		sum1+=pow(times[i]-avgTime, 2);
		sum2+=pow(sizes[i]-avgSize, 2);
	}
	fprintf(outfile, "Std.Dev. Time: %.2f microseconds\n", sqrt(sum1/(num-1)));
	fprintf(outfile, "Std.Dev. Size: %.2f\n", sqrt(sum2/(num-1)));
	printf("%.2f (%.2f)\t%.2f (%.2f)\n", avgSize, sqrt(sum2/(num-1)), avgTime, sqrt(sum1/(num-1)));
	fclose(outfile);
}
	

/******************************************************************************
** Function:	runRomeI
** Description:	Functions runs approximation algorithms on the graph and records
**		the time taken and performance on the Rome graphs stored in the
**		file
** Inputs:	filename - name of file
**		size - number of vertices in graphs
** Return Value:none
** Side Effects:Outputs information to screen
** Created:	Kerri Morgan	12 January, 2007
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void runRomeI(char* filename, int size)
{
	int n=size;
	int num1=2000;
	int num; 
	struct timeval startV, finishV;
	double start, finish;
	FILE* outfile = NULL;
	int i;
	int P=0;
	Vertex* G=NULL;
	char filename1[100];
	char filename2[100];
	char filename3[100];
	char temp[10];
	double avgTime=0;
	double avgSize=0;
	double sizes[2000];
	double elapsed;
	double times[2000];
	
        double sum1=0.0;
	double sum2=0.0;

	FILE* rubbish;

	strcpy(filename2, "TEST_RESULTS_AGAIN/");
	sprintf(temp, "%d", size);
	strcat(filename2, temp);	
	/* This is the current test*/
	/* change this line per algorithm */
	//strcat(filename2, "/INDEPENDENT_SET");
	//strcat(filename2, "/TREE");
	//strcat(filename2, "/HL");
	//strcat(filename2, "/OP1");
	//strcat(filename2, "/OP2");
	//strcat(filename2, "/PT");
	strcat(filename2, "/VA");
	//strcat(filename2, "/VR");
	//strcat(filename2, "/VSR");
	//strcat(filename2, "/M_MIPS");
	//strcat(filename2, "/M_MIPS_III");
	//strcat(filename2, "/Mixture");
        outfile= fopen(filename2, "w");
	if(outfile ==NULL)
	{
		fprintf(stderr, "ERROR 7.6: Unable to open %s\n", filename2);
		exit(1);
	}

	strcpy(filename1, filename);
	strcat(filename1, "test");
//	printf("TEST: size is %d", size);
	fprintf(outfile, "TEST: size is %d\n", size);
	/* change */
	/* This is the current test*/
	//fprintf(outfile,  "Algorithm is IS Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is TREE Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is HL Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is OP1 Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is OP2 Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is PT Algorithm\n\n");
	fprintf(outfile,  "Algorithm is VA Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is VR Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is VSR Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is M_MIPS Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is M_MIPS_III Algorithm\n\n");
	//fprintf(outfile,  "Algorithm is Mixture Algorithm\n\n");
	fprintf(outfile, "TIME(Microseconds):PERCENTAGE RETAINED:       SIZE OF P:\n");

	num=0;
	/* for each test graph*/
	for(i=0; i<num1; i++)
	{
                strcpy(filename3, filename1); /* filename/TEST_*/
                sprintf(temp, "%d", i);
	        strcat(filename3, temp);
	/* check if file exists*/
		rubbish =fopen(filename3, "r");
		if(rubbish==NULL)
		{
			break;
		}
		else
		{
			num=i+1;
			fclose(rubbish);
		}
	
		G = readGraphFromFile(filename3, n, 20);
		initMembers(G, n);
				               
	       	initAdjLists(G, n);
		start =0;
		finish=0;
		
		gettimeofday(&startV, NULL);
	        
	/* This is the current test*/
	        /* change this liner per algorithm */	
		/* independent set */	    
	        //P=growCleverSingleTreeAlg(G, n, 0);
                //P=GrowCleverTrees(G, n, 0);
                //P=H_L_alg(G, n, 0);
		//P=CreateOPGraph(G,size, 0); /* OP1*/
		//P=PT_2006(G, size, 0); /* OP 2*/
	        //P=M_MIPS_II(G,size, 0); /* PT */
	        P=addVertAlg(G,size, 0); /* VA */
	        //P=removeVertAlg(G,size, 0); /* VR */
	        //P=subsetRemove(G,size, 0); /* VSR */
	        //P=M_MIPS(G,size, 0); /* M_MIPS */
	        //P=M_MIPS_III(G,size, 0); /* M_MIPS_III */
	        //P=mixture_Alg(G,size, 0); /* Mixture */

	   	gettimeofday(&finishV, NULL);
		start =(double)startV.tv_sec * 1000000 + startV.tv_usec;
		finish =(double)finishV.tv_sec * 1000000 + finishV.tv_usec;

                elapsed = finish -start;

       		sizes[i]=P;
		times[i]=elapsed;
		avgTime+=elapsed;
		avgSize+=(double)P;
		fprintf(outfile, "%.2f \t\t", elapsed);
		fprintf(outfile, "%.8f\t", (float)P*100.0/n);
		fprintf(outfile, "%d\n", P);

		killGraph(G, n);
	}

	/* get mean and standard deviation*/
	avgTime=avgTime/num;
	fprintf(outfile, "Mean Time: %.2f microseconds\n", avgTime);
        avgSize = avgSize/num;
	fprintf(outfile, "Mean Size: %.2f\n", avgSize);
	for(i=0; i<num; i++)
	{
		sum1+=pow(times[i]-avgTime, 2);
		sum2+=pow(sizes[i]-avgSize, 2);
	}
	fprintf(outfile, "Std.Dev. Time: %.2f microseconds\n", sqrt(sum1/(num-1)));
	fprintf(outfile, "Std.Dev. Size: %.2f\n", sqrt(sum2/(num-1)));
	printf("%.2f (%.2f)\t%.2f (%.2f)\n", avgSize, sqrt(sum2/(num-1)), avgTime, sqrt(sum1/(num-1)));
	fclose(outfile);
}
	
