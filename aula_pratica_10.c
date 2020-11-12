#include <stdio.h>
#include <stdlib.h>
#define JOGADORES 5

// estrutura jogador
struct jogador_st {
    int totalpts;
    int fase;
    int vidas;
    char nomejogador[9];
};
typedef struct jogador_st jogador_st;

// prototipos das funcoes auxiliares
jogador_st le_dados();
void imprime_dados(jogador_st);
int vencedor(jogador_st[], int);

// declaracao e definicao da funcao principal
int main(){

    jogador_st jogador[JOGADORES];
    int i, idx;
    for (i = 0; i < JOGADORES; i++){
        jogador[i] = le_dados();
    }
    for (i = 0; i < JOGADORES; i++){
        printf("\n=== Jogador %d ===\n\n", i + 1);
        imprime_dados(jogador[i]);  
    }
    idx = vencedor(jogador, JOGADORES);
    printf("\nJogador com maior pontuacao:\n\n");
    imprime_dados(jogador[idx]);

    return 0;
}

// definicao das funcoes auxiliares
// funcao para coleta dos dados:
jogador_st le_dados(){
    jogador_st jogador;
    printf("Digite o nome do Jogador:\n");
    fgets(jogador.nomejogador, 8, stdin);
    fflush(stdin);

    printf("Digite a quantidade de vidas que o jogador possui no momento:\n");
    scanf(" %d", &jogador.vidas);
    fflush(stdin);

    printf("Digite o total de pontos do jogador ate' o momento:\n");
    scanf(" %d", &jogador.totalpts);
    fflush(stdin);

    printf("Digite a fase atual do jogador:\n");
    scanf(" %d", &jogador.fase);
    fflush(stdin);

    return jogador;
}

// funcao para impressao dos dados:
void imprime_dados(jogador_st jogador){
    printf("Jogador: %s", jogador.nomejogador);
    printf("Vidas: %d\n", jogador.vidas);
    printf("Total de pontos: %d\n", jogador.totalpts);
    printf("Fase: %d\n", jogador.fase);
}

int vencedor(jogador_st jogador[], int tamanho){
    int i, idx;
    idx = 0;
    for (i = 0; i < tamanho; i++){
        if (jogador[i].totalpts >= jogador[idx].totalpts)
            idx = i;
    }
    
    return idx;
}
    