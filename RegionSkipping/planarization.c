#include "planarity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
int comparator(const void *a,const void *b){
	int l=((struct VNode *)a)->degree;
	int r=((struct VNode *)b)->degree;
	return (r-l);
}
int main(int argc, char *argv[])
{
	int choice=1;
	while(choice!=0){
		LGraph *G_A;
		char infileName[20];
		printf("Program starts: input the file name \n");
		scanf("%s",infileName);
		graphP theGraph;
		theGraph = gp_New();
		G_A=contructAdList(infileName);
		newReadAdjList(theGraph,G_A);
		PostProcess(G_A);

		printf("begin execution \n");

		Optimization(theGraph);

		printf("\n");
	  }
}
int removeMaximumDegree(LGraph *G_A){
	int N=G_A->vexnum;
	int *list = (int*)malloc( (N)*sizeof(int) );
	for(int i=0;i<=N-1;i++){
		 list[i]=i+1;
	}

	for(int i=-1;i<N-1;i++){

		for(int j=N-1;j>=i+1;j--){
			int a=G_A->vexs[list[j]].degree;
			int b=G_A->vexs[list[j+1]].degree;
			if(a<b){
				int temp=list[j];
				list[j]=list[j+1];
				list[j+1]=temp;
			}
		}

	}
	int pieces=20;
    int remove=N/pieces;
    for(int i=1;i<=10;i++){
    		LGraph *G_B;
    		int count=0;
		int *obstructList = (int*)malloc( (N+1)*sizeof(int) );
		for(int i=0;i<=N;i++){
			 obstructList[i]=0;
		}
    		int pos=i*remove/10;
    		for(int j=0;j<pos;j++){
    			obstructList[list[j]]=1;
    			count++;
    		}
    		G_B=Isolate3(obstructList,count,G_A);

    		int NB=G_B->vexnum;
    		int edgeB=G_B->totalEdges;


    					char *infileName=(char*)malloc( (100)*sizeof(char) );
    					char *infileName1=(char*)malloc( (100)*sizeof(char) );

    					sprintf(infileName,"%.1f", (double)i/pieces*10);
    					sprintf(infileName1,"%.1f.txt", (double)i/pieces*10);
    					FILE *fp;
    					FILE *fp1;
    					if((fp=fopen(infileName,"w"))==NULL)
    					{
    					      printf("can not open file\n");
    					}
    					if((fp1=fopen(infileName1,"w"))==NULL)
    					{
    						  printf("can not open file\n");
    					}

    					fprintf(fp,"%d\n",NB);
    					fprintf(fp,"%d\n",edgeB);
    					fprintf(fp1,"N=%d V=%d\n",NB,edgeB);
    					for(int i=1;i<=NB;i++){

    							fprintf(fp,"%d ",G_B->vexs[i].degree);
    							fprintf(fp1,"%d:",G_B->vexs[i].data);
    							ENode *node;
    							node=G_B->vexs[i].first_edge;
    							while(node!=NULL){
    								//printf("yes\n");
    								fprintf(fp,"%d ",(node->ivex-1));
    								fprintf(fp1,"%d ",(node->ivex));
    								node=node->next_edge;
    							}
    							fprintf(fp1,"0\n");
    							fprintf(fp,"\n");
    							fflush(fp);
    							fflush(fp1);
    					}

    }

	return 1;
}

int insertEdges(LGraph *G_A){
	double percent=1;
	int orgEdges=143904;
	for(int i=10;i<=10;i++){
		int extraEdges=(int)orgEdges*percent*0.01;

		int edgeCount=0;
		int lower=1;
		int upper=G_A->vexnum;

		while(edgeCount<=extraEdges){

			int node1 = lower + rand() % (upper - lower + 1);
			int node2 = lower + rand() % (upper - lower + 1);


			if(node1!=node2){
					ENode *node;
					int mark=0;
					node=G_A->vexs[node1].first_edge;
					while(node!=NULL){
						if(node->ivex==node2)
							mark=1;
						node=node->next_edge;
					}
					if(mark==1)
						continue;
					//printf("node1 is %d and node2 is %d\n",node1,node2);

					ENode *temp1 = (ENode*)malloc(sizeof(ENode));
					G_A->vexs[node1].degree++;
					G_A->vexs[node2].degree++;
					ENode *swap;
					swap=G_A->vexs[node1].first_edge;
					temp1->ivex=node2;
					temp1->next_edge=swap;
					G_A->vexs[node1].first_edge=temp1;


					temp1 = (ENode*)malloc(sizeof(ENode));
					swap=G_A->vexs[node2].first_edge;
					temp1->ivex=node1;
					temp1->next_edge=swap;
					G_A->vexs[node2].first_edge=temp1;


			}
			edgeCount++;

		}

		G_A->totalEdges+=extraEdges;
		G_A->edgeLimit=G_A->totalEdges*2/G_A->vexnum+1;

		int num=G_A->vexnum;
		int edge=G_A->totalEdges;
		char *infileName=(char*)malloc( (100)*sizeof(char) );
		char *infileName1=(char*)malloc( (100)*sizeof(char) );
		double output=(double)i/10;
		sprintf(infileName,"%.1f", output);
		sprintf(infileName1,"%.1f.txt", output);
		FILE *fp;
		FILE *fp1;
		if((fp=fopen(infileName,"w"))==NULL)
		{
		      printf("can not open file\n");
		      exit(0);
		}
		if((fp1=fopen(infileName1,"w"))==NULL)
		{
		      printf("can not open file\n");
		      exit(0);
		}
			fprintf(fp,"%d\n",num);
			fprintf(fp,"%d\n",edge);
			fprintf(fp1,"N=%d V=%d\n",num,edge);
	    for(int i=1;i<=num;i++){

	    		fprintf(fp,"%d ",G_A->vexs[i].degree);
	    		fprintf(fp1,"%d:",G_A->vexs[i].data);
	    		ENode *node;
	    		node=G_A->vexs[i].first_edge;
	    		while(node!=NULL){
	    			//printf("yes\n");
	    			fprintf(fp,"%d ",(node->ivex-1));
	    			fprintf(fp1,"%d ",(node->ivex));
	    			node=node->next_edge;
	    		}
	    		fprintf(fp1,"0\n");
	    		fprintf(fp,"\n");
	    		fflush(fp);
	    		fflush(fp1);
	    }

	}

	return 1;
}

int writeAdFile(LGraph *G_A){
	char *infileName=(char*)malloc( (100)*sizeof(char) );
	printf("input the writefile name \n");
	scanf("%s",infileName);
	int num=G_A->vexnum;
	int edge=G_A->totalEdges;
	FILE *fp;

	if((fp=fopen(infileName,"w"))==NULL)
	{
	      printf("can not open file\n");
	      exit(0);
	}
		fprintf(fp,"N=%d ",num);
		fprintf(fp,"V=%d\n",edge);
    for(int i=1;i<=num;i++){
    		//printf("iteration %d \n",i);
    		fprintf(fp,"%d: ",G_A->vexs[i].data);
    		//printf("degree is %d \n",G_A->vexs[i].degree);
    		ENode *node;
    		node=G_A->vexs[i].first_edge;
    		while(node!=NULL){
    			//printf("yes\n");
    			fprintf(fp,"%d ",(node->ivex));
    			node=node->next_edge;
    		}
    		fprintf(fp,"0\n");
    		fflush(fp);
    }


	return 1;
}



int writeBaselineFile(LGraph *G_A){
	char *infileName=(char*)malloc( (100)*sizeof(char) );
	printf("input the writefile name \n");
	scanf("%s",infileName);
	int num=G_A->vexnum;
	int edge=G_A->totalEdges;
	FILE *fp;

	if((fp=fopen(infileName,"w"))==NULL)
	{
	      printf("can not open file\n");
	      exit(0);
	}
		fprintf(fp,"%d\n",num);
		fprintf(fp,"%d\n",edge);
    for(int i=1;i<=num;i++){
    		//printf("iteration %d \n",i);
    		fprintf(fp,"%d ",G_A->vexs[i].degree);
    		//printf("degree is %d \n",G_A->vexs[i].degree);
    		ENode *node;
    		node=G_A->vexs[i].first_edge;
    		while(node!=NULL){
    			//printf("yes\n");
    			fprintf(fp,"%d ",(node->ivex-1));
    			node=node->next_edge;
    		}
    		fprintf(fp,"\n");
    		fflush(fp);
    }


	return 1;
}
int PostProcess(LGraph *G_A){
	int edge=0;
	for(int i=1;i<=G_A->vexnum;i++){

		ENode *enode= (ENode*)malloc(sizeof(ENode));
		enode=G_A->vexs[i].first_edge;

		int count=0;
		while(enode!=NULL){
			edge++;
			count++;
			enode=enode->next_edge;
		}
		G_A->vexs[i].degree=count;
		G_A->vexs[i].color=0;
	}
	edge=edge/2;
	G_A->totalEdges=edge;
	return 1;
}
int Optimization(graphP theGraph){
	clock_t start, finish;
	double runtime=0.0;
	int leftVertices=0;

    int total=theGraph->N;

    start=clock();
	optimizedPlanarization(theGraph, 1);
	finish=clock();
	runtime+=(double)(finish-start);
	leftVertices=total-theGraph->obstructCount;
	printf("the running time is %f secs\n",runtime/ CLOCKS_PER_SEC);
	printf("the percentage is %.2f%%\n",(double)leftVertices/total*100);


	return 1;
}

