#ifndef _AFD_H
#define _AFD_H

#define CHUNK_SIZE 16

typedef struct {
    char simbolo;
    size_t estado;
} Relacao;

Relacao relacao_inicializa(char simbolo, size_t estado);

typedef struct {
    size_t id;
    size_t interacoes;
    Relacao *relacoes;
} Estado;

Estado estado_inicializa(size_t id, size_t interacoes, Relacao *relacoes);
int estado_enviar(Estado *estado, char simbolo);
void estado_free(Estado *estado);

typedef struct {
    Estado *estados;
    size_t max_estados;
    size_t num_estados;

    int estado_atual;
    size_t estado_inicial;
} AFD;

AFD *afd_inicializa(int estado_inicial);
void afd_adiciona_estado(AFD *afd, Estado estado);
Estado *afd_get_estado_atual(AFD *afd);
void afd_set_estado_atual(AFD *afd, int estado_atual);
int afd_validate(AFD *afd, char *word);
void afd_get_estado_final(AFD *afd, size_t *arr);
void afd_free(AFD *afd);

#endif