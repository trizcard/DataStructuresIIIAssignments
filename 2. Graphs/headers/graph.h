#include <map>
#include <set>
#include <string>
#include <iostream>
#include <queue>
#include "../headers/struct.h"
#include "../headers/funcRW.h"
#include "../headers/funcCheck.h"
#define WHITE 0 // Not visited
#define YELLOW 1 // Visited
#define RED 2 // Visited and in the stack

class No;

// Auxiliar function for the graph
int calculateSpeed(regData* registers);

class Graph{
    // Attributes
    private:
        std::map<int,No> noList; // Order the idConnect keys (of type int) and stores the No's (which colorrespond to the vertices)
    
    // Methods 
        void addNo(const No& no);
    
    // Constructor
    public:
        Graph(FILE* inputFile);

    // Methods
        friend std::ostream& operator<<(std::ostream& os,const Graph& graph); 
        int shortestPath(int idOrigin, int idDestiny);
        int maxFlow(int idOrigin, int idDestiny);
        void depthSearch( int idOrigin,  std::map <int,int>& color, int *cycles, int noOriginal ) const;
        int colorDepth() const;

};

class No{
    // Attributes
    private:
        std::map<int,int> edgeList; // Order internal idPopsConnecta keys, with the weights
        int idConnect;
        std::string namePoPs;
        std::string nameCountry;
        std::string country;
    
    // Constructor
    public:
        No(regData* registers);
        No(int idConnect);

    // Methods
        int getIdConnect() const;
        const std::map<int,int>& getEdgeList() const;
        void addEdge(int idPoPsConnected, int speed);
        void completeNo(const No& completedNo);

    // Public attributes
        bool complete;
    
    // Methods
        friend std::ostream& operator<<(std::ostream& os,const No& no);

};

