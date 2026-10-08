# 📒 Agenda de Contatos (CRUD em Arquivo)

Sistema de terminal em C para **adicionar, listar, buscar e remover** contatos. Os dados ficam salvos em um **arquivo binário** (`contatos.dat`), então continuam lá na próxima vez que o programa for aberto.

## Funcionalidades

- **Adicionar** contato (nome, telefone, e-mail) — bloqueia nomes duplicados
- **Listar** todos os contatos em formato de tabela
- **Buscar** por trecho do nome, telefone ou e-mail (sem diferenciar maiúsculas)
- **Remover** contato pelo nome
- Salvamento automático ao sair, com `fopen`, `fwrite` e `fclose`
- Carregamento ao iniciar, com `fopen`, `fread` e `fclose`

## Estrutura

```
agenda-contatos/
├── include/
│   └── agenda.h    # Struct Contato e declarações do CRUD
├── src/
│   ├── agenda.c    # CRUD e leitura/gravação do arquivo
│   └── main.c      # Menu e interação com o usuário
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
   cd agenda-contatos
   ```
2. Compile:
   ```bash
   make
   ```
3. Execute:
   ```bash
   ./bin/agenda
   ```
   Opcionalmente, informe outro arquivo de dados:
   ```bash
   ./bin/agenda meus_contatos.dat
   ```
4. Para apagar os arquivos compilados:
   ```bash
   make clean
   ```

## Exemplo de uso

```
====== AGENDA DE CONTATOS ======
1. Adicionar contato
2. Listar contatos
3. Buscar contato
4. Remover contato
0. Sair
Opcao: 2

  #  Nome                      | Telefone        | E-mail
---------------------------------------------------------------------
  1. Maria Silva               | 84 99999-0000   | maria@email.com

Total: 1 contato(s)
```

## Formato do arquivo

O `contatos.dat` guarda primeiro a quantidade de contatos (`size_t`) e, em seguida, o vetor de structs `Contato`. Ele é ignorado pelo Git (veja o `.gitignore`), pois são dados pessoais de cada usuário.

## Conceitos praticados

`struct`, vetores de structs, manipulação de arquivos binários (`fopen`, `fread`, `fwrite`, `fclose`), `memmove`, alocação dinâmica com `malloc`/`free`, leitura segura com `fgets`.
