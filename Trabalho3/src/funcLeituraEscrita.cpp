#include "../headers/funcLeituraEscrita.h"
#include "../headers/funcFornecidas.h"
/*

Função responsável por ler o registro de cabeçalho de um arquivo binário e colocar numa struct. Recebe um ponteiro para
arquivo e um ponteiro para uma struct do tipo (reg_cabecalho). Os campos são lidos em sequência conforme o número de bytes
ocupados. É colocado na segunda posição de "status" do registro um '\0' para tratamento interno do programa como string.
Ao fim, é dado um (fseek) para o fim do registro de cabeçalho, dado que o tamanho é sempre fixo em uma página de disco,
utilizando o cálculo do tamanho da página de disco - o tamanho do registro de cabeçalho.

*/
void ler_reg_cabecalho(FILE* arquivo, reg_cabecalho* reg){
    fread(reg->status, sizeof(char), 1, arquivo);
    reg->status[1] = '\0';
    fread(&reg->topo, sizeof(int), 1, arquivo);
    fread(&reg->proxRRN, sizeof(int), 1, arquivo);//lê o próximo RRN disponível
    fread(&reg->nroRegRem, sizeof(int), 1, arquivo);//lê o número de registros removidos
    fread(&reg->nroPagDisco, sizeof(int), 1, arquivo);//lê o número de páginas de disco
    fread(&reg->qttCompacta, sizeof(int), 1, arquivo);//lê o qttCompacta
    fseek(arquivo, (TAM_PAG_DISCO - TAM_REG_CABECALHO), SEEK_CUR);//pula o lixo
}

/*

Função responsável por ler todos os campos do registro. Recebe um ponteiro para 
arquivo e um ponteiro para uma struct do tipo (reg_dados). 
Então, sao lidos os campos de tamanho fixo com a função le_campos_tam_fixo() e 
os campos de tamanho variavel com a função le_campos_tam_variavel()

*/
void le_registro(reg_dados* reg, FILE* arquivo_entrada){
  le_campos_tam_fixo(reg,arquivo_entrada);
  le_campos_tam_variavel(reg,arquivo_entrada);
}


/*

Função responsável por ler os campos de tamanho fixo de um registro de dados. Recebe um ponteiro para
arquivo e um ponteiro para uma struct do tipo (reg_dados). 
Os campos são lidos em sequência conforme o número de bytes ocupados, com a função fread().

*/
void le_campos_tam_fixo(reg_dados* reg, FILE* arquivo_entrada){

  // le campos fixos
    fread(&reg->encadeamento, sizeof(int), 1, arquivo_entrada);
    fread(&reg->idConecta,sizeof(int), 1, arquivo_entrada);
    fread(reg->siglaPais,sizeof(char)*2, 1, arquivo_entrada);
    reg->siglaPais[2]='\0';
    fread(&reg->idPoPsConectado,sizeof(int), 1, arquivo_entrada);
    fread(reg->unidadeMedida,sizeof(char), 1, arquivo_entrada);
    reg->unidadeMedida[1]='\0';
    fread(&reg->velocidade,sizeof(int), 1, arquivo_entrada);

}

/*

Função responsável por ler os campos de tamanho variavel de um registro de dados. 
Recebe um ponteiro para arquivo e um ponteiro para uma struct do tipo (reg_dados). 
Inicialmente é inicializado uma variavel do tipo char, com nome buffer, como '0'. 
Tambem são declarados os inteiros pos_nomePoPs e pos_nomePais, que indicam a posiçao 
nas respectivas strings, inicializados como 0, e o inteiro tam_lixo, que indica o tamanho do lixo. 

Entao, é feito um loop while() para cada um dos campos variaveis. Enquanto o buffer, 
que guarda o char lido não for | (que mostra que o campo terminou) nem $ 
(no caso em que o registro for removido e só houver lixo) é feita a sequencia:  é 
lido um byte, ao campo desejado, na posição do contaodor pos, é adicionado o char 
lido (buffer), e a pos é atualizada (aumenta-se 1). Antes de ler um novo campo, a 
variavel buffer é resetada para '0'. 

Apos ler os campos, o |, na RAM, é removido e substituido por um /0, indicador que a 
string chegou ao fim. Então, o tamanho do lixo é calculado com base no tamanho dos contadores 
de posição, tamanho do registro e tamanho dos campos de tamanho fixo. Por fim, se o lixo existe, 
ou seja, seu tamanho é diferente de 0, é declarada uma string buffer de lixo e o lixo é lido com um fread(). 

*/
void le_campos_tam_variavel(reg_dados* reg, FILE* arquivo_entrada){
char buffer = '0'; // armazena character

    int pos_nomePoPs=0; // posição na string de nomePops
    int pos_nomePais=0; // posição na string de nomePais
    int tam_lixo; // tamanho do lixo 

                
    
    //le campos variaveis
    while (buffer !='|' && buffer != '$') // 
    {
        fread(&buffer, sizeof(char), 1, arquivo_entrada); // le o char

        reg->nomePoPs[pos_nomePoPs]=buffer; //constroi a string
        pos_nomePoPs+=1; //atualiza a posição
                
    }
        
    buffer = '0'; //reseta o buffer

        while (buffer !='|' && buffer != '$')
    {
        //fscanf(arquivo_entrada,"%c",buffer);
        fread(&buffer, sizeof(char), 1, arquivo_entrada); // le o char
                
        reg->nomePais[pos_nomePais]=buffer; //constroi a string
        pos_nomePais+=1; //atualiza a posição
                

    }
             
    reg->nomePoPs[pos_nomePoPs-1]='\0'; // substitui o | por um \0
    reg->nomePais[pos_nomePais-1]='\0';

           
    tam_lixo=TAM_REG_DADOS-pos_nomePoPs-pos_nomePais-20; //descobre o tamanho do lixo 
         
    if (tam_lixo!=0){  //confere se existe lixo

    char lixo[tam_lixo];  //cria a string lixo
    fread(lixo,sizeof(char)*tam_lixo, 1, arquivo_entrada); //le o lixo

    }
}

/*

Função responsável por ler o arquivo completo. Recebe um ponteiro para arquivo e um ponteiro para 
uma struct do tipo (reg_dados), alem de um ponteiro de inteiro num_RRN, que atualiza o RRN lido. 

Se o fread() feito para o primeiro campo do registro nao retornar 0 significa que o arquivo não 
chegou ao fim. Logo, sera adicionado um \0 na RAM na segunda posição da string removido, marcando seu fim, 
e serão lidos todos os campos do registro com a função le_todos_campos_do_registro(). 
Então, o numero do RRN aumenta, já  que será lido um novo registro, e é retornado 1.

Se o fread() para o primeiro campo retornar 0 significa que o arquivo chegou ao fim, e 
a flag é passada a frente por meio de um retorno 0. 

*/
int le_arquivo(reg_dados* reg, FILE* arquivo_entrada){
    if(fread(reg->removido,sizeof(char), 1, arquivo_entrada) != 0){//se não chegou ao fim
        reg->removido[1] = '\0';
        le_registro(reg,arquivo_entrada);
        return 1;
    }
    return 0;
}



/*

Função que recebe uma string, um ponteiro de arquivo, e o tamanho de uma string,
escreve usando frwrite e o tamanho informado no arquivo apontado

*/
void escreve_string_no_arquivo(char* str, FILE* arquivo, int tam){
    fwrite(str, sizeof(char), tam, arquivo);
}


/*

Função que escreve os dados da struct do registro de cabeçalho no arquivo, recebe um ponteiro de arquivo e um ponteiro para
registro de cabeçalho. Chama a função escreve_string_no_arquivo para campos referentes à strings, e fwrite para campos
referentes à inteiros. 

Como o registro de cabeçalho deve ocupar o tamanho de uma página de disco (960 bytes), é feito o cálculo do valor do tamanho
da página de disco - o tamanho do registro de cabeçalho (que é fixo), para utilizar-se como condição de parada num loop
que escreve '$' até completar o tamanho necessário.

*/
void escrever_no_arquivo_cabecalho(FILE* arquivo, reg_cabecalho* reg){
    
    escreve_string_no_arquivo(reg->status, arquivo, 1);
    fwrite(&reg->topo, sizeof(int), 1, arquivo);
    fwrite(&reg->proxRRN, sizeof(int), 1, arquivo);
    fwrite(&reg->nroRegRem, sizeof(int), 1, arquivo);
    fwrite(&reg->nroPagDisco, sizeof(int), 1, arquivo);
    fwrite(&reg->qttCompacta, sizeof(int), 1, arquivo);
    
    for(int i = 0; i < (TAM_PAG_DISCO - TAM_REG_CABECALHO); i++){
        fwrite(LIXO, sizeof(char), 1, arquivo);
    }
}

