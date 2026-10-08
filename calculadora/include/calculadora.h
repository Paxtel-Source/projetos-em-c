#ifndef CALCULADORA_H
#define CALCULADORA_H

/* Códigos de retorno das operações */
typedef enum {
    CALC_OK = 0,
    CALC_ERRO_DIVISAO_ZERO,
    CALC_ERRO_OPERADOR_INVALIDO
} CalcStatus;

/*
 * Executa a operação "a op b" e grava o resultado em *resultado.
 * Operadores aceitos: + - * / (e 'x' como sinônimo de multiplicação).
 */
CalcStatus calcular(double a, char op, double b, double *resultado);

/*
 * Converte uma string para double, validando a entrada inteira.
 * Retorna 1 em caso de sucesso e 0 se a string não for um número válido.
 */
int ler_numero(const char *texto, double *valor);

/* Retorna uma mensagem legível para o código de status. */
const char *calc_mensagem_erro(CalcStatus status);

#endif /* CALCULADORA_H */
