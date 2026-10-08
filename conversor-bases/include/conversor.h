#ifndef CONVERSOR_H
#define CONVERSOR_H

#include <stddef.h>
#include <stdint.h>

/* 64 dígitos binários + '\0' */
#define TAM_BUFFER 65

/*
 * Converte o texto (na base informada: 2, 8, 10 ou 16) para um inteiro
 * sem sinal de 64 bits. Aceita prefixos opcionais 0b, 0o e 0x.
 * Retorna 1 em caso de sucesso e 0 se o texto for inválido ou houver overflow.
 */
int texto_para_numero(const char *texto, int base, uint64_t *valor);

/* Conversões usando operadores bit a bit (>>, &) */
void para_binario(uint64_t valor, char *saida, size_t tamanho);
void para_octal(uint64_t valor, char *saida, size_t tamanho);
void para_hexadecimal(uint64_t valor, char *saida, size_t tamanho);

/* Conversão para decimal usando divisão e resto (/ e %) */
void para_decimal(uint64_t valor, char *saida, size_t tamanho);

#endif /* CONVERSOR_H */
