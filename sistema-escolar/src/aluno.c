#include "aluno.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 8

/* ---------- Auxiliares ---------- */

static int contem_sem_caso(const char *texto, const char *termo)
{
    size_t n = strlen(termo);
    if (n == 0) {
        return 1;
    }
    for (; *texto; texto++) {
        size_t i = 0;
        while (i < n && texto[i] &&
               tolower((unsigned char)texto[i]) == tolower((unsigned char)termo[i])) {
            i++;
        }
        if (i == n) {
            return 1;
        }
    }
    return 0;
}

static int garantir_capacidade(ListaAlunos *lista)
{
    if (lista->total < lista->capacidade) {
        return 0;
    }
    size_t nova = lista->capacidade ? lista->capacidade * 2 : CAPACIDADE_INICIAL;
    Aluno *tmp = realloc(lista->itens, nova * sizeof *tmp);
    if (tmp == NULL) {
        return -1;
    }
    lista->itens = tmp;
    lista->capacidade = nova;
    return 0;
}

/* ---------- Ciclo de vida ---------- */

void lista_iniciar(ListaAlunos *lista)
{
    lista->itens = NULL;
    lista->total = 0;
    lista->capacidade = 0;
    lista->proxima_matricula = 1;
}

void lista_liberar(ListaAlunos *lista)
{
    free(lista->itens);
    lista_iniciar(lista);
}

/* ---------- CRUD ---------- */

int aluno_cadastrar(ListaAlunos *lista, const Aluno *novo)
{
    if (garantir_capacidade(lista) != 0) {
        return -1;
    }
    Aluno *destino = &lista->itens[lista->total++];
    *destino = *novo;
    destino->matricula = lista->proxima_matricula++;
    return destino->matricula;
}

Aluno *aluno_buscar_matricula(ListaAlunos *lista, int matricula)
{
    for (size_t i = 0; i < lista->total; i++) {
        if (lista->itens[i].matricula == matricula) {
            return &lista->itens[i];
        }
    }
    return NULL;
}

size_t aluno_buscar_nome(const ListaAlunos *lista, const char *termo)
{
    size_t encontrados = 0;
    for (size_t i = 0; i < lista->total; i++) {
        if (contem_sem_caso(lista->itens[i].nome, termo)) {
            if (encontrados == 0) {
                aluno_exibir_cabecalho();
            }
            aluno_exibir(&lista->itens[i]);
            encontrados++;
        }
    }
    return encontrados;
}

int aluno_atualizar(ListaAlunos *lista, int matricula, const Aluno *dados)
{
    Aluno *alvo = aluno_buscar_matricula(lista, matricula);
    if (alvo == NULL) {
        return -1;
    }
    *alvo = *dados;
    alvo->matricula = matricula; /* a matrícula nunca muda */
    return 0;
}

int aluno_remover(ListaAlunos *lista, int matricula)
{
    Aluno *alvo = aluno_buscar_matricula(lista, matricula);
    if (alvo == NULL) {
        return -1;
    }
    size_t indice = (size_t)(alvo - lista->itens); /* aritmética de ponteiros */
    memmove(alvo, alvo + 1, (lista->total - indice - 1) * sizeof *alvo);
    lista->total--;
    return 0;
}

/* ---------- Exibição ---------- */

float aluno_media(const Aluno *aluno)
{
    float soma = 0.0f;
    for (int i = 0; i < NUM_NOTAS; i++) {
        soma += aluno->notas[i];
    }
    return soma / NUM_NOTAS;
}

void aluno_exibir_cabecalho(void)
{
    printf("\n%-5s | %-28s | %-6s | %5s %5s %5s | %5s | %s\n",
           "Mat.", "Nome", "Turma", "N1", "N2", "N3", "Media", "Situacao");
    printf("------+------------------------------+--------+-------------------+-------+----------\n");
}

void aluno_exibir(const Aluno *a)
{
    float media = aluno_media(a);
    printf("%-5d | %-28.28s | %-6.6s | %5.1f %5.1f %5.1f | %5.2f | %s\n",
           a->matricula, a->nome, a->turma, a->notas[0], a->notas[1], a->notas[2],
           media, media >= MEDIA_MINIMA ? "Aprovado" : "Reprovado");
}

void aluno_listar(const ListaAlunos *lista)
{
    if (lista->total == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    float soma_medias = 0.0f;
    size_t aprovados = 0;

    aluno_exibir_cabecalho();
    for (size_t i = 0; i < lista->total; i++) {
        float m = aluno_media(&lista->itens[i]);
        soma_medias += m;
        if (m >= MEDIA_MINIMA) {
            aprovados++;
        }
        aluno_exibir(&lista->itens[i]);
    }
    printf("\nTotal: %zu aluno(s) | Aprovados: %zu | Media geral: %.2f\n",
           lista->total, aprovados, soma_medias / (float)lista->total);
}
