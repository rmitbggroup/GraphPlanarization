/*
Copyright (c) 1997-2015, John M. Boyer
All rights reserved.
See the LICENSE.TXT file for licensing information.
*/

#include <stdlib.h>
#include <string.h>

#include "graph.h"

/* Private functions (exported to system) */

int  _ReadAdjMatrix(graphP theGraph, FILE *Infile);
int  _ReadAdjList(graphP theGraph, FILE *Infile);
int  _WriteAdjList(graphP theGraph, FILE *Outfile);
int  _WriteAdjMatrix(graphP theGraph, FILE *Outfile);
int  _WriteDebugInfo(graphP theGraph, FILE *Outfile);




/****************
 * extract obstructive vertices from the original adjacent list
 *******************/
LGraph* IsolateForGreedy(int obstructList[], int obstructCount,const LGraph *orgAdjacent){
	int N=orgAdjacent->vexnum;

		//printf("save %d vertices \n",orgAdjacent->vexnum-obstructCount);
		//orgAdjacent->percentage=1-(float)obstructCount/(orgAdjacent->vexnum);
		//printf("the percentage is %f \n",orgAdjacent->percentage);
		//orgAdjacent->vexnum=orgAdjacent->vexnum-obstructCount;

		int count=0;

		for(int i=1;i<=N;i++){
			if(obstructList[i]==1){

				count++;
				obstructList[i]=count;
				//orgAdjacent->vexs[i].data=-1;

			}
			else{
				obstructList[i]=-count;
			}
		}



		LGraph* G;
		 if ((G=(LGraph*)malloc(sizeof(LGraph))) == NULL )
		        return NULL;
		    memset(G, 0, sizeof(LGraph));

		G->vexnum=obstructCount;
		G->next_graph=NULL;
		G->edgeLimit=orgAdjacent->edgeLimit;
		G->vexs = (VNode*)malloc( (obstructCount+1)*sizeof(VNode) );

		LGraph* G2;
		 if ((G2=(LGraph*)malloc(sizeof(LGraph))) == NULL )
		        return NULL;
		    memset(G2, 0, sizeof(LGraph));

		G2->vexnum=orgAdjacent->vexnum-obstructCount;
		G2->next_graph=NULL;
		G2->edgeLimit=orgAdjacent->edgeLimit;
		G2->vexs = (VNode*)malloc( (orgAdjacent->vexnum+1)*sizeof(VNode) );
		G2->percentage=orgAdjacent->percentage;
		int index=1;
		int index2=1;
		for(int i=1;i<=N;i++){

			ENode *orgNode, *track, *node;
			orgNode=(ENode*)malloc(sizeof(ENode));
			orgNode=orgAdjacent->vexs[i].first_edge;
			track = (ENode*)malloc(sizeof(ENode));
			//this line is moved to a new adjacent list
			if(obstructList[i]>0){
				/*
				int degree=0;
				G->vexs[index].data=obstructList[i];
				G->vexs[index].color=0;
				G->vexs[index].first_edge=NULL;
				while(orgNode!=NULL){

					int value=obstructList[orgNode->ivex];

					if(value>0){
						degree++;
						node = (ENode*)malloc(sizeof(ENode));
						node->ivex=value;
						node->next_edge=NULL;

						if(G->vexs[index].first_edge==NULL){

							G->vexs[index].first_edge=node;
							track=node;
						}
						else{
							track->next_edge=node;
							track=node;
						}

					}

					orgNode=orgNode->next_edge;
				}

				G->vexs[index].degree=degree;
				index++;
				*/
			}
			//this line keep in the current list
			else{
	    			G2->vexs[index2].data=orgAdjacent->vexs[i].data+obstructList[i];
	    			//printf("index and data are %d and %d \n",index2,orgAdjacent->vexs[i].data);
	    			G2->vexs[index2].color=0;
	    			G2->vexs[index2].first_edge=NULL;
	    			G2->vexs[index2].degree=orgAdjacent->vexs[i].degree;
	    			ENode *track1=(ENode*)malloc(sizeof(ENode));


				//track=orgAdjacent->vexs[i].first_edge;
				int mark=0;
				while(orgNode!=NULL){

					int value=obstructList[orgNode->ivex];
					if(value>0){
						G2->vexs[index2].degree--;
					}
					else{
						ENode *temp=(ENode*)malloc(sizeof(ENode));
						temp->ivex=orgNode->ivex+value;
						temp->next_edge=NULL;
						if(G2->vexs[index2].first_edge==NULL){
							G2->vexs[index2].first_edge=temp;
							track1=temp;
						}
						else{
							track1->next_edge=temp;
							track1=temp;
						}

					}
					orgNode=orgNode->next_edge;

				}



				index2++;
			}


		}
	//*orgAdjacent=*G2;


	return G2;
}
// this method is designed for iterative planarization, return graph formed by obstructive vertices
LGraph* Isolate2(int obstructList[], int obstructCount,LGraph *orgAdjacent){
	int N=orgAdjacent->vexnum;
	//printf("the count is %d \n",obstructCount);
	int count=0;

	for(int i=1;i<=N;i++){
		if(obstructList[i]==1){

			count++;
			obstructList[i]=count;
			//orgAdjacent->vexs[i].data=-1;

		}
		else{
			obstructList[i]=-count;
		}
	}



	LGraph* G;
	 if ((G=(LGraph*)malloc(sizeof(LGraph))) == NULL )
	        return NULL;
	    memset(G, 0, sizeof(LGraph));

	G->vexnum=obstructCount;
	G->next_graph=NULL;
	G->edgeLimit=orgAdjacent->edgeLimit;
	G->vexs = (VNode*)malloc( (obstructCount+1)*sizeof(VNode) );

	LGraph* G2;
	 if ((G2=(LGraph*)malloc(sizeof(LGraph))) == NULL )
	        return NULL;
	    memset(G2, 0, sizeof(LGraph));
	int edgeCount=0;
	G2->vexnum=orgAdjacent->vexnum-obstructCount;
	G2->next_graph=NULL;
	G2->maximumDegree=0;
	G2->edgeLimit=orgAdjacent->edgeLimit;
	G2->vexs = (VNode*)malloc( (orgAdjacent->vexnum+1)*sizeof(VNode) );
	G2->percentage=(double)G2->vexnum/(orgAdjacent->vexnum);
	//printf("a is %d and b is %d \n",orgAdjacent->vexnum-obstructCount,G2->vexnum);
	int index=1;
	int index2=1;
	for(int i=1;i<=N;i++){

		ENode *orgNode, *track, *node;
		orgNode=(ENode*)malloc(sizeof(ENode));
		orgNode=orgAdjacent->vexs[i].first_edge;
		track = (ENode*)malloc(sizeof(ENode));
		//this line is moved to a new adjacent list
		if(obstructList[i]>0){
			int degree=0;
			G->vexs[index].data=obstructList[i];
			G->vexs[index].color=0;
			G->vexs[index].first_edge=NULL;
			while(orgNode!=NULL){

				int value=obstructList[orgNode->ivex];

				if(value>0){
					degree++;
					node = (ENode*)malloc(sizeof(ENode));
					node->ivex=value;
					node->next_edge=NULL;

					if(G->vexs[index].first_edge==NULL){

						G->vexs[index].first_edge=node;
						track=node;
					}
					else{
						track->next_edge=node;
						track=node;
					}

				}

				orgNode=orgNode->next_edge;
			}

			G->vexs[index].degree=degree;

			index++;
		}
		//this line keep in the current list
		else{
    			G2->vexs[index2].data=orgAdjacent->vexs[i].data+obstructList[i];
    			//printf("index and data are %d and %d \n",index2,orgAdjacent->vexs[i].data);
    			G2->vexs[index2].color=0;
    			G2->vexs[index2].first_edge=NULL;
    			G2->vexs[index2].degree=orgAdjacent->vexs[i].degree;
    			ENode *track1=(ENode*)malloc(sizeof(ENode));


			//track=orgAdjacent->vexs[i].first_edge;
			int mark=0;
			while(orgNode!=NULL){

				int value=obstructList[orgNode->ivex];
				if(value>0){
					G2->vexs[index2].degree--;
				}
				else{
					ENode *temp=(ENode*)malloc(sizeof(ENode));
					temp->ivex=orgNode->ivex+value;
					temp->next_edge=NULL;
					if(G2->vexs[index2].first_edge==NULL){
						G2->vexs[index2].first_edge=temp;
						track1=temp;
					}
					else{
						track1->next_edge=temp;
						track1=temp;
					}

				}
				orgNode=orgNode->next_edge;

			}

			edgeCount+=G2->vexs[index2].degree;
			if(G2->maximumDegree<G2->vexs[index2].degree)
				G2->maximumDegree=G2->vexs[index2].degree;
			index2++;
		}


	}
	G2->totalEdges=edgeCount/2;
	*orgAdjacent=*G2;


return G;
}

LGraph* Isolate3(int obstructList[], int obstructCount,LGraph *orgAdjacent){
	int N=orgAdjacent->vexnum;
	//printf("the count is %d \n",obstructCount);
	int count=0;

	for(int i=1;i<=N;i++){
		if(obstructList[i]==1){

			count++;
			obstructList[i]=count;
			//orgAdjacent->vexs[i].data=-1;

		}
		else{
			obstructList[i]=-count;
		}
	}



	LGraph* G;
	 if ((G=(LGraph*)malloc(sizeof(LGraph))) == NULL )
	        return NULL;
	    memset(G, 0, sizeof(LGraph));

	G->vexnum=obstructCount;
	G->next_graph=NULL;
	G->edgeLimit=orgAdjacent->edgeLimit;
	G->vexs = (VNode*)malloc( (obstructCount+1)*sizeof(VNode) );

	LGraph* G2;
	 if ((G2=(LGraph*)malloc(sizeof(LGraph))) == NULL )
	        return NULL;
	    memset(G2, 0, sizeof(LGraph));
	int edgeCount=0;
	G2->vexnum=orgAdjacent->vexnum-obstructCount;
	G2->next_graph=NULL;
	G2->maximumDegree=0;
	G2->edgeLimit=orgAdjacent->edgeLimit;
	G2->vexs = (VNode*)malloc( (orgAdjacent->vexnum+1)*sizeof(VNode) );
	G2->percentage=(double)G2->vexnum/(orgAdjacent->vexnum);
	//printf("a is %d and b is %d \n",orgAdjacent->vexnum-obstructCount,G2->vexnum);
	int index=1;
	int index2=1;
	for(int i=1;i<=N;i++){

		ENode *orgNode, *track, *node;
		orgNode=(ENode*)malloc(sizeof(ENode));
		orgNode=orgAdjacent->vexs[i].first_edge;
		track = (ENode*)malloc(sizeof(ENode));
		//this line is moved to a new adjacent list
		if(obstructList[i]>0){
			int degree=0;
			G->vexs[index].data=obstructList[i];
			G->vexs[index].color=0;
			G->vexs[index].first_edge=NULL;
			while(orgNode!=NULL){

				int value=obstructList[orgNode->ivex];

				if(value>0){
					degree++;
					node = (ENode*)malloc(sizeof(ENode));
					node->ivex=value;
					node->next_edge=NULL;

					if(G->vexs[index].first_edge==NULL){

						G->vexs[index].first_edge=node;
						track=node;
					}
					else{
						track->next_edge=node;
						track=node;
					}

				}

				orgNode=orgNode->next_edge;
			}

			G->vexs[index].degree=degree;

			index++;
		}
		//this line keep in the current list
		else{
    			G2->vexs[index2].data=orgAdjacent->vexs[i].data+obstructList[i];
    			//printf("index and data are %d and %d \n",index2,orgAdjacent->vexs[i].data);
    			G2->vexs[index2].color=0;
    			G2->vexs[index2].first_edge=NULL;
    			G2->vexs[index2].degree=orgAdjacent->vexs[i].degree;
    			ENode *track1=(ENode*)malloc(sizeof(ENode));


			//track=orgAdjacent->vexs[i].first_edge;
			int mark=0;
			while(orgNode!=NULL){

				int value=obstructList[orgNode->ivex];
				if(value>0){
					G2->vexs[index2].degree--;
				}
				else{
					ENode *temp=(ENode*)malloc(sizeof(ENode));
					temp->ivex=orgNode->ivex+value;
					temp->next_edge=NULL;
					if(G2->vexs[index2].first_edge==NULL){
						G2->vexs[index2].first_edge=temp;
						track1=temp;
					}
					else{
						track1->next_edge=temp;
						track1=temp;
					}

				}
				orgNode=orgNode->next_edge;

			}

			edgeCount+=G2->vexs[index2].degree;
			if(G2->maximumDegree<G2->vexs[index2].degree)
				G2->maximumDegree=G2->vexs[index2].degree;
			index2++;
		}


	}
	G2->totalEdges=edgeCount/2;



return G2;
}


/********************************************************************
 _ReadAdjMatrix()
 This function reads the undirected graph in upper triangular matrix format.
 Though O(N^2) time is required, this routine is useful during
 reliability testing due to the wealth of graph generating software
 that uses this format for output.
 Returns: OK, NOTOK on internal error, NONEMBEDDABLE if too many edges
 ********************************************************************/

int _ReadAdjMatrix(graphP theGraph, FILE *Infile)
{
	int N, v, w, Flag;

    if (Infile == NULL) return NOTOK;
    fscanf(Infile, " %d ", &N);
    if (gp_InitGraph(theGraph, N) != OK)
        return NOTOK;

    for (v = gp_GetFirstVertex(theGraph); gp_VertexInRange(theGraph, v); v++)
    {
         gp_SetVertexIndex(theGraph, v, v);
         for (w = v+1; gp_VertexInRange(theGraph, w); w++)
         {
              fscanf(Infile, " %1d", &Flag);
              if (Flag)
              {
                  if (gp_AddEdge(theGraph, v, 0, w, 0) != OK)
               	      return NOTOK;
              }
         }
    }

    return OK;
}

/********************************************************************
 _ReadAdjList()
 This function reads the graph in adjacency list format.

 The file format is
 On the first line    : N= number of vertices
 On N subsequent lines: #: a b c ... -1
 where # is a vertex number and a, b, c, ... are its neighbors.

 NOTE:  The vertex number is for file documentation only.  It is an
        error if the vertices are not in sorted order in the file.

 NOTE:  If a loop edge is found, it is ignored without error.

 NOTE:  This routine supports digraphs.  For a directed arc (v -> W),
        an edge record is created in both vertices, v and W, and the
        edge record in v's adjacency list is marked OUTONLY while the
        edge record in W's list is marked INONLY.
        This makes it easy to used edge directedness when appropriate
        but also seamlessly process the corresponding undirected graph.

 Returns: OK on success, NONEMBEDDABLE if success except too many edges
 	 	  NOTOK on file content error (or internal error)
 ********************************************************************/

int  _ReadAdjList(graphP theGraph, FILE *Infile)
{
     int N, v, W, adjList, e, indexValue, ErrorCode;
     int zeroBased = FALSE;

     if (Infile == NULL) return NOTOK;
     fgetc(Infile);                             /* Skip the N= */
     fgetc(Infile);
     fscanf(Infile, " %d ", &N);                /* Read N */
     if (gp_InitGraph(theGraph, N) != OK)
     {
    	  printf("Failed to init graph");
          return NOTOK;
     }

     theGraph->obstructList = (int*)malloc( N*sizeof(int) );
     //memset(theGraph->obstructList,0,theGraph->obstructList);
     theGraph->pertinentList = (int*)malloc( N*sizeof(int) );
     //memset(theGraph->pertinentList,0,theGraph->pertinentList);

     // Clear the visited members of the vertices so they can be used
     // during the adjacency list read operation
     for (v = gp_GetFirstVertex(theGraph); gp_VertexInRange(theGraph, v); v++){
          gp_SetVertexVisitedInfo(theGraph, v, NIL);

     }
     // Do the adjacency list read operation for each vertex in order
     for (v = gp_GetFirstVertex(theGraph); gp_VertexInRange(theGraph, v); v++)
     {
          // Read the vertex number
          fscanf(Infile, "%d", &indexValue);

          if (indexValue == 0 && v == gp_GetFirstVertex(theGraph))
        	  zeroBased = TRUE;
          indexValue += zeroBased ? gp_GetFirstVertex(theGraph) : 0;

          gp_SetVertexIndex(theGraph, v, indexValue);

          // The vertices are expected to be in numeric ascending order
          if (gp_GetVertexIndex(theGraph, v) != v)
        	  return NOTOK;

          // Skip the colon after the vertex number
          fgetc(Infile);

          // If the vertex already has a non-empty adjacency list, then it is
          // the result of adding edges during processing of preceding vertices.
          // The list is removed from the current vertex v and saved for use
          // during the read operation for v.  Adjacencies to preceding vertices
          // are pulled from this list, if present, or added as directed edges
          // if not.  Adjacencies to succeeding vertices are added as undirected
          // edges, and will be corrected later if the succeeding vertex does not
          // have the matching adjacency using the following mechanism.  After the
          // read operation for a vertex v, any adjacency nodes left in the saved
          // list are converted to directed edges from the preceding vertex to v.
          adjList = gp_GetFirstArc(theGraph, v);
          if (gp_IsArc(adjList))
          {
        	  // Store the adjacency node location in the visited member of each
        	  // of the preceding vertices to which v is adjacent so that we can
        	  // efficiently detect the adjacency during the read operation and
        	  // efficiently find the adjacency node.
        	  e = gp_GetFirstArc(theGraph, v);
			  while (gp_IsArc(e))
			  {
				  gp_SetVertexVisitedInfo(theGraph, gp_GetNeighbor(theGraph, e), e);
				  e = gp_GetNextArc(theGraph, e);
			  }

        	  // Make the adjacency list circular, for later ease of processing
			  gp_SetPrevArc(theGraph, adjList, gp_GetLastArc(theGraph, v));
			  gp_SetNextArc(theGraph, gp_GetLastArc(theGraph, v), adjList);

        	  // Remove the list from the vertex
			  gp_SetFirstArc(theGraph, v, NIL);
			  gp_SetLastArc(theGraph, v, NIL);
          }

          // Read the adjacency list.
          while (1)
          {
        	 // Read the value indicating the next adjacent vertex (or the list end)
             fscanf(Infile, " %d ", &W);
             W += zeroBased ? gp_GetFirstVertex(theGraph) : 0;

             // A value below the valid range indicates the adjacency list end
             if (W < gp_GetFirstVertex(theGraph))
            	 break;

             // A value above the valid range is an error
             if (W > gp_GetLastVertex(theGraph))
            	 return NOTOK;

             // Loop edges are not supported
             else if (W == v)
            	 return NOTOK;

             // If the adjacency is to a succeeding, higher numbered vertex,
             // then we'll add an undirected edge for now
             else if (v < W)
             {
             	 if ((ErrorCode = gp_AddEdge(theGraph, v, 0, W, 0)) != OK)
             		 return ErrorCode;
             }

             // If the adjacency is to a preceding, lower numbered vertex, then
             // we have to pull the adjacency node from the preexisting adjList,
             // if it is there, and if not then we have to add a directed edge.
             else
             {
            	 // If the adjacency node (arc) already exists, then we add it
            	 // as the new first arc of the vertex and delete it from adjList
            	 if (gp_IsArc(gp_GetVertexVisitedInfo(theGraph, W)))
            	 {
            		 e = gp_GetVertexVisitedInfo(theGraph, W);

            		 // Remove the arc e from the adjList construct
            		 gp_SetVertexVisitedInfo(theGraph, W, NIL);
            		 if (adjList == e)
            		 {
            			 if ((adjList = gp_GetNextArc(theGraph, e)) == e)
            				 adjList = NIL;
            		 }
            		 gp_SetPrevArc(theGraph, gp_GetNextArc(theGraph, e), gp_GetPrevArc(theGraph, e));
            		 gp_SetNextArc(theGraph, gp_GetPrevArc(theGraph, e), gp_GetNextArc(theGraph, e));

            		 gp_AttachFirstArc(theGraph, v, e);
            	 }

            	 // If an adjacency node to the lower numbered vertex W does not
            	 // already exist, then we make a new directed arc from the current
            	 // vertex v to W.
            	 else
            	 {
            		 // It is added as the new first arc in both vertices
                	 if ((ErrorCode = gp_AddEdge(theGraph, v, 0, W, 0)) != OK)
                		 return ErrorCode;

					 // Note that this call also sets OUTONLY on the twin arc
					 gp_SetDirection(theGraph, gp_GetFirstArc(theGraph, W), EDGEFLAG_DIRECTION_INONLY);
            	 }
             }
          }
          // If there are still adjList entries after the read operation
          // then those entries are not representative of full undirected edges.
          // Rather, they represent incoming directed arcs from other vertices
          // into vertex v. They need to be added back into v's adjacency list but
          // marked as "INONLY", while the twin is marked "OUTONLY" (by the same function).
          while (gp_IsArc(adjList))
          {
        	  e = adjList;

        	  gp_SetVertexVisitedInfo(theGraph, gp_GetNeighbor(theGraph, e), NIL);

 			  if ((adjList = gp_GetNextArc(theGraph, e)) == e)
 				  adjList = NIL;

     		  gp_SetPrevArc(theGraph, gp_GetNextArc(theGraph, e), gp_GetPrevArc(theGraph, e));
     		  gp_SetNextArc(theGraph, gp_GetPrevArc(theGraph, e), gp_GetNextArc(theGraph, e));

     		  gp_AttachFirstArc(theGraph, v, e);
     		  gp_SetDirection(theGraph, e, EDGEFLAG_DIRECTION_INONLY);
          }
     }

     if (zeroBased)
    	 theGraph->internalFlags |= FLAGS_ZEROBASEDIO;

     return OK;
}
//This method directly read the adjacent list.
int  newReadAdjList(graphP theGraph, LGraph *G)
{
     int N, v, W, adjList, e, indexValue, ErrorCode;
     int zeroBased = FALSE;

      N=G->vexnum;
     theGraph->arcCapacity=2*(G->edgeLimit)*N;

     //Message("-1 \n");
     if (gp_InitGraph(theGraph, N) != OK)
     {
    	  printf("Failed to init graph");
          return NOTOK;
     }
   //  printf("the memroy is %d \n",theGraph->N);
     //Message("0 \n");
     theGraph->obstructList = (int*)malloc( (N+1)*sizeof(int) );

     //memset(theGraph->obstructList,0,theGraph->obstructList);
     theGraph->pertinentList = (int*)malloc( (N+1)*sizeof(int) );
     //memset(theGraph->pertinentList,0,theGraph->pertinentList);
    // Message("1 \n");
     for(int i=0;i<=N;i++){
    	 	 theGraph->obstructList[i]=0;
    	 	theGraph->pertinentList[i]=0;
     }
     // Clear the visited members of the vertices so they can be used
     // during the adjacency list read operation
     //Message("2 \n");
     int pos=1;
     for (v = gp_GetFirstVertex(theGraph); gp_VertexInRange(theGraph, v); v++){
          gp_SetVertexVisitedInfo(theGraph, v, NIL);
          theGraph->V[v].noisiness=-1;
          //theGraph->VI[v].pNVCount=0;
          theGraph->V[v].stoppingCount=0;
          theGraph->V[v].originalIndex=v;

          theGraph->obstructCount=0;
          theGraph->minObstruct=theGraph->N;
     }
     //Message("3 \n");
     // Do the adjacency list read operation for each vertex in order
     pos=1;
     for (v = gp_GetFirstVertex(theGraph); gp_VertexInRange(theGraph, v); v++)
     {
          // Read the vertex number

          indexValue=G->vexs[pos].data;
          while(indexValue!=v){
        	  	  pos++;
        	  	indexValue=G->vexs[pos].data;
          }



          if (indexValue == 0 && v == gp_GetFirstVertex(theGraph))
        	  zeroBased = TRUE;
          indexValue += zeroBased ? gp_GetFirstVertex(theGraph) : 0;

          gp_SetVertexIndex(theGraph, v, indexValue);

          // The vertices are expected to be in numeric ascending order
          if (gp_GetVertexIndex(theGraph, v) != v)
        	  return NOTOK;

          // If the vertex already has a non-empty adjacency list, then it is
          // the result of adding edges during processing of preceding vertices.
          // The list is removed from the current vertex v and saved for use
          // during the read operation for v.  Adjacencies to preceding vertices
          // are pulled from this list, if present, or added as directed edges
          // if not.  Adjacencies to succeeding vertices are added as undirected
          // edges, and will be corrected later if the succeeding vertex does not
          // have the matching adjacency using the following mechanism.  After the
          // read operation for a vertex v, any adjacency nodes left in the saved
          // list are converted to directed edges from the preceding vertex to v.
          adjList = gp_GetFirstArc(theGraph, v);
          if (gp_IsArc(adjList))
          {
        	  // Store the adjacency node location in the visited member of each
        	  // of the preceding vertices to which v is adjacent so that we can
        	  // efficiently detect the adjacency during the read operation and
        	  // efficiently find the adjacency node.
        	  e = gp_GetFirstArc(theGraph, v);
			  while (gp_IsArc(e))
			  {
				  gp_SetVertexVisitedInfo(theGraph, gp_GetNeighbor(theGraph, e), e);
				  e = gp_GetNextArc(theGraph, e);
			  }

        	  // Make the adjacency list circular, for later ease of processing
			  gp_SetPrevArc(theGraph, adjList, gp_GetLastArc(theGraph, v));
			  gp_SetNextArc(theGraph, gp_GetLastArc(theGraph, v), adjList);

        	  // Remove the list from the vertex
			  gp_SetFirstArc(theGraph, v, NIL);
			  gp_SetLastArc(theGraph, v, NIL);
          }

  		ENode *node;


  		node=G->vexs[pos].first_edge;

          // Read the adjacency list.
          while (node!=NULL)
          {
        	 // Read the value indicating the next adjacent vertex (or the list end)
             W=node->ivex;

             node=node->next_edge;
             W += zeroBased ? gp_GetFirstVertex(theGraph) : 0;

             // A value below the valid range indicates the adjacency list end
             if (W < gp_GetFirstVertex(theGraph))
            	 break;

             // A value above the valid range is an error
             if (W > gp_GetLastVertex(theGraph)){
            	 printf("the current vertex is %d and the edge is %d \n",v,W);
            	 return NOTOK;
             }
             // Loop edges are not supported
             else if (W == v)
            	 return NOTOK;

             // If the adjacency is to a succeeding, higher numbered vertex,
             // then we'll add an undirected edge for now
             else if (v < W)
             {
             	 if ((ErrorCode = gp_AddEdge(theGraph, v, 0, W, 0)) != OK)
             		 return ErrorCode;
             }

             // If the adjacency is to a preceding, lower numbered vertex, then
             // we have to pull the adjacency node from the preexisting adjList,
             // if it is there, and if not then we have to add a directed edge.
             else
             {
            	 // If the adjacency node (arc) already exists, then we add it
            	 // as the new first arc of the vertex and delete it from adjList
            	 if (gp_IsArc(gp_GetVertexVisitedInfo(theGraph, W)))
            	 {
            		 e = gp_GetVertexVisitedInfo(theGraph, W);

            		 // Remove the arc e from the adjList construct
            		 gp_SetVertexVisitedInfo(theGraph, W, NIL);
            		 if (adjList == e)
            		 {
            			 if ((adjList = gp_GetNextArc(theGraph, e)) == e)
            				 adjList = NIL;
            		 }
            		 gp_SetPrevArc(theGraph, gp_GetNextArc(theGraph, e), gp_GetPrevArc(theGraph, e));
            		 gp_SetNextArc(theGraph, gp_GetPrevArc(theGraph, e), gp_GetNextArc(theGraph, e));

            		 gp_AttachFirstArc(theGraph, v, e);
            	 }

            	 // If an adjacency node to the lower numbered vertex W does not
            	 // already exist, then we make a new directed arc from the current
            	 // vertex v to W.
            	 else
            	 {
            		 // It is added as the new first arc in both vertices
                	 if ((ErrorCode = gp_AddEdge(theGraph, v, 0, W, 0)) != OK)
                		 return ErrorCode;

					 // Note that this call also sets OUTONLY on the twin arc
					 gp_SetDirection(theGraph, gp_GetFirstArc(theGraph, W), EDGEFLAG_DIRECTION_INONLY);
            	 }
             }

          }

          // If there are still adjList entries after the read operation
          // then those entries are not representative of full undirected edges.
          // Rather, they represent incoming directed arcs from other vertices
          // into vertex v. They need to be added back into v's adjacency list but
          // marked as "INONLY", while the twin is marked "OUTONLY" (by the same function).
          while (gp_IsArc(adjList))
          {
        	  e = adjList;

        	  gp_SetVertexVisitedInfo(theGraph, gp_GetNeighbor(theGraph, e), NIL);

 			  if ((adjList = gp_GetNextArc(theGraph, e)) == e)
 				  adjList = NIL;

     		  gp_SetPrevArc(theGraph, gp_GetNextArc(theGraph, e), gp_GetPrevArc(theGraph, e));
     		  gp_SetNextArc(theGraph, gp_GetPrevArc(theGraph, e), gp_GetNextArc(theGraph, e));

     		  gp_AttachFirstArc(theGraph, v, e);
     		  gp_SetDirection(theGraph, e, EDGEFLAG_DIRECTION_INONLY);
          }

     }

     if (zeroBased)
    	 theGraph->internalFlags |= FLAGS_ZEROBASEDIO;

     return OK;
}

/********************************************************************
 _ReadLEDAGraph()
 Reads the edge list from a LEDA file containing a simple undirected graph.
 LEDA files use a one-based numbering system, which is converted to
 zero-based numbers if the graph reports starting at zero as the first vertex.

 Returns: OK on success, NONEMBEDDABLE if success except too many edges
 	 	  NOTOK on file content error (or internal error)
 ********************************************************************/

int  _ReadLEDAGraph(graphP theGraph, FILE *Infile)
{
	char Line[256];
	int N, M, m, u, v, ErrorCode;
	int zeroBasedOffset = gp_GetFirstVertex(theGraph)==0 ? 1 : 0;

    /* Skip the lines that say LEDA.GRAPH and give the node and edge types */
    fgets(Line, 255, Infile);
    fgets(Line, 255, Infile);
    fgets(Line, 255, Infile);

    /* Read the number of vertices N, initialize the graph, then skip N. */
    fgets(Line, 255, Infile);
    sscanf(Line, " %d", &N);

    if (gp_InitGraph(theGraph, N) != OK)
         return NOTOK;

    for (v = gp_GetFirstVertex(theGraph); gp_VertexInRange(theGraph, v); v++)
        fgets(Line, 255, Infile);

    /* Read the number of edges */
    fgets(Line, 255, Infile);
    sscanf(Line, " %d", &M);

    /* Read and add each edge, omitting loops and parallel edges */
    for (m = 0; m < M; m++)
    {
        fgets(Line, 255, Infile);
        sscanf(Line, " %d %d", &u, &v);
        if (u != v && !gp_IsNeighbor(theGraph, u-zeroBasedOffset, v-zeroBasedOffset))
        {
             if ((ErrorCode = gp_AddEdge(theGraph, u-zeroBasedOffset, 0, v-zeroBasedOffset, 0)) != OK)
                 return ErrorCode;
        }
    }

    if (zeroBasedOffset)
    	theGraph->internalFlags |= FLAGS_ZEROBASEDIO;

    return OK;
}
LGraph* contructAdList(char *fileName){
	FILE *Infile;
	Infile = fopen(fileName, READTEXT);
	if(Infile == NULL)
	printf("Open file Error!");
	int N;
	int W;
	int E;
	int indexValue;
	ENode *node,*track;
	VNode *vnode,*vtrack;
	LGraph* G;

	 if ((G=(LGraph*)malloc(sizeof(LGraph))) == NULL )
	        return NULL;
	    memset(G, 0, sizeof(LGraph));

    if (Infile == NULL) return NOTOK;

    fgetc(Infile);                             /* Skip the N= */
    fgetc(Infile);
    fscanf(Infile, " %d ", &N);                /* Read N */

    fgetc(Infile);                             /* Skip the E= */
    fgetc(Infile);
    fscanf(Infile, " %d ", &E);
    G->vexnum=N;
    G->edgeLimit=E*2/N+1;
    G->next_graph=NULL;
    G->vexs = (VNode*)malloc( (N+1)*sizeof(VNode) );
    vnode = (VNode*)malloc(sizeof(VNode));
    	for(int i=1;i<=N;i++){
    		//printf("the current vertex is %d \n",i);
    		track = (ENode*)malloc(sizeof(ENode));
    		int degree=0;
    		if(track==NULL)
    			printf("memory required and the current vertex is %d \n",i);


            // Read the vertex number
            fscanf(Infile, "%d", &indexValue);

            G->vexs[i].data = indexValue;
            G->vexs[i].first_edge = NULL;

            // Skip the colon after the vertex number
            fgetc(Infile);

            // Read the adjacency list.
            while (1)
            {

          	 // Read the value indicating the next adjacent vertex (or the list end)
               fscanf(Infile, " %d ", &W);

               node = (ENode*)malloc(sizeof(ENode));
               if(node==NULL)
            	      Message("memory required \n");
               node->ivex=W;

               // A value below the valid range indicates the adjacency list end
               if (W < 1)
              	 break;
               if(W==G->vexs[i].data)
            	   	   continue;
               if(G->vexs[i].first_edge == NULL){
            	   	   G->vexs[i].first_edge=node;
            	   	   track=node;
               }
               else{
            	   	   track->next_edge=node;
            	   	   node->next_edge=NULL;
            	   	   track=node;
               }
               degree++;

            }
            G->vexs[i].degree=degree;

    	}
    	return G;
}
/********************************************************************
 gp_Read()
 Opens the given file, determines whether it is in adjacency list or
 matrix format based on whether the file start with N or just a number,
 calls the appropriate read function, then closes the file and returns
 the graph.

 Digraphs and loop edges are not supported in the adjacency matrix format,
 which is upper triangular.

 In the adjacency list format, digraphs are supported.  Loop edges are
 ignored without producing an error.

 Pass "stdin" for the FileName to read from the stdin stream

 Returns: OK, NOTOK on internal error, NONEMBEDDABLE if too many edges
 ********************************************************************/

int gp_Read(graphP theGraph, char *FileName)
{
FILE *Infile;
char Ch;
int RetVal;

     if (strcmp(FileName, "stdin") == 0)
          Infile = stdin;
     else if ((Infile = fopen(FileName, READTEXT)) == NULL)
          return NOTOK;

     Ch = (char) fgetc(Infile);
     ungetc(Ch, Infile);
     if (Ch == 'N')
          RetVal = _ReadAdjList(theGraph, Infile);
     else if (Ch == 'L')
          RetVal = _ReadLEDAGraph(theGraph, Infile);
     else RetVal = _ReadAdjMatrix(theGraph, Infile);

     if (RetVal == OK)
     {
         void *extraData = NULL;
         long filePos = ftell(Infile);
         long fileSize;

         fseek(Infile, 0, SEEK_END);
         fileSize = ftell(Infile);
         fseek(Infile, filePos, SEEK_SET);

         if (filePos < fileSize)
         {
            extraData = malloc(fileSize - filePos + 1);
            fread(extraData, fileSize - filePos, 1, Infile);
         }
/*// Useful for quick debugging of IO extensibility
         if (extraData == NULL)
             printf("extraData == NULL\n");
         else
         {
             char *extraDataString = (char *) extraData;
             extraDataString[fileSize - filePos] = '\0';
             printf("extraData = '%s'\n", extraDataString);
         }
*/

         if (extraData != NULL)
         {
             RetVal = theGraph->functions.fpReadPostprocess(theGraph, extraData, fileSize - filePos);
             free((void *) extraData);
         }
     }

     if (strcmp(FileName, "stdin") != 0)
         fclose(Infile);

     return RetVal;
}

int  _ReadPostprocess(graphP theGraph, void *extraData, long extraDataSize)
{
     return OK;
}

/********************************************************************
 _WriteAdjList()
 For each vertex, we write its number, a colon, the list of adjacent vertices,
 then a NIL.  The vertices occupy the first N positions of theGraph.  Each
 vertex is also has indicators of the first and last adjacency nodes (arcs)
 in its adjacency list.

 Returns: NOTOK if either param is NULL; OK otherwise (after printing
                adjacency list representation to Outfile).
 ********************************************************************/

int  _WriteAdjList(graphP theGraph, FILE *Outfile)
{
	 int v, e;
	 int zeroBasedOffset = (theGraph->internalFlags & FLAGS_ZEROBASEDIO) ? gp_GetFirstVertex(theGraph) : 0;

     if (theGraph==NULL || Outfile==NULL) return NOTOK;

     fprintf(Outfile, "N=%d\n", theGraph->N);
     for (v = gp_GetFirstVertex(theGraph); gp_VertexInRange(theGraph, v); v++)
     {
          fprintf(Outfile, "%d:", v - zeroBasedOffset);

          e = gp_GetLastArc(theGraph, v);
          while (gp_IsArc(e))
          {
        	  if (gp_GetDirection(theGraph, e) != EDGEFLAG_DIRECTION_INONLY)
                  fprintf(Outfile, " %d", gp_GetNeighbor(theGraph, e) - zeroBasedOffset);

              e = gp_GetPrevArc(theGraph, e);
          }

          // Write NIL at the end of the adjacency list (in zero-based I/O, NIL was -1)
          fprintf(Outfile, " %d\n", (theGraph->internalFlags & FLAGS_ZEROBASEDIO) ? -1 : NIL);
     }
     return OK;
}

/********************************************************************
 _WriteAdjMatrix()
 Outputs upper triangular matrix representation capable of being
 read by _ReadAdjMatrix()

 Note: This routine does not support digraphs and will return an
       error if a directed edge is found.

 returns OK for success, NOTOK for failure
 ********************************************************************/

int  _WriteAdjMatrix(graphP theGraph, FILE *Outfile)
{
int  v, e, K;
char *Row = NULL;

     if (theGraph != NULL)
         Row = (char *) malloc((theGraph->N+1)*sizeof(char));

     if (Row==NULL || theGraph==NULL || Outfile==NULL)
     {
         if (Row != NULL) free(Row);
         return NOTOK;
     }

     fprintf(Outfile, "%d\n", theGraph->N);
     for (v = gp_GetFirstVertex(theGraph); gp_VertexInRange(theGraph, v); v++)
     {
          for (K = gp_GetFirstVertex(theGraph); K <= v; K++)
               Row[K - gp_GetFirstVertex(theGraph)] = ' ';
          for (K = v+1; gp_VertexInRange(theGraph, v); K++)
               Row[K - gp_GetFirstVertex(theGraph)] = '0';

          e = gp_GetFirstArc(theGraph, v);
          while (gp_IsArc(e))
          {
        	  if (gp_GetDirection(theGraph, e) == EDGEFLAG_DIRECTION_INONLY)
        		  return NOTOK;

              if (gp_GetNeighbor(theGraph, e) > v)
                  Row[gp_GetNeighbor(theGraph, e) - gp_GetFirstVertex(theGraph)] = '1';

              e = gp_GetNextArc(theGraph, e);
          }

          Row[theGraph->N] = '\0';
          fprintf(Outfile, "%s\n", Row);
     }

     free(Row);
     return OK;
}

/********************************************************************
 ********************************************************************/

char _GetEdgeTypeChar(graphP theGraph, int e)
{
	char type = 'U';

	if (gp_GetEdgeType(theGraph, e) == EDGE_TYPE_CHILD)
		type = 'C';
	else if (gp_GetEdgeType(theGraph, e) == EDGE_TYPE_FORWARD)
		type = 'F';
	else if (gp_GetEdgeType(theGraph, e) == EDGE_TYPE_PARENT)
		type = 'P';
	else if (gp_GetEdgeType(theGraph, e) == EDGE_TYPE_BACK)
		type = 'B';
	else if (gp_GetEdgeType(theGraph, e) == EDGE_TYPE_RANDOMTREE)
		type = 'T';

	return type;
}

/********************************************************************
 ********************************************************************/

char _GetVertexObstructionTypeChar(graphP theGraph, int v)
{
	char type = 'U';

	if (gp_GetVertexObstructionType(theGraph, v) == VERTEX_OBSTRUCTIONTYPE_HIGH_RXW)
		type = 'X';
	else if (gp_GetVertexObstructionType(theGraph, v) == VERTEX_OBSTRUCTIONTYPE_LOW_RXW)
		type = 'x';
	if (gp_GetVertexObstructionType(theGraph, v) == VERTEX_OBSTRUCTIONTYPE_HIGH_RYW)
		type = 'Y';
	else if (gp_GetVertexObstructionType(theGraph, v) == VERTEX_OBSTRUCTIONTYPE_LOW_RYW)
		type = 'y';

	return type;
}

/********************************************************************
 _WriteDebugInfo()
 Writes adjacency list, but also includes the type value of each
 edge (e.g. is it DFS child  arc, forward arc or back arc?), and
 the L, A and DFSParent of each vertex.
 ********************************************************************/

int  _WriteDebugInfo(graphP theGraph, FILE *Outfile)
{
int v, e, EsizeOccupied;

     if (theGraph==NULL || Outfile==NULL) return NOTOK;

     /* Print parent copy vertices and their adjacency lists */

     fprintf(Outfile, "DEBUG N=%d M=%d\n", theGraph->N, theGraph->M);
     for (v = gp_GetFirstVertex(theGraph); gp_VertexInRange(theGraph, v); v++)
     {
          fprintf(Outfile, "%d(P=%d,lA=%d,LowPt=%d,v=%d):",
                             v, gp_GetVertexParent(theGraph, v),
                                gp_GetVertexLeastAncestor(theGraph, v),
                                gp_GetVertexLowpoint(theGraph, v),
                                gp_GetVertexIndex(theGraph, v));

          e = gp_GetFirstArc(theGraph, v);
          while (gp_IsArc(e))
          {
              fprintf(Outfile, " %d(e=%d)", gp_GetNeighbor(theGraph, e), e);
              e = gp_GetNextArc(theGraph, e);
          }

          fprintf(Outfile, " %d\n", NIL);
     }

     /* Print any root copy vertices and their adjacency lists */

     for (v = gp_GetFirstVirtualVertex(theGraph); gp_VirtualVertexInRange(theGraph, v); v++)
     {
          if (!gp_VirtualVertexInUse(theGraph, v))
              continue;

          fprintf(Outfile, "%d(copy of=%d, DFS child=%d):",
                           v, gp_GetVertexIndex(theGraph, v),
                           gp_GetDFSChildFromRoot(theGraph, v));

          e = gp_GetFirstArc(theGraph, v);
          while (gp_IsArc(e))
          {
              fprintf(Outfile, " %d(e=%d)", gp_GetNeighbor(theGraph, e), e);
              e = gp_GetNextArc(theGraph, e);
          }

          fprintf(Outfile, " %d\n", NIL);
     }

     /* Print information about vertices and root copy (virtual) vertices */
     fprintf(Outfile, "\nVERTEX INFORMATION\n");
     for (v = gp_GetFirstVertex(theGraph); gp_VertexInRange(theGraph, v); v++)
     {
         fprintf(Outfile, "V[%3d] index=%3d, type=%c, first arc=%3d, last arc=%3d\n",
                          v,
                          gp_GetVertexIndex(theGraph, v),
                          (gp_IsVirtualVertex(theGraph, v) ? 'X' : _GetVertexObstructionTypeChar(theGraph, v)),
                          gp_GetFirstArc(theGraph, v),
                          gp_GetLastArc(theGraph, v));
     }
     for (v = gp_GetFirstVirtualVertex(theGraph); gp_VirtualVertexInRange(theGraph, v); v++)
     {
         if (gp_VirtualVertexNotInUse(theGraph, v))
             continue;

         fprintf(Outfile, "V[%3d] index=%3d, type=%c, first arc=%3d, last arc=%3d\n",
                          v,
                          gp_GetVertexIndex(theGraph, v),
                          (gp_IsVirtualVertex(theGraph, v) ? 'X' : _GetVertexObstructionTypeChar(theGraph, v)),
                          gp_GetFirstArc(theGraph, v),
                          gp_GetLastArc(theGraph, v));
     }

     /* Print information about edges */

     fprintf(Outfile, "\nEDGE INFORMATION\n");
     EsizeOccupied = gp_EdgeInUseIndexBound(theGraph);
     for (e = gp_GetFirstEdge(theGraph); e < EsizeOccupied; e++)
     {
          if (gp_EdgeInUse(theGraph, e))
          {
              fprintf(Outfile, "E[%3d] neighbor=%3d, type=%c, next arc=%3d, prev arc=%3d\n",
                               e,
                               gp_GetNeighbor(theGraph, e),
                               _GetEdgeTypeChar(theGraph, e),
                               gp_GetNextArc(theGraph, e),
                               gp_GetPrevArc(theGraph, e));
          }
     }

     return OK;
}

/********************************************************************
 gp_Write()
 Writes theGraph into the file.
 Pass "stdout" or "stderr" to FileName to write to the corresponding stream
 Pass WRITE_ADJLIST, WRITE_ADJMATRIX or WRITE_DEBUGINFO for the Mode

 NOTE: For digraphs, it is an error to use a mode other than WRITE_ADJLIST

 Returns NOTOK on error, OK on success.
 ********************************************************************/

int  gp_Write(graphP theGraph, char *FileName, int Mode)
{
FILE *Outfile;
int RetVal;

     if (theGraph == NULL || FileName == NULL)
    	 return NOTOK;

     if (strcmp(FileName, "nullwrite") == 0)
    	  return OK;

     if (strcmp(FileName, "stdout") == 0)
          Outfile = stdout;
     else if (strcmp(FileName, "stderr") == 0)
          Outfile = stderr;
     else if ((Outfile = fopen(FileName, WRITETEXT)) == NULL)
          return NOTOK;

     switch (Mode)
     {
         case WRITE_ADJLIST   :
        	 RetVal = _WriteAdjList(theGraph, Outfile);
             break;
         case WRITE_ADJMATRIX :
        	 RetVal = _WriteAdjMatrix(theGraph, Outfile);
             break;
         case WRITE_DEBUGINFO :
        	 RetVal = _WriteDebugInfo(theGraph, Outfile);
             break;
         default :
        	 RetVal = NOTOK;
        	 break;
     }

     if (RetVal == OK)
     {
         void *extraData = NULL;
         long extraDataSize;

         RetVal = theGraph->functions.fpWritePostprocess(theGraph, &extraData, &extraDataSize);

         if (extraData != NULL)
         {
             if (!fwrite(extraData, extraDataSize, 1, Outfile))
                 RetVal = NOTOK;
             free(extraData);
         }
     }

     if (strcmp(FileName, "stdout") == 0 || strcmp(FileName, "stderr") == 0)
         fflush(Outfile);

     else if (fclose(Outfile) != 0)
         RetVal = NOTOK;

     return RetVal;
}

/********************************************************************
 _WritePostprocess()

 By default, no additional information is written.
 ********************************************************************/

int  _WritePostprocess(graphP theGraph, void **pExtraData, long *pExtraDataSize)
{
     return OK;
}

/********************************************************************
 _Log()

 When the project is compiled with LOGGING enabled, this method writes
 a string to the file PLANARITY.LOG in the current working directory.
 On first write, the file is created or cleared.
 Call this method with NULL to close the log file.
 ********************************************************************/

void _Log(char *Str)
{
static FILE *logfile = NULL;

    if (logfile == NULL)
    {
        if ((logfile = fopen("PLANARITY.LOG", WRITETEXT)) == NULL)
        	return;
    }

    if (Str != NULL)
    {
        fprintf(logfile, "%s", Str);
        fflush(logfile);
    }
    else
        fclose(logfile);
}

void _LogLine(char *Str)
{
	_Log(Str);
	_Log("\n");
}

static char LogStr[512];

char *_MakeLogStr1(char *format, int one)
{
	sprintf(LogStr, format, one);
	return LogStr;
}

char *_MakeLogStr2(char *format, int one, int two)
{
	sprintf(LogStr, format, one, two);
	return LogStr;
}

char *_MakeLogStr3(char *format, int one, int two, int three)
{
	sprintf(LogStr, format, one, two, three);
	return LogStr;
}

char *_MakeLogStr4(char *format, int one, int two, int three, int four)
{
	sprintf(LogStr, format, one, two, three, four);
	return LogStr;
}

char *_MakeLogStr5(char *format, int one, int two, int three, int four, int five)
{
	sprintf(LogStr, format, one, two, three, four, five);
	return LogStr;
}
