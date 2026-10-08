# 🗜️ Compactador de Arquivos RLE

Programa de linha de comando em C que **compacta e descompacta arquivos** usando o algoritmo **RLE (Run-Length Encoding)**. O algoritmo troca sequências de caracteres repetidos pela quantidade seguida do caractere:

```
AAAAABBB  ->  5A3B
```

## Funcionalidades

- **Compactar** (`-c`) e **descompactar** (`-d`) arquivos, lendo e gravando caractere por caractere com `fgetc` / `fputc`
- **Modo demonstração** (`-t`) para ver a compactação de um texto direto no terminal
- Mostra o tamanho antes e depois e a **porcentagem de economia**
- Avisa quando o arquivo **aumenta** de tamanho, o que acontece com textos sem repetições
- Funciona com **qualquer arquivo**, inclusive binários, pois abre em modo `"rb"` / `"wb"`
- Detecta arquivos `.rle` corrompidos e não deixa uma saída pela metade
- `make test` compacta e descompacta os exemplos e confere se o resultado é idêntico ao original

## Como funciona o formato

Cada sequência vira `<quantidade><caractere>`. O problema aparece quando o próprio texto tem **números**: `"111"` viraria `"31"`, e na volta não daria para saber onde termina a quantidade.

A solução é um **caractere de escape** (`\`): quando o caractere repetido é um dígito ou uma barra invertida, ele é precedido por `\`.

| Original    | Compactado     | Explicação                       |
|-------------|----------------|----------------------------------|
| `AAAAABBB`  | `5A3B`         | 5×A, 3×B                         |
| `ABC`       | `1A1B1C`       | Sem repetição, o texto aumenta   |
| `1112`      | `3\11\2`       | 3×'1', 1×'2', com escape         |

Na descompactação, o programa lê todos os dígitos (a quantidade), pula um `\` se houver e repete o caractere seguinte.

## Estrutura

```
compactador-rle/
├── include/
│   └── rle.h          # Formato, códigos de erro e declarações
├── src/
│   ├── rle.c          # Algoritmo RLE (arquivos e strings)
│   └── main.c         # Argumentos de linha de comando e estatísticas
├── exemplos/
│   ├── arte.txt       # Texto com muitas repetições (compacta bem)
│   └── numeros.txt    # Texto com dígitos (testa o escape)
├── Makefile
└── README.md
```

## Dependências

- Compilador C (GCC ou Clang) com suporte a C11
- `make`
- No Windows: [MSYS2](https://www.msys2.org/), MinGW ou WSL

## Como compilar e executar

1. Entre na pasta do projeto:
   ```bash
   cd compactador-rle
   ```
2. Compile:
   ```bash
   make
   ```
3. Compacte um arquivo:
   ```bash
   ./bin/rle -c exemplos/arte.txt arte.rle
   ```
4. Descompacte de volta:
   ```bash
   ./bin/rle -d arte.rle arte_restaurado.txt
   ```
5. Veja a compactação de um texto qualquer:
   ```bash
   ./bin/rle -t "AAAAABBB"
   ```
6. Rode os testes automáticos:
   ```bash
   make test
   ```
7. Para apagar os arquivos compilados:
   ```bash
   make clean
   ```

## Exemplo de uso

```
$ ./bin/rle -c exemplos/arte.txt arte.rle
Compactacao concluida: exemplos/arte.txt -> arte.rle
  Tamanho de entrada : 716 bytes
  Tamanho de saida   : 167 bytes
  Economia           : 76.7%

$ ./bin/rle -t "AAAAABBB"
Original     : "AAAAABBB" (8 caracteres)
Compactado   : "5A3B" (4 caracteres)
Restaurado   : "AAAAABBB"
Verificacao  : OK, identico ao original
```

## Conceitos praticados

Leitura e escrita caractere por caractere (`fgetc`, `fputc`), **ponteiros e aritmética de ponteiros** (a versão em memória percorre as strings só com ponteiros), conversão de número para texto sem `sprintf`, tratamento de erros com códigos de retorno, argumentos de linha de comando e arquivos binários.

## Limitações conhecidas

O RLE só é eficiente quando há **muitas repetições seguidas**, como em imagens simples, arte ASCII ou dados com padrões. Em textos comuns o arquivo pode até crescer. É por isso que compactadores reais, como ZIP e GZIP, usam algoritmos mais elaborados (LZ77, Huffman).
