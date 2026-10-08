#include "rle.h"

#include <ctype.h>
#include <limits.h>

/* Limite de segurança para a quantidade lida de um arquivo compactado */
#define MAX_REPETICOES 100000000UL

static int precisa_escape(int c)
{
    return isdigit(c) || c == RLE_ESCAPE;
}

/*
 * Converte 'n' em dígitos decimais (sem sprintf), preenchendo o buffer
 * de trás para frente com um ponteiro. Retorna o início do número.
 */
static char *numero_para_texto(unsigned long n, char *fim_buffer)
{
    char *p = fim_buffer;
    *p = '\0';
    do {
        *--p = (char)('0' + n % 10);
        n /= 10;
    } while (n > 0);
    return p;
}

/* ========================================================== */
/*                 Versão com arquivos (FILE *)               */
/* ========================================================== */

/* Grava uma sequência "<quantidade>[\]<caractere>". Retorna bytes gravados ou -1. */
static long gravar_sequencia(FILE *saida, unsigned long quantidade, int c)
{
    char buffer[24];
    long gravados = 0;

    for (const char *p = numero_para_texto(quantidade, buffer + sizeof buffer - 1); *p; p++) {
        if (fputc(*p, saida) == EOF) return -1;
        gravados++;
    }
    if (precisa_escape(c)) {
        if (fputc(RLE_ESCAPE, saida) == EOF) return -1;
        gravados++;
    }
    if (fputc(c, saida) == EOF) return -1;
    return gravados + 1;
}

long rle_comprimir(FILE *entrada, FILE *saida)
{
    long total = 0;
    int atual = fgetc(entrada);

    while (atual != EOF) {
        unsigned long quantidade = 1;
        int proximo;

        /* Conta quantas vezes o caractere se repete em sequência */
        while ((proximo = fgetc(entrada)) == atual && quantidade < ULONG_MAX) {
            quantidade++;
        }

        long n = gravar_sequencia(saida, quantidade, atual);
        if (n < 0) {
            return RLE_ERRO_ESCRITA;
        }
        total += n;
        atual = proximo;
    }

    return ferror(entrada) ? RLE_ERRO_LEITURA : total;
}

long rle_descomprimir(FILE *entrada, FILE *saida)
{
    long total = 0;
    int c;

    while ((c = fgetc(entrada)) != EOF) {
        /* 1) Toda sequência começa com a quantidade */
        if (!isdigit(c)) {
            return RLE_ERRO_FORMATO;
        }
        unsigned long quantidade = 0;
        while (isdigit(c)) {
            quantidade = quantidade * 10 + (unsigned long)(c - '0');
            if (quantidade > MAX_REPETICOES) {
                return RLE_ERRO_FORMATO;
            }
            c = fgetc(entrada);
        }

        /* 2) Depois vem o caractere (com escape opcional) */
        if (c == RLE_ESCAPE) {
            c = fgetc(entrada);
        }
        if (c == EOF || quantidade == 0) {
            return RLE_ERRO_FORMATO;
        }

        /* 3) Repete o caractere 'quantidade' vezes */
        for (unsigned long i = 0; i < quantidade; i++) {
            if (fputc(c, saida) == EOF) {
                return RLE_ERRO_ESCRITA;
            }
        }
        total += (long)quantidade;
    }

    return ferror(entrada) ? RLE_ERRO_LEITURA : total;
}

/* ========================================================== */
/*                Versão em memória (ponteiros)               */
/* ========================================================== */

long rle_comprimir_texto(const char *origem, char *destino, size_t tamanho)
{
    const char *p = origem;     /* percorre a entrada */
    char *saida = destino;      /* posição de escrita */
    char *limite = destino + tamanho;

    while (*p != '\0') {
        const char *inicio = p;
        while (*p == *inicio) {
            p++;
        }
        unsigned long quantidade = (unsigned long)(p - inicio);

        char buffer[24];
        const char *num = numero_para_texto(quantidade, buffer + sizeof buffer - 1);

        while (*num) {
            if (saida >= limite) return -1;
            *saida++ = *num++;
        }
        if (precisa_escape((unsigned char)*inicio)) {
            if (saida >= limite) return -1;
            *saida++ = RLE_ESCAPE;
        }
        if (saida >= limite) return -1;
        *saida++ = *inicio;
    }

    if (saida >= limite) return -1;
    *saida = '\0';
    return (long)(saida - destino);
}

long rle_descomprimir_texto(const char *origem, char *destino, size_t tamanho)
{
    const char *p = origem;
    char *saida = destino;
    char *limite = destino + tamanho;

    while (*p != '\0') {
        if (!isdigit((unsigned char)*p)) return -1;

        unsigned long quantidade = 0;
        while (isdigit((unsigned char)*p)) {
            quantidade = quantidade * 10 + (unsigned long)(*p++ - '0');
            if (quantidade > MAX_REPETICOES) return -1;
        }
        if (*p == RLE_ESCAPE) p++;
        if (*p == '\0' || quantidade == 0) return -1;

        if ((size_t)(limite - saida) <= quantidade) return -1; /* reserva espaço para '\0' */
        while (quantidade-- > 0) {
            *saida++ = *p;
        }
        p++;
    }

    if (saida >= limite) return -1;
    *saida = '\0';
    return (long)(saida - destino);
}
