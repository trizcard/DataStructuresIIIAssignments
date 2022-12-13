int Grafo::maior_caminho(int idOrigem, int idDestino){
    std::map<int,int> velocidade;
    std::map<int,int> antecessores;
    std::map<int,bool> visitados;
    std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::greater<std::pair<int,int>>> fila_de_prioridade;

    for (const auto& vertice: lista_de_nos){
        velocidade.insert({vertice.first,0}); // initialize distances to 0
        antecessores.insert({vertice.first,-1});
        visitados.insert({vertice.first,false});
    }

    velocidade.at(idOrigem) = 10000000; // set initial distance to maximum value
    fila_de_prioridade.push({10000000,idOrigem}); // push initial node to priority queue

    while(!fila_de_prioridade.empty()){
        int id_atual = fila_de_prioridade.top().second;
        fila_de_prioridade.pop();

        if(visitados.at(id_atual) == true){
            continue;
        }

        visitados.at(id_atual) = true;

        for (const auto& aresta: lista_de_nos.at(
            

int Grafo::maior_caminho(int idOrigem, int idDestino){
    std::map<int,int> velocidade;
    std::map<int,int> antecessores;
    std::map<int,bool> visitados;
    std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::less<std::pair<int,int>>> fila_de_prioridade;

    for (const auto& vertice: lista_de_nos){
        velocidade.insert({vertice.first,0});
        antecessores.insert({vertice.first,-1});
        visitados.insert({vertice.first,false});
    }

    velocidade.at(idOrigem) = 0;
    fila_de_prioridade.push({0,idOrigem});

    while(!fila_de_prioridade.empty()){
        int id_atual = fila_de_prioridade.top().second;
        fila_de_prioridade.pop();

        if(visitados.at(id_atual) == true){
            continue;
        }

        visitados.at(id_atual) = true;

        for (const auto& aresta: lista_de_nos.at(id_atual).pegar_lista_de_arestas()){
            int id_adjacente = aresta.first;
            int peso = aresta.second;

            if(velocidade.at(id_adjacente) < velocidade.at(id_atual) + peso){
                velocidade.at(id_adjacente) = velocidade.at(id_atual) + peso;
                antecessores.at(id_adjacente) = id_atual;
                fila_de_prioridade.push({velocidade.at(id_adjacente),id_adjacente});
            }
        }
    }

    return velocidade.at(idDestino);
}
