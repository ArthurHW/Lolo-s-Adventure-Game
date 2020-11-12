#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Construir uma interface simplificada para o menu de um jogo, contendo 4 opcoes:
// Novo jogo: ler o nome do jogador e imprimir uma mensagem dizendo que o jogo esta comecando
// Carregar jogo: imprimir uma mensagem dizendo que a lista de jogos esta sendo carregada
// Mostrar creditos: imprime os creditos dos desenvolvedores do codigo (nesse caso o nome e o cartao UFRGS)
// Sair: imprime uma mensagem dizendo que o jogo esta encerrando

void menu(); // funcao para o menu do jogo
char validaentrada(); // funcao para validar a entrada da opcao do jogo
void novoJogo(); // funcao para comecar um novo jogo
void carregarJogo(); // funcao para carregar um jogo ja comecado
void mostraCreditos(); // funcao para mostrar os creditos
void sair(); // funcao para mostrar a mensagem de saida do jogo

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
    // fiquei confuso em relacao ao tamanho, achei que era 9, ja que sao os caracteres vao de [0,9] e ele nao le a quebra de linha
    // mas se eu colocasse 9, ele lia apenas 8 caracteres
    // o unico jeito que eu achei de ele ler 10 caracteres, foi colocando 12, mas nao entendi o pq isso possa ter dado certo
    // testei para compiladores online, e nao estava funcionando essa parte do programa
    // gostaria, se possivel, de um retorno onde eu possa ter me equivocado,
    // ja que, apesar de o programa estar funcionando no codeblocks, eu nao entendi a logica dessa parte, e nem pq nao esta funcionando para compiladores online
    printf("Jogador %s: se prepare, jogo iniciando!!\n", nome);
}

void carregarJogo()
{
    printf("Carregando a lista de jogos salvos, aguarde um minuto.\n");
}

void mostraCreditos()
{
    printf("Creditos do jogo:\nDesenvolvido por: Vitor Caruso Rodrigues Ferrer (00327023)\n");
}

void sair()
{
    printf("Encerrando o jogo, ateh a proxima!\n");
}

