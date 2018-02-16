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
#define FILE_LEN 100

void runRomeTests(void);
void runRomeTestsI(void);
//void runRomeTestsI(void)
void runRomeI(char* filename, int size);
void runRomeII(char* filename, float d);

#endif
