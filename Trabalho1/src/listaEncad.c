#include "listaEncad.h"

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

// implementar lista dinamica encadeada de registros
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

void imprimirListaReg(Lista *lista){
    elemento *aux;
    aux = *lista;
    
    while(aux != NULL){
        imprimeRegistro(aux->reg);
        aux = aux->prox;
    }
}