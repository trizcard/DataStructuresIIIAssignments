#include "../headers/functionalities.h"
#include "../headers/funcOpen.h"
#include "../headers/funcRW.h"
#include "../headers/funcCheck.h"
#include "../headers/graph.h"

/**
 * @brief Function responsible for performing functionality 11 of the program, which reads a binary file and prints the graph.
 * 
 * @param fileName String containing the name of the file
 */

void command11(char* inputFileName){

  FILE* inputFile = openReadBin(inputFileName);
  if(inputFile == NULL) return;

  regHead *newRegHead = createRegHead();

  readHeadReg(inputFile, newRegHead);

  if (checkConsistency(newRegHead) != 0){
    free(newRegHead);
    fclose(inputFile);
    return;
  }

  Graph graph {inputFile}; // Create the graph from a binary file

  std::cout << graph; // Print the graph

  // Free memory and close file
  free(newRegHead);
  fclose(inputFile);
}

/**
 * @brief Function responsible for performing functionality 12 of the program, which finds out how many cycles exist in the graph.
 * 
 * @param fileName String containing the name of the file
 */

void command12(char* inputFileName) {
  FILE* inputFile = openReadBin(inputFileName);
  if(inputFile == NULL) return;

  regHead *newRegHead = createRegHead();

  readHeadReg(inputFile, newRegHead);

  if (checkConsistency(newRegHead) != 0){
    free(newRegHead);
    fclose(inputFile);
    return;
  }

  Graph graph {inputFile}; // Create the graph from a binary file

  int cycles = graph.colorDepth(); // Call the function that finds out how many cycles exist in the graph

  std::cout << "Quantidade de cycles: " << cycles <<std::endl; // Print the number of cycles

  // Free memory and close file
  free(newRegHead);
  fclose(inputFile);

}

/**
 * @brief Function responsible for performing functionality 13 of the program, which finds the maximum flow between two vertices of the graph.
 * 
 * @param fileName String containing the name of the file
 * @param n Number of times the functionality will be executed
 */

void command13(char* inputFileName, int n) {
  FILE* inputFile = openReadBin(inputFileName);
  if(inputFile == NULL) return;

  regHead *newRegHead = createRegHead();

  readHeadReg(inputFile, newRegHead);

  if (checkConsistency(newRegHead) != 0){
    free(newRegHead);
    fclose(inputFile);
    return;
  }


  Graph graph {inputFile}; // Create the graph from a binary file
  int orig, end, cMax; // Variables that will store the origin vertex, the destination vertex and the maximum flow between them

  for (int i = 0; i < n; i++){ // Execute the functionality n times
    scanf("%d %d", &orig, &end);

    cMax = graph.maxFlow(orig, end); // Find out the maximum flow between two vertices, based on Edmonds-Karp

    if(cMax <= 0){ // If it is not possible to identify the value of the maximum flow, print -1
      printf("Fluxo máximo entre %d e %d: -1\n", orig, end);
    }
    else{ 
      printf("Fluxo máximo entre %d e %d: %d Mbps\n", orig, end, cMax);
    }
  }
  
  // Free memory and close file
  free(newRegHead);
  fclose(inputFile);
}

/**
 * @brief Function responsible for performing functionality 14 of the program, which finds the minimum path between two vertices of the graph
 * going through a mandatory intermediate vertex.
 * 
 * @param fileName String containing the name of the file
 * @param n Number of times the functionality will be executed
 */

void command14(char* inputFileName, int n) {
  FILE* inputFile = openReadBin(inputFileName);
  if(inputFile == NULL) return;

  regHead *newRegHead = createRegHead();

  readHeadReg(inputFile, newRegHead);

  if (checkConsistency(newRegHead) != 0){
    free(newRegHead);
    fclose(inputFile);
    return;
  }


  Graph graph {inputFile}; // Create the graph from a binary file
  int orig, end, stop; // Variables that will store the origin vertex, the destination vertex and the intermediate vertex
  int c1; // Variable that will store the minimum path between the origin vertex and the intermediate vertex
  int c2; // Variable that will store the minimum path between the intermediate vertex and the destination vertex

  for (int i = 0; i < n; i++){ 
    scanf("%d %d %d", &orig, &end, &stop);

    c1 = graph.shortestPath(orig, stop); // Call the function that finds the minimum path between the origin vertex and the intermediate vertex, with djikstra
    c2 = graph.shortestPath(stop, end); // Call the function that finds the minimum path between the intermediate vertex and the destination vertex, with djikstra
    
    if(c1 == -1 || c2 == -1){ // If it is not possible to identify the value of the minimum path, print -1
      printf("Comprimento do caminho entre %d e %d parando em %d: -1\n", orig, end, stop); 
    }
    else{ // Print the minimum path
      printf("Comprimento do caminho entre %d e %d parando em %d: %dMbps\n", orig, end, stop, (c1+c2));
    }
  }

  // Free memory and close file
  free(newRegHead);
  fclose(inputFile);
}