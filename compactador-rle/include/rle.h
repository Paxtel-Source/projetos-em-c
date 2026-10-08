#ifndef RLE_H
#define RLE_H

#include <stdio.h>
#include <stddef.h>

/*
 * Formato RLE usado por este programa
 * -----------------------------------
 * Cada sequência de caracteres repetidos vira: <quantidade><caractere>
 *
 *     "AAAAABBB"  ->  "5A3B"
 *
 * Se o caractere for um dígito ou uma barra invertida, ele recebe
 * o prefixo '\' para não ser confundido com a quantidade:
 *
 *     "1112"      ->  "3\11\2"
 */
#define RLE_ESCAPE '\\'

/* Códigos de erro (valores negativos) */
#define RLE_ERRO_LEITURA  -1
#define RLE_ERRO_ESCRITA  -2
#define RLE_ERRO_FORMATO  -3

/*
 * Compacta/descompacta o conteúdo de 'entrada' gravando em 'saida',
 * lendo e escrevendo caractere por caractere (fgetc / fputc).
 * Retornam o número de bytes gravados ou um código de erro negativo.
 */
long rle_comprimir(FILE *entrada, FILE *saida);
long rle_descomprimir(FILE *entrada, FILE *saida);

/*
 * Versão em memória, trabalhando com ponteiros para strings.
 * Grava o resultado em 'destino' (com '\0' no final).
 * Retorna o tamanho do resultado ou -1 se 'destino' for pequeno demais.
 */
long rle_comprimir_texto(const char *origem, char *destino, size_t tamanho);
long rle_descomprimir_texto(const char *origem, char *destino, size_t tamanho);

#endif /* RLE_H */
