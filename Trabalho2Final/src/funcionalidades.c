#include "funcionalidades.h"
/*
void funcSETE(char nomeArq[25], char nomeArqIndice[25]){
     //abrir o arquivo de entrada
    FILE *arqEntrada;
    arqEntrada = fopen(nomeArq, "wb");
    if(arqEntrada == NULL){
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    // verifica inconsistencias no arquivo
    char status;
    fread(&status, sizeof(char), 1, arqEntrada);
    if(status == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(arqEntrada);
        return;
    }

    // altera status do arquivo para 0
    fseek(arqEntrada, 0, SEEK_SET);
    status = '0';
    fwrite(&status, sizeof(char), 1, arqEntrada);   

    //Criar o arquivo de indice
    FILE *arqIndice;
    arqIndice = fopen(nomeArqIndice, "wb");
    if(arqIndice == NULL){
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    //Criar o cabecalho do arquivo de indice
    cabecalhoArv cabIndice;
    criarCabecalhoIndice(&cabIndice);

    //Adicionar dados arquivo de entrada ao arquivo de indice
    //percorrer arquivo de entrada
    fseek(arqEntrada, 960, SEEK_SET);
    int RRN = 0;
    int qtdRRN = pegarRRN(arqEntrada);
    while(RRN < qtdRRN){
        //criar o registro
        registro reg;

        //Inicializar Registro
        inicializarRegistro(&reg);

        //Pegar o registro do arquivo de entrada
        if(lerRegistro(arqEntrada, &reg)){
            //Adicionar o registro no arquivo de indice
                //adicionarRegArqIndice(arqIndice, &reg, RRN);
        }

        //Desalocar variaveis   
        desalocarRegistro(&reg);

        RRN++;
    }

    //Mudar o status do arquivo de indice para 1 
    fseek(arqIndice, 0, SEEK_SET);
    status = '1';
    fwrite(&status, sizeof(char), 1, arqIndice);

    //Mudar o status do arquivo de entrada para 1
    fseek(arqEntrada, 0, SEEK_SET);
    status = '1';
    fwrite(&status, sizeof(char), 1, arqEntrada);

    //Fechar o arquivo de entrada e de indice
    fclose(arqEntrada);
    fclose(arqIndice);

}
*/
void funcOITO(char nomeArq[25], char nomeArqDados[25], int n){
    // abre arquivo e verifica se funcionou de acordo com a funcao
    FILE *arq = NULL;
    arq = fopen(nomeArq, "rb");

    if (arq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    filtro *filtros;
    filtros = (filtro*) malloc(n * sizeof(filtro));

    // cria cabecalho
    cabecalho cab;
    cab.lixo = (char*) malloc(939*sizeof(char));
    lerCabecalho(arq, &cab);

    int flagJaLeu = 0;
    Lista *listaCompleta;
    listaCompleta = (Lista*) malloc(sizeof(Lista));

    int paginas;
    
    lerFiltros(filtros, n);
    for (int i = 0; i < n; i++){
        printf("Busca %d\n", (i+1));

        // caso seja busca pela chave
        if (!strcmp(filtros[i].nomeCampo, "idConecta")){
            // abre arquivo e verifica se funcionou de acordo com a funcao
            FILE *arqDados = NULL;
            arqDados = fopen(nomeArqDados, "rb");
            
            paginas = filtrarChave(arq, arqDados, filtros[i].valorCampo);
            fclose(arqDados);
        }

        // caso não seja
        else {
            if (flagJaLeu == 0){
                // cria lista com todos os registros
                lerArquivo(arq, &cab, listaCompleta);
                flagJaLeu = 1;
            }
            if (filtrar(&cab, listaCompleta, (filtros[i])) == 0){
                printf("Registro inexistente.\n");
            }
            paginas = cab.nPagDisco;
        }
        printf("Numero de paginas de disco: %d\n\n", paginas);
    }

    free(listaCompleta);
    free(filtros);
    fclose(arq);
}

void funcNOVE(char nomeArq[25], char nomeArqDados[25], int n){
    FILE *arq;
    arq = fopen(nomeArq, "rb+");
    if (arq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE *arqArvore;
    arqArvore = fopen(nomeArqDados, "rb+");
    if (arqArvore == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    cabecalho cab;
    cab.lixo = (char*) malloc(959 * sizeof(char));
    lerCabecalho(arq, &cab);

    cabecalhoArv cabArv;
    cabArv.lixo = (char*) malloc(55 * sizeof(char));
    lerCabecalhoArv(arqArvore, &cabArv);

    if (cab.status == '0' || cabArv.status == '0'){
        printf("Falha no processamento do arquivo.\n");
        free(cab.lixo);
        free(cabArv.lixo);
        fclose(arq);
        fclose(arqArvore);
        return;
    }
    char status = '0';
    fseek(arq, 0, SEEK_SET);
    fwrite(&status, 1, sizeof(char), arq);

    fseek(arqArvore, 0, SEEK_SET);
    fwrite(&status, 1, sizeof(char), arqArvore);

    registro regAux;
    alocaRegistro(&regAux);
    for (int i = 0; i < n; i++){
        regAux.removido = '0';
        regAux.encadeamento = -1;
        regAux.veloc = -1;
        
        entradaRegistro(&regAux);
        adicionarLixoCampFixo(&regAux);

        int RRN = cab.topo;
        if (RRN == -1){
            RRN = cab.proxRRN;
            fseek(arq, (960 + (RRN * 64)), SEEK_SET);
            adicionarRegArqSaida(arq, &regAux);
            cab.proxRRN ++;
        }
        else{
            int RRNRemovido = pegarRRNencadeado(arq, RRN);
            cab.topo = RRNRemovido;
            cab.nRegRemov--;
            fseek(arq, (960 + (RRN * 64)), SEEK_SET);
            adicionarRegArqSaida(arq, &regAux);
        }
        promovidos prom;
        prom.chave = -1;
        prom.RRN = -1;
        prom.filho = -1; 
        inserirArvore(arqArvore, regAux.idConecta, RRN, cabArv.noRaiz, &prom);

        float qtdRegPag = 960/64;
        float nroPagDisco = (RRN/qtdRegPag)+1;
        int nroPagDiscoInt = ceil(nroPagDisco); 

        if (nroPagDiscoInt > cab.nPagDisco){
            cab.nPagDisco = nroPagDiscoInt;
        }
    }
    cab.status = '1';
    atualizarCab(arq, &cab);
    desalocarRegistro(&regAux);
    free(cab.lixo);
    fclose(arq);
    fclose(arqArvore);
    binarioNaTela(nomeArq);
    binarioNaTela(nomeArqDados);
}

void funcDEZ(char nomeArq[25]){

}