#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definição da estrutura Caixa
typedef struct {
    int numGarrafas;    // Quantidade de garrafas (4-8)
    int tipo;           // 0-Laranja, 1-Ananas
    bool validada;      // Se passou pela validação
    bool etiquetada;    // Se foi etiquetada
    float custo;        // Custo de produção (110 KZ por caixa)
    float preco;        // Preço de venda (400 KZ por caixa)
} Caixa;

// Definição do nó para o tapete rolante (lista circular)
typedef struct NodeTapete {
    Caixa caixa;
    struct NodeTapete* proximo;
} NodeTapete;

// Estrutura do Tapete Rolante
typedef struct {
    NodeTapete* inicio;
    NodeTapete* fim;
    int direcao;        // 1 = horário, -1 = anti-horário
} TapeteRolante;

// Estrutura da Fila de Empilhamento
typedef struct {
    Caixa caixas[2];    // Armazena até 2 caixas para empilhar
    int frente;
    int tras;
} FilaEmpilhamento;

// Estrutura de uma Pilha (máximo 4 empilhamentos = 8 caixas)
typedef struct {
    Caixa caixas[8];    // Pilha de caixas (2 caixas por empilhamento)
    int topo;           // Índice do topo da pilha
} Pilha;

// Estrutura do Empilhador (TAD)
typedef struct {
    Pilha pilhaLaranja; // Pilha para caixas de laranja
    Pilha pilhaAnanas;  // Pilha para caixas de ananás
    FilaEmpilhamento fila;
    int totalEmpilhamentos;
} Empilhador;

// Estrutura para estatísticas
typedef struct {
    int caixasValidasLaranja;
    int caixasValidasAnanas;
    int caixasDescartadasLaranja;
    int caixasDescartadasAnanas;
    float lucroLaranja;
    float lucroAnanas;
    float prejuizoTotal;
} Estatisticas;

// Protótipos de funções
TapeteRolante* inicializarTapete();
Empilhador* inicializarEmpilhador();
void liberarMemoria(TapeteRolante* tapete, Empilhador* empilhador);

void carregarCaixasFicheiro(TapeteRolante* tapete, const char* nomeArquivo);
void inserirCaixa(TapeteRolante* tapete, Caixa caixa);
void validarCaixas(TapeteRolante* tapete, Estatisticas* stats);
void inverterSentido(TapeteRolante* tapete);
void etiquetarCaixas(TapeteRolante* tapete, Estatisticas* stats);
Caixa removerCaixa(TapeteRolante* tapete);
void enfileirarCaixa(Empilhador* empilhador, Caixa caixa);
void empilharCaixas(Empilhador* empilhador, Estatisticas* stats);
void imprimirTapete(TapeteRolante* tapete);
void imprimirFilaEmpilhamento(Empilhador* empilhador);
void imprimirPilhas(Empilhador* empilhador);
void gerarRelatorios(Estatisticas* stats, const char* nomeArquivo);
void verificarEstadoSistema(TapeteRolante* tapete, Empilhador* empilhador, Estatisticas* stats);
int contarCaixasNoTapete(TapeteRolante* tapete);

#endif
