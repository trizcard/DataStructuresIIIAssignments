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

/*void comando12(char* nome_do_arquivo_entrada) {

}

void comando13(char* nome_do_arquivo_entrada, int n) {

}
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

    
    Grafo grafo {arquivo_entrada};
    int orig, fim, parada, cMin;
    
    for (int i = 0; i < n; i++){
        scanf("%d %d %d", &orig, &fim, &parada);
        cMin = 0;
        // implementar Algoritmo de Dijkstra para encontrar o caminho minimo
        cMin += grafo.menor_caminho(orig, parada);
        cMin += grafo.menor_caminho(parada, fim);
        if(cMin > 10000000){
          printf("Comprimento do caminho entre %d e %d parando em %d: -1\n", orig, fim, parada);
        }
        else{
          printf("Comprimento do caminho entre %d e %d parando em %d: %dMbps\n", orig, fim, parada, cMin);
        }
    }
}
