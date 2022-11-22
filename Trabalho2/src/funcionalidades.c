#include "funcionalidades.h"

void funcUM(char nomeArq[25], char nomeArq2[25]){
    //abrir o arquivo de entrada
    FILE *arqEntrada;
    arqEntrada = fopen(nomeArq, "r");
    if(arqEntrada == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //abrir o arquivo de saida
    FILE *arqSaida;
    arqSaida = fopen(nomeArq2, "wb");

    //criar o cabecalho
    cabecalho cab;
    criarCabecalho(&cab);   

    //adicionar o cabecalho no arquivo de saida
    adicionarCabecalhoArq(arqSaida, &cab);


    //Variavel Contabilizar pagDisco
    int qtdByte= 960;
    int qtdPagDisco = 1;

    //variável para contabilizar RRN
    int RRN = 0;


    char *linha = (char*) malloc(200 * sizeof(char));
    //Pular a primeira linha que conta o nome das colunas
    fgets(linha, 200, arqEntrada);

    //ler os REGISTROS do arquivo de entrada
    while(fgets(linha, 200, arqEntrada)){
        //criar o registro
        registro reg;
        //inicializar o registro
        inicializarRegistro(&reg);
    
        //Pegar os campos do registro
        //Função que separa os campos e chama função para adicionar os valores no registro
        quebrarString(linha, &reg);
        
        //Verificar se o registro cabe na pagina de disco
        if(qtdByte + 64 > TAMANHO_REG){
            qtdPagDisco++;
            qtdByte = 64;       
        }else{
            qtdByte += 64; 
        }
        
        //Adicionar o registro no arquivo de saida
        adicionarRegArqSaida(arqSaida,&reg);
        RRN++;
        //imprimir registro para teste imprimirRegistro(&reg);

        //Desalocar variaveis   
        desalocarRegistro(&reg);
    }
    //Desalocar linha de entrada
    free(linha);
    
    //atualizar o cabecalho no arquivo de saida
    cab.nPagDisco = qtdPagDisco;
    atualizarCabecalho(arqSaida, &cab, RRN, 0);
    //Desalocar campo cabeçalho
    //Desalocarlixo cab
    free(cab.lixo);

    //fechar os arquivos
    fclose(arqEntrada);
    fclose(arqSaida);

    //imprimir o resultado
    binarioNaTela(nomeArq2);
}

void funcDOIS(char nomeArq[25]){
    // abre arquivo e verifica se funcionou de acordo com a funcao
    FILE *arq = NULL;
    arq = fopen(nomeArq, "rb");

    if (arq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    imprimirSaida(arq);
    fclose(arq);
}

void funcTRES(char nomeArq[25]){
    // abre arquivo e verifica se funcionou de acordo com a funcao
    FILE *arq = NULL;
    arq = fopen(nomeArq, "rb");

    if (arq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    int n;
    scanf("%d", &n);
    filtro *filtros;
    filtros = (filtro*) malloc(n * sizeof(filtro));

    lerFiltros(filtros, n);
    filtrar(arq, filtros, n);
    free(filtros);
    fclose(arq);
}

void funcQUATRO(char nomeArq[25]){
    // abre arquivo e verifica se funcionou de acordo com a funcao
    FILE *arq = NULL;
    arq = fopen(nomeArq, "rb+");
    int flag;

    if (arq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    flag = remover(arq);
    fseek(arq, 0, SEEK_SET);
    char status = '1';
    fwrite(&status, sizeof(char), 1, arq);
    fclose(arq);
    if (flag){
        binarioNaTela(nomeArq);
    }
}

void funcCINCO(char nomeArq[25], int n){
    //abrir o arquivo de entrada
    FILE *arqEntrada;
    arqEntrada = fopen(nomeArq, "rb+");
    if(arqEntrada == NULL){
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    // Verificar status do arquivo
    char status;
    fread(&status, sizeof(char), 1, arqEntrada);
    if (status == '0'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Alterar status do arquivo para 0
    fseek(arqEntrada, 0, SEEK_SET);
    status = '0';
    fwrite(&status, sizeof(char), 1, arqEntrada);
    fseek(arqEntrada, 0, SEEK_SET);

    //Entrada de n registros
    for(int i = 0; i < n; i++){
        //criar o registro
        registro reg;

        //Inicializar Registro
        inicializarRegistro(&reg);

        //Entrada dos campos do registro
        entradaRegistro(&reg);

        //Adicionar lixo nos campo de tamanho fixo que não foram preenchidos
        adicionarLixoCampFixo(&reg);

        //Verificar se tem algum registro removido no arquivo, pega o topo
        int RRN = verificarRemovido(arqEntrada);

        //Se tiver, adicionar o registro no lugar do registro removido
        if(RRN != -1){
            //Pegar o RRN do registro removido anterior para adicionar na pilha (Topo)
            int RRNRemovido = pegarRRNencadeado(arqEntrada, RRN);

            //Adicionar o registro no arquivo de saida
            fseek(arqEntrada, (960 + (RRN * 64)), SEEK_SET);
            adicionarRegArqSaida(arqEntrada, &reg);

            //Atualizar o encadeamento do registro removido
            atualizarTopo(arqEntrada,RRNRemovido);

            //Atualizar o nroRegRemovido
            atualizarNroRegRemovidos(arqEntrada);
        
        }else{ //Se não tiver, adicionar o registro no final do arquivo
            RRN = pegarRRN(arqEntrada);
            
            fseek(arqEntrada, (960 + (RRN * 64)), SEEK_SET);
            adicionarRegArqSaida(arqEntrada, &reg);

            //Atualizar proxRRN
            atualizarRRN(arqEntrada, RRN);

            //Atualizar o nroPAgDisco
            atualizarNroPagDisco(arqEntrada, RRN);
        }

        //desalocar registro
        desalocarRegistro(&reg);
    }

    // alterar status do arquivo para 1
    fseek(arqEntrada, 0, SEEK_SET);
    status = '1';
    fwrite(&status, sizeof(char), 1, arqEntrada);

    //fechar o arquivo
    fclose(arqEntrada);

    //imprimir o resultado
    binarioNaTela(nomeArq); 
}

void funcSEIS(char nomeArq[25]){

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

    // verificar quatidade de RRN no arquivo de entrada
    fseek(arqEntrada, 5, SEEK_SET);
    int qtdRRN;
    fread(&qtdRRN, sizeof(int), 1, arqEntrada);

    // Verificar se há registros removidos
    int nroRegRemovidos = pegarNroRegRemovidos(arqEntrada);

    if (nroRegRemovidos != 0){//Se tiver, remover os registros removidos
        //Abrir outro arquivo para salvar os registros não removidos
        FILE *arqSaida;
        arqSaida = fopen("arquivo.bin", "wb");
        if (arqSaida == NULL){
            printf("Falha no carregamento do arquivo.\n");
            return;
        }

        //copiar o cabeçalho
        cabecalho cab;
        criarCabecalho(&cab);

        //adicionar o cabecalho no arquivo de saida
        adicionarCabecalhoArq(arqSaida, &cab);

        //Variavel Contabilizar pagDisco
        int qtdByte = 0;
        cab.nPagDisco++;

        //variável para contabilizar RRN e variavel do RRN atual do arquivo de entrada
        int RRN = 0;
        int RRNatual = 0;

        //Pular o cabeçalho para percorrer o arquivo
        fseek(arqEntrada, 960, SEEK_SET);

        //Percorrer os registros e ir adicionando no arquivo de saida
        while(RRNatual < qtdRRN){
            //criar o registro
            registro reg;

            //Inicializar Registro
            inicializarRegistro(&reg);

            //Pegar o registro do arquivo de entrada
            if(lerRegistro(arqEntrada, &reg)){
                //Verificar se o registro cabe na pagina de disco
                if(qtdByte + 64 > TAMANHO_REG){
                    cab.nPagDisco++;
                    qtdByte = 64;       
                }else{
                    qtdByte += 64; 
                }

                //Adicionar o registro no arquivo de saida
                adicionarRegArqSaida(arqSaida,&reg);
                RRN++;
            }

            //Desalocar variaveis   
            desalocarRegistro(&reg);

            RRNatual++;
        }
       

        //Atualizar o cabecalho
        int quantidadeCompactada = pegarQuantidadeCompactada(arqEntrada);
        quantidadeCompactada++;
        atualizarCabecalho(arqSaida, &cab, RRN, quantidadeCompactada);

        //alterar status do arquivo para 1
        fseek(arqEntrada, 0, SEEK_SET);
        status = '1';
        fwrite(&status, sizeof(char), 1, arqEntrada);

        //Desalocarlixo cab
        free(cab.lixo);
        
        //fechar o arquivo
        fclose(arqEntrada);
        fclose(arqSaida);
        
        //Mudar os arq
        remove(nomeArq);
        rename("arquivo.bin", nomeArq);
       
    }else{
        atualizarQuantidadeCompactada(arqEntrada);
        //fechar o arquivo de entrada
        fclose(arqEntrada);
    }

    
    //imprimir o resultado
    binarioNaTela(nomeArq);  
}

void funcSETE(char nomeArq[25]){

}

void funcOITO(char nomeArq[25]){
    int n;
    scanf("%d", &n);
}

void funcNOVE(char nomeArq[25]){

}

void funcDEZ(char nomeArq[25]){

}