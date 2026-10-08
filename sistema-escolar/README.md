# 🎓 Sistema de Gerenciamento Escolar (CRUD)

Sistema de terminal em C para **cadastrar, listar, buscar, atualizar e remover** alunos com suas notas. Os dados ficam salvos em um **arquivo binário** (`alunos.dat`), então continuam disponíveis depois que o programa é fechado.

## Funcionalidades

- **Cadastrar** aluno com nome, turma e 3 notas. A matrícula é gerada automaticamente.
- **Listar** todos os alunos em tabela, com média, situação (Aprovado/Reprovado) e estatísticas da turma
- **Buscar** por matrícula ou por parte do nome (sem diferenciar maiúsculas)
- **Atualizar** os dados de um aluno, mantendo a matrícula
- **Remover** aluno, com confirmação
- **Validação de entradas:** notas só entre 0 e 10, aceitando vírgula ou ponto (`7,5` ou `7.5`)
- **Persistência:** carrega o arquivo ao abrir e salva ao sair (`fopen`, `fread`, `fwrite`, `fclose`)

## Estrutura

```
sistema-escolar/
├── include/
│   ├── aluno.h      # struct Aluno, lista dinâmica e funções do CRUD
│   └── arquivo.h    # Funções de leitura e gravação em disco
├── src/
│   ├── aluno.c      # Regras do CRUD, médias e exibição
│   ├── arquivo.c    # Persistência com fread/fwrite
│   └── main.c       # Menu interativo e leitura segura do teclado
├── Makefile
└── README.md
```

O código é **modularizado**: `aluno.c` não sabe nada sobre arquivos, `arquivo.c` não sabe nada sobre o menu, e `main.c` só conversa com o usuário.

## Dependências

- Compilador C (GCC ou Clang) com suporte a C11
- `make`
- No Windows: [MSYS2](https://www.msys2.org/), MinGW ou WSL

## Como compilar e executar

1. Entre na pasta do projeto:
   ```bash
   cd sistema-escolar
   ```
2. Compile:
   ```bash
   make
   ```
3. Execute:
   ```bash
   ./bin/sistema-escolar
   ```
   Opcionalmente, informe outro arquivo de dados, por exemplo um para cada turma:
   ```bash
   ./bin/sistema-escolar turma_3A.dat
   ```
4. Para apagar os arquivos compilados:
   ```bash
   make clean
   ```

## Exemplo de uso

```
========= SISTEMA ESCOLAR =========
 1. Cadastrar aluno
 2. Listar alunos
 3. Buscar aluno
 4. Atualizar aluno
 5. Remover aluno
 0. Salvar e sair
===================================
Opcao: 2

Mat.  | Nome                         | Turma  |    N1    N2    N3 | Media | Situacao
------+------------------------------+--------+-------------------+-------+----------
1     | Ana Souza                    | 3A     |   8.0   7.5   9.0 |  8.17 | Aprovado
2     | Bruno Lima                   | 3B     |   5.0   6.0   4.0 |  5.00 | Reprovado

Total: 2 aluno(s) | Aprovados: 1 | Media geral: 6.58
```

## Detalhes técnicos

- **Lista dinâmica:** os alunos ficam em um vetor alocado com `malloc` e expandido com `realloc`, dobrando de tamanho quando enche. Assim não existe um limite fixo de cadastros.
- **Formato do arquivo:** `[assinatura][próxima matrícula][total][Aluno × total]`. A assinatura (`0x414C554E`, ou "ALUN" em ASCII) permite detectar se o arquivo é mesmo deste programa.
- **Remoção:** usa aritmética de ponteiros para achar o índice e `memmove` para fechar o espaço.
- **Leitura segura:** toda entrada passa por `fgets` e é convertida com `strtol`/`strtof`, evitando os problemas clássicos do `scanf`.
- O arquivo `.dat` é ignorado pelo Git, pois contém os dados de cada usuário.

## Conceitos praticados

`struct`, alocação dinâmica (`malloc`, `realloc`, `free`), ponteiros, arquivos binários (`fopen`, `fread`, `fwrite`, `fclose`), modularização em vários `.c`/`.h`, validação de entrada.
