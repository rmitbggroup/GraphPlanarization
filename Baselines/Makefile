#CFLAGS = -g -Wall -pg
#CFLAGS =   -Wall 
CFLAGS = -g  -Wall 
#CFLAGS = -D DEBUG -g -Wall

planarise:   main.o linkList.o node.o menu.o graph.o addVertexAlg.o subsetRem.o removeVertexAlg.o graphGen.o genRandGraph.o runTests.o bollobas.o hybridAlg.o tree.o H_Lau.o K_Alg.o K2_Alg.o linkList2.o node2.o mixture.o mixture2.o rome.o
	gcc $(CFLAGS) -o planarise main.o linkList.o node.o menu.o graph.o addVertexAlg.o subsetRem.o removeVertexAlg.o graphGen.o genRandGraph.o runTests.o bollobas.o hybridAlg.o tree.o H_Lau.o K2_Alg.o K_Alg.o linkList2.o node2.o mixture.o mixture2.o rome.o -lm

main.o: main.c 
	gcc $(CFLAGS) -c main.c

addVertexAlg.o: addVertexAlg.c
	gcc $(CFLAGS) -c addVertexAlg.c

subsetRem.o: subsetRem.c
	gcc $(CFLAGS) -c subsetRem.c

removeVertexAlg.o: removeVertexAlg.c 
	gcc $(CFLAGS) -c removeVertexAlg.c 

hybridAlg.o: hybridAlg.c
	gcc $(CFLAGS) -c hybridAlg.c

graphGen.o:	graphGen.c 
	gcc $(CFLAGS) -c graphGen.c

genRandGraph.o:  genRandGraph.c
	gcc $(CFLAGS) -c genRandGraph.c

linkList.o:	linkList.c 
	gcc $(CFLAGS) -c linkList.c

node.o:	node.c
	gcc $(CFLAGS) -c node.c

linkList2.o:	linkList2.c 
	gcc $(CFLAGS) -c linkList2.c

node2.o:	node2.c
	gcc $(CFLAGS) -c node2.c

menu.o:	menu.c
	gcc $(CFLAGS) -c menu.c

graph.o: graph.c
	gcc $(CFLAGS) -c graph.c

runTests.o: runTests.c
	gcc $(CFLAGS) -c runTests.c

bollobas.o: bollobas.c
	gcc $(CFLAGS) -c bollobas.c

tree.o: tree.c
	gcc $(CFLAGS) -c tree.c

H_Lau.o : H_Lau.c
	gcc $(CFLAGS) -c H_Lau.c

K_Alg.o : K_Alg.c
	gcc $(CFLAGS) -c K_Alg.c

K2_Alg.o : K2_Alg.c
	gcc $(CFLAGS) -c K2_Alg.c

mixture.o : mixture.c
	gcc $(CFLAGS) -c mixture.c

mixture2.o : mixture2.c
	gcc $(CFLAGS) -c mixture2.c

rome.o : rome.c
	gcc $(CFLAGS) -c rome.c

clean:
	rm -f *.o planarise

