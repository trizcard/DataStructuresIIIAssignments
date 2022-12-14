#include "../headers/grafo.h"
#include <climits>

//grafos

Grafo::Grafo(FILE* arquivo_entrada){

    reg_dados *novo_reg_dados= cria_registro_dados();

    while (le_arquivo(novo_reg_dados, arquivo_entrada)!=0){ //enquanto o arquivo não terminar, continuar lendo os registros
        adicionar_no({novo_reg_dados}); //cria No que recebe o registro de dados (idConecta-nomePoPs-nomePais-siglaPais)
        
        if(novo_reg_dados->idPoPsConectado!=-1){ // se existir conexao
            adicionar_no(novo_reg_dados->idPoPsConectado); //cria No que recebe o idPopsConectado de cada idConecta
            lista_de_nos.at(novo_reg_dados->idConecta).adicionar_aresta(novo_reg_dados->idPoPsConectado, calcula_velocidade(novo_reg_dados));//encontra o idConecta na lista de no e cria a ligação das arestas, com os pesos calculados
            lista_de_nos.at(novo_reg_dados->idPoPsConectado).adicionar_aresta(novo_reg_dados->idConecta, calcula_velocidade(novo_reg_dados));//encontra o idConecta na lista de no e cria a ligação das arestas, com os pesos calculados        
        }
    }

    free(novo_reg_dados);
}

void Grafo::adicionar_no(const No& no){
    lista_de_nos.insert({no.pegar_idConecta(),no}); //insere no na lista de nos
    if(no.completo == true){ //se o no a ser inserido esta completo, ou seja, é um idConecta e nao um idPopsConectado
        lista_de_nos.at(no.pegar_idConecta()).completar_no(no); //completa as informações do no, se for um idPopsConectado inserido, que nao possuia infos, sera completado
    }
}

// implementação do Algoritmo de Dijkstra para encontrar o menor caminho entre dois nós
int Grafo::menor_caminho(int idOrigem, int idDestino){
    std::map<int,int> velocidade; // relaciona a velocidade ate o no
    std::map<int,int> antecessores; // relaciona o antecessor ao no
    std::map<int,bool> visitados; // indica se o no foi visitado ou nao (booleano)

    // cria uma fila de prioridade que ordena os nos de acordo com a velocidade
    std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::greater<std::pair<int,int>>> pilha_de_prioridade;

    // inicializa as velocidades, antecessores e visitados
    for (const auto& vertice: lista_de_nos){
        velocidade.insert({vertice.first, INT_MAX}); // "infinita"
        antecessores.insert({vertice.first,-1}); // nenhum antecessor
        visitados.insert({vertice.first,false}); // nenhum no visitado
    }

    // inicializa a velocidade do no de origem como 0 (dele até ele mesmo) e insere na pilha
    velocidade.at(idOrigem) = 0;
    pilha_de_prioridade.push({0,idOrigem});

    while(!pilha_de_prioridade.empty()){ // enquanto a pilha nao estiver vazia
        // pega a segunda velocidade (a primeira é do nó até ele mesmo) da pilha
        int no_atual = pilha_de_prioridade.top().second;
        pilha_de_prioridade.pop();

        // se o no ja foi visitado, pula para o proximo
        if(visitados.at(no_atual) == true){
            continue;
        }

        visitados.at(no_atual) = true;

        for (const auto& aresta: lista_de_nos.at(no_atual).pegar_lista_de_arestas()){ // para cada aresta do no atual
            int no_adjacente = aresta.first; 
            int peso = aresta.second;

            // se a velocidade para chegar no nó adjacente passando pelo nó atual for menor que a velocidade no nó adjacente
            if(velocidade.at(no_adjacente) > velocidade.at(no_atual) + peso){ 
                velocidade.at(no_adjacente) = velocidade.at(no_atual) + peso; // atualiza a velocidade do nó adjacente
                antecessores.at(no_adjacente) = no_atual; // atualiza o antecessor como nó atual
                pilha_de_prioridade.push({velocidade.at(no_adjacente),no_adjacente}); // insere o nó adjacente na pilha
            }
        }
    }

    // se os nós de origem e destino não estão conectados, retorna -1
    if (velocidade.at(idDestino) >= INT_MAX){
        return -1;
    }
    
    // retorna a velocidade minima entre os nós
    return velocidade.at(idDestino);
}

// implementação do Algoritmo de Edmonds-Karp para encontrar o fluxo máximo entre dois nós
int Grafo::fluxo_maximo(int idOrigem, int idDestino){ 
    std::map<int,std::map<int,int>> fluxo; // relaciona o fluxo de cada nó com cada nó adjacente
    // <[valor de fluxo][nó origem][aresta com nó adjacente]>

    std::map<int,std::map<int,int>> capacidade; // relaciona a capacidade (poder de transmissão) de cada nó com cada nó adjacente
    // <[velocidade][nó origem][aresta com nó adjacente]>

    std::map<int,int> antecessores; // relaciona o antecessor ao nó
    std::queue<int> fila; // fila para armazenar os nós

    for (const auto& vertice: lista_de_nos){
        for (const auto& aresta: lista_de_nos.at(vertice.first).pegar_lista_de_arestas()){
            capacidade[vertice.first][aresta.first] = aresta.second; // inicializa a capacidade como a velocidade entre os nós
            fluxo[vertice.first][aresta.first] = 0; // inicializa o fluxo como 0 para todos os nós
        }
    }

    int fluxo_maximo = 0;

    while(true){
        for (const auto& vertice: lista_de_nos){ // inicializa todos os antecessores do nó como -1
            antecessores[vertice.first] = -1;
        }

        fila.push(idOrigem); // insere o nó de origem na fila

        while(!fila.empty()){ 
            int no_atual = fila.front(); // pega o primeiro nó da fila
            fila.pop(); // remove o nó da fila

            for (const auto& aresta: lista_de_nos.at(no_atual).pegar_lista_de_arestas()){ // percorre as arestas do nó atual
                int no_adjacente = aresta.first; // pega o nó adjacente

                // verifica se o nó adjacente não foi visitado
                // caso não, verifica se capacidade do nó atual para o nó adjacente é maior que o fluxo do nó atual para o nó adjacente
                if(antecessores[no_adjacente] == -1 && capacidade[no_atual][no_adjacente] > fluxo[no_atual][no_adjacente]){
                    antecessores[no_adjacente] = no_atual; // o antecessor do nó adjacente se torna o nó atual
                    fila.push(no_adjacente); // insere o nó adjacente na fila
                }
            }
        }

        // se o antecessor do nó de destino for -1, significa que não há caminho entre os nós
        if(antecessores[idDestino] == -1){
            break;
        }

        int fluxo_minimo = INT_MAX;

        // percorre o nó de destino até a origem e pega o fluxo mínimo
        for (int no_atual = idDestino; no_atual != idOrigem; no_atual = antecessores[no_atual]){
            int no_anterior = antecessores[no_atual];

            // fluxo mínimo é o menor valor entre o fluxo mínimo já encontrado antes e diferença entre a capacidade e o fluxo
            fluxo_minimo = std::min(fluxo_minimo, capacidade[no_anterior][no_atual] - fluxo[no_anterior][no_atual]);
        }

        // percorre novamente o nó de destino até a origem e atualiza o fluxo
        for (int no_atual = idDestino; no_atual != idOrigem; no_atual = antecessores[no_atual]){
            int no_anterior = antecessores[no_atual];

            // soma o fluxo mínimo ao fluxo do nó anterior para o nó atual
            fluxo[no_anterior][no_atual] += fluxo_minimo;
            // subtrai o fluxo mínimo do fluxo do nó atual para o nó anterior
            fluxo[no_atual][no_anterior] -= fluxo_minimo;
        }

        fluxo_maximo += fluxo_minimo;
    }

    return fluxo_maximo;
}

std::ostream& operator<<(std::ostream& os,const Grafo& grafo){
    for (const auto& vertice: grafo.lista_de_nos){
        os<<vertice.second;
    }
    return os;
    
}


/////////// começa no

No::No(reg_dados* registro){
    idConecta = registro->idConecta;
    nomePoPs = registro->nomePoPs;
    nomePais = registro->nomePais;
    siglaPais = registro->siglaPais;
    completo = true;

}
No::No(int idConecta){
    this->idConecta = idConecta;
    completo = false;
}

int No::pegar_idConecta() const {return idConecta;}
//std::string No::pegar_nomePops() const{return nomePoPs;}
//std::string No::pegar_nomePais() const{return nomePais;}
//std::string No::pegar_siglaPais() const{return siglaPais;}
const std::map<int,int>& No::pegar_lista_de_arestas() const{return lista_de_arestas;}

void No::adicionar_aresta(int idPoPsConectado, int velocidade){lista_de_arestas.insert({idPoPsConectado,velocidade});}

void No::completar_no(const No& no_completo){
    if(completo==false){
        nomePoPs = no_completo.nomePoPs;
        nomePais = no_completo.nomePais;
        siglaPais = no_completo.siglaPais;
        completo = true;
    }
}

std::ostream& operator<<(std::ostream& os,const No& no){

    for(const auto& aresta: no.lista_de_arestas){
        os<<no.idConecta<<" "<<no.nomePoPs<<" "<<no.nomePais<<" "<<no.siglaPais<<" "<<aresta.first<<" "<<aresta.second<<"Mbps"<<std::endl;
    }
    return os;
}


int calcula_velocidade(reg_dados* registro){

    if (registro->unidadeMedida[0] == 'G'){
        return registro->velocidade*1024;
    }
    else return registro->velocidade;

}

//Função pega a quantidade de vértices
int Grafo ::qtd_vertices() const {
    return lista_de_nos.size();
}

void Grafo::busca_profundidade( int idOrigem, std::map <int,int> cor, int *ciclos, int noOriginal) const {
    //Marca o vértice que está como amarelo, que significa que ele foi visitado com map
    cor[idOrigem] = AMARELO;

    //Percorre todos os vértices adjacentes
    for(const auto& aresta: lista_de_nos.at(idOrigem).pegar_lista_de_arestas()){
        int id_adjacente = aresta.first;

        //Se o vértice adjacente não foi visitado, chama a função recursivamente
        if(cor[id_adjacente] == BRANCO){
            busca_profundidade(id_adjacente, cor, ciclos, noOriginal);
        }
    }

    //Verificar se é ciclo simples, ou seja, se o vértice adjacente é o vértice original
    if(cor[idOrigem] != BRANCO && idOrigem == noOriginal){
        *ciclos = *ciclos + 1;
    }

    //Marca o vértice como vermelho, que significa que ele foi visitado e todos os seus adjacentes também
    cor[idOrigem] = VERMELHO;
}

//Função demarca cor da profundidade (O índice do vértice do arranjo adjacente em grafo será o mesmo do arranjo de cor)
int Grafo:: cor_profundidade() const{
    //int qtd_vertice = qtd_vertices();
    std::map <int,int> cor;
    //int cor[qtd_vertice];
    int ciclos = 0;

    //Marca todos os vértices como branco, que significa que eles não foram visitados no map de cor e colocar a posi dos vértices
    for(const auto& vertice: lista_de_nos){
        cor.insert({vertice.first, BRANCO});
    }   
    //imprimir chegou aqui
    //Chama a função recursiva para todos os vértices com map
    for(const auto& vertice: cor){
        if(vertice.second == BRANCO){
            busca_profundidade(vertice.first, cor, &ciclos, vertice.first);
        }
    std::cout<<"chegou aqui"<<std::endl;

    }
    
    return ciclos;
}