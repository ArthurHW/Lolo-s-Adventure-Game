#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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
    char elementos[13][13];
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
void carregarJogo(); // funcao para carregar um jogo ja comecado
void mostraCreditos(); // funcao para mostrar os creditos
void sair(); // funcao para mostrar a mensagem de saida do jogo
void imprime_save(save);// dado um save imprime ele na tela formatado



// Função principal
int main()
{

    menu();

    return 0;
}


// Funções Auxiliares
void menu(){

    char opcao;
    save jogo;
    printf("---- Lolo's Adventure ----\n");


        imprime_menu();
        opcao = validaentrada();
        switch (opcao)
        {
            case 'N':   jogo = novoJogo();
                        break;
            case 'C':   carregarJogo();
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
    int id = 0;
    save novo_jogador, buffer;
    FILE *arq;

    // abrir o arquivo com os saves
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");

    // ver quantos saves tem e salvar na variavel id
    else {
        while(!feof(arq)){
            if(fread(&buffer, sizeof(save), 1, arq) == 1)
                id++;
            else
                printf("Erro na leitura do arquivo\n");    
        }


        // solicitar e salvar o nome do jogador
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
    fclose(arq);

    return novo_jogador;
}

void carregarJogo()
{
    save buffer;
    FILE *arq;
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");
    else {
        while(!feof(arq)){
            if(fread(&buffer, sizeof(save), 1, arq) == 1)
                imprime_save(buffer);
  
        }
    }
    fclose(arq);
}

void imprime_save(save jogador){
    
    printf("\nJogador %d\n", jogador.identificador);
    printf("Nome: %s\n", jogador.nomejogador);
    printf("Pontos: %d\n", jogador.totalpts);
    printf("Fase: %d\n", jogador.ultimafase);
    printf("Vidas: %d\n", jogador.vidas);

}
