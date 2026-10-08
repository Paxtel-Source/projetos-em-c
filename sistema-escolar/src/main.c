#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aluno.h"
#include "arquivo.h"

#define TAM_ENTRADA 128

/* ---------- Leitura segura do teclado ---------- */

/* Lê uma linha, removendo o '\n'. Retorna 0 em EOF. */
static int ler_texto(const char *rotulo, char *destino, size_t tamanho)
{
    char buffer[TAM_ENTRADA];

    printf("%s", rotulo);
    fflush(stdout);
    if (fgets(buffer, sizeof buffer, stdin) == NULL) {
        return 0;
    }
    if (strchr(buffer, '\n') == NULL) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
    buffer[strcspn(buffer, "\n")] = '\0';

    snprintf(destino, tamanho, "%s", buffer);
    return 1;
}

/* Repete a pergunta até receber um inteiro válido. Retorna 0 em EOF. */
static int ler_inteiro(const char *rotulo, int *valor)
{
    char buffer[TAM_ENTRADA];
    char *fim;

    while (ler_texto(rotulo, buffer, sizeof buffer)) {
        errno = 0;
        long n = strtol(buffer, &fim, 10);
        if (fim != buffer && *fim == '\0' && errno == 0 && n >= 0 && n <= 1000000) {
            *valor = (int)n;
            return 1;
        }
        printf("  Valor invalido. Digite um numero inteiro.\n");
    }
    return 0;
}

/* Repete a pergunta até receber uma nota entre 0 e 10. Retorna 0 em EOF. */
static int ler_nota(const char *rotulo, float *nota)
{
    char buffer[TAM_ENTRADA];
    char *fim;

    while (ler_texto(rotulo, buffer, sizeof buffer)) {
        /* Aceita vírgula como separador decimal (ex.: 7,5) */
        char *virgula = strchr(buffer, ',');
        if (virgula) {
            *virgula = '.';
        }
        float n = strtof(buffer, &fim);
        if (fim != buffer && *fim == '\0' && n >= 0.0f && n <= 10.0f) {
            *nota = n;
            return 1;
        }
        printf("  Nota invalida. Digite um valor entre 0 e 10.\n");
    }
    return 0;
}

/* Preenche nome, turma e notas. Retorna 0 se o usuário cancelar (EOF). */
static int ler_dados_aluno(Aluno *a)
{
    memset(a, 0, sizeof *a);

    do {
        if (!ler_texto("Nome: ", a->nome, sizeof a->nome)) return 0;
        if (a->nome[0] == '\0') printf("  O nome nao pode ficar vazio.\n");
    } while (a->nome[0] == '\0');

    if (!ler_texto("Turma (ex: 3A): ", a->turma, sizeof a->turma)) return 0;

    for (int i = 0; i < NUM_NOTAS; i++) {
        char rotulo[32];
        snprintf(rotulo, sizeof rotulo, "Nota %d (0-10): ", i + 1);
        if (!ler_nota(rotulo, &a->notas[i])) return 0;
    }
    return 1;
}

/* ---------- Ações do menu ---------- */

static void acao_cadastrar(ListaAlunos *lista)
{
    Aluno novo;
    printf("\n--- Cadastrar aluno ---\n");
    if (!ler_dados_aluno(&novo)) {
        return;
    }
    int matricula = aluno_cadastrar(lista, &novo);
    if (matricula < 0) {
        printf("Erro: memoria insuficiente.\n");
    } else {
        printf("Aluno cadastrado com a matricula %d.\n", matricula);
    }
}

static void acao_buscar(ListaAlunos *lista)
{
    char opcao[8];
    printf("\n--- Buscar aluno ---\n");
    if (!ler_texto("Buscar por (1) matricula ou (2) nome? ", opcao, sizeof opcao)) {
        return;
    }

    if (opcao[0] == '1') {
        int mat;
        if (!ler_inteiro("Matricula: ", &mat)) return;
        Aluno *a = aluno_buscar_matricula(lista, mat);
        if (a) {
            aluno_exibir_cabecalho();
            aluno_exibir(a);
        } else {
            printf("Aluno nao encontrado.\n");
        }
    } else if (opcao[0] == '2') {
        char termo[TAM_NOME];
        if (!ler_texto("Nome (ou parte dele): ", termo, sizeof termo)) return;
        if (aluno_buscar_nome(lista, termo) == 0) {
            printf("Nenhum aluno encontrado.\n");
        }
    } else {
        printf("Opcao invalida.\n");
    }
}

static void acao_atualizar(ListaAlunos *lista)
{
    int mat;
    printf("\n--- Atualizar aluno ---\n");
    if (!ler_inteiro("Matricula do aluno: ", &mat)) return;

    Aluno *atual = aluno_buscar_matricula(lista, mat);
    if (atual == NULL) {
        printf("Aluno nao encontrado.\n");
        return;
    }

    aluno_exibir_cabecalho();
    aluno_exibir(atual);
    printf("\nDigite os novos dados:\n");

    Aluno dados;
    if (ler_dados_aluno(&dados) && aluno_atualizar(lista, mat, &dados) == 0) {
        printf("Aluno atualizado.\n");
    }
}

static void acao_remover(ListaAlunos *lista)
{
    int mat;
    char confirma[8];

    printf("\n--- Remover aluno ---\n");
    if (!ler_inteiro("Matricula do aluno: ", &mat)) return;

    Aluno *a = aluno_buscar_matricula(lista, mat);
    if (a == NULL) {
        printf("Aluno nao encontrado.\n");
        return;
    }

    printf("Remover '%s'? (s/n): ", a->nome);
    if (ler_texto("", confirma, sizeof confirma) && (confirma[0] == 's' || confirma[0] == 'S')) {
        aluno_remover(lista, mat);
        printf("Aluno removido.\n");
    } else {
        printf("Remocao cancelada.\n");
    }
}

static void exibir_menu(void)
{
    printf("\n========= SISTEMA ESCOLAR =========\n");
    printf(" 1. Cadastrar aluno\n");
    printf(" 2. Listar alunos\n");
    printf(" 3. Buscar aluno\n");
    printf(" 4. Atualizar aluno\n");
    printf(" 5. Remover aluno\n");
    printf(" 0. Salvar e sair\n");
    printf("===================================\n");
}

int main(int argc, char *argv[])
{
    const char *caminho = (argc > 1) ? argv[1] : ARQUIVO_PADRAO;
    ListaAlunos lista;

    if (arquivo_carregar(&lista, caminho) != 0) {
        fprintf(stderr, "Aviso: '%s' esta corrompido ou e de outro formato. "
                        "Iniciando com a lista vazia.\n", caminho);
        lista_iniciar(&lista);
    } else if (lista.total > 0) {
        printf("%zu aluno(s) carregado(s) de '%s'.\n", lista.total, caminho);
    }

    char opcao[8];
    int rodando = 1;

    while (rodando) {
        exibir_menu();
        if (!ler_texto("Opcao: ", opcao, sizeof opcao)) {
            break;
        }
        switch (opcao[0]) {
        case '1': acao_cadastrar(&lista); break;
        case '2': aluno_listar(&lista);   break;
        case '3': acao_buscar(&lista);    break;
        case '4': acao_atualizar(&lista); break;
        case '5': acao_remover(&lista);   break;
        case '0': rodando = 0;            break;
        default:  printf("Opcao invalida.\n");
        }
    }

    int status = EXIT_SUCCESS;
    if (arquivo_salvar(&lista, caminho) != 0) {
        fprintf(stderr, "Erro ao salvar os dados em '%s'.\n", caminho);
        status = EXIT_FAILURE;
    } else {
        printf("Dados salvos em '%s'. Ate mais!\n", caminho);
    }

    lista_liberar(&lista);
    return status;
}
