#include "../headers/grafo.h"
#include <climits>

///////////////////////////////// Funções da classe Grafo 

/**
 * @brief Construtor da classe Grafo
 * 
 * @param arquivo_entrada Ponteiro para o arquivo de entrada
 */

Grafo::Grafo(FILE* arquivo_entrada){

    reg_dados *novo_reg_dados= cria_registro_dados();

    while (le_arquivo(novo_reg_dados, arquivo_entrada)!=0){ //enquanto o arquivo não terminar, continuar lendo os registros

        adicionar_no({novo_reg_dados}); //cria No que recebe o registro de dados (idConecta-nomePoPs-nomePais-siglaPais)
        
        if(novo_reg_dados->idPoPsConectado!=-1){ // se existir conexao
            
            adicionar_no(novo_reg_dados->idPoPsConectado); //cria No que recebe o idPopsConectado de cada idConecta
            
            //encontra o idConecta na lista de no e cria a ligação das arestas, com os pesos calculados
            lista_de_nos.at(novo_reg_dados->idConecta).adicionar_aresta(novo_reg_dados->idPoPsConectado, calcula_velocidade(novo_reg_dados));
            //encontra o idPopsConectado (idConecta do ponto conectado) na lista de no e cria a ligação das arestas, com os pesos calculados        
            lista_de_nos.at(novo_reg_dados->idPoPsConectado).adicionar_aresta(novo_reg_dados->idConecta, calcula_velocidade(novo_reg_dados));
        }
    }

    //libera o registro de dados
    free(novo_reg_dados);
}

/**
 * @brief Função responsável por adicionar um novo no na lista de nos
 * 
 * @param no No a ser adicionado
 */

void Grafo::adicionar_no(const No& no){
    lista_de_nos.insert({no.pegar_idConecta(),no}); //insere no na lista de nos

    if(no.completo == true){ //se o no a ser inserido esta completo, ou seja, é um idConecta e nao um idPopsConectado
        lista_de_nos.at(no.pegar_idConecta()).completar_no(no); //completa as informações do no, se for um idPopsConectado inserido, que nao possuia infos, sera completado
    }
}

/**
 * @brief Função responsável por encontrar o menor caminho entre dois nós, com base no algoritmo de Dijkstra
 * 
 * @param idOrigem Id do nó de origem
 * @param idDestino Id do nó de destino
 * @return int Retorna a velocidade do menor caminho, ou -1 caso não exista caminho
 */

int Grafo::menor_caminho(int idOrigem, int idDestino){
    std::map<int,int> velocidade; // representa a velocidade ate o no
    std::map<int,int> antecessores; // representa o antecessor de cada no
    std::map<int,bool> visitados; // indica se o no foi visitado ou nao (booleano)

    // cria uma fila de prioridade que ordena os nos de acordo com a sua velocidade
    std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::greater<std::pair<int,int>>> pilha_de_prioridade;

    // inicializa as velocidades, antecessores e visitados
    for (const auto& vertice: lista_de_nos){
        velocidade.insert({vertice.first, INT_MAX}); // "infinita", assim qualquer valor será menor que este
        antecessores.insert({vertice.first,-1}); // nenhum antecessor, portanto é igual -1
        visitados.insert({vertice.first,false}); // nenhum nó visitado, portanto igual a false
    }

    // inicializa a velocidade do no de origem como 0 (dele até ele mesmo) e insere na pilha
    velocidade.at(idOrigem) = 0;
    pilha_de_prioridade.push({0,idOrigem});

    // percorrer todos os nós
    while(!pilha_de_prioridade.empty()){ // enquanto a pilha nao estiver vazia
        // pega a segunda velocidade (a primeira é do nó até ele mesmo) da pilha e desempilha
        int no_atual = pilha_de_prioridade.top().second;
        pilha_de_prioridade.pop();

        // se o nó ja foi visitado, pula para o proximo
        if(visitados.at(no_atual) == true){
            continue;
        }

        visitados.at(no_atual) = true; // marca o no como visitado

        // visita todas as arestas do nó atual
        for (const auto& aresta: lista_de_nos.at(no_atual).pegar_lista_de_arestas()){ 
            int no_adjacente = aresta.first; // pega o nó adjacente
            int peso = aresta.second; // pega o peso da aresta

            // se a velocidade para chegar no nó adjacente passando pelo nó atual for menor que a velocidade registrada no nó adjacente até ele, ela é substituida
            if(velocidade.at(no_adjacente) > velocidade.at(no_atual) + peso){ 
                velocidade.at(no_adjacente) = velocidade.at(no_atual) + peso; // atualiza a velocidade do nó adjacente, como a velocidade até o nó atual + a velocidade da aresta, pois representa o menor caminho
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

/**
 * @brief Função responsável por encontrar o fluxo máximo entre dois nós, com base no algoritmo de Edmonds-Karp
 * 
 * @param idOrigem Id do nó de origem
 * @param idDestino Id do nó de destino
 * @return int Retorna o fluxo máximo entre os nós
 */

int Grafo::fluxo_maximo(int idOrigem, int idDestino){ 
    std::map<int,std::map<int,int>> fluxo; // relaciona o fluxo de cada nó até o nó adjacente
    std::map<int,std::map<int,int>> capacidade; // relaciona a capacidade (velocidade) de cada nó até o nó adjacente
    
    std::map<int,int> antecessores; // verifica quem é o antecessor ao nó
    std::queue<int> fila; // fila para armazenar os nós a serem analisados

    // esse for inicializa a capacidade (com a velocidade) e o fluxo como 0 para todos os nós conectados
    for (const auto& vertice: lista_de_nos){ // para cada nó
        for (const auto& aresta: lista_de_nos.at(vertice.first).pegar_lista_de_arestas()){ // para cada aresta do nó
            // aresta.first é o nó adjacente e aresta.second é a velocidade entre os nós
            capacidade[vertice.first][aresta.first] = aresta.second; // inicializa a capacidade como a velocidade entre os nós
            fluxo[vertice.first][aresta.first] = 0; // inicializa o fluxo como 0 para todos os nós
        }
    }

    int fluxo_maximo = 0; // inicializa o fluxo máximo como 0

    while(true){ // enquanto houver caminhos para passaagem do fluxo
        for (const auto& vertice: lista_de_nos){ // inicializa todos os antecessores do nó como -1
            antecessores[vertice.first] = -1;
        }

        fila.push(idOrigem); // insere o nó de origem na fila

        // esse loop irá criar diversos caminhos possiveis a partir do id de Origem, considerando a capacidade disponível e fluxo já existente no caminho
        while(!fila.empty()){  // enquanto a fila não estiver vazia
            int no_atual = fila.front(); // pega o primeiro nó da fila
            fila.pop(); // remove esse mesmo nó da fila

            for (const auto& aresta: lista_de_nos.at(no_atual).pegar_lista_de_arestas()){ // percorre as arestas do nó atual
                int no_adjacente = aresta.first; // pega o nó adjacente

                // verifica se o nó adjacente não foi visitado
                // caso não, verifica se capacidade do nó atual para o nó adjacente é maior que o fluxo do nó atual para o nó adjacente
                // isso acontece, porque após a primeira repetição alguns fluxos serão diferentes de 0, mas não necessariamente a capacidade
                // estará cheia, portanto, analisa se ainda é possivel passar fluxo naquela aresta e cria esse caminho
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

        // inicializa o fluxo minimo como "infinito", dessa forma o fluxo minimo será sempre menor que esse valor, o que é útil mais abaixo
        int fluxo_minimo = INT_MAX; 

        // percorre o nó de destino até a origem e pega o fluxo mínimo a fim de traçar o caminho reverso, como no loop acima o inicio é o nó de origem
        // e, sabe-se pela verificação que o nó de destino tem um antecessor, ir traçando o caminho com os seus antecessores irá retornar a origem
        for (int no_atual = idDestino; no_atual != idOrigem; no_atual = antecessores[no_atual]){
            int no_anterior = antecessores[no_atual]; // pega o nó anterior

            // fluxo mínimo é o menor valor entre o fluxo mínimo já encontrado antes e diferença entre a capacidade e o fluxo
            // é necessário utilizar a diferença, para encontrar o fluxo disponível naquele trecho
            fluxo_minimo = std::min(fluxo_minimo, capacidade[no_anterior][no_atual] - fluxo[no_anterior][no_atual]);
        }

        // percorre novamente o nó de destino até a origem e atualiza o fluxo, sabendo qual o fluxo daquele trecho, uma vez que este é limitado pela menor veloc.
        for (int no_atual = idDestino; no_atual != idOrigem; no_atual = antecessores[no_atual]){
            int no_anterior = antecessores[no_atual];

            // soma o fluxo mínimo ao fluxo do nó anterior para o nó atual, pois esse é o fluxo que passará nesse sentido
            fluxo[no_anterior][no_atual] += fluxo_minimo;
            // subtrai o fluxo mínimo do fluxo do nó atual para o nó anterior, pois fluxo é contrário a esse sentido
            fluxo[no_atual][no_anterior] -= fluxo_minimo;
        }

        fluxo_maximo += fluxo_minimo; // atualiza o fluxo máximo, uma vez que este é a soma do fluxo de todos os caminhos
    }

    return fluxo_maximo;
}

/**
 * @brief Função responsável por fazer a busca em profundidade no grafo
 * 
 * @param idOrigem Id do nó de origem
 * @param cor Mapa de cores dos nós
 * @param ciclos Vetor de ciclos
 * @param noOriginal Id do nó original
 */

void Grafo::busca_profundidade( int idOrigem, std::map <int,int>& cor, int *ciclos, int noOriginal) const {

    //Marca o vértice que está como amarelo, que significa que ele foi visitado com map
    cor[idOrigem] = AMARELO;
    

    //Percorre todos os vértices adjacentes
    for(const auto& aresta: lista_de_nos.at(idOrigem).pegar_lista_de_arestas()){ 
        int id_adjacente = aresta.first; //Pega o id do vértice adjacente
        
        //Se o vértice adjacente não foi visitado, chama a função recursivamente
        if(cor.at(id_adjacente) == BRANCO){ 
            busca_profundidade(id_adjacente, cor, ciclos, noOriginal);
        }else if(cor.at(id_adjacente) == VERMELHO){ //Se o vértice adjacente já foi visitado, significa que existe um ciclo
            *ciclos = *ciclos + 1;
        }
    }

    //Marca o vértice como vermelho, que significa que ele foi visitado e todos os seus adjacentes também
    cor[idOrigem] = VERMELHO;
}

/**
 * @brief Função responsável por demarcar a cor da profundidade
 * 
 * @return int Retorna o número de ciclos
 */

int Grafo:: cor_profundidade() const{
    std::map <int,int> cor; //Mapa de cores dos vértices
    int ciclos = 0; //Número de ciclos
    int arv = 0; //Número de árvores

    //Marca todos os vértices como branco, que significa que eles não foram visitados no map de cor e colocar a posi dos vértices
    for(const auto& vertice: lista_de_nos){
        cor.insert({vertice.first, BRANCO});
    }   
    
    for(const auto& vertice: cor){ //Percorre todos os vértices
        if(vertice.second == BRANCO){ //Se o vértice não foi visitado, chama a função recursivamente
            busca_profundidade(vertice.first, cor, &ciclos, vertice.first); 
            arv +=1; //Incrementa o número de árvores
        }
    } 
    
    return ciclos;
}

/**
 * @brief Função responsável por percorrer o grafo e imprimir os nós
 * 
 * @param os Objeto do tipo ostream
 * @param grafo Objeto do tipo Grafo
 * @return std::ostream& Retorna o objeto do tipo ostream
 */

std::ostream& operator<<(std::ostream& os,const Grafo& grafo){
    for (const auto& vertice: grafo.lista_de_nos){ // para cada nó
        os<<vertice.second; // imprime o nó
    }
    return os;
    
}

///////////////////////////Funções da classe No]

/**
 * @brief Construtor da classe No, para um No completo (idConecta inserido)
 * 
 * @param registro Ponteiro para o registro de dados
 */

No::No(reg_dados* registro){
    idConecta = registro->idConecta;
    nomePoPs = registro->nomePoPs;
    nomePais = registro->nomePais;
    siglaPais = registro->siglaPais;
    completo = true;

}

/**
 * @brief Construtor da classe No, para um No incompleto (idPoPsConectado inserido)
 * 
 * @param idConecta Id do nó, idPoPsConectado do no completo
 */

No::No(int idConecta){
    this->idConecta = idConecta;
    completo = false;
}

/**
 * @brief Função responsável por retornar o idConecta do nó
 * 
 * @return int Retorna o idConecta do nó
 */
int No::pegar_idConecta() const {return idConecta;}

/**
 * @brief Função responsável por retornar o mapa de arestas do nó
 * 
 * @return const std::map<int,int>& Retorna o mapa de arestas do nó
 */
const std::map<int,int>& No::pegar_lista_de_arestas() const{return lista_de_arestas;}

/**
 * @brief Função responsável por adicionar uma aresta ao nó
 * 
 * @param idPoPsConectado Id do nó conectado
 * @param velocidade Velocidade da aresta
 */
void No::adicionar_aresta(int idPoPsConectado, int velocidade){lista_de_arestas.insert({idPoPsConectado,velocidade});}

/**
 * @brief Função responsável por completar o nó
 * 
 * @param no_completo Nó completo
 */
void No::completar_no(const No& no_completo){
    if(completo==false){
        nomePoPs = no_completo.nomePoPs;
        nomePais = no_completo.nomePais;
        siglaPais = no_completo.siglaPais;
        completo = true;
    }
}

/**
 * @brief Função responsável por imprimir as arestas do nó
 * 
 * @param os Objeto do tipo ostream
 * @param no Objeto do tipo No
 * @return std::ostream& Retorna o objeto do tipo ostream
 */

std::ostream& operator<<(std::ostream& os,const No& no){

    for(const auto& aresta: no.lista_de_arestas){ // para cada aresta de um no, imprime a aresta
        os<<no.idConecta<<" "<<no.nomePoPs<<" "<<no.nomePais<<" "<<no.siglaPais<<" "<<aresta.first<<" "<<aresta.second<<"Mbps"<<std::endl;
    }
    return os;
}

///////////////////////////Função auxiliar


/**
 * @brief Função responsável por calcular a velocidade da aresta
 * 
 * @param registro Ponteiro para o registro de dados
 * @return int Retorna a velocidade da aresta
 */

int calcula_velocidade(reg_dados* registro){

    if (registro->unidadeMedida[0] == 'G'){ //Se a unidade de medida for Giga, multiplica por 1024 para converter para Mega
        return registro->velocidade*1024;
    }
    else return registro->velocidade;

}


