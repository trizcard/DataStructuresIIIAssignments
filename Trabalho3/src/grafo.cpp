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

int Grafo::menor_caminho(int idOrigem, int idDestino){
    std::map<int,int> velocidade;
    std::map<int,int> antecessores;
    std::map<int,bool> visitados;
    std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::greater<std::pair<int,int>>> fila_de_prioridade;

    for (const auto& vertice: lista_de_nos){
        velocidade.insert({vertice.first, INT_MAX});
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

            if(velocidade.at(id_adjacente) > velocidade.at(id_atual) + peso){
                velocidade.at(id_adjacente) = velocidade.at(id_atual) + peso;
                antecessores.at(id_adjacente) = id_atual;
                fila_de_prioridade.push({velocidade.at(id_adjacente),id_adjacente});
            }
        }
    }

    if (velocidade.at(idDestino) >= INT_MAX){
        return -1;
    }

    return velocidade.at(idDestino);
}

int Grafo::fluxo_maximo(int idOrigem, int idDestino){
    // cria um map com a velocidade, os antecessores e uma flag se a aresta foi visitada (booleano)
    std::map<int,int> velocidade;
    std::map<int,int> antecessores;
    std::map<int,bool> visitados;

    std::priority_queue<std::pair<int,int>,std::vector<std::pair<int,int>>,std::greater<std::pair<int,int>>> fila_de_prioridade;

    int menor_velocidade = INT_MAX;
    for (const auto& vertice: lista_de_nos){
        velocidade.insert({vertice.first, 0}); // seta velocidades como 0
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
                if (menor_velocidade > peso){
                    menor_velocidade = peso;
                }
            }
        }
    }

    if (velocidade.at(idDestino) == 0){
        return -1;
    }

    // o menor valor da soma de velocidades é o maior caminho
    return (menor_velocidade);
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

