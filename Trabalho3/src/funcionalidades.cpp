#include "../headers/funcionalidades.h"
#include "../headers/funcAbertura.h"
#include "../headers/funcLeituraEscrita.h"
#include "../headers/funcChecagem.h"
#include "../headers/grafo.h"

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

  
  Grafo grafo {arquivo_entrada};

  std::cout << grafo;

  free(novo_reg_cabecalho);

  fclose(arquivo_entrada);
}

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

    Grafo grafo {arquivo_entrada};

    //Chamar a função que faz a busca em profundidade
    int ciclos = grafo.cor_profundidade();


    //Imprimir o resultado<
    std::cout << "Quantidade de ciclos: " << ciclos <<std::endl;
    
    //Fechar arquivo e liberar memória
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);

}

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

    
    Grafo grafo {arquivo_entrada};
    int orig, fim, cMax;
    
    for (int i = 0; i < n; i++){
        scanf("%d %d", &orig, &fim);
        cMax = 0;
        // implementar Algoritmo de Dijkstra reverso para encontrar o caminho maximo
        cMax += grafo.fluxo_maximo(orig, fim);
        if(cMax <= 0){
          printf("Fluxo máximo entre %d e %d: -1\n", orig, fim);
        }
        else{
          printf("Fluxo máximo entre %d e %d: %d Mbps\n", orig, fim, cMax);
        }
    }
    
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
}

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

    
    Grafo grafo {arquivo_entrada};
    int orig, fim, parada, c1, c2;
    
    for (int i = 0; i < n; i++){
        scanf("%d %d %d", &orig, &fim, &parada);
        
        // implementar Algoritmo de Dijkstra para encontrar o caminho minimo
        c1 = grafo.menor_caminho(orig, parada);
        c2 = grafo.menor_caminho(parada, fim);
        if(c1 == -1 || c2 == -1){
          printf("Comprimento do caminho entre %d e %d parando em %d: -1\n", orig, fim, parada);
        }
        else{
          printf("Comprimento do caminho entre %d e %d parando em %d: %dMbps\n", orig, fim, parada, (c1+c2));
        }
    }
    
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
}