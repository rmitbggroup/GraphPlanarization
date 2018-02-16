#ifndef PLANARITY_H
#define PLANARITY_H

/*
Copyright (c) 1997-2015, John M. Boyer
All rights reserved.
See the LICENSE.TXT file for licensing information.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "graph.h"
#include "platformTime.h"

#include "graphK23Search.h"
#include "graphK33Search.h"
#include "graphK4Search.h"
#include "graphDrawPlanar.h"
int ReduceGraph(LGraph *G_A);
int removeMaximumDegree(LGraph *G_A);
void ProjectTitle();
int helpMessage(char *param);
int writeAdFile(LGraph *G_A);
int Optimization(graphP theGraph);
int insertEdges(LGraph *G_A);
int writeBaselineFile(LGraph *G_A);
/* Functions that call the Graph Library */
int SpecificGraph(char command, char *infileName, char *outfileName, char *outfile2Name);
LGraph* graphCopy(LGraph *G_A);
LGraph* IsolateForGreedy(int obstructList[], int obstructCount,const LGraph *orgAdjacent);
int drawGraph(char command, LGraph *G, char *outfileName, char *outfile2Name);
int RandomGraph(char command, int extraEdges, int numVertices, char *outfileName, char *outfile2Name);
int RandomGraphs(char command, int, int);
LGraph* contructAdList(char *Infile);

int makeg_main(char command, int argc, char *argv[]);

int optimizedPlanarization(graphP theGraph, int embedFlags);
LGraph* Isolate(graphP theGraph, LGraph *orgAdjacent);
int  newReadAdjList(graphP theGraph, LGraph *G);
LGraph* contructAdList(char *fileName);
/* Command line, Menu, and Configuration */
int commandLine(int argc, char *argv[]);
int legacyCommandLine(int argc, char *argv[]);
int PostProcess(LGraph *G_A);
int menu();
int test();
LGraph* Isolate2(int obstructList[], int obstructCount,LGraph *orgAdjacent);
LGraph* Isolate3(int obstructList[], int obstructCount,LGraph *orgAdjacent);
int NonAdjacent(int i,int j, LGraph G_A);

char Mode,
     OrigOut,
     EmbeddableOut,
     ObstructedOut,
     AdjListsForEmbeddingsOut,
     quietMode;

void Reconfigure();

/* Low-level Utilities */
#define MAXLINE 1024
char Line[MAXLINE];

void Message(char *message);
void ErrorMessage(char *message);
void FlushConsole(FILE *f);
void Prompt(char *message);

void SaveAsciiGraph(graphP theGraph, char *filename);

int  TextFilesEqual(char *file1Name, char *file2Name);

int GetEmbedFlags(char command);
char *GetAlgorithmName(char command);
void AttachAlgorithm(graphP theGraph, char command);

char *ConstructInputFilename(char *infileName);
char *ConstructPrimaryOutputFilename(char *infileName, char *outfileName, char command);
void WriteAlgorithmResults(graphP theGraph, int Result, char command, platform_time start, platform_time end, char *infileName);

#ifdef __cplusplus
}
#endif

#endif
