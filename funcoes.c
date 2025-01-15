#include "funcoes.h"

// Funções do Tapete Rolante
TapeteRolante* inicializarTapete() {
    TapeteRolante* tapete = (TapeteRolante*)malloc(sizeof(TapeteRolante));
    if (tapete == NULL) return NULL;
    
    tapete->atual = NULL;
    tapete->sentidoHorario = 1;
    tapete->tamanho = 0;
    return tapete;
}

void inserirCaixa(TapeteRolante* tapete, Caixa caixa) {
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) return;
    
    novoNo->caixa = caixa;
    
    if (tapete->atual == NULL) {
        novoNo->proximo = novoNo;
        novoNo->anterior = novoNo;
        tapete->atual = novoNo;
    } else {
        novoNo->proximo = tapete->atual->proximo;
        novoNo->anterior = tapete->atual;
        tapete->atual->proximo->anterior = novoNo;
        tapete->atual->proximo = novoNo;
    }
    tapete->tamanho++;
}

void inverterSentido(TapeteRolante* tapete) {
    if (tapete == NULL) return;
    tapete->sentidoHorario = !tapete->sentidoHorario;
}

void validarCaixas(TapeteRolante* tapete, Estatisticas* stats) {
    if (tapete == NULL || tapete->atual == NULL) return;
    
    No* atual = tapete->atual;
    do {
        if (!atual->caixa.validada) {
            if (atual->caixa.numGarrafas >= 4 && atual->caixa.numGarrafas <= 8) {
                atual->caixa.validada = 1;
            } else {
                // Atualizar estatísticas para caixas descartadas
                if (atual->caixa.tipo == LARANJA) {
                    stats->laranjaDescartadas++;
                } else {
                    stats->ananasDescartadas++;
                }
            }
        }
        atual = atual->proximo;
    } while (atual != tapete->atual);
}

void etiquetarCaixas(TapeteRolante* tapete) {
    if (tapete == NULL || tapete->atual == NULL) return;
    
    No* atual = tapete->atual;
    do {
        if (atual->caixa.validada && !atual->caixa.etiquetada) {
            atual->caixa.etiquetada = 1;
            atual->caixa.custo = atual->caixa.numGarrafas * CUSTO_CAIXA;
            atual->caixa.preco = atual->caixa.numGarrafas * PRECO_VENDA;
        }
        atual = atual->proximo;
    } while (atual != tapete->atual);
}

Caixa removerCaixa(TapeteRolante* tapete) {
    Caixa caixaVazia = {0, LARANJA, 0, 0, 0.0, 0.0};
    if (tapete == NULL || tapete->atual == NULL) return caixaVazia;
    
    No* noRemover = tapete->atual;
    Caixa caixaRemovida = noRemover->caixa;
    
    if (tapete->tamanho == 1) {
        free(noRemover);
        tapete->atual = NULL;
    } else {
        tapete->atual = tapete->sentidoHorario ? noRemover->proximo : noRemover->anterior;
        noRemover->anterior->proximo = noRemover->proximo;
        noRemover->proximo->anterior = noRemover->anterior;
        free(noRemover);
    }
    
    tapete->tamanho--;
    return caixaRemovida;
}

// Funções do Empilhador
Empilhador* inicializarEmpilhador() {
    Empilhador* emp = (Empilhador*)malloc(sizeof(Empilhador));
    if (emp == NULL) return NULL;
    
    strcpy(emp->nome, "Empilhador1");
    emp->caixasEmpilhadas = 0;
    emp->fila.frente = emp->fila.tras = NULL;
    emp->fila.tamanho = 0;
    
    // Inicializar pilhas
    emp->pilhaLaranja.pilha = (Caixa*)malloc(MAX_PILHA * sizeof(Caixa));
    emp->pilhaAnanas.pilha = (Caixa*)malloc(MAX_PILHA * sizeof(Caixa));
    
    if (emp->pilhaLaranja.pilha == NULL || emp->pilhaAnanas.pilha == NULL) {
        free(emp->pilhaLaranja.pilha);
        free(emp->pilhaAnanas.pilha);
        free(emp);
        return NULL;
    }
    
    emp->pilhaLaranja.topo = -1;
    emp->pilhaLaranja.capacidade = MAX_PILHA;
    emp->pilhaAnanas.topo = -1;
    emp->pilhaAnanas.capacidade = MAX_PILHA;
    
    return emp;
}

void enfileirarCaixa(Empilhador* empilhador, Caixa caixa) {
    if (empilhador == NULL) return;
    
    NoFila* novoNo = (NoFila*)malloc(sizeof(NoFila));
    if (novoNo == NULL) return;
    
    novoNo->caixa = caixa;
    novoNo->proximo = NULL;
    
    if (empilhador->fila.tras == NULL) {
        empilhador->fila.frente = empilhador->fila.tras = novoNo;
    } else {
        empilhador->fila.tras->proximo = novoNo;
        empilhador->fila.tras = novoNo;
    }
    empilhador->fila.tamanho++;
}

void empilharCaixas(Empilhador* empilhador) {
    if (empilhador == NULL || empilhador->fila.frente == NULL) return;
    
    while (empilhador->fila.frente != NULL) {
        NoFila* temp = empilhador->fila.frente;
        Pilha* pilhaDestino = temp->caixa.tipo == LARANJA ? 
                             &empilhador->pilhaLaranja : &empilhador->pilhaAnanas;
        
        if (pilhaDestino->topo < pilhaDestino->capacidade - 1) {
            pilhaDestino->topo++;
            pilhaDestino->pilha[pilhaDestino->topo] = temp->caixa;
            empilhador->caixasEmpilhadas++;
        }
        
        empilhador->fila.frente = temp->proximo;
        if (empilhador->fila.frente == NULL) {
            empilhador->fila.tras = NULL;
        }
        free(temp);
        empilhador->fila.tamanho--;
    }
}

// Funções de arquivo e relatórios
void carregarCaixasFicheiro(TapeteRolante* tapete, const char* nomeFicheiro) {
    FILE* arquivo = fopen(nomeFicheiro, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeFicheiro);
        return;
    }
    
    Caixa novaCaixa;
    int tipo;
    while (fscanf(arquivo, "%d %d", &tipo, &novaCaixa.numGarrafas) == 2) {
        novaCaixa.tipo = (TipoSumo)tipo;
        novaCaixa.validada = 0;
        novaCaixa.etiquetada = 0;
        novaCaixa.custo = 0.0;
        novaCaixa.preco = 0.0;
        inserirCaixa(tapete, novaCaixa);
    }
    
    fclose(arquivo);
}

void entradaManualCaixa(TapeteRolante* tapete) {
    Caixa novaCaixa;
    printf("Tipo de sumo (0-Laranja, 1-Ananas): ");
    scanf("%d", (int*)&novaCaixa.tipo);
    printf("Número de garrafas (4-8): ");
    scanf("%d", &novaCaixa.numGarrafas);
    
    novaCaixa.validada = 0;
    novaCaixa.etiquetada = 0;
    novaCaixa.custo = 0.0;
    novaCaixa.preco = 0.0;
    
    inserirCaixa(tapete, novaCaixa);
}

void gerarRelatorios(Estatisticas* stats, const char* nomeFicheiro) {
    FILE* arquivo = fopen(nomeFicheiro, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de relatório\n");
        return;
    }
    
    fprintf(arquivo, "=== RELATÓRIO DE PRODUÇÃO ===\n");
    fprintf(arquivo, "Caixas de Laranja:\n");
    fprintf(arquivo, "- Etiquetadas: %d\n", stats->laranjaEtiquetadas);
    fprintf(arquivo, "- Descartadas: %d\n", stats->laranjaDescartadas);
    fprintf(arquivo, "- Lucro: %.2f€\n", stats->lucroLaranja);
    
    fprintf(arquivo, "\nCaixas de Ananás:\n");
    fprintf(arquivo, "- Etiquetadas: %d\n", stats->ananasEtiquetadas);
    fprintf(arquivo, "- Descartadas: %d\n", stats->ananasDescartadas);
    fprintf(arquivo, "- Lucro: %.2f€\n", stats->lucroAnanas);
    
    fprintf(arquivo, "\nLucro Total: %.2f€\n", stats->lucroLaranja + stats->lucroAnanas);
    
    fclose(arquivo);
}

// Funções de impressão
void imprimirTapete(TapeteRolante* tapete) {
    if (tapete == NULL || tapete->atual == NULL) {
        printf("Tapete vazio\n");
        return;
    }
    
    No* atual = tapete->atual;
    printf("\n=== ESTADO DO TAPETE ===\n");
    printf("Sentido: %s\n", tapete->sentidoHorario ? "Horário" : "Anti-horário");
    printf("Número de caixas: %d\n\n", tapete->tamanho);
    
    do {
        printf("Tipo: %s, Garrafas: %d, ", 
               atual->caixa.tipo == LARANJA ? "Laranja" : "Ananás",
               atual->caixa.numGarrafas);
        printf("Validada: %s, Etiquetada: %s\n",
               atual->caixa.validada ? "Sim" : "Não",
               atual->caixa.etiquetada ? "Sim" : "Não");
        atual = atual->proximo;
    } while (atual != tapete->atual);
}

void imprimirFilaEmpilhamento(Empilhador* empilhador) {
    if (empilhador == NULL || empilhador->fila.frente == NULL) {
        printf("Fila de empilhamento vazia\n");
        return;
    }
    
    printf("\n=== FILA DE EMPILHAMENTO ===\n");
    NoFila* atual = empilhador->fila.frente;
    int posicao = 1;
    
    while (atual != NULL) {
        printf("Posição %d: Tipo: %s, Garrafas: %d\n",
               posicao++,
               atual->caixa.tipo == LARANJA ? "Laranja" : "Ananás",
               atual->caixa.numGarrafas);
        atual = atual->proximo;
    }
}

void imprimirPilhas(Empilhador* empilhador) {
    if (empilhador == NULL) return;
    
    printf("\n=== ESTADO DAS PILHAS ===\n");
    
    printf("Pilha de Laranja (capacidade: %d):\n", empilhador->pilhaLaranja.capacidade);
    for (int i = empilhador->pilhaLaranja.topo; i >= 0; i--) {
        printf("Nível %d: %d garrafas\n", i + 1, empilhador->pilhaLaranja.pilha[i].numGarrafas);
    }
    
    printf("\nPilha de Ananás (capacidade: %d):\n", empilhador->pilhaAnanas.capacidade);
    for (int i = empilhador->pilhaAnanas.topo; i >= 0; i--) {
        printf("Nível %d: %d garrafas\n", i + 1, empilhador->pilhaAnanas.pilha[i].numGarrafas);
    }
}

// Funções auxiliares
void liberarMemoria(TapeteRolante* tapete, Empilhador* empilhador) {
    if (tapete != NULL) {
        while (tapete->tamanho > 0) {
            removerCaixa(tapete);
        }
        free(tapete);
    }
    
    if (empilhador != NULL) {
        // Liberar fila de empilhamento
        while (empilhador->fila.frente != NULL) {
            NoFila* temp = empilhador->fila.frente;
            empilhador->fila.frente = temp->proximo;
            free(temp);
        }
        
        // Liberar pilhas
        free(empilhador->pilhaLaranja.pilha);
        free(empilhador->pilhaAnanas.pilha);
        free(empilhador);
    }
}

void atualizarEstatisticas(Estatisticas* stats) {
    if (stats == NULL) return;
    
    // Calcular lucros
    stats->lucroLaranja = (PRECO_VENDA - CUSTO_CAIXA) * stats->laranjaEtiquetadas;
    stats->lucroAnanas = (PRECO_VENDA - CUSTO_CAIXA) * stats->ananasEtiquetadas;
}

void verificarEstadoSistema(TapeteRolante* tapete, Empilhador* empilhador, Estatisticas* stats) {
    printf("\n=== VERIFICAÇÃO DO SISTEMA ===\n");
    
   // Verificar tapete
    printf("Tapete: %d caixas\n", tapete->tamanho);
    
    // Verificar fila de empilhamento
    printf("Fila de empilhamento: %d caixas\n", empilhador->fila.tamanho);
    
    // Verificar pilhas
    printf("Pilha de Laranja: %d/%d caixas\n", 
           empilhador->pilhaLaranja.topo + 1, 
           empilhador->pilhaLaranja.capacidade);
    printf("Pilha de Ananás: %d/%d caixas\n", 
           empilhador->pilhaAnanas.topo + 1, 
           empilhador->pilhaAnanas.capacidade);
    
    // Estatísticas
    printf("\nEstatísticas:\n");
    printf("Caixas de Laranja etiquetadas: %d\n", stats->laranjaEtiquetadas);
    printf("Caixas de Ananás etiquetadas: %d\n", stats->ananasEtiquetadas);
    printf("Caixas de Laranja descartadas: %d\n", stats->laranjaDescartadas);
    printf("Caixas de Ananás descartadas: %d\n", stats->ananasDescartadas);
    printf("Lucro total atual: %.2f€\n", stats->lucroLaranja + stats->lucroAnanas);
}

int pilhaCheia(Pilha* pilha) {
    if (pilha == NULL) return 1;
    return pilha->topo >= pilha->capacidade - 1;
}

int pilhaVazia(Pilha* pilha) {
    if (pilha == NULL) return 1;
    return pilha->topo == -1;
}

void criarNovaPilha(Pilha* pilha) {
    if (pilha == NULL) return;
    
    pilha->pilha = (Caixa*)malloc(MAX_PILHA * sizeof(Caixa));
    if (pilha->pilha != NULL) {
        pilha->topo = -1;
        pilha->capacidade = MAX_PILHA;
    }
}