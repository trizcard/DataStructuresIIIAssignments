#include "funcionalidades.h"


void funcSETE(char nomeArq[25], char nomeArqIndice[25]){
     //abrir o arquivo de entrada
    FILE *arqEntrada;
    arqEntrada = fopen(nomeArq, "rb");
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
    arqIndice = fopen(nomeArqIndice, "wb+");
    if(arqIndice == NULL){
        printf("Falha no carregamento do arquivo.\n");
        return;
    }
    //Criar o cabecalho do arquivo de indice
    cabecalhoArv cabIndice;
    criarCabArqIndice(arqIndice, &cabIndice);
    //Adicionar dados arquivo de entrada ao arquivo de indice
    //percorrer arquivo de entrada
    int RRN = 0;
    int qtdRRN = pegarRRN(arqEntrada);
    fseek(arqEntrada, 960, SEEK_SET);//Pular o cabecalho
    while(RRN < qtdRRN){
        //criar o registro
        registro reg;
        //Inicializar Registro
        alocaRegistro(&reg);
        //Pegar o registro do arquivo de entrada
        if(lerRegistro(arqEntrada, &reg)){
            //Adicionar o registro no arquivo de indice
            promovidos prom;
            prom.chave = -1;
            prom.RRN = -1;
            prom.filho = -1; 
            int promovido;

            promovido = inserirArv(arqIndice, reg.idConecta, RRN, cabIndice.noRaiz, &prom, &cabIndice);
            if (promovido == 1){
                cabIndice.alturaArvore = 1;
                cabIndice.noRaiz = criaRaiz(arqIndice, prom.chave, prom.RRN, cabIndice.noRaiz, prom.filho);
            }
            cabIndice.nroChavesTotal++;

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

    binarioNaTela(nomeArqIndice);
}


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
        int promovido;


        promovido = inserirArv(arqArvore, regAux.idConecta, RRN, cabArv.noRaiz, &prom, &cabArv);
        if (promovido == 1){
            cabArv.alturaArvore++;
            cabArv.noRaiz = criaRaiz(arqArvore, prom.chave, prom.RRN, cabArv.noRaiz, prom.filho);
        }
        cabArv.nroChavesTotal++;

        float qtdRegPag = 960/64;
        float nroPagDisco = (RRN/qtdRegPag)+1;
        int nroPagDiscoInt = ceil(nroPagDisco); 

        if (nroPagDiscoInt > cab.nPagDisco){
            cab.nPagDisco = nroPagDiscoInt;
        }
    }
    cab.status = '1';
    cabArv.status = '1';
    atualizarCab(arq, &cab);
    atualizarCabArv(arqArvore, &cabArv);
    desalocarRegistro(&regAux);
    free(cab.lixo);
    fclose(arq);
    fclose(arqArvore);
    binarioNaTela(nomeArq);
    binarioNaTela(nomeArqDados);
}




void funcDEZ(char nomeArq1[25], char nomeArq2[25],char nomeCampoArq1[25], char nomeCampoArq2[25], char nomeArqIndex[25]){
  //Abrir o arquivo de entrada 1
    FILE *arqEntrada;
    arqEntrada = abrirArquivo(nomeArq1, "rb");

    //Abrir o arquivo de entrada 2
    FILE *arqEntrada2;
    arqEntrada2 = abrirArquivo(nomeArq2, "rb");

    //Abrir o arquivo de indice
    FILE *arqIndice;
    arqIndice = abrirArquivo(nomeArqIndex, "rb");

    //Percore o arquivo de entrada
    int RRN = 0;
    int qtdRRN = pegarRRN(arqEntrada);
    fseek(arqEntrada, 960, SEEK_SET);//Pular o cabecalho
   

    while(RRN < qtdRRN){
        //criar o registro
        registro reg;

        //Inicializar Registro
        alocaRegistro(&reg);

        //Pegar o registro do arquivo de entrada
        if(lerRegistro(arqEntrada, &reg)){
           //Pegar o valor do campo de busca do reg entrada
            char *valorCampo;
            valorCampo = (char*) malloc(100*sizeof(char));
            strcpy(valorCampo, pegarValorCampo(&reg, nomeCampoArq1));

            //Buscar o valor do campo 
            if (verificarChave(nomeCampoArq2)){
               //Fazer busca pelo indice
               fseek(arqIndice, 0, SEEK_SET);
               cabecalhoArv cabArvore;
               cabArvore.lixo = (char*)malloc(49*sizeof(char));
               lerCabecalhoArv(arqIndice, &cabArvore);

               int chave = atoi(valorCampo);
               int RRN = 0;
               int posiNO = 0;
               int pagAcessadas = 2;// inicia com 2, pois conta a pagina do cabeçalho dos dois arquivos
                
               pagAcessadas = buscarArvore(arqIndice, cabArvore.noRaiz, chave, &RRN, &posiNO, pagAcessadas);
               //Imprimir o valor encontrado
               if (pagAcessadas !=0){
                no pagAux;
                fseek(arqIndice, ((RRN+1) * TAMANHO_REG_DADOS), SEEK_SET);
                lerPgDados(arqIndice, &pagAux);

                // imprime registro
                fseek(arqEntrada2, ((pagAux.CP[posiNO].Pr) * TAMANHO_REG) + 960, SEEK_SET);
                
                
                registro reg2;
                alocaRegistro(&reg2);
                pagAcessadas++; // acesso a pagina do registro
                lerRegistro(arqEntrada2, &reg2);
    
                //Imprimir o registro
                imprimeJoin(&reg, &reg2);                

                //Desalocar Registros 
                desalocarRegistro(&reg2);          
               }                            
            }else{
                //Fazer busca sequencial no arquivo de entrada 2
                // cria filtro
                filtro fil;
                //Copiar o campo e o valor no filtro 
                strcpy(fil.nomeCampo, nomeCampoArq2);
                //Copiar o 
                strcpy(fil.valorCampo, valorCampo);

                //Percorrer o arquivo de entrada 2
                int RRN = 0;
                int qtdRRN = pegarRRN(arqEntrada2);
                //Pular o cabecalho
                fseek(arqEntrada2, 960, SEEK_SET);
                while(RRN < qtdRRN){
                    //criar o registro
                    registro reg2;

                    //Inicializar Registro
                    alocaRegistro(&reg2);

                    //Pegar o registro do arquivo de entrada
                    if(lerRegistro(arqEntrada2, &reg2)){
                        //Verificar se o registro tem o valor do campo
                        if (analisarCampo(fil, &reg2)){
                            //Imprimir o registro
                            imprimeJoin(&reg, &reg2);
                        }
                    }
                    //Desalocar o registro
                    desalocarRegistro(&reg2);
                    //Incrementar o RRN
                    RRN++;
                }                
            }

            //Liberar valorCampo
            free(valorCampo);
        }

        //Desalocar variaveis   
        desalocarRegistro(&reg);

        RRN++;
    }


    //Fechar arquivos 
    fclose(arqEntrada);
    fclose(arqEntrada2);
    fclose(arqIndice);


}