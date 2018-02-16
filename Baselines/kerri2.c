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
#include <string.h>
#include <stdlib.h>

#define numCols 12
#define numRows 100
//#define numRows 19
int main()
{
	int i,j,k,m;
	FILE* infile;
	char filename[1000];
	char num[10];
	char temp[1000];
	char number[19][10];
	float n;

	strcpy(number[0], "1.85");
	strcpy(number[1], "1.95");
	strcpy(number[2], "2.05");
	strcpy(number[3], "2.15");
	strcpy(number[4], "2.25");
	strcpy(number[5], "2.35");
	strcpy(number[6], "2.45");
	strcpy(number[7], "2.55");
	strcpy(number[8], "2.65");
	strcpy(number[9], "2.75");
	strcpy(number[10], "2.85");
	strcpy(number[11], "2.95");
	strcpy(number[12], "3.05");
	strcpy(number[13], "3.15");
	strcpy(number[14], "3.25");
	strcpy(number[15], "3.35");
	strcpy(number[16], "3.45");
	strcpy(number[17], "3.55");
	strcpy(number[18], "3.65");

	printf("MMIPS MMIPSIII Mixture VSR VR VA PT OP1 OP2 T IS HL\n");
	//printf("IS T HL OP1 OP2 PT VA VR VSR M_MIPS M_MIPS_III Mixture\n");
	/* each row is a test*/
	for(i=10; i<=numRows; i++)
//	for(i=0; i<numRows; i++)
	{
		/* put a horizontal line */
		printf("\\hline\n");
		printf("%d ", i);
//		printf("%s ", number[i]);
		/* for each algorithm */
		for(j=0; j<numCols; j++)
		{
			//strcpy(filename, "TEST_RESULTS_AVG/");
			strcpy(filename, "TEST_RESULTS_AGAIN/");
			sprintf(num, "%d", i);
			strcat(filename, num);
		//	strcat(filename, number[i]);
			strcat(filename, "/");
			if(j==0)
			{
				strcat(filename, "M_MIPS");
			}
			if(j==1)
			{
				strcat(filename, "M_MIPS_III");
			}
			if(j==2)
			{
				strcat(filename, "Mixture");
			}
			if(j==3)
			{
				strcat(filename, "VSR");
			}
			if(j==4)
			{
				strcat(filename, "VR");
			}
			if(j==5)
			{
				strcat(filename, "VA");
			}
			if(j==6)
			{
				strcat(filename, "PT");
			}
			if(j==7)
			{
				strcat(filename, "OP1");
			}
			if(j==8)
			{
				strcat(filename, "OP2");
			}
			if(j==9)
			{
				strcat(filename, "TREE");
			}
			if(j==10)
			{
				strcat(filename, "INDEPENDENT_SET");
			}
			if(j==11)
			{
				strcat(filename, "HL");
			}

			infile=fopen(filename, "r");
			if(infile==NULL)
			{
				fprintf(stderr, "Unable to open %s\n", filename);
				exit(1);
			}
			while(fgets(temp, 1000, infile)!=NULL)
			{
				if(strncmp(temp, "Mean Time", 9)==0)
				//if(strncmp(temp, "Mean Size", 9)==0)
				{
					k=0;
					while(temp[k]!=':')
					{
						k++;
					}
					/* eat up space and colon*/
					k++;
					k++;
					m=0;
					while((temp[k]!=' ') && (temp[k]!='\n'))
					{
						num[m]=temp[k];
						m++;
						k++;
					}
					num[m]='\0';
					sscanf(num, "%f", &n);
					//printf(" %s ", num);
					printf("& %s ", num);
					//printf("& %f ", n/i);
					//printf(" %f ", n/i);
					break;
				}
			}
			fclose(infile);
		}
		printf("\\\\\n");
		
		/* this is not done for proportions */
		/* for each algorithm */
		for(j=0; j<numCols; j++)
		{
			//break; /* remove for standard deviations*/
			//strcpy(filename, "TEST_RESULTS_AVG/");
			strcpy(filename, "TEST_RESULTS_AGAIN/");
			sprintf(num, "%d", i);
			strcat(filename, num);
		//	strcat(filename, number[i]);
			strcat(filename, "/");
			if(j==0)
			{
				strcat(filename, "M_MIPS");
			}
			if(j==1)
			{
				strcat(filename, "M_MIPS_III");
			}
			if(j==2)
			{
				strcat(filename, "Mixture");
			}
			if(j==3)
			{
				strcat(filename, "VSR");
			}
			if(j==4)
			{
				strcat(filename, "VR");
			}
			if(j==5)
			{
				strcat(filename, "VA");
			}
			if(j==6)
			{
				strcat(filename, "PT");
			}
			if(j==7)
			{
				strcat(filename, "OP1");
			}
			if(j==8)
			{
				strcat(filename, "OP2");
			}
			if(j==9)
			{
				strcat(filename, "TREE");
			}
			if(j==10)
			{
				strcat(filename, "INDEPENDENT_SET");
			}
			if(j==11)
			{
				strcat(filename, "HL");
			}

			infile=fopen(filename, "r");
			if(infile==NULL)
			{
				fprintf(stderr, "Unable to open %s\n", filename);
				exit(1);
			}
			while(fgets(temp, 1000, infile)!=NULL)
			{
				if(strncmp(temp, "Std.Dev. Time", 13)==0)
				//if(strncmp(temp, "Std.Dev. Size", 13)==0)
				{
					k=0;
					while(temp[k]!=':')
					{
						k++;
					}
					/* eat up space and colon */
					k++;
					k++;
					m=0;
					while((temp[k]!=' ') && (temp[k]!='\n'))
					{
						num[m]=temp[k];
						m++;
						k++;
					}
					num[m]='\0';
					printf("& (\\textit{%s}) ", num);
					break;
				}
			}
			fclose(infile);
		}
		printf("\\\\\n");
	}

}
