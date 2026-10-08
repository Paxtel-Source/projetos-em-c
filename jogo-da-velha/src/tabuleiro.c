#include "tabuleiro.h"

#include <stdio.h>

void tabuleiro_iniciar(char tab[TAM][TAM])
{
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tab[i][j] = VAZIO;
        }
    }
}

void tabuleiro_exibir(char tab[TAM][TAM])
{
    printf("\n");
    for (int i = 0; i < TAM; i++) {
        printf(" ");
        for (int j = 0; j < TAM; j++) {
            /* Casas vazias mostram o número da posição para guiar o jogador */
            char c = (tab[i][j] == VAZIO) ? (char)('1' + i * TAM + j) : tab[i][j];
            printf(" %c ", c);
            if (j < TAM - 1) {
                printf("|");
            }
        }
        printf("\n");
        if (i < TAM - 1) {
            printf(" ---+---+---\n");
        }
    }
    printf("\n");
}

int tabuleiro_jogar(char tab[TAM][TAM], int posicao, char jogador)
{
    if (posicao < 1 || posicao > TAM * TAM) {
        return 0;
    }
    int linha = (posicao - 1) / TAM;
    int coluna = (posicao - 1) % TAM;

    if (tab[linha][coluna] != VAZIO) {
        return 0;
    }
    tab[linha][coluna] = jogador;
    return 1;
}

EstadoJogo tabuleiro_verificar(char tab[TAM][TAM], char jogador)
{
    int diag_principal = 1, diag_secundaria = 1;

    for (int i = 0; i < TAM; i++) {
        int linha_completa = 1, coluna_completa = 1;

        for (int j = 0; j < TAM; j++) {
            if (tab[i][j] != jogador) linha_completa = 0;
            if (tab[j][i] != jogador) coluna_completa = 0;
        }
        if (linha_completa || coluna_completa) {
            return VITORIA;
        }

        if (tab[i][i] != jogador) diag_principal = 0;
        if (tab[i][TAM - 1 - i] != jogador) diag_secundaria = 0;
    }
    if (diag_principal || diag_secundaria) {
        return VITORIA;
    }

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tab[i][j] == VAZIO) {
                return EM_ANDAMENTO;
            }
        }
    }
    return EMPATE;
}
