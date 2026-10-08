#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agenda.h"

/* Lê uma linha do teclado, removendo o '\n'. Retorna 0 em EOF. */
static int ler_linha(const char *rotulo, char *destino, size_t tamanho)
{
    printf("%s", rotulo);
    fflush(stdout);

    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        return 0;
    }

    if (strchr(destino, '\n') == NULL) {
        /* Linha maior que o buffer: descarta o restante */
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    } else {
        destino[strcspn(destino, "\n")] = '\0';
    }
    return 1;
}

static void menu(void)
{
    printf("\n====== AGENDA DE CONTATOS ======\n");
    printf("1. Adicionar contato\n");
    printf("2. Listar contatos\n");
    printf("3. Buscar contato\n");
    printf("4. Remover contato\n");
    printf("0. Sair\n");
}

static void opcao_adicionar(Agenda *agenda)
{
    Contato c;

    if (!ler_linha("Nome: ", c.nome, sizeof c.nome) || c.nome[0] == '\0') {
        printf("Nome nao pode ficar vazio.\n");
        return;
    }
    ler_linha("Telefone: ", c.telefone, sizeof c.telefone);
    ler_linha("E-mail: ", c.email, sizeof c.email);

    switch (agenda_adicionar(agenda, &c)) {
    case 0:
        printf("Contato adicionado!\n");
        break;
    case -1:
        printf("Agenda cheia (limite de %d contatos).\n", MAX_CONTATOS);
        break;
    case -2:
        printf("Ja existe um contato com esse nome.\n");
        break;
    }
}

static void opcao_buscar(const Agenda *agenda)
{
    char termo[TAM_NOME];
    if (!ler_linha("Buscar por (nome, telefone ou e-mail): ", termo, sizeof termo)) {
        return;
    }
    if (agenda_buscar(agenda, termo) == 0) {
        printf("Nenhum contato encontrado.\n");
    }
}

static void opcao_remover(Agenda *agenda)
{
    char nome[TAM_NOME];
    if (!ler_linha("Nome do contato a remover: ", nome, sizeof nome)) {
        return;
    }
    if (agenda_remover(agenda, nome) == 0) {
        printf("Contato removido.\n");
    } else {
        printf("Contato nao encontrado.\n");
    }
}

int main(int argc, char *argv[])
{
    /* Permite escolher outro arquivo: ./bin/agenda meus_contatos.dat */
    const char *arquivo = (argc > 1) ? argv[1] : ARQUIVO_PADRAO;

    Agenda *agenda = malloc(sizeof *agenda);
    if (agenda == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        return EXIT_FAILURE;
    }

    if (agenda_carregar(agenda, arquivo) != 0) {
        fprintf(stderr, "Aviso: arquivo '%s' corrompido. Iniciando agenda vazia.\n", arquivo);
    }

    char entrada[16];
    int rodando = 1;

    while (rodando) {
        menu();
        if (!ler_linha("Opcao: ", entrada, sizeof entrada)) {
            break;
        }

        switch (entrada[0]) {
        case '1':
            opcao_adicionar(agenda);
            break;
        case '2':
            agenda_listar(agenda);
            break;
        case '3':
            opcao_buscar(agenda);
            break;
        case '4':
            opcao_remover(agenda);
            break;
        case '0':
            rodando = 0;
            break;
        default:
            printf("Opcao invalida.\n");
        }
    }

    int status = EXIT_SUCCESS;
    if (agenda_salvar(agenda, arquivo) != 0) {
        fprintf(stderr, "Erro ao salvar em '%s'.\n", arquivo);
        status = EXIT_FAILURE;
    } else {
        printf("Contatos salvos em '%s'. Ate mais!\n", arquivo);
    }

    free(agenda);
    return status;
}
