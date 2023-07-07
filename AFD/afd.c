#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "afd.h"

//-RELACAO---------------------------------------------------------------------

Relacao relacao_inicializa(char simbolo, size_t estado) {
    return (Relacao){.simbolo = simbolo, .estado = estado};
}

//-ESTADO---------------------------------------------------------------------

Estado estado_inicializa(size_t id, size_t interacoes, Relacao *relacoes) {
    Estado estado;

    estado.id = id;
    estado.interacoes = interacoes;
    estado.relacoes = relacoes;

    return estado;
}

int estado_enviar(Estado *estado, char simbolo) {
    for (size_t i = 0; i < estado->interacoes; i++) {
        Relacao rel = estado->relacoes[i];
        if (simbolo == rel.simbolo) return rel.estado;
    }

    return -1;
}

void estado_free(Estado *estado) {
    free(estado->relacoes);
    free(estado);
}

//-AFD---------------------------------------------------------------------

AFD *afd_inicializa(int estado_inicial) {
    AFD *afd = malloc(sizeof(AFD));

    afd->num_estados = 0;
    afd->max_estados = CHUNK_SIZE;
    afd->estados = malloc(sizeof(Estado)*CHUNK_SIZE);
    afd->estado_inicial = estado_inicial;

    return afd;
}

void afd_adiciona_estado(AFD *afd, Estado estado) {
    afd->num_estados++;

    if (afd->num_estados > afd->max_estados) {
        afd->estados = realloc(afd->estados, sizeof(Estado)*afd->max_estados*2);
        afd->max_estados += CHUNK_SIZE;
        printf("!! estados aumentou: %zu, %zu\n", afd->num_estados, afd->max_estados);
    }

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
    char c;

    while ((c = word[++i]) != '\0') {
        char c = word[i];
        Estado *atual = afd_get_estado_atual(afd);

        int novo = estado_enviar(atual, c);

        afd_set_estado_atual(afd, novo);
    }

    return afd->estado_atual;
}

void afd_get_estado_final(AFD *afd, size_t *arr) {
    for (size_t j = 0, i = 0; i < CHUNK_SIZE; i++) {
        Estado estado = afd->estados[i];

        if (estado.interacoes == 0) {
            arr[j++] = estado.id;
        }
    }
}

void afd_free(AFD *afd) {
    estado_free(afd->estados);
    free(afd);
}