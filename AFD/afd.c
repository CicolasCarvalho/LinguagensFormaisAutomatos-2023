#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_ESTADOS 64
#define MAX_ESTADOS_FINAIS 16
#define MAX_INTERACOES 16

//-RELAÇÃO---------------------------------------------------------------------

typedef struct {
    char simbolo;
    int estado;
} Relacao;

Relacao relacao_inicializa(char simbolo, int estado) {
    return (Relacao){.simbolo = simbolo, .estado = estado};
}

//-ESTADO---------------------------------------------------------------------

typedef struct {
    int id;
    int interacoes;
    Relacao *relacoes;
} Estado;

Estado estado_inicializa(int id, int interacoes, Relacao *relacoes) {
    Estado estado;

    estado.id = id;
    estado.interacoes = interacoes;
    estado.relacoes = relacoes;

    return estado;
}

int estado_enviar(Estado *estado, char simbolo) {
    for (int i = 0; i < estado->interacoes; i++) {
        Relacao rel = estado->relacoes[i];
        if (simbolo == rel.simbolo) return rel.estado;
    }
    return -1;
}

//-AFD---------------------------------------------------------------------

typedef struct {
    Estado estados[MAX_ESTADOS];

    int estado_atual;
    int estado_inicial;
    int estado_final[MAX_ESTADOS_FINAIS];
} AFD;

AFD *afd_inicializa(int estado_inicial, int estado_final[MAX_ESTADOS_FINAIS]) {
    AFD *afd = malloc(sizeof(AFD));

    afd->estado_inicial = estado_inicial;
    *afd->estado_final = *estado_final;

    return afd;
}

void afd_adiciona_estado(AFD *afd, Estado estado) {
    // TODO: Verificar se o estado já existe
    // TODO: Verificar overflow
    afd->estados[estado.id] = estado;
}

Estado *afd_get_estado_atual(AFD *afd) {
    return &afd->estados[afd->estado_atual];
}

void afd_set_estado_atual(AFD *afd, int estado_atual) {
    afd->estado_atual = estado_atual;
}

int afd_validate(AFD *afd, char *word) {
    afd_set_estado_atual(afd, afd->estado_inicial);

    int i = -1;
    char c = word[i];

    while ((c = word[++i]) != '\0') {
        char c = word[i];
        Estado *atual = afd_get_estado_atual(afd);

        int novo = estado_enviar(atual, c);

        afd_set_estado_atual(afd, novo);
    }

    return afd->estado_atual;
}

//----------------------------------------------------------------------------

void teste(void) {
    int arr[MAX_ESTADOS_FINAIS] = {3};

    AFD *afd = afd_inicializa(0, arr);

    afd_adiciona_estado(afd, (Estado){
        .id = 0,
        .interacoes = 1,
        .relacoes = (Relacao[]){relacao_inicializa('a', 1)}
    });

    afd_adiciona_estado(afd, (Estado){
        .id = 1,
        .interacoes = 2,
        .relacoes = (Relacao[]){relacao_inicializa('b', 2), relacao_inicializa('a', 3)}
    });

    afd_adiciona_estado(afd, (Estado){
        .id = 2,
        .interacoes = 1,
        .relacoes = (Relacao[]){relacao_inicializa('b', 1)}
    });

    afd_adiciona_estado(afd, (Estado){
        .id = 3,
        .interacoes = 0,
        .relacoes = NULL
    });

    assert(afd_validate(afd, "abbbba\0")==3);
    assert(afd_validate(afd, "abbaa\0")==-1);
}

int main(void) {
    teste();

    return 0;
}