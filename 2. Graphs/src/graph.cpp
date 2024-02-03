#include "../headers/graph.h"
#include <climits> 

/**
 * @brief Constructor for the Graph class.
 * 
 * @param inputFile Pointer to the input file
 */

Graph::Graph(FILE* inputFile){

    regData *newRegData= createRegData();

    while (readFile(newRegData, inputFile)!=0){ 
        addNo({newRegData});
        
        if(newRegData->idPoPsConnected!=-1){ // if there is a connected point, add the edge
            
            addNo(newRegData->idPoPsConnected); // add the connected point to the graph
            
            // find the idConnect (idConnect of the connected point) in the list of nodes and create the edge connections, with the calculated weights
            noList.at(newRegData->idConnect).addEdge(newRegData->idPoPsConnected, calculateSpeed(newRegData));
            // find the idPoPsConnected (idPoPsConnected of the connected point) in the list of nodes and create the edge connections, with the calculated weights      
            noList.at(newRegData->idPoPsConnected).addEdge(newRegData->idConnect, calculateSpeed(newRegData));
        }
    }

    // free memory
    free(newRegData);
}

/**
 * @brief Function responsible for adding a new node to the node list.
 * 
 * @param no Node to be added
 */

void Graph::addNo(const No& no){
    noList.insert({no.getIdConnect(),no}); // insert the node in the list of nodes

    if(no.complete == true){ 
        noList.at(no.getIdConnect()).completeNo(no); 
        }
}

/**
 * @brief Function responsible for finding the shortest path between two nodes, based on Dijkstra's algorithm.
 * 
 * @param idOrigin Id of the origin node
 * @param idDestiny Id of the destination node
 * @return int Returns the length of the shortest path, or -1 if no path exists.
 */


int Graph::shortestPath(int idOrigin, int idDestiny){
    std::map<int,int> speed; // represents the speed to reach each node
    std::map<int,int> predecessor; // represents the antecessor of each node
    std::map<int,bool> visited; // represents if the node has been visited or not

    // priority queue to store the nodes to be visited, with the smallest speed at the top
    std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::greater<std::pair<int,int>>> priorityQueue;

    // Inicialize speeds, predecessor and visited
    for (const auto& vertice: noList){
        speed.insert({vertice.first, INT_MAX}); 
        predecessor.insert({vertice.first,-1}); 
        visited.insert({vertice.first,false}); 
    }

    // Inicialize the speed of the origin node as 0 and insert it in the priority queue
    speed.at(idOrigin) = 0;
    priorityQueue.push({0,idOrigin});

    // Dijkstra's algorithm
    while(!priorityQueue.empty()){ 
        int currentNo = priorityQueue.top().second;
        priorityQueue.pop();

        // if the node has already been visited, continue
        if(visited.at(currentNo) == true){
            continue;
        }

        visited.at(currentNo) = true; // mark the node as visited

        // Visit all the adjacent nodes
        for (const auto& edge: noList.at(currentNo).getEdgeList()){ 
            int adjacentNo = edge.first; 
            int peso = edge.second; 

            // if the speed to reach the adjacent node is greater than the speed to reach the current node plus the speed of the edge, update the speed of the adjacent node and the predecessor
            if(speed.at(adjacentNo) > speed.at(currentNo) + peso){ 
                speed.at(adjacentNo) = speed.at(currentNo) + peso; // update the speed
                predecessor.at(adjacentNo) = currentNo; // update the predecessor
                priorityQueue.push({speed.at(adjacentNo),adjacentNo}); // insert the adjacent node in the priority queue
            }
        }
    }

    // if the speed to reach the destination node is equal to INT_MAX, return -1
    if (speed.at(idDestiny) >= INT_MAX){
        return -1;
    }
    
    // return the speed to reach the destination node
    return speed.at(idDestiny);
}

/**
 * @brief Function responsible for finding the maximum flow between two nodes, based on the Edmonds-Karp algorithm.
 * 
 * @param idOrigin Id of the origin node
 * @param idDestiny Id of the destination node
 * @return int Returns the maximum flow between the nodes.
 */

int Graph::maxFlow(int idOrigin, int idDestiny){ 
    std::map<int,std::map<int,int>> flow; // Relates the flow from each node to the adjacent node
    std::map<int,std::map<int,int>> capacity; // Relates the capacity from each node to the adjacent node
    
    std::map<int,int> predecessor; // vector to store the predecessor of each node
    std::queue<int> queue; // queue to store the nodes to be visited

    // inicialize the flow and capacity
    for (const auto& vertice: noList){ 
        for (const auto& edge: noList.at(vertice.first).getEdgeList()){ // for each edge of a node
            // edge.first adjacente node and edge.second speed
            capacity[vertice.first][edge.first] = edge.second; // inicialize the capacity as the speed of the edge
            flow[vertice.first][edge.first] = 0; // inicialize the flow as 0
        }
    }

    int maxFlow = 0; 

    while(true){ // while there is a path between the origin and the destination
        for (const auto& vertice: noList){ // inicialize the predecessor of each node as -1
            predecessor[vertice.first] = -1;
        }

        queue.push(idOrigin); // insert the origin node in the queue

        // Edmonds-Karp algorithm
        while(!queue.empty()){  
            int currentNo = queue.front(); // get the first node in the queue
            queue.pop(); 

            for (const auto& edge: noList.at(currentNo).getEdgeList()){ // for each edge of the current node
                int adjacentNo = edge.first; // get the adjacent node

                if(predecessor[adjacentNo] == -1 && capacity[currentNo][adjacentNo] > flow[currentNo][adjacentNo]){
                    predecessor[adjacentNo] = currentNo; // update the predecessor of the adjacent node
                    queue.push(adjacentNo); // insert the adjacent node in the queue
                }
            }
        }

        // if there is no path between the origin and the destination, break
        if(predecessor[idDestiny] == -1){
            break;
        }

        // find the flow minimum
        int flowMin = INT_MAX; 

        for (int currentNo = idDestiny; currentNo != idOrigin; currentNo = predecessor[currentNo]){
            int pastNo = predecessor[currentNo]; // get the predecessor of the current node

            // find the minimum flow of the path, based on the capacity of the edge and the flow
            flowMin = std::min(flowMin, capacity[pastNo][currentNo] - flow[pastNo][currentNo]);
        }

        // update the flow
        for (int currentNo = idDestiny; currentNo != idOrigin; currentNo = predecessor[currentNo]){
            int pastNo = predecessor[currentNo];

            // add the flow minimum to the flow from the predecessor to the current node
            flow[pastNo][currentNo] += flowMin;
            // subtract the flow minimum from the flow from the current node to the predecessor
            flow[currentNo][pastNo] -= flowMin;
        }

        maxFlow += flowMin; // update the maximum flow
    }

    return maxFlow;
}

/**
 * @brief Function responsible for performing depth-first search in the graph.
 * 
 * @param idOrigin Id of the origin node
 * @param color Map of node colors
 * @param cycles Vector of cycles
 * @param noOriginal Id of the original node
 */

void Graph::depthSearch( int idOrigin, std::map <int,int>& color, int *cycles, int noOriginal) const {

    // Paint the node as yellow, which means that it is being visited
    color[idOrigin] = YELLOW;
    

    // Visit all adjacent nodes
    for(const auto& edge: noList.at(idOrigin).getEdgeList()){ 
        int adjacentId = edge.first; //Pega o id do vértice adjacente
        
        // If the adjacent vertex has not been visited, call the function recursively
        if(color.at(adjacentId) == WHITE){ 
            depthSearch(adjacentId, color, cycles, noOriginal);
        }
        else if(color.at(adjacentId) == RED){ // If the adjacent vertex has been visited and is red, it means that it is the origin vertex
            *cycles = *cycles + 1;
        }
    }

    // Paint the node as red, which means that it has already been visited
    color[idOrigin] = RED;
}

/**
 * @brief Function responsible for marking the depth color.
 * 
 * @return int Returns the number of cycles.
 */

int Graph:: colorDepth() const{
    std::map <int,int> color; // Map of node colors
    int cycles = 0; // Number of cycles
    int tree = 0; // Number of trees

    // Inicialize the color of all nodes as white
    for(const auto& vertice: noList){
        color.insert({vertice.first, WHITE});
    }   
    
    for(const auto& vertice: color){ 
        if(vertice.second == WHITE){ // If the node has not been visited, call the function depthSearch
            depthSearch(vertice.first, color, &cycles, vertice.first); 
            tree +=1; // Increment the number of trees
        }
    } 
    
    return cycles;
}

/**
 * @brief Function responsible for traversing the graph and printing the nodes.
 * 
 * @param os Object of type ostream
 * @param graph Object of type Graph
 * @return std::ostream& Returns the object of type ostream
 */

std::ostream& operator<<(std::ostream& os,const Graph& graph){
    for (const auto& vertice: graph.noList){ 
        os<<vertice.second; // Print the node
    }
    return os;
    
}

/**
 * @brief Constructor for the Node class, for a complete Node (with idConnect inserted).
 * 
 * @param register Pointer to the data register
 */

No::No(regData* registers){
    idConnect = registers->idConnect;
    namePoPs = registers->namePoPs;
    nameCountry = registers->nameCountry;
    country = registers->country;
    complete = true;
}

/**
 * @brief Constructor for the Node class, for an incomplete Node (with idPoPsConnected inserted).
 * 
 * @param idConnect Id of the node, idPoPsConnected of the complete node
 */

No::No(int idConnect){
    this->idConnect = idConnect;
    complete = false;
}

/**
 * @brief Function responsible for returning the idConnect of the node.
 * 
 * @return int Returns the idConnect of the node.
 */

int No::getIdConnect() const {return idConnect;}

/**
 * @brief Function responsible for returning the map of edges of the node.
 * 
 * @return const std::map<int, int>& Returns the map of edges of the node.
 */

const std::map<int,int>& No::getEdgeList() const{return edgeList;}

/**
 * @brief Function responsible for adding an edge to the node.
 * 
 * @param idPoPsConnected Id of the connected node
 * @param speed Speed of the edge
 */

void No::addEdge(int idPoPsConnected, int speed){edgeList.insert({idPoPsConnected,speed});}

/**
 * @brief Function responsible for completing the node.
 * 
 * @param completedNode Complete node
 */

void No::completeNo(const No& completedNo){
    if(complete==false){
        namePoPs = completedNo.namePoPs;
        nameCountry = completedNo.nameCountry;
        country = completedNo.country;
        complete = true;
    }
}

/**
 * @brief Function responsible for printing the edges of the node.
 * 
 * @param os Object of type ostream
 * @param node Object of type Node
 * @return std::ostream& Returns the object of type ostream
 */

std::ostream& operator<<(std::ostream& os,const No& no){

    for(const auto& edge: no.edgeList){ // para cada edge de um no, imprime a edge
        os<<no.idConnect<<" "<<no.namePoPs<<" "<<no.nameCountry<<" "<<no.country<<" "<<edge.first<<" "<<edge.second<<"Mbps"<<std::endl;
    }
    return os;
}


/**
 * @brief Function responsible for calculating the speed of the edge.
 * 
 * @param register Pointer to the data register
 * @return int Returns the speed of the edge.
 */

int calculateSpeed(regData* registers){

    if (registers->unitMeas[0] == 'G'){ // if the unit of measurement is Gbps, return the speed in Mbps
        return registers->speed*1024;
    }
    else return registers->speed;

}
