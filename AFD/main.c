// Nícolas dos Santos Carvalho - 128660
// Ciência da Computação - UEM
// 06 07 2022

// DISCLAIMER:
// Feito por motivos de estudo. a implementação não está 100% correta
// nem 100% coerente com o que seria uma AFD, mas com certeza é baseado em o que seria uma

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include "afd.h"

void teste(void) {
    AFD *afd = afd_inicializa(0);

    afd_adiciona_estado(afd, estado_inicializa(
        0,                                      // id
        1,                                      // saídas
        (Relacao[]){relacao_inicializa('a', 1)} // Relações
    ));

    afd_adiciona_estado(afd, estado_inicializa(
        1,
        2,
        (Relacao[]){relacao_inicializa('b', 2), relacao_inicializa('a', 3)}
    ));

    afd_adiciona_estado(afd, estado_inicializa(
        2,
        1,
        (Relacao[]){relacao_inicializa('b', 1)}
    ));

    afd_adiciona_estado(afd, estado_inicializa(
        3,
        0,
        NULL
    ));

    size_t arr[CHUNK_SIZE];
    afd_get_estado_final(afd, arr);

    assert(afd_validate(afd, "abbbba\0")==3);
    assert(afd_validate(afd, "abbaa\0")==-1);
    assert(arr[0] == 3);

    printf("Testes foram executados corretamente!\n");

    afd_free(afd);
}

int main(void) {
    teste();

    return 0;
}