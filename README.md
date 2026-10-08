# 💻 Projetos em C

Coleção de projetos de terminal em **linguagem C** para praticar os fundamentos da linguagem: entrada e saída, validação de dados, structs, arquivos, matrizes e operadores bit a bit.

| Projeto | Descrição | Conceitos principais |
|---------|-----------|----------------------|
| [🧮 Calculadora](calculadora/) | Operações `+ - * /` via terminal, com validação e tratamento de divisão por zero | `argc/argv`, `strtod`, `switch`, `enum` |
| [📒 Agenda de Contatos](agenda-contatos/) | CRUD de contatos salvos em arquivo binário | `struct`, `fopen`, `fread`, `fwrite`, `fclose` |
| [❌⭕ Jogo da Velha](jogo-da-velha/) | Jogo para dois jogadores no terminal, com placar | Matrizes, vetores, laços, lógica condicional |
| [🔢 Conversor de Bases](conversor-bases/) | Converte entre decimal, binário, octal e hexadecimal | Operadores bit a bit (`&`, `>>`), `uint64_t` |
| [🎓 Sistema Escolar](sistema-escolar/) | CRUD completo de alunos e notas, com médias e situação | `struct`, `malloc`/`realloc`, `fread`/`fwrite`, modularização |
| [🗜️ Compactador RLE](compactador-rle/) | Compacta e descompacta arquivos com Run-Length Encoding | `fgetc`/`fputc`, ponteiros, strings, arquivos binários |

## Organização

Todos os projetos seguem a mesma estrutura:

```
projeto/
├── include/     # Cabeçalhos (.h)
├── src/         # Código-fonte (.c)
├── Makefile     # Compila com um simples "make"
├── README.md    # O que faz e como usar
└── .gitignore   # Ignora binários e arquivos gerados
```

## Requisitos

- GCC (ou Clang) com suporte a C11
- `make`
- **Windows:** use [MSYS2](https://www.msys2.org/), MinGW ou WSL

## Como compilar

Compilar **todos** os projetos de uma vez, a partir da raiz:

```bash
make
```

Ou compilar apenas um deles:

```bash
cd calculadora
make
./bin/calculadora 10 + 5
```

Para limpar todos os arquivos compilados:

```bash
make clean
```

Os executáveis ficam na pasta `bin/` de cada projeto. Veja o README de cada pasta para instruções detalhadas.
