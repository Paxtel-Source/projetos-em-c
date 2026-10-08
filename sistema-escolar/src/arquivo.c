#include "arquivo.h"

#include <stdio.h>
#include <stdlib.h>

/* Identifica o arquivo como sendo deste programa ("ALUN" em hexadecimal) */
#define ASSINATURA 0x414C554E

int arquivo_carregar(ListaAlunos *lista, const char *caminho)
{
    lista_iniciar(lista);

    FILE *fp = fopen(caminho, "rb");
    if (fp == NULL) {
        return 0; /* primeira execução: ainda não há arquivo */
    }

    int assinatura = 0, proxima = 1;
    size_t total = 0;

    if (fread(&assinatura, sizeof assinatura, 1, fp) != 1 || assinatura != ASSINATURA ||
        fread(&proxima, sizeof proxima, 1, fp) != 1 ||
        fread(&total, sizeof total, 1, fp) != 1) {
        fclose(fp);
        return -1;
    }

    if (total > 0) {
        Aluno *itens = malloc(total * sizeof *itens);
        if (itens == NULL || fread(itens, sizeof *itens, total, fp) != total) {
            free(itens);
            fclose(fp);
            return -1;
        }
        lista->itens = itens;
        lista->total = total;
        lista->capacidade = total;
    }
    lista->proxima_matricula = proxima;

    fclose(fp);
    return 0;
}

int arquivo_salvar(const ListaAlunos *lista, const char *caminho)
{
    FILE *fp = fopen(caminho, "wb");
    if (fp == NULL) {
        return -1;
    }

    int assinatura = ASSINATURA;
    int ok = fwrite(&assinatura, sizeof assinatura, 1, fp) == 1 &&
             fwrite(&lista->proxima_matricula, sizeof lista->proxima_matricula, 1, fp) == 1 &&
             fwrite(&lista->total, sizeof lista->total, 1, fp) == 1 &&
             fwrite(lista->itens, sizeof *lista->itens, lista->total, fp) == lista->total;

    if (fclose(fp) != 0) {
        ok = 0;
    }
    return ok ? 0 : -1;
}
