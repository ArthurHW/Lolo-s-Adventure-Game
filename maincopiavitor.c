#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio2.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define S_CIMA 72
#define S_BAIXO 80
#define S_ESQ 75
#define S_DIR 77
#define ESC 27

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
    int inimigos;
    char elementos[14][14]; // 13 por 14 ele nao lia uma parte do mapa ai tive que botar 14x14
}fase;

typedef struct ponto {
    char x;
    char y;
}ponto;


// protótipos das funções auxiliares:
void menu();
void imprime_menu(); // funcao para imprimir o menu do jogo
char validaentrada(); // funcao para validar a entrada da opcao do jogo
int novoJogo(); // funcao para comecar um novo jogo
void carregarJogo(); // funcao para carregar um jogo ja comecado
void mostraCreditos(); // funcao para mostrar os creditos
void sair(); // funcao para mostrar a mensagem de saida do jogo
void imprime_saves(FILE*);// dado um save imprime ele na tela formatado
fase gera_fase(int); // dado o numero de uma fase, preenche os elementos daquela fase em uma matriz contendo suas posicoes, e imprime a fase na tela
void movimentacao(fase*, save*); // funcao para a movimentacao, dada uma fase e um save
void hidecursor(); // funcao pra esconder o cursor
void contato_lolo(int, ponto*, int*, fase*, save*);
// contato do lolo com os blocos, dada uma seta(direcao), o ponteiro para a posicao do lolo (ponto), contagem de coracoes (poder) do lolo, uma fase e um save (para atualizar os dados)
void mostra_info(save, int); // dado um save passado por cópia, mostra na tela seu nome, fase atual, total de pts e número de vidas.
void salvar_arquivo(save); // dado um save passado por cópia, escreve os dados dele alterados no arquivo de saves

// Função principal
int main()
{
    save save1 = {0, 0, 1, 3, "Teste"};
    fase fase1;

//    menu();
    fase1 = gera_fase(save1.ultimafase);
    movimentacao(&fase1, &save1);

    return 0;
}


// Funções Auxiliares

void hidecursor() // https://stackoverflow.com/questions/30126490/how-to-hide-console-cursor-in-c
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void menu(){
    int volta = 0;
    char opcao;

        do {
            printf("---- Lolo's Adventure ----\n");
            imprime_menu();
            opcao = validaentrada();
            switch (opcao)
            {
                case 'N':   novoJogo();
                            break;
                case 'C':   carregarJogo();
                            break;
                case 'M':   mostraCreditos();
                            break;
                case 'S':   sair();
                            break;
            }
        } while (opcao != 'S' && volta != 0);

}

void imprime_menu()
{
    printf("(N) - Novo Jogo\n(C) - Carregar Jogo\n(M) - Mostrar Creditos\n(S) - Sair\n");

}

void mostraCreditos()
{
    clrscr();
    printf("Creditos do jogo:\nDesenvolvido por: Vitor Caruso Rodrigues Ferrer (00327023)\nArthur Henrique Wiebusch (00324318)\n");
    system("pause");
    fflush(stdin);
    clrscr();
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

// Cria um novo jogo para o usuario
int novoJogo()
{
    char op = '0';
    char idchar = '0';
    int id;
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
            printf("(V) - Voltar\n");
            do{
                scanf(" %c", &op);
            } while(op != '1' && op != '2');
            if (toupper(op) == 'V')
                return 0;
        }

        if (op != '2'){
            // solicitar e salvar o nome do jogador
            if (op == '1'){
                imprime_saves(arq);
                printf("Qual id deseja sobrecrever?\n");
                printf("(V) - Voltar\n");
                do {
                scanf(" %c", &idchar);
                if (toupper(idchar) == 'V')
                    return 1;
                id = atoi(idchar);
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
    }
    fclose(arq);
    return 0;

}

// Função para carregar um jogo salvo
void carregarJogo()
{
    save buffer;
    FILE *arq;
    int id;
    // abre o arquivo dos saves
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");
    else {
        // imprime na tela os dados
        imprime_saves(arq);
        // solicita o jogo que o usuario deseja acessar
        printf("Digite o id que deseja acessar: \n");
        scanf("%d", &id);
        fseek(arq, sizeof(save)*id, SEEK_SET);
        if(fread(&buffer, sizeof(save), 1, arq) != 1)
            printf("Erro na abertura do arquivo!\n");
    }
    fclose(arq);
}

// dado o arquivo saves.bin, com as gravações de todos os jogadores, imprime na tela as
// informações dos jogadores, com formatação adequada
void imprime_saves(FILE* arq){
    save jogador;
    rewind(arq);
    // enquanto não chegar no final do arquivo
    while(!feof(arq))
    {       // lê o arquivo
            if(fread(&jogador, sizeof(save), 1, arq) == 1)
                {
                // e imprime os saves
                printf("\nJogador %d\n", jogador.identificador);
                printf("Nome: %s\n", jogador.nomejogador);
                printf("Pontos: %d\n", jogador.totalpts);
                printf("Fase: %d\n", jogador.ultimafase);
                printf("Vidas: %d\n", jogador.vidas);
                }
    }
}

fase gera_fase(int numerofase)
{
    FILE* arq;
    int linha, coluna;
    fase fasea;
    char nome[10];

    switch (numerofase) // coloca o nome do arquivo dependendo do numero da fase
    {
        case 1: strncpy(nome, "Fase1.txt", 10);
                break;
        case 2: strncpy(nome, "Fase2.txt", 10);
                break;
        case 3: strncpy(nome, "Fase3.txt", 10);
                break;
    }

    arq = fopen(nome, "r");
    if (arq == NULL)
        perror("Erro ao abrir o arquivo");
    else
    {
        fasea.inimigos = 0;
        fasea.tamanhox = 14;
        fasea.tamanhoy = 14;
        for (linha = 0; linha < fasea.tamanhox; linha++){
            for (coluna = 0; coluna < fasea.tamanhoy; coluna++){
                fasea.elementos[linha][coluna] = getc(arq); // preenche os elementos da fase
                if (fasea.elementos[linha][coluna] == 'E')
                    fasea.inimigos++;
                printf("%c", fasea.elementos[linha][coluna]); // imprime esses elementos
        }
    }
    fclose(arq);
    return fasea;
    }
}

void movimentacao(fase *fasea, save *jogador)
{
    char caracter, lolo = 'L';
    ponto pos_lolo, oldpos_lolo;
    int poder = 0, linha, coluna;

    hidecursor();
    for (linha = 0; linha < fasea->tamanhoy; linha++){
        for (coluna = 0; coluna < fasea->tamanhox; coluna++){
            if (fasea->elementos[linha][coluna] == 'L')
            {
                pos_lolo.x = coluna+1; // +1 porque a matriz comeca em [0][0] e as coordenada em (1,1)
                pos_lolo.y = linha+1;
            }
        }
    }
    oldpos_lolo.x = pos_lolo.x;
    oldpos_lolo.y = pos_lolo.y;
    hidecursor();
    gotoxy(13,13); // posiciona antes pra aparecer a info zerada
    printf("\n");
    mostra_info(*jogador, poder); // funcao da info
    gotoxy(pos_lolo.x, pos_lolo.y);
    while (caracter != ESC) // o usuario pode se movimentar ate clicar esc (da para por aqui a tecla para voltar para o menu)
    {
        fasea->elementos[oldpos_lolo.y-1][oldpos_lolo.x-1] = ' ';
        gotoxy(oldpos_lolo.x,  oldpos_lolo.y);
        printf(" ");
        fasea->elementos[pos_lolo.y-1][pos_lolo.x-1] = 'L';
        gotoxy(pos_lolo.x, pos_lolo.y);
        printf("%c", lolo);
        caracter = getch();
        oldpos_lolo.x = pos_lolo.x;
        oldpos_lolo.y = pos_lolo.y;
        switch (caracter)
        {
            case S_CIMA:  contato_lolo(S_CIMA, &pos_lolo, &poder, fasea, jogador);
                          break;
            case S_BAIXO: contato_lolo(S_BAIXO, &pos_lolo, &poder, fasea, jogador);
                          break;
            case S_ESQ:   contato_lolo(S_ESQ, &pos_lolo, &poder, fasea, jogador);
                          break;
            case S_DIR:   contato_lolo(S_DIR, &pos_lolo, &poder, fasea, jogador);
                          break;
        }
        gotoxy(13,13);
        printf("\n");
        mostra_info(*jogador, poder); // funcao da info
    }
}

void contato_lolo(int seta, ponto *pos_lolo, int *poder, fase *fasea, save *jogador)
{
    ponto newpos_lolo, newpos_B;
    char caracter;

    newpos_lolo.x = pos_lolo->x-1; // -1 porque eles vao ser posicoes da matriz
    newpos_lolo.y = pos_lolo->y-1;
    newpos_B.x = pos_lolo->x-1;
    newpos_B.y = pos_lolo->y-1;
    switch(seta) // atualiza o x ou o y dependendo da tecla apertada pelo usuario
    {
        case S_CIMA:
            newpos_lolo.y--;
            newpos_B.y = newpos_B.y - 2; // atualiza a posicao do bloco movivel, ja que eh sempre 1 a mais dependendo da direcao p onde o usuario se moveu
            break;
        case S_BAIXO:
            newpos_lolo.y++;
            newpos_B.y = newpos_B.y + 2;
            break;
        case S_ESQ:
            newpos_lolo.x--;
            newpos_B.x = newpos_B.x - 2;
            break;
        case S_DIR:
            newpos_lolo.x++;
            newpos_B.x = newpos_B.x + 2;
            break;
    }
    gotoxy(newpos_lolo.x, newpos_lolo.y);
    caracter = fasea->elementos[newpos_lolo.y][newpos_lolo.x];
    switch (caracter)
    {
        case ' ': // espaco vazio
            pos_lolo->x = newpos_lolo.x+1; // e volta para o +1, ja que agora eles sao coordenadas
            pos_lolo->y = newpos_lolo.y+1;
            break;
        case 'C': // coracao
            fasea->elementos[newpos_lolo.y][newpos_lolo.x] = ' '; // apaga a posicao
            pos_lolo->x = newpos_lolo.x+1;
            pos_lolo->y = newpos_lolo.y+1;
            (*poder)++;
            break;
        case 'E': // inimigo
            if (*poder != 0) // se o lolo tiver poder
            {
                fasea->elementos[newpos_lolo.y][newpos_lolo.x] = ' ';
                pos_lolo->x = newpos_lolo.x+1;
                pos_lolo->y = newpos_lolo.y+1;
                (*poder)--; // diminui 1 do poder e dos inimigos
                fasea->inimigos--;
            }
            break;
        case 'T': // bau
            if (fasea->inimigos == 0) // caso nao haja mais nenhum inimigo, ele pode pegar o bau
            {

                fasea->elementos[newpos_lolo.y][newpos_lolo.x] = ' ';
                pos_lolo->x = newpos_lolo.x+1;
                pos_lolo->y = newpos_lolo.y+1;
            }
            break;
        case 'B': // bloco movivel
            if (fasea->elementos[newpos_B.y][newpos_B.x] == ' ') //  || fasea->elementos[blocomovivelnovoY][blocomovivelnovoX] == ' ', caso o espaco que o bloco movivel vai ir seja vazio
            {
                fasea->elementos[newpos_lolo.y][newpos_lolo.x] = ' '; // apaga o espaco q tava o bloco movivel
                pos_lolo->x = newpos_lolo.x+1;
                pos_lolo->y = newpos_lolo.y+1;
                fasea->elementos[newpos_B.y][newpos_B.x] = 'B'; // coloca um b na posicao desse novo espaco
                gotoxy(newpos_B.x+1,newpos_B.y+1); // posiciona e imprime o b (lembrando do +1 por causa da diferenca de posicao da matriz e coordenada)
                printf("%c", fasea->elementos[newpos_B.y][newpos_B.x]);
            }
            break;

    }
}

// função para mostrar as informações do jogador durante o jogo
void mostra_info(save jogador, int poder){

    cprintf("Fase: %d\nCoracoes: %d\nPontos: %d\nVidas: %d", jogador.ultimafase, poder, jogador.totalpts, jogador.vidas);
}

// função para salvar os dados do save a cada fase
void salvar_arquivo(save jogador){
    FILE *arq;
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");
    else{
        fseek(arq,sizeof(jogador)* jogador.identificador, SEEK_SET);
        if ((fwrite(&jogador,sizeof(save), 1, arq)) != 1)
            printf("Erro ao escrever no arquivo\n");
        fclose(arq);
    }
}
