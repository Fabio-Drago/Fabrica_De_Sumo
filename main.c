#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Função para validar se uma string contém apenas dígitos
int apenasDigitos(const char* str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

// Função para imprimir o menu principal
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

// Função para imprimir o submenu de inserção de caixa
void imprimirSubmenuInserirCaixa() {
    printf("\n=== SUBMENU: INSERIR CAIXA ===\n");
    printf("1. Automático (arquivo)\n");
    printf("2. Manual\n");
}

// Função para imprimir o menu de impressão
void imprimirSubmenuImpressao() {
    printf("\n=== MENU DE IMPRESSÃO ===\n");
    printf("1. Imprimir Tapete\n");
    printf("2. Imprimir Fila de Empilhamento\n");
    printf("3. Imprimir Pilhas\n");
}

// Função melhorada para obter escolha válida do menu
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
            printf("Entrada vazia. Por favor, digite um número entre 1 e 9: ");
            continue;
        }

        if (!apenasDigitos(buffer)) {
            printf("Entrada inválida. Digite apenas números: ");
            continue;
        }

        escolha = atoi(buffer);
        
        if (escolha < 1 || escolha > 9) {
            printf("Opção inválida! Por favor, escolha um número entre 1 e 9: ");
            continue;
        }

        return escolha;
    }
}

// Função melhorada para obter escolha válida do submenu
int obterEscolhaSubmenu(int limiteMin, int limiteMax) {
    char buffer[100];
    int escolha;
    
    while (1) {
        printf("Escolha uma opção (%d-%d): ", limiteMin, limiteMax);
        
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Erro na leitura. Tente novamente.\n");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strlen(buffer) == 0) {
            printf("Entrada vazia. Por favor, digite um número entre %d e %d.\n", limiteMin, limiteMax);
            continue;
        }

        if (!apenasDigitos(buffer)) {
            printf("Entrada inválida. Digite apenas números.\n");
            continue;
        }

        escolha = atoi(buffer);
        
        if (escolha < limiteMin || escolha > limiteMax) {
            printf("Opção inválida! Por favor, escolha um número entre %d e %d.\n", limiteMin, limiteMax);
            continue;
        }

        return escolha;
    }
}

// Função melhorada para validar o tipo de sumo
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
            printf("Opção inválida! Digite apenas 0 para Laranja ou 1 para Ananas.\n");
            continue;
        }

        return tipo;
    }
}

// Função melhorada para validar o número de garrafas
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
            printf("Número inválido! Por favor, escolha um número entre 4 e 8.\n");
            continue;
        }

        return numGarrafas;
    }
}

// Função principal
int main() {
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

    Estatisticas stats = {0};
    int escolha;

    do {
        imprimirMenu();
        escolha = obterEscolhaMenu();

        switch (escolha) {
            case 1: {
                imprimirSubmenuInserirCaixa();
                int subEscolha = obterEscolhaSubmenu(1, 2);

                if (subEscolha == 1) {
                    carregarCaixasFicheiro(tapete, "entrada.txt");
                } else {
                    int tipo = obterTipoSumo();
                    int numGarrafas = obterNumeroGarrafas();
                    Caixa novaCaixa = {tipo, numGarrafas, 0}; // 0 indica não etiquetada
                    inserirCaixa(tapete, novaCaixa);
                }
                break;
            }

            case 2:
                validarCaixas(tapete, &stats);
                break;

            case 3:
                inverterSentido(tapete);
                break;

            case 4:
                etiquetarCaixas(tapete);
                break;

            case 5: {
                Caixa caixa = removerCaixa(tapete);
                if (caixa.etiquetada) {
                    enfileirarCaixa(empilhador, caixa);
                }
                break;
            }

            case 6:
                empilharCaixas(empilhador);
                break;

            case 7: {
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

            case 8:
                printf("\nGerando relatórios e finalizando simulação...\n");
                gerarRelatorios(&stats, "saida.txt");
                atualizarEstatisticas(&stats);
                verificarEstadoSistema(tapete, empilhador, &stats);

                liberarMemoria(tapete, empilhador);
                tapete = inicializarTapete();
                empilhador = inicializarEmpilhador();
                stats = (Estatisticas){0};
                printf("Simulação reiniciada!\n");
                break;

            case 9:
                printf("Encerrando programa...\n");
                break;
        }

    } while (escolha != 9);

    liberarMemoria(tapete, empilhador);
    printf("Programa encerrado com sucesso!\n");
    return 0;
}