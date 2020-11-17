#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void menu(); // funcao para o menu do jogo
char validaentrada(); // funcao para validar a entrada da opcao do jogo
void novoJogo(); // funcao para comecar um novo jogo
void carregarJogo(); // funcao para carregar um jogo ja comecado
void mostraCreditos(); // funcao para mostrar os creditos
void sair(); // funcao para mostrar a mensagem de saida do jogo

typedef struct gravacao {
    int identificador;
    int totalpts;
    int ultimafase;
    int vidas;
    char nomejogador[9];
}save;

typedef struct fase {
    int tamanhox;
    int tamanhoy;
    char elementos[x][y];
}fase;

typedef struct ponto {
    char x;
    char y;
}ponto;

typedef struct jogador_st {
    int totalpts;
    int fase;
    int vidas;
    char nomejogador[9];
}jogador;

int main()
{
    char opcao;
    printf("Lolo's Adventure\n");
    menu();
    do
    {
        opcao = validaentrada();
        switch (opcao)
        {
            case 'N': novoJogo();
                      break;
            case 'C': carregarJogo();
                      break;
            case 'M': mostraCreditos();
                      break;
        }
    }
    while (opcao != 'S'); // enquanto o usuario nao digitar S, ja que o programa repetira ate o usuario querer sair
    sair();
    return 0;
}

void menu()
{
    printf("(N) - Novo Jogo\n(C) - Carregar Jogo\n(M) - Mostrar Creditos\n(S) - Sair\n");
}

void mostraCreditos()
{
    printf("Creditos do jogo:\nDesenvolvido por: Vitor Caruso Rodrigues Ferrer (00327023)\n");
}

void sair()
{
    printf("Encerrando o jogo, ateh a proxima!\n");
}

char validaentrada()
{
    char opcao;
    do
    {
        printf("Digite uma opcao: ");
        fflush(stdin);
        scanf(" %c", &opcao);
        opcao = toupper(opcao); // para sempre ficar maiusculo, mesmo que o usuario digite minusculo
        if (opcao != 'N' && opcao != 'C' && opcao != 'M' && opcao != 'S')
        {
            printf("Opcao invalida\n");
            menu(); // imprime o menu para o usuario novamente
        }
    }
    while ((opcao != 'N') && (opcao != 'C') && (opcao != 'M') && (opcao != 'S')); // enquanto nao for um dos caracteres validos
    return opcao;
}

void novoJogo()
{
    char nome [13];

    printf("Digite seu nome (maximo de 10 caracteres): ");
    fflush(stdin);
    fgets(nome, 12, stdin);
    nome[strlen(nome)-1] = '\0';

}

void carregarJogo()
{
    printf("Carregando a lista de jogos salvos, aguarde um minuto.\n");
}



