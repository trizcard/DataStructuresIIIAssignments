#include "../headers/grafo.h"



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
//const std::map<int,int>& No::pegar_lista_de_arestas() const{return lista_de_arestas;}

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