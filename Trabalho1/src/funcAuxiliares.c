#include "funcAuxiliares.h"

void addLixo(char *lixo, int inicio, int tam){
    for (int i = inicio; i < (inicio + tam); i++){
        lixo[i] = LIXO;
    }
}

// Atualiza cabeçalho a quantidade de compactações
void atualizarQuantidadeCompactada(FILE* arqSaida){
    int quantidadeCompactada = pegarQuantidadeCompactada(arqSaida);
    quantidadeCompactada++;
    fseek(arqSaida, 17, SEEK_SET);
    fwrite(&quantidadeCompactada, sizeof(int), 1, arqSaida);
}	

// Le a quantidade de compactações do arquivo
int pegarQuantidadeCompactada(FILE *arq){
    int qtdCompactada;
    fseek(arq, 17, SEEK_SET);
    fread(&qtdCompactada, sizeof(int), 1, arq);
    return qtdCompactada;
}

// Função de quebrar a string do csv
void quebrarString(char *linha, registro *reg){
    // São 7 campos, ent  6 virgulas   
    int i = 0;
    int qtdVirgulas = 0;
    
    //Percorrer a string linha toda
    while(linha[i] != '\n'){
        if(qtdVirgulas == 0){
            //string auxiliar para guardar o idConecta
            char *idConecta = (char*) malloc(10 * sizeof(char));
            percorrerCampo(&reg->idConecta, &i, linha, idConecta, 1);
            free(idConecta);
            qtdVirgulas++;
        } else if(qtdVirgulas == 1){//Percorrer o campo nomePoPs
            //string auxiliar para guardar o nomePoPs
            char *nomePoPs = (char*) malloc(44 * sizeof(char));
            percorrerCampo(reg->nomePoPs, &i, linha, nomePoPs, 0);
            free(nomePoPs);
            qtdVirgulas++;
        }else if(qtdVirgulas == 2){
            //String auxiliar para guardar o nomePais
            char *nomePais = (char*) malloc(44 * sizeof(char));
            percorrerCampo(reg->nomePais, &i, linha, nomePais, 0);
            free(nomePais);
            qtdVirgulas++;         
        }else if(qtdVirgulas == 3){
            //string auxiliar para guardar o siglaPais
            char *siglaPais = (char*) malloc(3 * sizeof(char));
            percorrerCampo(reg->siglaPais, &i, linha, siglaPais, 0);
            free(siglaPais);
            verificarCampoString(reg->siglaPais, 2, 0);
            qtdVirgulas++;
        }else if(qtdVirgulas == 4){
            // String auxiliar para guardar o idPoPsConec
            char *idPoPsConec = (char*) malloc(10 * sizeof(char));
            percorrerCampo(&reg->idPoPsConec, &i, linha, idPoPsConec, 1);
            free(idPoPsConec);
            qtdVirgulas++; 
        }else if(qtdVirgulas == 5){
            //String auxiliar para guardar o undMedida
            char *undMedida = (char*) malloc(2 * sizeof(char));
            percorrerCampo(reg->undMedida, &i, linha, undMedida, 0);
            free(undMedida);
            verificarCampoString(reg->undMedida, 1, 0);
            qtdVirgulas++;         
        }else if(qtdVirgulas == 6){
            //string auxiliar para guardar a veloc
            char *veloc = (char*) malloc(10 * sizeof(char));
            percorrerCampo(&reg->veloc, &i, linha, veloc, 1);
            free(veloc);
            qtdVirgulas++;
        }
        i++;
    }  
}

// Verificar se o campo de tamanho fixo existe e adicionar o lixo caso não tenha sido preenchido
void verificarCampoString(char *campo, int tam, int inicio){
    if (!strcmp(campo, "")){
        addLixo(campo, inicio, tam);
    }   
}

// Função que adiciona o campo de tamanho variável no arquivo de saída
void adicionarCampoVariavel(FILE *arqSaida, char *campo){
    if(strcmp(campo, "")){
        fwrite(campo, sizeof(char) , strlen(campo), arqSaida);
    }
    fwrite("|", sizeof(char), 1, arqSaida);
}

// Função que adiciona o valor do campo Int no registro
void addValorInt(int *set, int flag, char *valor){
    if(flag){
        *set = atoi(valor);
    }else{
        *set = -1;
    }
}

// Função que adiciona o valor do campo String no registro
void addValorString(char *set, int flag, char* valor){
    if(flag){
        strcpy(set, valor);
    }else{
        strcpy(set, "");
    }
}

// Percorrer campo da linha registro de entrada e adicionar no registro
void percorrerCampo(void *campo, int *posi, char *linha, char *varAux, int inteiro){
    int j = 0;
            
    //Flag para saber se tem valor setado no campo
    int flag=0;

    //Percorre o campo, até encontrar o separador
    while(linha[*posi] != ',' && linha[*posi] != '\0'){
        varAux[j] = linha[*posi];
        if(!flag){
        flag = 1;
        }
        *posi += 1;
        j++;
    }

    if(varAux[j-1] == ' '){
        varAux[j-1] = '\0';
    }else{
        varAux[j] = '\0';
    }

    //Se tem valor, converter para inteiro e adicionar no registro
    if(inteiro){
        addValorInt(campo, flag, varAux);
    }else{
        addValorString(campo, flag, varAux);
    }
}

// Função que pega a entrada de Registro
void entradaRegistro(registro *reg){
    //Pegar o idConecta
    char *idConecta = (char*) malloc(10 * sizeof(char));
    scanf("%s ",idConecta );
    verificaCampoNulo(idConecta, &reg->idConecta,1);
    free(idConecta);

    //Pegar o nomePais
    scan_quote_string(reg->nomePais);
    verificaCampoNulo(reg->nomePais, reg->nomePais, 0);

    //Pegar o nomePoPs
    scan_quote_string(reg->nomePoPs);
    verificaCampoNulo(reg->nomePoPs, &reg->nomePoPs, 0);

    //Pegar a siglaPais
    scan_quote_string(reg->siglaPais);
    verificaCampoNulo(reg->siglaPais,&reg->siglaPais, 0);

    //Pegar o idPoPsConec
    char *idPoPsConec = (char*) malloc(10 * sizeof(char));
    scanf("%s ", idPoPsConec);
    verificaCampoNulo(idPoPsConec, &reg->idPoPsConec,1);
    free(idPoPsConec);

    //Pegar a undMedida
    scan_quote_string(reg->undMedida);
    verificaCampoNulo(reg->undMedida, &reg->undMedida, 0);

    //Pegar a veloc
    char *veloc = (char*) malloc(10 * sizeof(char));
    scanf("%s", veloc);
    verificaCampoNulo(veloc, &reg->veloc,1);
    free(veloc);
}

// Função que adiciona o registro no arquivo de saída
void adicionarRegArqSaida(FILE *arqSaida, registro *reg){
    //Adicionar os campos no registro
    //Campos tamanho fixo
    fwrite(&reg->removido, sizeof(char), 1, arqSaida);
    fwrite(&reg->encadeamento, sizeof (int), 1, arqSaida);
    fwrite(&reg->idConecta, sizeof (int), 1, arqSaida);
    fwrite(reg->siglaPais, sizeof (char), 2, arqSaida);
    fwrite(&reg->idPoPsConec, sizeof (int), 1, arqSaida);
    fwrite(reg->undMedida, sizeof(char), 1, arqSaida);
    fwrite(&reg->veloc, sizeof (int), 1, arqSaida);
    
    //Campos tamanho variavel
    adicionarCampoVariavel(arqSaida, reg->nomePoPs);
    adicionarCampoVariavel(arqSaida, reg->nomePais);

    //Adicionar lixo no restante da página de disco
    int qtdLixoFinal = 64 - (strlen(reg->nomePoPs) + strlen(reg->nomePais) + 22);
    char *lixo = malloc(qtdLixoFinal * sizeof(char));
    addLixo(lixo, 0, qtdLixoFinal);
    fwrite(lixo, sizeof(char) , qtdLixoFinal, arqSaida);
    free(lixo);
}

void adicionarLixoCampFixo(registro *reg){
    //Verificar se o campo é nulo
    verificarCampoString(reg->siglaPais, 2, 0);
    verificarCampoString(reg->undMedida, 1, 0);
}

// Implementar lista dinamica encadeada de registros
void adicionarListaReg(Lista *lista, registro *regOrig){
    elemento *novo;
    novo = (elemento*) malloc(sizeof(elemento));
    novo->reg = (registro*) malloc(sizeof(registro));
    passarReg(novo->reg, regOrig);
    novo->prox = NULL;

    if ((*lista) == NULL){
        *lista = novo;
    } else {
        elemento *aux;
        aux = *lista;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

// Desaloca a lista dinamica encadeada de registros
void libera_lista(Lista* li){
    if(li != NULL){
        elemento* no;
        while((*li) != NULL){
            no = *li;
            *li = (*li)->prox;
            free(no->reg->nomePais);
            free(no->reg->nomePoPs);
            free(no->reg->undMedida);
            free(no->reg->siglaPais);
            free(no->reg);
            free(no);
        }
        free(li);
    }
}

// Passa as informações do registro origem para o registro destino
void passarReg(registro *reg, registro *regOrig){
    reg->idConecta = regOrig->idConecta;

    reg->siglaPais = malloc(3 * sizeof(char));
    reg->siglaPais[0] = regOrig->siglaPais[0];
    reg->siglaPais[1] = regOrig->siglaPais[1];
    reg->siglaPais[2] = '\0';

    reg->idPoPsConec = regOrig->idPoPsConec;
    
    reg->undMedida = malloc(2 * sizeof(char));
    reg->undMedida[0] = regOrig->undMedida[0];
    reg->undMedida[1] = '\0';

    reg->veloc = regOrig->veloc;

    reg->nomePais = malloc(strlen(regOrig->nomePais) * sizeof(char));
    strcpy(reg->nomePais, regOrig->nomePais);
    reg->nomePoPs = malloc(strlen(regOrig->nomePoPs) * sizeof(char));
    strcpy(reg->nomePoPs, regOrig->nomePoPs);
}

// Verifica se o campo de inteiro é nulo, caso que 
void converterInt(int *campo, int flagNulo, char *valor){
    if(flagNulo){
        *campo = -1;
    }else{
        *campo = atoi(valor);
    }
}

// Função que verifica se entrada é nula
void verificaCampoNulo(char* campo, void *campoReg, int flagInt){
    if(!strcmp(campo, "NULO")){
        if(flagInt == 1){
            converterInt(campoReg, 1, NULL);
        }else{
            strcpy(campoReg, "");
        }
    }else{
        if(flagInt == 1){
            converterInt(campoReg, 0, campo);
        }
    }
}

//Verificar se tem algum registro removido no arquivo, pega o topo
int verificarRemovido(FILE* arqSaida){
    int removido;
    fseek(arqSaida, 1, SEEK_SET);
    fread (&removido, sizeof(int), 1, arqSaida);
    return removido;
}

// Função que atualiza o nro de registro Removidos
void atualizarNroRegRemovidos(FILE* arqSaida){
    //Pegar o numero de registros removidos e decrementar 1
    int regRemovidos = pegarNroRegRemovidos(arqSaida);
    regRemovidos--;
    //Atualizar o numero de registros removidos
    fseek(arqSaida,9, SEEK_SET);
    fwrite(&regRemovidos, sizeof(int), 1, arqSaida);
}

// Função atualiza o encademaneto do topo da pilha
void atualizarTopo(FILE* arqSaida, int RRN){
    fseek(arqSaida, 1, SEEK_SET);
    fwrite(&RRN, sizeof(int), 1, arqSaida);
}

// Função que atualiza o RRN do próximo registro
void atualizarRRN(FILE* arqSaida, int RRN){
    int proxRRN = RRN+1;
    fseek(arqSaida, 5, SEEK_SET);
    fwrite(&proxRRN, sizeof(int), 1, arqSaida);
}

// Função que atuliza o nroPagDisco
void atualizarNroPagDisco(FILE* arqSaida, int RRN){
    float qtdRegPag = 960/64;
    float nroPagDisco = (RRN/qtdRegPag)+1;
    int nroPagDiscoInt = ceil(nroPagDisco); 
    fseek(arqSaida, 13, SEEK_SET);
    fwrite(&nroPagDiscoInt, sizeof(int), 1, arqSaida);
}

// Pega o numero de registros removidos
int pegarNroRegRemovidos(FILE* arqSaida){
    int nroRegRemovidos;
    fseek(arqSaida, 9, SEEK_SET);
    fread(&nroRegRemovidos, sizeof(int), 1, arqSaida);
    return nroRegRemovidos;
}

//Pega o RRN do próximo registro removido
int pegarRRNencadeado(FILE* arqSaida, int RRN){
    int encadeamento;
    fseek(arqSaida, (1 +(960 + (RRN * 64))), SEEK_SET);
    fread(&encadeamento, sizeof(int), 1, arqSaida);
    return encadeamento;
}

// Pega o RRN do próximo registro
int pegarRRN(FILE* arqSaida){
    int RRN;
    fseek(arqSaida, 5, SEEK_SET);
    fread(&RRN, sizeof(int), 1, arqSaida);
    return RRN;
}
