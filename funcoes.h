// funcoes.h
#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições de constantes
#define MAX_NOME 50
#define MAX_PILHA 8  // 4 empilhamentos * 2 caixas
#define CUSTO_CAIXA 110
#define PRECO_VENDA 400

// Definições de tipos
typedef enum {
    LARANJA,
    ANANAS
} TipoSumo;

typedef struct {
    int numGarrafas;
    TipoSumo tipo;
    int validada;
    int etiquetada;
    double custo;
    double preco;
} Caixa;

typedef struct No {
    Caixa caixa;
    struct No* proximo;
    struct No* anterior;
} No;

typedef struct {
    No* atual;
    int sentidoHorario;
    int tamanho;
} TapeteRolante;

typedef struct NoFila {
    Caixa caixa;
    struct NoFila* proximo;
} NoFila;

typedef struct {
    NoFila* frente;
    NoFila* tras;
    int tamanho;
} FilaEmpilhamento;

typedef struct {
    Caixa* pilha;
    int topo;
    int capacidade;
} Pilha;

typedef struct {
    char nome[MAX_NOME];
    int caixasEmpilhadas;
    FilaEmpilhamento fila;
    Pilha pilhaLaranja;
    Pilha pilhaAnanas;
} Empilhador;

typedef struct {
    int laranjaEtiquetadas;
    int ananasEtiquetadas;
    int laranjaDescartadas;
    int ananasDescartadas;
    double lucroLaranja;
    double lucroAnanas;
} Estatisticas;

// Protótipos das funções do Tapete Rolante
TapeteRolante* inicializarTapete(void);
void inserirCaixa(TapeteRolante* tapete, Caixa caixa);
void inverterSentido(TapeteRolante* tapete);
void validarCaixas(TapeteRolante* tapete, Estatisticas* stats);
void etiquetarCaixas(TapeteRolante* tapete);
Caixa removerCaixa(TapeteRolante* tapete);
void imprimirTapete(TapeteRolante* tapete);

// Protótipos das funções do Empilhador
Empilhador* inicializarEmpilhador(void);
void enfileirarCaixa(Empilhador* empilhador, Caixa caixa);
void empilharCaixas(Empilhador* empilhador);
void imprimirFilaEmpilhamento(Empilhador* empilhador);
void imprimirPilhas(Empilhador* empilhador);

// Protótipos das funções de arquivo e relatórios
void carregarCaixasFicheiro(TapeteRolante* tapete, const char* nomeFicheiro);
void entradaManualCaixa(TapeteRolante* tapete);
void gerarRelatorios(Estatisticas* stats, const char* nomeFicheiro);
void liberarMemoria(TapeteRolante* tapete, Empilhador* empilhador);

// Protótipos das funções auxiliares
void atualizarEstatisticas(Estatisticas* stats);
int pilhaCheia(Pilha* pilha);
int pilhaVazia(Pilha* pilha);
void criarNovaPilha(Pilha* pilha);
void verificarEstadoSistema(TapeteRolante* tapete, Empilhador* empilhador, Estatisticas* stats);

#endif