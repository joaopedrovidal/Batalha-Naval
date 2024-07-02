#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 8
#define AGUA '~'
#define ACERTO 'X'
#define ERRO 'O'
#define SUBMARINO 'S'
#define NAVIO_DE_GUERRA 'B'
#define PORTA_AVIOES 'C'

typedef struct {
    char grade[TAMANHO][TAMANHO];
    int naviosRestantes;
} Tabuleiro;

void inicializarTabuleiro(Tabuleiro *tabuleiro) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            tabuleiro->grade[i][j] = AGUA;
        }
    }
    tabuleiro->naviosRestantes = 8;
}

void imprimirTabuleiro(Tabuleiro *tabuleiro, int esconderNavios) {
    printf("  ");
    for (int i = 0; i < TAMANHO; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < TAMANHO; i++) {
        printf("%d ", i);
        for (int j = 0; j < TAMANHO; j++) {
            if (esconderNavios && (tabuleiro->grade[i][j] == SUBMARINO || tabuleiro->grade[i][j] == NAVIO_DE_GUERRA || tabuleiro->grade[i][j] == PORTA_AVIOES)) {
                printf("%c ", AGUA);
            } else {
                printf("%c ", tabuleiro->grade[i][j]);
            }
        }
        printf("\n");
    }
}

int posicaoValida(Tabuleiro *tabuleiro, int x, int y, int comprimento, int direcao) {

    if (direcao == 0) {
        if (y + comprimento > TAMANHO) return 0;
        for (int i = 0; i < comprimento; i++) {
            if (tabuleiro->grade[x][y + i] != AGUA) return 0;
        }
    } else {
        if (x + comprimento > TAMANHO) return 0;
        for (int i = 0; i < comprimento; i++) {
            if (tabuleiro->grade[x + i][y] != AGUA) return 0;
        }
    }
    return 1;
}

void posicionarNavio(Tabuleiro *tabuleiro, int comprimento, char simbolo) {
    int x, y, direcao;
    do {
        x = rand() % TAMANHO;
        y = rand() % TAMANHO;
        direcao = rand() % 2;
    } while (!posicaoValida(tabuleiro, x, y, comprimento, direcao));

    for (int i = 0; i < comprimento; i++) {
        if (direcao == 0) {
            tabuleiro->grade[x][y + i] = simbolo;
        } else {
            tabuleiro->grade[x + i][y] = simbolo;
        }
    }
}

void inicializarNavios(Tabuleiro *tabuleiro) {
    for (int i = 0; i < 5; i++) {
        posicionarNavio(tabuleiro, 1, SUBMARINO);
    }
    for (int i = 0; i < 2; i++) {
        posicionarNavio(tabuleiro, 2, NAVIO_DE_GUERRA);
    }
    posicionarNavio(tabuleiro, 3, PORTA_AVIOES);
}

int atirar(Tabuleiro *tabuleiro, int x, int y) {
    if (tabuleiro->grade[x][y] == AGUA) {
        tabuleiro->grade[x][y] = ERRO;
        printf("Errou!\n");
        return 0;
    } else if (tabuleiro->grade[x][y] == SUBMARINO || tabuleiro->grade[x][y] == NAVIO_DE_GUERRA || tabuleiro->grade[x][y] == PORTA_AVIOES) {
        printf("Acertou!\n");
        tabuleiro->grade[x][y] = ACERTO;
        tabuleiro->naviosRestantes--;
        return 1;
    } else {
        printf("Já atirou aqui!\n");
        return 0;
    }
}

void vezDoUsuario(Tabuleiro *tabuleiroComputador) {
    int x, y;
    do {
        printf("Digite as coordenadas para atirar (linha e coluna): ");
        scanf("%d %d", &x, &y);
    } while (x < 0 || x >= TAMANHO || y < 0 || y >= TAMANHO || tabuleiroComputador->grade[x][y] == ACERTO || tabuleiroComputador->grade[x][y] == ERRO);
    atirar(tabuleiroComputador, x, y);
}

void vezDoComputador(Tabuleiro *tabuleiroUsuario) {
    int x, y;
    do {
        x = rand() % TAMANHO;
        y = rand() % TAMANHO;
    } while (tabuleiroUsuario->grade[x][y] == ACERTO || tabuleiroUsuario->grade[x][y] == ERRO);
    printf("Computador atira em %d %d\n", x, y);
    atirar(tabuleiroUsuario, x, y);
}

int main() {
    srand(time(NULL));
    Tabuleiro tabuleiroUsuario, tabuleiroComputador;
    inicializarTabuleiro(&tabuleiroUsuario);
    inicializarTabuleiro(&tabuleiroComputador);
    inicializarNavios(&tabuleiroUsuario);
    inicializarNavios(&tabuleiroComputador);

    printf("Seu Tabuleiro:\n");
    imprimirTabuleiro(&tabuleiroUsuario, 0);
    printf("\nTabuleiro do Computador:\n");
    imprimirTabuleiro(&tabuleiroComputador, 1);

    while (tabuleiroUsuario.naviosRestantes > 0 && tabuleiroComputador.naviosRestantes > 0) {
        vezDoUsuario(&tabuleiroComputador);
        if (tabuleiroComputador.naviosRestantes == 0) {
            printf("Você venceu!\n");
            break;
        }
        vezDoComputador(&tabuleiroUsuario);
        if (tabuleiroUsuario.naviosRestantes == 0) {
            printf("Computador venceu!\n");
            break;
        }
        printf("\nSeu Tabuleiro:\n");
        imprimirTabuleiro(&tabuleiroUsuario, 0);
        printf("\nTabuleiro do Computador:\n");
        imprimirTabuleiro(&tabuleiroComputador, 1);
    }

    return 0;
}
