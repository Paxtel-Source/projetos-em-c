#ifndef TABULEIRO_H
#define TABULEIRO_H

#define TAM 3
#define VAZIO ' '

typedef enum {
    EM_ANDAMENTO,
    VITORIA,
    EMPATE
} EstadoJogo;

/* Preenche o tabuleiro (matriz 3x3) com espaços vazios */
void tabuleiro_iniciar(char tab[TAM][TAM]);

/* Desenha o tabuleiro no terminal */
void tabuleiro_exibir(char tab[TAM][TAM]);

/*
 * Tenta marcar a posição (1 a 9) com o símbolo do jogador.
 * Retorna 1 se a jogada foi válida e 0 caso contrário.
 */
int tabuleiro_jogar(char tab[TAM][TAM], int posicao, char jogador);

/* Verifica se o último jogador venceu, se houve empate ou se o jogo continua */
EstadoJogo tabuleiro_verificar(char tab[TAM][TAM], char jogador);

#endif /* TABULEIRO_H */
