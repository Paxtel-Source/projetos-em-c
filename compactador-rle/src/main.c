#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rle.h"

#define TAM_TEXTO 4096

static void mostrar_uso(const char *programa)
{
    printf("Compactador RLE (Run-Length Encoding)\n\n");
    printf("Uso:\n");
    printf("  %s -c <entrada> <saida>   Compacta um arquivo\n", programa);
    printf("  %s -d <entrada> <saida>   Descompacta um arquivo\n", programa);
    printf("  %s -t \"<texto>\"           Demonstra a compactacao de um texto\n\n", programa);
    printf("Exemplos:\n");
    printf("  %s -c exemplos/arte.txt arte.rle\n", programa);
    printf("  %s -d arte.rle arte_restaurada.txt\n", programa);
    printf("  %s -t \"AAAAABBB\"\n", programa);
}

static const char *descrever_erro(long codigo)
{
    switch (codigo) {
    case RLE_ERRO_LEITURA: return "falha ao ler o arquivo de entrada";
    case RLE_ERRO_ESCRITA: return "falha ao gravar o arquivo de saida";
    case RLE_ERRO_FORMATO: return "o arquivo nao esta no formato RLE esperado";
    default:               return "erro desconhecido";
    }
}

/* Retorna o tamanho de um arquivo aberto, preservando a posição atual */
static long tamanho_arquivo(FILE *fp)
{
    long posicao = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long tamanho = ftell(fp);
    fseek(fp, posicao, SEEK_SET);
    return tamanho;
}

static int processar_arquivo(int compactar, const char *caminho_entrada, const char *caminho_saida)
{
    if (strcmp(caminho_entrada, caminho_saida) == 0) {
        fprintf(stderr, "Erro: entrada e saida devem ser arquivos diferentes.\n");
        return EXIT_FAILURE;
    }

    /* Modo binário ("rb"/"wb") para funcionar com qualquer arquivo */
    FILE *entrada = fopen(caminho_entrada, "rb");
    if (entrada == NULL) {
        perror(caminho_entrada);
        return EXIT_FAILURE;
    }
    FILE *saida = fopen(caminho_saida, "wb");
    if (saida == NULL) {
        perror(caminho_saida);
        fclose(entrada);
        return EXIT_FAILURE;
    }

    long original = tamanho_arquivo(entrada);
    long resultado = compactar ? rle_comprimir(entrada, saida) : rle_descomprimir(entrada, saida);

    fclose(entrada);
    if (fclose(saida) != 0 && resultado >= 0) {
        resultado = RLE_ERRO_ESCRITA;
    }

    if (resultado < 0) {
        fprintf(stderr, "Erro: %s.\n", descrever_erro(resultado));
        remove(caminho_saida); /* não deixa um arquivo pela metade */
        return EXIT_FAILURE;
    }

    printf("%s concluida: %s -> %s\n", compactar ? "Compactacao" : "Descompactacao",
           caminho_entrada, caminho_saida);
    printf("  Tamanho de entrada : %ld bytes\n", original);
    printf("  Tamanho de saida   : %ld bytes\n", resultado);

    if (compactar && original > 0) {
        double taxa = 100.0 * (1.0 - (double)resultado / (double)original);
        if (taxa >= 0) {
            printf("  Economia           : %.1f%%\n", taxa);
        } else {
            printf("  Aviso: o arquivo ficou %.1f%% MAIOR. O RLE funciona melhor com\n"
                   "  textos que tem muitos caracteres repetidos em sequencia.\n", -taxa);
        }
    }
    return EXIT_SUCCESS;
}

static int demonstrar_texto(const char *texto)
{
    static char compactado[TAM_TEXTO * 3];
    static char restaurado[TAM_TEXTO];

    if (strlen(texto) >= TAM_TEXTO) {
        fprintf(stderr, "Erro: texto muito longo (maximo %d caracteres).\n", TAM_TEXTO - 1);
        return EXIT_FAILURE;
    }

    long tam_c = rle_comprimir_texto(texto, compactado, sizeof compactado);
    long tam_r = rle_descomprimir_texto(compactado, restaurado, sizeof restaurado);

    if (tam_c < 0 || tam_r < 0) {
        fprintf(stderr, "Erro ao processar o texto.\n");
        return EXIT_FAILURE;
    }

    printf("Original     : \"%s\" (%zu caracteres)\n", texto, strlen(texto));
    printf("Compactado   : \"%s\" (%ld caracteres)\n", compactado, tam_c);
    printf("Restaurado   : \"%s\"\n", restaurado);
    printf("Verificacao  : %s\n", strcmp(texto, restaurado) == 0 ? "OK, identico ao original"
                                                                : "FALHOU");
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc == 4 && (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "-d") == 0)) {
        return processar_arquivo(argv[1][1] == 'c', argv[2], argv[3]);
    }
    if (argc == 3 && strcmp(argv[1], "-t") == 0) {
        return demonstrar_texto(argv[2]);
    }

    mostrar_uso(argv[0]);
    return EXIT_FAILURE;
}
