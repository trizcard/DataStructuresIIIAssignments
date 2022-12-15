#include "../headers/funcionalidades.h"
#include "../headers/funcAbertura.h"
#include "../headers/funcLeituraEscrita.h"
#include "../headers/funcChecagem.h"
#include "../headers/grafo.h"

/**
 * @brief Função responsável por realizar a funcionalidade 11 do programa, que lê um arquivo binário e imprime o grafo
 * 
 * @param nome_do_arquivo String que contém o nome do arquivo
 */

void comando11(char* nome_do_arquivo_entrada){

  FILE* arquivo_entrada = abrir_leitura_binario(nome_do_arquivo_entrada);
  if(arquivo_entrada == NULL) return;

  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();

  ler_reg_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  if (checa_consistencia(novo_reg_cabecalho) != 0){
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
    return;
  }

  Grafo grafo {arquivo_entrada}; //Cria o grafo a partir de um arquivo binário

  std::cout << grafo; //Imprime o grafo segundo o formato especificado

  //Fechar arquivo e liberar memória
  free(novo_reg_cabecalho);
  fclose(arquivo_entrada);
}

/**
 * @brief Função responsável por realizar a funcionalidade 12 do programa, que descobre quantos ciclos existem no grafo
 * 
 * @param nome_do_arquivo String que contém o nome do arquivo
 */

void comando12(char* nome_do_arquivo_entrada) {
  FILE* arquivo_entrada = abrir_leitura_binario(nome_do_arquivo_entrada);
  if(arquivo_entrada == NULL) return;

  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();

  ler_reg_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  if (checa_consistencia(novo_reg_cabecalho) != 0){
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
    return;
  }

  Grafo grafo {arquivo_entrada}; //Cria o grafo a partir de um arquivo binário

  int ciclos = grafo.cor_profundidade(); //Chama a função que faz a busca em profundidade, descobrindo quantos ciclos existem no grafo

  std::cout << "Quantidade de ciclos: " << ciclos <<std::endl; //Imprime a quantidade de ciclos

  //Fechar arquivo e liberar memória
  free(novo_reg_cabecalho);
  fclose(arquivo_entrada);

}

/**
 * @brief Função responsável por realizar a funcionalidade 13 do programa, que descobre o fluxo máximo entre dois vértices do grafo
 * 
 * @param nome_do_arquivo String que contém o nome do arquivo
 * @param n Número de vezes que a funcionalidade será executada
 */

void comando13(char* nome_do_arquivo_entrada, int n) {
  FILE* arquivo_entrada = abrir_leitura_binario(nome_do_arquivo_entrada);
  if(arquivo_entrada == NULL) return;

  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();

  ler_reg_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  if (checa_consistencia(novo_reg_cabecalho) != 0){
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
    return;
  }


  Grafo grafo {arquivo_entrada}; //Cria o grafo a partir de um arquivo binário
  int orig, fim, cMax; //Variáveis que armazenarão o vértice de origem, o vértice de destino e o fluxo máximo entre eles

  for (int i = 0; i < n; i++){ //Executa a funcionalidade n vezes
    scanf("%d %d", &orig, &fim);

    cMax = grafo.fluxo_maximo(orig, fim); //Chama a função que descobre o fluxo máximo entre dois vértices, com base em Edmonds-Karp

    if(cMax <= 0){ //Se não for possivel identificar o valor do fluxo maximo, imprime -1
      printf("Fluxo máximo entre %d e %d: -1\n", orig, fim);
    }
    else{ //Se for possivel identificar o valor do fluxo maximo, imprime o valor
      printf("Fluxo máximo entre %d e %d: %d Mbps\n", orig, fim, cMax);
    }
  }
  
  //Fechar arquivo e liberar memória
  free(novo_reg_cabecalho);
  fclose(arquivo_entrada);
}

/**
 * @brief Função responsável por realizar a funcionalidade 14 do programa, que descobre o caminho mínimo entre dois vértices do grafo
 * passando por um vértice intermediário obrigatório
 * 
 * @param nome_do_arquivo String que contém o nome do arquivo
 * @param n Número de vezes que a funcionalidade será executada
 */

void comando14(char* nome_do_arquivo_entrada, int n) {
  FILE* arquivo_entrada = abrir_leitura_binario(nome_do_arquivo_entrada);
  if(arquivo_entrada == NULL) return;

  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();

  ler_reg_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  if (checa_consistencia(novo_reg_cabecalho) != 0){
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
    return;
  }


  Grafo grafo {arquivo_entrada}; //Cria o grafo a partir de um arquivo binário
  int orig, fim, parada; //Variáveis que armazenarão o vértice de origem, o vértice de destino e vértice intermediário 
  int c1; //Variável que armazenará o caminho mínimo entre o vértice de origem e o vértice intermediário
  int c2; //Variável que armazenará o caminho mínimo entre o vértice intermediário e o vértice de destino

  for (int i = 0; i < n; i++){ //Executa a funcionalidade n vezes
    scanf("%d %d %d", &orig, &fim, &parada);

    c1 = grafo.menor_caminho(orig, parada); //Chama a função que descobre o caminho mínimo entre os vértices origem e intermediário, com djikstra
    c2 = grafo.menor_caminho(parada, fim); //Chama a função que descobre o caminho mínimo entre intermediário e destino, com djikstra
    
    if(c1 == -1 || c2 == -1){ //Se não for possivel identificar o valor do caminho mínimo, imprime -1
      printf("Comprimento do caminho entre %d e %d parando em %d: -1\n", orig, fim, parada); 
    }
    else{ //Se for possivel identificar o valor do caminho mínimo, imprime o valor
      printf("Comprimento do caminho entre %d e %d parando em %d: %dMbps\n", orig, fim, parada, (c1+c2));
    }
  }

  //Fechar arquivo e liberar memória
  free(novo_reg_cabecalho);
  fclose(arquivo_entrada);
}