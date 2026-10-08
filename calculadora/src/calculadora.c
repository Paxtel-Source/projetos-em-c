#include "calculadora.h"

#include <errno.h>
#include <math.h>
#include <stdlib.h>

CalcStatus calcular(double a, char op, double b, double *resultado)
{
    switch (op) {
    case '+':
        *resultado = a + b;
        return CALC_OK;
    case '-':
        *resultado = a - b;
        return CALC_OK;
    case '*':
    case 'x':
    case 'X':
        *resultado = a * b;
        return CALC_OK;
    case '/':
        if (b == 0.0) {
            return CALC_ERRO_DIVISAO_ZERO;
        }
        *resultado = a / b;
        return CALC_OK;
    default:
        return CALC_ERRO_OPERADOR_INVALIDO;
    }
}

int ler_numero(const char *texto, double *valor)
{
    char *fim = NULL;

    if (texto == NULL || *texto == '\0') {
        return 0;
    }

    errno = 0;
    *valor = strtod(texto, &fim);

    /* Rejeita textos como "12abc", overflow e valores NaN/infinito */
    if (errno != 0 || *fim != '\0' || !isfinite(*valor)) {
        return 0;
    }
    return 1;
}

const char *calc_mensagem_erro(CalcStatus status)
{
    switch (status) {
    case CALC_OK:
        return "Sucesso";
    case CALC_ERRO_DIVISAO_ZERO:
        return "Erro: divisao por zero nao e permitida.";
    case CALC_ERRO_OPERADOR_INVALIDO:
        return "Erro: operador invalido. Use +, -, * (ou x) ou /.";
    }
    return "Erro desconhecido.";
}
