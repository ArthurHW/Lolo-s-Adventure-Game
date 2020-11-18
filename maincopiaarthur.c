#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include<conio2.h>

// definição das structs:
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
    char elementos[13][14];
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

// protótipos das funções auxiliares:
void menu();
void imprime_menu(); // funcao para imprimir o menu do jogo
char validaentrada(); // funcao para validar a entrada da opcao do jogo
save novoJogo(); // funcao para comecar um novo jogo
save carregarJogo(); // funcao para carregar um jogo ja comecado
void mostraCreditos(); // funcao para mostrar os creditos
void sair(); // funcao para mostrar a mensagem de saida do jogo
void imprime_saves(FILE*);// dado um save imprime ele na tela formatado
fase gera_fase(FILE*);
void imprime_mapa(fase);



// Função principal
int main()
{
    menu();

    return 0;
}


// Funções Auxiliares
void menu(){

    char opcao;
    save jogador;
    printf("---- Lolo's Adventure ----\n");


        imprime_menu();
        opcao = validaentrada();
        switch (opcao)
        {
            case 'N':   jogador = novoJogo();
                        break;
            case 'C':   jogador = carregarJogo();
                        break;
            case 'M':   mostraCreditos();
                        break;
            case 'S':   sair();
                        break;
        }

}

void imprime_menu()
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
            imprime_menu(); // imprime o menu para o usuario novamente
        }
    }
    while ((opcao != 'N') && (opcao != 'C') && (opcao != 'M') && (opcao != 'S')); // enquanto nao for um dos caracteres validos
    return opcao;
}

save novoJogo()
{
    int op = 0;
    int id = 0;
    save novo_jogador, buffer;
    FILE* arq;

    // abrir o arquivo com os saves
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");

    // ver quantos saves tem e salvar na variavel id
    else {
        while(!feof(arq)){
            if(fread(&buffer, sizeof(save), 1, arq) == 1)
                id++;

        }
        if(id >= 3){
            printf("Limite de Jogos salvos atingido (3), deseja sobrescrever seus dados?\n(1) - Sim\n(2)- Nao\n");
            do{
                scanf(" %d", &op);
            } while(op != 1 && op != 2);
        }

        if (op != 2){
            // solicitar e salvar o nome do jogador
            if (op == 1){
                imprime_saves(arq);
                printf("Qual id deseja sobrecrever?\n");
                do {
                scanf(" %d", &id);
                } while(id != 0 && id != 1 && id != 2);
                fseek(arq, sizeof(save) * id, SEEK_SET);
            }
            printf("Digite seu nome (maximo de 8 caracteres): ");
            fflush(stdin);
            fgets(novo_jogador.nomejogador, 8, stdin);
            novo_jogador.nomejogador[strlen(novo_jogador.nomejogador)-1] = '\0';

            // salvar os outros dados do jogador
            novo_jogador.identificador = id;
            novo_jogador.totalpts = 0;
            novo_jogador.ultimafase = 1;
            novo_jogador.vidas = 3;


            // escrever os dados no arquivo binario
            if(fwrite(&novo_jogador,sizeof(save), 1, arq) != 1)
                printf("Erro na escrita do save!\n");
        }
        else {
            novo_jogador.identificador = -1;
            novo_jogador.totalpts = 0;
            novo_jogador.ultimafase = 0;
            novo_jogador.vidas = 0;
            strcpy(novo_jogador.nomejogador, "");
        }
    }
    fclose(arq);

    return novo_jogador;
}

save carregarJogo()
{
    save buffer;
    FILE *arq;
    int id;
    // abre o arquivo dos saves
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");
    // imprime na tela os dados
    else {
        imprime_saves(arq);
        printf("Digite o id que deseja acessar: \n");
        scanf("%d", &id);
        fseek(arq, sizeof(save)*id, SEEK_SET);
        //if(fread(&buffer, sizeof(save), 1, arq) != 1)
           // printf("Erro na abertura do arquivo!\n");
    }
    fclose(arq);
    return buffer;
}


void imprime_saves(FILE* arq){
    save jogador;
    rewind(arq);
    while(!feof(arq))
    {
            if(fread(&jogador, sizeof(save), 1, arq) == 1)
                {
                // impressao das informações dos saves
                printf("\nJogador %d\n", jogador.identificador);
                printf("Nome: %s\n", jogador.nomejogador);
                printf("Pontos: %d\n", jogador.totalpts);
                printf("Fase: %d\n", jogador.ultimafase);
                printf("Vidas: %d\n", jogador.vidas);
                }
    }
}

fase gera_fase(FILE *arq)
{
    int linha, coluna;
    fase fasea;

    fasea.tamanhox = 13;
    fasea.tamanhoy = 14;
    for (linha = 0; linha < 13; linha++){
        for (coluna = 0; coluna < 14; coluna++){
            fasea.elementos[linha][coluna] = getc(arq);
        }
    }
    return fasea;
}


void imprime_mapa(fase fasea)
{
    int linha, coluna;

    for (linha = 0; linha < 13; linha++){
        for (linha = 0; linha < 14; linha++){
            //cprintf("%c", fasea.elementos[linha][coluna]);
        }
    }
    cprintf("\n");

}
