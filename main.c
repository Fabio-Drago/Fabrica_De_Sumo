#include "funcoes.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Função para validar entrada de apenas dígitos
int apenasDigitos(const char* str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

// Funções do Menu
void imprimirMenu() {
    printf("\n===== FÁBRICA DE SUMOS =====\n");
    printf("1. Inserir caixa no tapete\n");
    printf("2. Validar\n");
    printf("3. Inverter\n");
    printf("4. Etiquetar\n");
    printf("5. Encaminhar\n");
    printf("6. Empilhar\n");
    printf("7. Imprimir\n");
    printf("8. Terminar simulação\n");
    printf("9. Sair\n");
    printf("Escolha uma opção: ");
}

void imprimirSubmenuInserirCaixa() {
    printf("\n=== SUBMENU: INSERIR CAIXA ===\n");
    printf("1. Automático (arquivo)\n");
    printf("2. Manual\n");
}

void imprimirSubmenuImpressao() {
    printf("\n=== MENU DE IMPRESSÃO ===\n");
    printf("1. Imprimir Tapete\n");
    printf("2. Imprimir Fila de Empilhamento\n");
    printf("3. Imprimir Pilhas\n");
}

// Funções de validação de entrada
int obterEscolhaMenu() {
    char buffer[100];
    int escolha;
    
    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Erro na leitura. Tente novamente: ");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strlen(buffer) == 0) {
            printf("Entrada vazia. Digite um número entre 1 e 9: ");
            continue;
        }

        if (!apenasDigitos(buffer)) {
            printf("Entrada inválida. Digite apenas números: ");
            continue;
        }

        escolha = atoi(buffer);
        if (escolha < 1 || escolha > 9) {
            printf("Opção inválida! Escolha um número entre 1 e 9: ");
            continue;
        }

        return escolha;
    }
}

int obterEscolhaSubmenu(int min, int max) {
    char buffer[100];
    int escolha;
    
    while (1) {
        printf("Escolha uma opção (%d-%d): ", min, max);
        
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Erro na leitura. Tente novamente.\n");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strlen(buffer) == 0) {
            printf("Entrada vazia. Digite um número entre %d e %d.\n", min, max);
            continue;
        }

        if (!apenasDigitos(buffer)) {
            printf("Entrada inválida. Digite apenas números.\n");
            continue;
        }

        escolha = atoi(buffer);
        if (escolha < min || escolha > max) {   
            printf("Opção inválida! Escolha um número entre %d e %d.\n", min, max);
            continue;
        }

        return escolha;
    }
}

int obterTipoSumo() {
    char buffer[100];
    int tipo;
    
    while (1) {
        printf("Tipo de sumo (0-Laranja, 1-Ananas): ");
        
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Erro na leitura. Tente novamente.\n");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strlen(buffer) == 0) {
            printf("Entrada vazia. Digite 0 para Laranja ou 1 para Ananas.\n");
            continue;
        }

        if (!apenasDigitos(buffer)) {
            printf("Entrada inválida. Digite apenas 0 ou 1.\n");
            continue;
        }

        tipo = atoi(buffer);
        if (tipo != 0 && tipo != 1) {
            printf("Tipo inválido! Digite 0 para Laranja ou 1 para Ananas.\n");
            continue;
        }

        return tipo;
    }
}

int obterNumeroGarrafas() {
    char buffer[100];
    int numGarrafas;
    
    while (1) {
        printf("Número de garrafas (4-8): ");
        
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Erro na leitura. Tente novamente.\n");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strlen(buffer) == 0) {
            printf("Entrada vazia. Digite um número entre 4 e 8.\n");
            continue;
        }

        if (!apenasDigitos(buffer)) {
            printf("Entrada inválida. Digite apenas números.\n");
            continue;
        }

        numGarrafas = atoi(buffer);
        if (numGarrafas < 4 || numGarrafas > 8) {
            printf("Número inválido! Digite um número entre 4 e 8.\n");
            continue;
        }

        return numGarrafas;
    }
}

int main() {
    // Inicialização do sistema
    TapeteRolante* tapete = inicializarTapete();
    if (tapete == NULL) {
        printf("Erro ao inicializar tapete. Encerrando programa.\n");
        return 1;
    }

    Empilhador* empilhador = inicializarEmpilhador();
    if (empilhador == NULL) {
        printf("Erro ao inicializar empilhador. Encerrando programa.\n");
        liberarMemoria(tapete, NULL);
        return 1;
    }

    Estatisticas stats = {0};  // Inicializa todas as estatísticas com zero
    int escolha;

    // Loop principal do programa
    do {
        imprimirMenu();
        escolha = obterEscolhaMenu();

        switch (escolha) {
            case 1: { // Inserir caixa
                imprimirSubmenuInserirCaixa();
                int subEscolha = obterEscolhaSubmenu(1, 2);

                if (subEscolha == 1) {
                    carregarCaixasFicheiro(tapete, "entrada.txt");
                } else {
                    int tipo = obterTipoSumo();
                    int numGarrafas = obterNumeroGarrafas();
                    Caixa novaCaixa = {numGarrafas, tipo, 0, 0, 0.0, 0.0};
                    inserirCaixa(tapete, novaCaixa);
                    printf("Caixa inserida com sucesso!\n");
                }
                break;
            }

            case 2: { // Validar
                validarCaixas(tapete, &stats);
                printf("Caixas validadas com sucesso!\n");
                break;
            }

            case 3: { // Inverter
                inverterSentido(tapete);
                printf("Sentido do tapete invertido!\n");
                break;
            }

            case 4: { // Etiquetar
                // Validar caixas antes de etiquetar
                validarCaixas(tapete, &stats);
                etiquetarCaixas(tapete, &stats);
                printf("Caixas etiquetadas com sucesso!\n");
                break;
            }

            case 5: { // Encaminhar
                // Encaminhar duas caixas por vez para empilhamento
                for (int i = 0; i < 2; i++) {
                    Caixa caixa = removerCaixa(tapete);
                    if (caixa.numGarrafas > 0) { 
                        if (caixa.validada && caixa.etiquetada) {
                            enfileirarCaixa(empilhador, caixa);
                        } else {
                            printf("Caixa removida do tapete (não validada/etiquetada).\n");
                        }
                    } else {
                        if (i == 0) printf("Nenhuma caixa disponível para encaminhar.\n");
                        break;
                    }
                }
                printf("Caixas encaminhadas para empilhamento!\n");
                break;
            }

            case 6: { // Empilhar
                empilharCaixas(empilhador, &stats);
                printf("Caixas empilhadas com sucesso!\n");
                break;
            }

            case 7: { // Imprimir
                imprimirSubmenuImpressao();
                int subEscolha = obterEscolhaSubmenu(1, 3);
                
                switch (subEscolha) {
                    case 1:
                        imprimirTapete(tapete);
                        break;
                    case 2:
                        imprimirFilaEmpilhamento(empilhador);
                        break;
                    case 3:
                        imprimirPilhas(empilhador);
                        break;
                }
                break;
            }

            case 8: { // Terminar simulação
                printf("\nGerando relatórios e finalizando simulação...\n");
                gerarRelatorios(&stats, "saida.txt");
                verificarEstadoSistema(tapete, empilhador, &stats);

                // Reiniciar sistema
                liberarMemoria(tapete, empilhador);
                tapete = inicializarTapete();
                empilhador = inicializarEmpilhador();
                stats = (Estatisticas){0};
                printf("Simulação reiniciada!\n");
                break;
            }

            case 9: // Sair
                printf("Encerrando programa...\n");
                break;
        }

    } while (escolha != 9);

    // Limpeza final
    liberarMemoria(tapete, empilhador);
    printf("Programa encerrado com sucesso!\n");
    return 0;
}
