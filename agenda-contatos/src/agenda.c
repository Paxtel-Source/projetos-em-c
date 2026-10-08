#include "agenda.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* ---------- Funções auxiliares ---------- */

/* Compara strings ignorando maiúsculas/minúsculas */
static int iguais_sem_caso(const char *a, const char *b)
{
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == *b;
}

/* Verifica se "termo" aparece dentro de "texto" (sem diferenciar caso) */
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

static void imprimir_contato(size_t indice, const Contato *c)
{
    printf("%3zu. %-25s | %-15s | %s\n", indice + 1, c->nome, c->telefone, c->email);
}

/* ---------- Arquivo ---------- */

int agenda_carregar(Agenda *agenda, const char *arquivo)
{
    agenda->total = 0;

    FILE *fp = fopen(arquivo, "rb");
    if (fp == NULL) {
        return 0; /* Arquivo ainda não existe: agenda vazia */
    }

    size_t total = 0;
    if (fread(&total, sizeof total, 1, fp) != 1 || total > MAX_CONTATOS) {
        fclose(fp);
        return -1;
    }
    if (fread(agenda->contatos, sizeof(Contato), total, fp) != total) {
        fclose(fp);
        return -1;
    }

    agenda->total = total;
    fclose(fp);
    return 0;
}

int agenda_salvar(const Agenda *agenda, const char *arquivo)
{
    FILE *fp = fopen(arquivo, "wb");
    if (fp == NULL) {
        return -1;
    }

    int ok = fwrite(&agenda->total, sizeof agenda->total, 1, fp) == 1 &&
             fwrite(agenda->contatos, sizeof(Contato), agenda->total, fp) == agenda->total;

    if (fclose(fp) != 0) {
        ok = 0;
    }
    return ok ? 0 : -1;
}

/* ---------- CRUD ---------- */

int agenda_adicionar(Agenda *agenda, const Contato *novo)
{
    if (agenda->total >= MAX_CONTATOS) {
        return -1; /* Agenda cheia */
    }
    for (size_t i = 0; i < agenda->total; i++) {
        if (iguais_sem_caso(agenda->contatos[i].nome, novo->nome)) {
            return -2; /* Nome duplicado */
        }
    }
    agenda->contatos[agenda->total++] = *novo;
    return 0;
}

void agenda_listar(const Agenda *agenda)
{
    if (agenda->total == 0) {
        printf("Nenhum contato cadastrado.\n");
        return;
    }
    printf("\n  #  %-25s | %-15s | %s\n", "Nome", "Telefone", "E-mail");
    printf("---------------------------------------------------------------------\n");
    for (size_t i = 0; i < agenda->total; i++) {
        imprimir_contato(i, &agenda->contatos[i]);
    }
    printf("\nTotal: %zu contato(s)\n", agenda->total);
}

int agenda_buscar(const Agenda *agenda, const char *termo)
{
    int encontrados = 0;
    for (size_t i = 0; i < agenda->total; i++) {
        const Contato *c = &agenda->contatos[i];
        if (contem_sem_caso(c->nome, termo) || contem_sem_caso(c->telefone, termo) ||
            contem_sem_caso(c->email, termo)) {
            imprimir_contato(i, c);
            encontrados++;
        }
    }
    return encontrados;
}

int agenda_remover(Agenda *agenda, const char *nome)
{
    for (size_t i = 0; i < agenda->total; i++) {
        if (iguais_sem_caso(agenda->contatos[i].nome, nome)) {
            /* Desloca os contatos seguintes uma posição para trás */
            memmove(&agenda->contatos[i], &agenda->contatos[i + 1],
                    (agenda->total - i - 1) * sizeof(Contato));
            agenda->total--;
            return 0;
        }
    }
    return -1;
}
