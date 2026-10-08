#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "aluno.h"

#define ARQUIVO_PADRAO "alunos.dat"

/*
 * Formato do arquivo binário:
 *   [int assinatura][int próxima matrícula][size_t total][Aluno x total]
 *
 * Retornam 0 em caso de sucesso e -1 em caso de erro.
 * Carregar um arquivo inexistente não é erro: a lista começa vazia.
 */
int arquivo_carregar(ListaAlunos *lista, const char *caminho);
int arquivo_salvar(const ListaAlunos *lista, const char *caminho);

#endif /* ARQUIVO_H */
