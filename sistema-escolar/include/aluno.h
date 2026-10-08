#ifndef ALUNO_H
#define ALUNO_H

#include <stddef.h>

#define TAM_NOME     60
#define TAM_TURMA    10
#define NUM_NOTAS    3
#define MEDIA_MINIMA 7.0f

typedef struct {
    int   matricula;
    char  nome[TAM_NOME];
    char  turma[TAM_TURMA];
    float notas[NUM_NOTAS];
} Aluno;

/* Lista dinâmica de alunos: cresce com realloc conforme necessário */
typedef struct {
    Aluno  *itens;
    size_t  total;
    size_t  capacidade;
    int     proxima_matricula;
} ListaAlunos;

/* Ciclo de vida da lista */
void lista_iniciar(ListaAlunos *lista);
void lista_liberar(ListaAlunos *lista);

/* CRUD */
int    aluno_cadastrar(ListaAlunos *lista, const Aluno *novo); /* retorna a matrícula ou -1 */
Aluno *aluno_buscar_matricula(ListaAlunos *lista, int matricula);
size_t aluno_buscar_nome(const ListaAlunos *lista, const char *termo);
int    aluno_atualizar(ListaAlunos *lista, int matricula, const Aluno *dados);
int    aluno_remover(ListaAlunos *lista, int matricula);

/* Exibição e cálculos */
float aluno_media(const Aluno *aluno);
void  aluno_exibir_cabecalho(void);
void  aluno_exibir(const Aluno *aluno);
void  aluno_listar(const ListaAlunos *lista);

#endif /* ALUNO_H */
