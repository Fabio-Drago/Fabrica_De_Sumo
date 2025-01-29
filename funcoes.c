#include "funcoes.h"
#include <string.h>

// Inicializa o tapete rolante
TapeteRolante* inicializarTapete() {
    TapeteRolante* tapete = (TapeteRolante*)malloc(sizeof(TapeteRolante));
    if (tapete == NULL) return NULL;
    tapete->inicio = tapete->fim = NULL;
    tapete->direcao = 1; // Sentido horário padrão
    return tapete;
}

// Inicializa o empilhador
Empilhador* inicializarEmpilhador() {
    Empilhador* empilhador = (Empilhador*)malloc(sizeof(Empilhador));
    if (empilhador == NULL) return NULL;
    
    empilhador->pilhaLaranja.topo = -1;
    empilhador->pilhaAnanas.topo = -1;
    empilhador->fila.frente = empilhador->fila.tras = -1;
    empilhador->totalEmpilhamentos = 0;
    return empilhador;
}

// Libera toda a memória alocada
void liberarMemoria(TapeteRolante* tapete, Empilhador* empilhador) {
    // Liberar tapete
    if (tapete != NULL) {
        NodeTapete* atual = tapete->inicio;
        while (atual != NULL) {
            NodeTapete* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        free(tapete);
    }
    
    // Liberar empilhador
    if (empilhador != NULL) {
        free(empilhador);
    }
}

// Insere caixas a partir de um arquivo
void carregarCaixasFicheiro(TapeteRolante* tapete, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    
    int tipo, garrafas;
    while (fscanf(arquivo, "%d %d", &tipo, &garrafas) != EOF) {
        Caixa caixa = {garrafas, tipo, 0, 0, 110.0, 400.0};
        inserirCaixa(tapete, caixa);
    }
    fclose(arquivo);
}

// Insere uma caixa no tapete rolante
void inserirCaixa(TapeteRolante* tapete, Caixa caixa) {
    NodeTapete* novoNode = (NodeTapete*)malloc(sizeof(NodeTapete));
    if (novoNode == NULL) return;
    
    novoNode->caixa = caixa;
    novoNode->proximo = NULL;
    
    if (tapete->inicio == NULL) {
        tapete->inicio = tapete->fim = novoNode;
        novoNode->proximo = novoNode; // Lista circular
    } else {
        tapete->fim->proximo = novoNode;
        novoNode->proximo = tapete->inicio;
        tapete->fim = novoNode;
    }
}

// Valida caixas (remove as com menos de 6 garrafas)
void validarCaixas(TapeteRolante* tapete, Estatisticas* stats) {
    if (tapete->inicio == NULL) return;
    
    NodeTapete* atual = tapete->inicio;
    NodeTapete* anterior = tapete->fim;
    
    do {
        if (atual->caixa.numGarrafas < 6) {
            // Atualiza estatísticas
            if (atual->caixa.tipo == 0) stats->caixasDescartadasLaranja++;
            else stats->caixasDescartadasAnanas++;
            stats->prejuizoTotal += atual->caixa.custo;
            
            // Remove a caixa
            NodeTapete* temp = atual;
            anterior->proximo = atual->proximo;
            if (atual == tapete->inicio) tapete->inicio = atual->proximo;
            if (atual == tapete->fim) tapete->fim = anterior;
            atual = atual->proximo;
            free(temp);
        } else {
            atual->caixa.validada = true;
            anterior = atual;
            atual = atual->proximo;
        }
    } while (atual != tapete->inicio);
}

// Inverte o sentido do tapete
void inverterSentido(TapeteRolante* tapete) {
    tapete->direcao *= -1;
    printf("Sentido alterado para %s\n", (tapete->direcao == 1) ? "horário" : "anti-horário");
}

// Etiqueta as caixas validadas
void etiquetarCaixas(TapeteRolante* tapete, Estatisticas* stats) {
    if (tapete->inicio == NULL) return;
    
    NodeTapete* atual = tapete->inicio;
    do {
        if (atual->caixa.validada && !atual->caixa.etiquetada) {
            atual->caixa.etiquetada = true;
            // Atualiza estatísticas
            if (atual->caixa.tipo == 0) stats->caixasValidasLaranja++;
            else stats->caixasValidasAnanas++;
        }
        atual = atual->proximo;
    } while (atual != tapete->inicio);
}

// Remove uma caixa do tapete
Caixa removerCaixa(TapeteRolante* tapete) {
    if (tapete->inicio == NULL) {
        Caixa vazia = {0};
        return vazia;
    }
    
    NodeTapete* removido = tapete->inicio;
    Caixa caixa = removido->caixa;
    
    if (tapete->inicio == tapete->fim) {
        tapete->inicio = tapete->fim = NULL;
    } else {
        tapete->inicio = tapete->inicio->proximo;
        tapete->fim->proximo = tapete->inicio;
    }
    
    free(removido);
    return caixa;
}

// Enfileira caixas para empilhamento
void enfileirarCaixa(Empilhador* empilhador, Caixa caixa) {
    if (empilhador->fila.tras == -1) {
        empilhador->fila.frente = empilhador->fila.tras = 0;
    } else {
        empilhador->fila.tras = (empilhador->fila.tras + 1) % 2;
    }
    empilhador->fila.caixas[empilhador->fila.tras] = caixa;
}

// Empilha duas caixas na pilha correta
void empilharCaixas(Empilhador* empilhador, Estatisticas* stats) {
    if (empilhador->fila.frente == -1) return;
    
    // Verifica se há duas caixas na fila
    if (empilhador->fila.tras != 1) {
        printf("A fila não tem duas caixas para empilhar!\n");
        return;
    }
    
    Caixa caixa1 = empilhador->fila.caixas[0];
    Caixa caixa2 = empilhador->fila.caixas[1];
    
    if (caixa1.tipo != caixa2.tipo) {
        printf("Erro: Caixas de sabores diferentes!\n");
        return;
    }
    
    Pilha* pilha = (caixa1.tipo == 0) ? &empilhador->pilhaLaranja : &empilhador->pilhaAnanas;
    
    // Verifica se a pilha está cheia (8 caixas = 4 empilhamentos)
    if (pilha->topo >= 7) {
        printf("Pilha cheia! Crie uma nova.\n");
        return;
    }
    
    pilha->caixas[++pilha->topo] = caixa1;
    pilha->caixas[++pilha->topo] = caixa2;
    empilhador->totalEmpilhamentos++;
    
    // Atualiza estatísticas de lucro
    if (caixa1.tipo == 0) {
        stats->lucroLaranja += (caixa1.preco - caixa1.custo) * 2;
    } else {
        stats->lucroAnanas += (caixa1.preco - caixa1.custo) * 2;
    }
    
    // Limpa a fila
    empilhador->fila.frente = empilhador->fila.tras = -1;
}

// Funções de impressão (exemplos simplificados)
void imprimirTapete(TapeteRolante* tapete) {
    if (tapete->inicio == NULL) {
        printf("Tapete vazio!\n");
        return;
    }
    
    NodeTapete* atual = tapete->inicio;
    do {
        printf("Caixa: %d garrafas, Sabor: %s, Validada: %s, Etiquetada: %s\n",
               atual->caixa.numGarrafas,
               (atual->caixa.tipo == 0) ? "Laranja" : "Ananás",
               atual->caixa.validada ? "Sim" : "Não",
               atual->caixa.etiquetada ? "Sim" : "Não");
        atual = atual->proximo;
    } while (atual != tapete->inicio);
}

void imprimirFilaEmpilhamento(Empilhador* empilhador) {
    if (empilhador->fila.frente == -1) {
        printf("Fila de empilhamento vazia!\n");
        return;
    }
    for (int i = 0; i <= empilhador->fila.tras; i++) {
        printf("Caixa %d: Sabor %s\n", i+1,
               (empilhador->fila.caixas[i].tipo == 0) ? "Laranja" : "Ananás");
    }
}

void imprimirPilhas(Empilhador* empilhador) {
    printf("=== Pilha de Laranja ===\n");
    for (int i = empilhador->pilhaLaranja.topo; i >= 0; i--) {
        printf("Caixa: %d garrafas\n", empilhador->pilhaLaranja.caixas[i].numGarrafas);
    }
    
    printf("\n=== Pilha de Ananás ===\n");
    for (int i = empilhador->pilhaAnanas.topo; i >= 0; i--) {
        printf("Caixa: %d garrafas\n", empilhador->pilhaAnanas.caixas[i].numGarrafas);
    }
}

// Gera relatórios em arquivo
void gerarRelatorios(Estatisticas* stats, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de saída!\n");
        return;
    }
    
    fprintf(arquivo, "=== Relatório Final ===\n");
    fprintf(arquivo, "Caixas etiquetadas (Laranja): %d\n", stats->caixasValidasLaranja);
    fprintf(arquivo, "Caixas etiquetadas (Ananás): %d\n", stats->caixasValidasAnanas);
    fprintf(arquivo, "Total etiquetadas: %d\n", stats->caixasValidasLaranja + stats->caixasValidasAnanas);
    
    fprintf(arquivo, "\nCaixas descartadas (Laranja): %d\n", stats->caixasDescartadasLaranja);
    fprintf(arquivo, "Caixas descartadas (Ananás): %d\n", stats->caixasDescartadasAnanas);
    fprintf(arquivo, "Total descartadas: %d\n", stats->caixasDescartadasLaranja + stats->caixasDescartadasAnanas);
    
    fprintf(arquivo, "\nLucro Laranja: %.2f KZ\n", stats->lucroLaranja);
    fprintf(arquivo, "Lucro Ananás: %.2f KZ\n", stats->lucroAnanas);
    fprintf(arquivo, "Prejuízo Total: %.2f KZ\n", stats->prejuizoTotal);
    
    fclose(arquivo);
}

void verificarEstadoSistema(TapeteRolante* tapete, Empilhador* empilhador, Estatisticas* stats) {
    // Verificar estado do Tapete Rolante
    if (tapete->inicio != NULL) {
        printf("[AVISO] Tapete não está vazio! %d caixa(s) restantes.\n", 
               contarCaixasNoTapete(tapete));
    }

    // Verificar estado da Fila de Empilhamento
    if (empilhador->fila.frente != -1) {
        printf("[AVISO] Fila de empilhamento não está vazia! %d caixa(s) pendentes.\n", 
               empilhador->fila.tras - empilhador->fila.frente + 1);
    }

    // Verificar estado das Pilhas
    if (empilhador->pilhaLaranja.topo >= 0) {
        printf("[AVISO] Pilha de Laranja contém %d caixa(s)\n", 
               empilhador->pilhaLaranja.topo + 1);
    }
    if (empilhador->pilhaAnanas.topo >= 0) {
        printf("[AVISO] Pilha de Ananás contém %d caixa(s)\n", 
               empilhador->pilhaAnanas.topo + 1);
    }

    // Verificar integridade de memória (exemplo simplificado)
    if (tapete->inicio != NULL && tapete->fim == NULL) {
        printf("[ERRO] Inconsistência no tapete: início não nulo com fim nulo!\n");
    }

    // Verificar estatísticas negativas (indicador de erro)
    if (stats->caixasDescartadasLaranja < 0 || stats->caixasValidasAnanas < 0) {
        printf("[ERRO] Estatísticas com valores negativos!\n");
    }
}

// Função auxiliar para contar caixas no tapete (usada na verificação)
int contarCaixasNoTapete(TapeteRolante* tapete) {
    if (tapete->inicio == NULL) return 0;
    
    int count = 0;
    NodeTapete* atual = tapete->inicio;
    do {
        count++;
        atual = atual->proximo;
    } while (atual != tapete->inicio);
    
    return count;
}
