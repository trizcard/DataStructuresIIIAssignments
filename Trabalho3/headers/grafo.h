#include <map>
#include <set>
#include <string>
#include <iostream>
#include <queue>
#include "../headers/estrutura.h"
#include "../headers/funcLeituraEscrita.h"
#include "../headers/funcChecagem.h"
#define BRANCO 0 //não visitado
#define AMARELO 1 //visitado
#define VERMELHO 2 //visitado e todas arestas visitadas

class No;

//função auxiliar para o grafo
int calcula_velocidade(reg_dados* registro);

class Grafo{
    //atributos
    private:
        std::map<int,No> lista_de_nos; //ordena as chaves idConecta(de tipo int) e guarda Nos(que correspondem aos vértices).
    
    //métodos privados
        void adicionar_no(const No& no);
    
    //construtor
    public:
        Grafo(FILE* arquivo_entrada);

    //função
        friend std::ostream& operator<<(std::ostream& os,const Grafo& grafo); 
        int menor_caminho(int idOrigem, int idDestino);
        int fluxo_maximo(int idOrigem, int idDestino);
        void busca_profundidade( int idOrigem,  std::map <int,int>& cor, int *ciclos, int noOriginal ) const;
        int cor_profundidade() const;

};

class No{
    //atributos
    private:
        std::map<int,int> lista_de_arestas; //ordena os idPopsConecta internos, com os pesos
        int idConecta;
        std::string nomePoPs;
        std::string nomePais;
        std::string siglaPais;
    
    //construtor
    public:
        No(reg_dados* registro);
        No(int idConecta);

    //métodos
        int pegar_idConecta() const;
        const std::map<int,int>& pegar_lista_de_arestas() const;
        void adicionar_aresta(int idPoPsConectado, int velocidade);
        void completar_no(const No& no_completo);

    //atributo público
        bool completo;
    
    //funcao 
        friend std::ostream& operator<<(std::ostream& os,const No& no);



};

