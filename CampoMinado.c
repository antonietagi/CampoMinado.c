#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MINA -1
#define REVELADA -2
#define MARCADA -3
#define MAX 20

int LINHAS, COLUNAS, MINAS;
int tabuleiro[MAX][MAX];
int visualizacao[MAX][MAX];
int modoMarcar = 0;

// Função para limpar o terminal
void limparTela() {
    system("cls");
}

// Função para exibir as regras e instruções do jogo
void imprimirRegras() {
    printf("\033[1;35m!!! BEM-VINDO(A) AO CAMPO MINADO !!!\033[0m\n\n");
    printf("\033[1;34mObjetivo:\033[0m Revelar todas as células sem minas.\n\n\n");
    printf("\033[1;34mInstruções:\033[0m\n\n");
    printf("\033[1;35m1.\033[0m Escolha o tamanho do tabuleiro e a quantidade de minas.\n");
    printf("\033[1;35m2.\033[0m Digite as coordenadas (linha e coluna) para revelar ou marcar uma mina.\n");
    printf("\033[1;35m3.\033[0m Se revelar uma mina, o jogo acaba! Se não, um número indicará quantas minas estão próximas.\n\n\n");
    printf("\033[1;34mBoa sorte!\033[0m\n");
    printf("\033[1;35m----------------------------------------------------------------------------------------------------\033[0m\n");
}

// Função que inicia o tabuleiro
void inicializarTabuleiro() {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            tabuleiro[i][j] = 0; // tabuleiro real: com minas e números de minas adjacentes
            visualizacao[i][j] = 0; // tabuleiro visível ao jogador: com células ocultas, reveladas e marcadas
        }
    }

    // Distribuindo minas aleatoriamente no tabuleiro
    int minasColocadas = 0;
    while (minasColocadas < MINAS) {
        int linha = rand() % LINHAS;
        int coluna = rand() % COLUNAS;

        // Verificando se a célula já contém uma mina
        if (tabuleiro[linha][coluna] != MINA) {
            tabuleiro[linha][coluna] = MINA;
            minasColocadas++;

            // Incrementando o valor de cada célula adjacente à mina colocada
            for (int i = linha - 1; i <= linha + 1; i++) {
                for (int j = coluna - 1; j <= coluna + 1; j++) {
                    if (i >= 0 && i < LINHAS && j >= 0 && j < COLUNAS && tabuleiro[i][j] != MINA) {
                        tabuleiro[i][j]++;
                    }
                }
            }
        }
    }
}

// Função que exibe o tabuleiro
void imprimirTabuleiro(int revelarTudo) { // o argumento determina se o tabuleiro real deve ser exibido ou não
    printf("\n     ");
    for (int j = 0; j < COLUNAS; j++) {
        printf("%2d  ", j + 1);
    }
    printf("\n");

    printf("   ");
    for (int j = 0; j < COLUNAS; j++) {
        printf("\033[1;30m----\033[0m");
    }
    printf("\033[1;30m---\033[0m\n");

    for (int i = 0; i < LINHAS; i++) {
        printf("%2d \033[1;30m|\033[0m", i + 1);

        for (int j = 0; j < COLUNAS; j++) {
            // Se o argumento for verdadeiro o tabuleiro exibirá todos os dados
            if (revelarTudo) {
                // Célula marcada que contém mina (X)
                if (tabuleiro[i][j] == MINA && visualizacao[i][j] == MARCADA) {
                    printf("  \033[1;31mX\033[0m ");
                // Célula marcada que não contém mina (!)
                } else if (visualizacao[i][j] == MARCADA && tabuleiro[i][j] != MINA) {
                    printf("  \033[1;34m!\033[0m ");
                // Célula não marcada que contém mina (X)
                } else if (tabuleiro[i][j] == MINA) {
                    printf("  \033[1;31mX\033[0m ");
                // Célula que não contém mina nem foi marcada, revela o número de minas adjacentes
                } else {
                    printf(" %2d ", tabuleiro[i][j]);
                }
            // Se o argumento for falso o tabuleiro exibe apenas as células reveladas e bandeiras
            } else {
                // Célula marcada (bandeira)
                if (visualizacao[i][j] == MARCADA) {
                    printf("  🚩 ");
                // Célula revelada (mina ou número de minas adjacentes)
                } else if (visualizacao[i][j] == REVELADA) {
                    if (tabuleiro[i][j] == MINA) {
                        printf("  \033[1;31mX\033[0m ");
                    } else {
                        printf(" %2d ", tabuleiro[i][j]);
                    }
                // Célula não revelada (?)
                } else {
                    printf("  \033[1;30m?\033[0m ");
                }
            }
        }
        printf("\033[1;30m |\033[0m\n");
    }

    printf("   ");
    for (int j = 0; j < COLUNAS; j++) {
        printf("\033[1;30m----\033[0m");
    }
    printf("\033[1;30m---\033[0m\n");
}

// Função para revelar uma célula no tabuleiro
int revelar(int linha, int coluna) {
    // Verifica que as coordenadas estão no limite do tabuleiro
    if (linha < 0 || linha >= LINHAS || coluna < 0 || coluna >= COLUNAS) {
        printf("Coordenadas inválidas!\n");
        return 0;
    }
    // Verifica se a célula já foi revelada
    if (visualizacao[linha][coluna] == REVELADA) {
        printf("Essa posição já foi revelada!\n");
        return 0;
    }
    // Marca a célula como revelada
    visualizacao[linha][coluna] = REVELADA;

    if (tabuleiro[linha][coluna] == MINA) {
        return -1; // Se a célula for uma mina, retorna que o jogador perdeu
    }
    // Caso a célula seja um espaço vazio
    if (tabuleiro[linha][coluna] == 0) {
        for (int i = linha - 1; i <= linha + 1; i++) {
            for (int j = coluna - 1; j <= coluna + 1; j++) {
                if (i >= 0 && i < LINHAS && j >= 0 && j < COLUNAS && visualizacao[i][j] != REVELADA) {
                    revelar(i, j); // chama recursivamente a função para revelar automaticamente
                }
            }
        }
    }
    return 1;
}
// Função para verificar se o jogador venceu
int checarVitoria() {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            // Se a célula não contém uma mina e não foi revelada, retorna 0, indicando que o jogo não acabou
            if (tabuleiro[i][j] != MINA && visualizacao[i][j] != REVELADA) {
                return 0;
            }
        }
    }
    // Se todas as células sem minas foram reveladas, retorna 1, sinalizando a vitória
    return 1;
}

// Função principal
int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); // configura caracteres do português
    srand(time(NULL)); // inicializa o gerador de números aleatórios

    imprimirRegras(); // imprime as regras
    printf("Pressione Enter para continuar...");
    getchar(); // espera que o jogador pressione Enter
    limparTela(); // limpa o terminal

    printf("Digite o \033[1;34mtamanho\033[0m do tabuleiro desejado (máximo %d): ", MAX);
    scanf("%d", &LINHAS); // recebe a quantidade de linhas
    COLUNAS = LINHAS; // recebe a quantidade de colunas (a mesma quantidade de linhas)

    printf("Digite quantas \033[1;35mminas\033[0m deseja esconder: ");
    scanf("%d", &MINAS); // recebe a quantidade de minas

    inicializarTabuleiro(); // inicializa o tabuleiro
    limparTela();

    int linha, coluna, resultado;
    char comando;

    while (1) { // laço para fazer o jogo continuar até a vitória ou derrota do jogador

        printf("\n");
        printf("Modo: %s\n", modoMarcar ? "\033[1;34mMarcar\033[0m" : "\033[1;35mRevelar\033[0m");
        imprimirTabuleiro(0); // imprime o tabuleiro visível
        printf("\n");
        printf("Para marcar uma possível mina, digite 'B'.\nDigite as coordenadas (linha e coluna): ");
        scanf(" %c", &comando); // espera coordenadas ou um comando para marcar
        limparTela();

        // Ativando o modo marcar
        if (comando == 'B' || comando == 'b') {
            modoMarcar = 1;
            limparTela();
            printf("Modo de marcação ativado. Digite as coordenadas para marcar uma possível mina com uma bandeira.\n");
        } else {

            ungetc(comando, stdin); // volta para o fluxo de entrada
            if (scanf("%d %d", &linha, &coluna) != 2) {
                limparTela();
                printf("Coordenadas inválidas!\n");
                getchar();
                continue;
            }

            linha--; coluna--; // ajustando o índice no tabuleiro

            // Verifica se a célula já foi revelada
            if (modoMarcar) {
                if (visualizacao[linha][coluna] == REVELADA) {
                    printf("Essa posição já foi revelada, não pode ser marcada!\n");
                } else {
                    visualizacao[linha][coluna] = MARCADA;
                    modoMarcar = 0;
                }
            // Chama a função revelar
            } else {
                resultado = revelar(linha, coluna);

                // Derrota
                if (resultado == -1) {
                    limparTela();
                    imprimirTabuleiro(1);
                    printf("\033[1;31mVocê revelou uma mina! Game Over!\033[0m\n\n");
                    break;
                // Vitória
                } else if (checarVitoria()) {
                    limparTela();
                    imprimirTabuleiro(1);
                    printf("\033[1;33mPARABÉNS! Você revelou todas as células sem minas!\033[0m\n");
                    break;
                }
            }
        }

    }

    return 0;
}
