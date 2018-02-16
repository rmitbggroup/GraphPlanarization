#ifndef RUN_TEST_H
#define RUN_TEST_H

#include "graph.h"
#include "H_Lau.h"
#include "mixture.h"
#include "K_Alg.h"
#include "hybridAlg.h"
#include "subsetRem.h"
#include "mixture.h"
#define NUMTESTS 10
#define MILLION 1000000L

void createTestGraphs(int size, int degree, int method);
void runTestA(int size, int d);
void runTestB(int size, int d);
void runTests(void);
void runTest_OP_2006(char* filename, int n, int d, int num, char* method);
void runTest_palmTree_2006(char* filename, int n, int d, int num, char* method);
void runTest_removalAlg(char* filename, int n, int d, int num, char* method);
void runTest_additionAlg(char* filename, int n, int d, int num, char* method);
void runTest_palmTree(char* filename, int n, int d, int num, char* method);
void runTest_M_MIPS_III(char* filename, int n, int d, int num, char* method);
void runTest_H_L(char* filename, int n, int d, int num, char* method);
void runTest_Trees(char* filename, int n, int d, int num, char* method);
void runTest_Subset(char* filename, int n, int d, int num, char* method);
void runTest_M_MIPS(char* filename, int n, int d, int num, char* method);
void runTest_Hybrid(char* filename, int n, int d, int num, char* method);
void runTest_mixture(char* filename, int n, int d, int num, char* method);
void runTest_IndependentSet(char* filename, int n, int d, int num, char* method);
void runTest_Outerplanar(char* filename, int n, int d, int num, char* method);
void runTest_Mixture(char* filename, int n, int d, int num, char* method);
void runCPUTests(int n, int d, int method);
float timeGraphReading(int n, int d, int method);

#endif
