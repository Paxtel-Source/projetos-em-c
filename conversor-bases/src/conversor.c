#include "conversor.h"

#include <ctype.h>
#include <string.h>

static const char DIGITOS[] = "0123456789ABCDEF";

/* Retorna o valor numérico de um caractere ('A' -> 10) ou -1 se inválido */
static int valor_digito(char c)
{
    c = (char)toupper((unsigned char)c);
    const char *p = strchr(DIGITOS, c);
    return (c != '\0' && p != NULL) ? (int)(p - DIGITOS) : -1;
}

/* Inverte uma string no próprio lugar */
static void inverter(char *s)
{
    size_t n = strlen(s);
    for (size_t i = 0; i < n / 2; i++) {
        char tmp = s[i];
        s[i] = s[n - 1 - i];
        s[n - 1 - i] = tmp;
    }
}

int texto_para_numero(const char *texto, int base, uint64_t *valor)
{
    if (base != 2 && base != 8 && base != 10 && base != 16) {
        return 0;
    }

    /* Remove prefixos opcionais compatíveis com a base */
    if (texto[0] == '0' && texto[1] != '\0') {
        char p = (char)tolower((unsigned char)texto[1]);
        if ((base == 2 && p == 'b') || (base == 8 && p == 'o') || (base == 16 && p == 'x')) {
            texto += 2;
        }
    }
    if (*texto == '\0') {
        return 0;
    }

    uint64_t resultado = 0;
    for (; *texto; texto++) {
        int d = valor_digito(*texto);
        if (d < 0 || d >= base) {
            return 0; /* Dígito não pertence à base */
        }
        if (resultado > (UINT64_MAX - (uint64_t)d) / (uint64_t)base) {
            return 0; /* Overflow de 64 bits */
        }
        resultado = resultado * (uint64_t)base + (uint64_t)d;
    }

    *valor = resultado;
    return 1;
}

/*
 * Bases que são potência de 2 podem ser convertidas só com bits:
 * cada dígito ocupa 'bits' bits, extraídos com "& mascara" e ">> bits".
 */
static void converter_potencia_de_2(uint64_t valor, int bits, char *saida, size_t tamanho)
{
    uint64_t mascara = (1u << bits) - 1u;
    size_t i = 0;

    do {
        if (i + 1 >= tamanho) break;
        saida[i++] = DIGITOS[valor & mascara];
        valor >>= bits;
    } while (valor != 0);

    saida[i] = '\0';
    inverter(saida);
}

void para_binario(uint64_t valor, char *saida, size_t tamanho)
{
    converter_potencia_de_2(valor, 1, saida, tamanho);
}

void para_octal(uint64_t valor, char *saida, size_t tamanho)
{
    converter_potencia_de_2(valor, 3, saida, tamanho);
}

void para_hexadecimal(uint64_t valor, char *saida, size_t tamanho)
{
    converter_potencia_de_2(valor, 4, saida, tamanho);
}

void para_decimal(uint64_t valor, char *saida, size_t tamanho)
{
    size_t i = 0;
    do {
        if (i + 1 >= tamanho) break;
        saida[i++] = DIGITOS[valor % 10u];
        valor /= 10u;
    } while (valor != 0);

    saida[i] = '\0';
    inverter(saida);
}
