#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include<conio2.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define S_CIMA 72
#define S_BAIXO 80
#define S_ESQ 75
#define S_DIR 77
#define ESC 27

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
    int inimigos;
    char elementos[14][14];
}fase;

typedef struct ponto {
    char x;
    char y;
}ponto;


// protótipos das funções auxiliares:
int menu(save*); /*função que administra os menus do jogo, ela retorna 1 para a main caso o usuario queira sair do programa
e 0 caso o usuario esteja carregando ou criando um novo jogo, ela recebe uma struct do tipo save por referência e atualiza ela
com os dados do save que o jogador escolheu; caso o usuario só queira sair do programa, não faz nada com o save dado.*/
void imprime_menu(); // funcao para imprimir o menu do jogo
char validaentrada(); // funcao para validar a entrada da opcao do jogo, retorna a opção selecionada pelo usuario
int novoJogo(save*); /* funcao para comecar um novo jogo, ela retorna 1, caso o usuario digitou para voltar ao menu principal
e 0 caso o usuario tenha ido até o fim do processo de criar um novo jogo. Ela recebe um save por referencia e atualiza ele
para corresponder aos dados do save que acabou de ser criado*/
int carregarJogo(save*); /* funcao para carregar um jogo ja comecado, assim como função novo jogo retorna 1 para voltar ao menu principal
e 0 para seguir para o jogo, também recebe um save que será atualizado na função*/
void mostraCreditos(); // funcao para mostrar os creditos
void sair(); // funcao para mostrar a mensagem de saida do jogo
void instrucoes(); // imprime na tela as instruções do jogo
int imprime_saves(FILE*);// dado um arquivo de saves, imprime todos os saves dele na tela formatados; retorna o número de saves do arquivo
void mostra_info(save, int); // dado um save passado por cópia, mostra na tela seu nome, fase atual, total de pts e número de vidas.
void salvar_arquivo(save); // dado um save passado por cópia, escreve os dados dele alterados no arquivo de saves
void movimenta_inimigo(ponto*, fase*);
void contato_lolo(int, int*, int*, int*, fase*, save*);
void movimentacao(fase*, save*);
void hidecursor();
fase gera_fase(int);
void game_over();


// Função principal
int main()
{
    save save1 = {0, 0, 1, 3, "Teste"};
    fase fase1;
    srand(time(NULL));
    menu(&save1);
    //game_over(save1);
    //fase1 = gera_fase(save1.ultimafase);
    //movimentacao(&fase1, &save1);

    return 0;
}


// Funções Auxiliares
int menu(save* jogador){
    int volta = 1;
    char opcao;

        do { // repetir enquanto
            clrscr(); // limpa a tela
            printf("---- Lolo's Adventure ----\n");
            imprime_menu(); // imprime o menu
            opcao = validaentrada(); // testa a entrada
            switch (opcao)
            {
                case 'N':   volta = novoJogo(jogador);
                            break;
                case 'C':   volta = carregarJogo(jogador);
                            break;
                case 'M':   mostraCreditos();
                            break;
                case 'S':   sair();
                            return 1;
                            break;
                case 'I':   instrucoes();
                            break;
            }
        } while (opcao != 'S'&& volta); // enquanto o usuario nao queia sair do programa, ou não tenha entrado em um "save"
    return 0;
}

// função que imprime os menus
void imprime_menu()
{
    printf("(N) - Novo Jogo\n(C) - Carregar Jogo\n(M) - Mostrar Creditos\n(I) - Instrucoes\n(S) - Sair\n");

}

// função para mostrar os créditos:
void mostraCreditos()
{
    clrscr();
    printf("Creditos do jogo:\nDesenvolvido por: \nVitor Caruso Rodrigues Ferrer (00327023)\nArthur Henrique Wiebusch (00324318)\n");
    system("pause");
    fflush(stdin);
    clrscr();
}

// função para mostrar as instrucoes para o usuario
void instrucoes()
{
    clrscr();
    printf("======================================================================================================================\n                                                  INSTRUCOES ");
    printf("\n======================================================================================================================\n\n");
    printf("  Controles:\nUtilize as setas do teclado para mover o Lolo\n\n  Cenario:\n@ - Lolo -> Personagem controlado pelo jogador\nP - Pedra -> Lolo nao pode se mover para esta posicao\n");
    printf("A - Agua -> Cuidado Lolo nao sabe nadar\nC - Coracao -> Ao pegar um coracao, Lolo pode tocar em um inimigo\nB - Bloco movivel -> Lolo pode movimentar esse bloco, mas cuidado para nao ficar preso!\n");
    printf("E - Inimigo -> Cuidado com eles, nao se esqueca dos coracoes\nT - Bau -> Apos derrotar todos os inimigos, corra ate ele para terminar a fase\n\n");
    printf("  Objetivo:\nO seu objetivo eh encontrar os coracoes, derrotar todos os inimigos e se dirigir ao bau para ir para a proxima fase\n\n");
    printf("  Pontuacao e vidas:\nLolo comeca o jogo com 3 vidas e 0 pontos, para cada inimigo eliminado Lolo ganha um ponto, a cada 10 pontos\nconsecutivos Lolo ganha uma vida extra");
    printf("se for eliminado a fase recomeca com a pontuacao zerada, alem de Lolo perder uma \nvida\n\n");
    printf("  Numero de gravacoes\nVoce pode ter ate 3 gravacoes simultaneas, com cada uma correspondendo ao ponto do jogo onde parou. \nObs: Ao entrar em um gravacao a fase em que voce estava eh resetada.\n");
    system("pause");
    clrscr();
}

// mensagem de encerramento do programa
void sair()
{
    printf("Encerrando o jogo, ateh a proxima!\n");
}

// função para validação da entrada do usuário para navegar entre os menus
char validaentrada()
{
    char opcao;
    do
    {
        printf("Digite uma opcao: ");
        fflush(stdin);
        scanf(" %c", &opcao);
        opcao = toupper(opcao); // para sempre ficar maiusculo, mesmo que o usuario digite minusculo
        if (opcao != 'N' && opcao != 'C' && opcao != 'M' && opcao != 'S' && opcao != 'I')
        {
            printf("Opcao invalida\n");
            imprime_menu(); // imprime o menu para o usuario novamente
        }
    }
    while ((opcao != 'N') && (opcao != 'C') && (opcao != 'M') && (opcao != 'S') && (opcao != 'I')); // enquanto nao for um dos caracteres validos
    return opcao;
}

// Cria um novo jogo para o usuario
int novoJogo(save* novo_jogador)
{
    clrscr();
    int op = 0;
    int id = 0;
    save buffer;
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

        // se o limite de saves foi atingido, perguntar para o usuario se ele quer sobrescrever seus dados
        if(id >= 3){
            printf("Limite de Jogos salvos atingido (3), deseja sobrescrever seus dados?\n(1) - Sim\n(2) - Nao - Voltar ao menu principal\n");
            do{
                scanf(" %d", &op);
            } while(op != 1 && op != 2);
            // se ele nao quer sobrescrever os dados, encerrar a funcao e retornar ao menu
            if (op == 2){
                fclose(arq);
                return 1;
            }


        }


        if (op != 2){
            // se o usuario quer sobrescrever seus dados, perguntar qual jogador sobrescrever
            if (op == 1){
                imprime_saves(arq);
                printf("Qual Jogador deseja sobrecrever?\n");
                printf("-1 -> Voltar ao menu principal\n");
                do {
                scanf(" %d", &id);
                // caso ele mude de ideia ainda pode voltar ao menu principal
                if (id == -1){
                    fclose(arq);
                    return 1;
                }
                } while(id != 0 && id != 1 && id != 2);
                fseek(arq, sizeof(save) * id, SEEK_SET); // buscar o id que ele quer sobrecrever
            }
            // solicitar o nome
            printf("Digite seu nome (maximo de 8 caracteres): ");
            fflush(stdin);
            fgets((*novo_jogador).nomejogador, 8, stdin);
            (*novo_jogador).nomejogador[strlen((*novo_jogador).nomejogador)-1] = '\0';

            // salvar os outros dados do jogador
            (*novo_jogador).identificador = id;
            (*novo_jogador).totalpts = 0;
            (*novo_jogador).ultimafase = 1;
            (*novo_jogador).vidas = 3;


            // escrever os dados no arquivo binario
            if(fwrite(novo_jogador,sizeof(save), 1, arq) != 1)
                printf("Erro na escrita do save!\n");
        }
    }
    // fechar o arquivo e não voltar ao menu, pois o retorno 1 encerra a função menu também
    clrscr();
    fclose(arq);
    return 0;

}

// Função para carregar um jogo salvo
int carregarJogo(save* jogador)
{
    clrscr();
    FILE *arq;
    int id, numSaves;
    // abre o arquivo dos saves
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");
    else {
        // imprime na tela os dados
        numSaves = imprime_saves(arq);
        // verifica se o numero de saves é 0, neste caso solicita ao usuario que crie um jogo antes
        if (numSaves == 0){
            printf("Parece que voce ainda nao possui nenhum jogo salvo, volte ao menu principal e crie um novo jogo!\n");
            system("pause");
            return 1;
        }
        else{
            // se ja existem jogos salvos, solicita ao usuario o jogo que ele deseja acessar
            printf("Caso deseje resetar todos os saves, digite 10 (nao escolha essa opcao a menos que tenha certeza de sua decisao!)\n");
            printf("Digite o id que deseja acessar: \n");
            printf("-1 -> Voltar ao menu principal\n");
            do{
                scanf("%d", &id);
                // o usuário pode voltar ao menu principal neste ponto também, digitando -1
                if (id == -1){
                    fclose(arq);
                    return 1;
                }
                else if(id == 10){
                    fclose(arq);
                    if(!(arq = fopen("saves.bin", "wb")))
                        printf("Erro na abertura do arquivo!\n");
                    fclose(arq);
                    return 1;
                }
            } while(0 > id || id > numSaves); // ler o id que o usuario está tentando acessar até que ele digite um id que existe, ou -1 para voltar ao menu
            // após isso, achar o arquivo, e passar seus dados para a variavel *jogador que foi passada por referência
            fseek(arq, sizeof(save)*id, SEEK_SET);
            if(fread(jogador, sizeof(save), 1, arq) != 1)
                printf("Erro na abertura do arquivo!\n");
        }
    }
    // caso o usuario chegue aqui, salva o arquivo e não retorna ao menu, pois ele pode iniciar o jogo
    fclose(arq);
    return 0;
}

// dado o arquivo saves.bin, com as gravações de todos os jogadores, imprime na tela as
// informações dos jogadores, com formatação adequada, o retorno da função é o número de saves lido
int imprime_saves(FILE* arq){
    save jogador;
    int numSaves = 0;
    rewind(arq);
    // enquanto não chegar no final do arquivo
    while(!feof(arq))
    {       // lê o arquivo
            if(fread(&jogador, sizeof(save), 1, arq) == 1)
                {
                // e imprime os saves
                numSaves++;
                printf("Jogador %d\n", jogador.identificador);
                printf("Nome: %s\n", jogador.nomejogador);
                printf("Pontos: %d\n", jogador.totalpts);
                printf("Fase: %d\n", jogador.ultimafase);
                printf("Vidas: %d\n\n", jogador.vidas);
                }
    }
    return numSaves; // retorna o número de saves lidos
    // a função não fecha o arquivo, pois isso é responsabilidade da função que a chamou
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

void hidecursor() // https://stackoverflow.com/questions/30126490/how-to-hide-console-cursor-in-c
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
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
    ponto inimigos[fasea->inimigos];
    int contador = 0;
    int x, y, poder = 0;
    int linha, coluna;
    int oldX, oldY;

    hidecursor();
    for (linha = 0; linha < fasea->tamanhoy; linha++){
        for (coluna = 0; coluna < fasea->tamanhox; coluna++){
            if (fasea->elementos[linha][coluna] == 'L')
            {
                x = coluna+1; // +1 porque a matriz comeca em [0][0] e as coordenada em (1,1)
                y = linha+1;
            }
            if (fasea->elementos[linha][coluna] == 'E'){
                (inimigos[contador]).x = coluna + 1;
                (inimigos[contador]).y = linha + 1;
                contador++;

            }
        }
    }
    oldX = x;
    oldY = y;
    hidecursor();
    gotoxy(13,13); // posiciona antes pra aparecer a info zerada
    printf("\n");
    mostra_info(*jogador, poder); // funcao da info
    gotoxy(x, y);
    while (caracter != ESC) // o usuario pode se movimentar ate clicar esc (da para por aqui a tecla para voltar para o menu)
    {
        gotoxy(oldX, oldY);
        cprintf(" ");
        gotoxy(x, y);
        cprintf("%c", lolo);
        fflush(stdin);
        caracter = getch();
        oldX = x;
        oldY = y;
        switch (caracter)
        {
            case S_CIMA:  contato_lolo(S_CIMA, &x, &y, &poder, fasea, jogador);
                          break;
            case S_BAIXO: contato_lolo(S_BAIXO, &x, &y, &poder, fasea, jogador);
                          break;
            case S_ESQ:   contato_lolo(S_ESQ, &x, &y, &poder, fasea, jogador);
                          break;
            case S_DIR:   contato_lolo(S_DIR, &x, &y, &poder, fasea, jogador);
                          break;
        }
        for (contador = 0; contador < fasea->inimigos; contador++){
            movimenta_inimigo(&inimigos[contador], fasea);
        }
        gotoxy(13,13);
        printf("\n");
        mostra_info(*jogador, poder); // funcao da info
    }
    salvar_arquivo(*jogador);
}

void contato_lolo(int seta, int *x, int *y, int *poder, fase *fasea, save *jogador)
{
    int novoX = *x-1, novoY = *y-1; // -1 porque eles vao ser posicoes da matriz
    int blocomovivelnovoX = *x-1, blocomovivelnovoY = *y-1;
    char caracter;

    switch(seta) // atualiza o x ou o y dependendo da tecla apertada pelo usuario
    {
        case S_CIMA:
            novoY--;
            blocomovivelnovoY = blocomovivelnovoY - 2; // atualiza a posicao do bloco movivel, ja que eh sempre 1 a mais dependendo da direcao p onde o usuario se moveu
            break;
        case S_BAIXO:
            novoY++;
            blocomovivelnovoY = blocomovivelnovoY + 2;
            break;
        case S_ESQ:
            novoX--;
            blocomovivelnovoX = blocomovivelnovoX - 2;
            break;
        case S_DIR:
            novoX++;
            blocomovivelnovoX = blocomovivelnovoX + 2;
            break;
    }
    gotoxy(novoX, novoY);
    caracter = fasea->elementos[novoY][novoX];
    switch (caracter)
    {
        case ' ':
            *x = novoX+1; // e volta para o +1, ja que agora eles sao coordenadas
            *y = novoY+1;
            break;
        case 'C':
            fasea->elementos[novoY][novoX] = ' '; // apaga a posicao
            *x = novoX+1;
            *y = novoY+1;
            (*poder)++;
            break;
        case 'L':
            fasea->elementos[novoY][novoX] = ' '; // apaga a posicao inicial, p n dar conflio com os blocos moviveis
            *x = novoX+1;
            *y = novoY+1;
            break;
        case 'E':
            if (*poder != 0) // se o lolo tiver poder
            {
                fasea->elementos[novoY][novoX] = ' ';
                *x = novoX+1;
                *y = novoY+1;
                (*poder)--; // diminui 1 do poder e dos inimigos
                fasea->inimigos--;
                jogador->totalpts++;
            }
            break;
        case 'T':
            if (fasea->inimigos == 0) // caso nao haja mais nenhum inimigo, ele pode pegar o bau
            {
                fasea->elementos[novoY][novoX] = ' ';
                *x = novoX+1;
                *y = novoY+1;
            }
            break;
        case 'B':
            if (fasea->elementos[blocomovivelnovoY][blocomovivelnovoX] == ' ') // caso o espaco que o bloco movivel vai ir seja vazio
            {
                fasea->elementos[novoY][novoX] = ' '; // apaga o espaco q tava o bloco movivel
                *x = novoX+1;
                *y = novoY+1;
                fasea->elementos[blocomovivelnovoY][blocomovivelnovoX] = 'B'; // coloca um b na posicao desse novo espaco
                gotoxy(blocomovivelnovoX+1,blocomovivelnovoY+1); // posiciona e imprime o b (lembrando do +1 por causa da diferenca de posicao da matriz e coordenada)
                printf("%c", fasea->elementos[blocomovivelnovoY][blocomovivelnovoX]);
            }
            break;

    }
}


void movimenta_inimigo(ponto* inimigo, fase* fasea){
    int x, y;
    int direcao;
    char caracter;
    x = inimigo->x - 1; // atualizar para posições da matriz
    y = inimigo->y - 1;
    direcao = (rand() % 4);
    switch(direcao){
        case 1: x++;
            break;
        case 2: x--;
            break;
        case 3: y++;
            break;
        case 4: y--;
            break;
    }
    caracter = fasea->elementos[x][y];
    if (caracter == ' ' || caracter == 'L'){
        fasea->elementos[inimigo->x-1][inimigo->y-1] = ' ';
        fasea->elementos[x][y] = 'E';
        x = x + 1; // atualizar para posições do terminal
        y = y + 1;
        gotoxy(inimigo->x, inimigo->y);
        cprintf(" ");
        inimigo->x = x;
        inimigo->y = y;
        gotoxy(x, y);
        cprintf("E");
    }
}

// essa função apaga o registro do jogador, que perdeu
void game_over(save jogador){
    FILE* arqTemp;
    FILE* arq;
    save buffer;
    int id = 0;

    if (!(arqTemp = fopen("savestemp.bin", "wb+")))
        printf("Erro na abertura do arquivo temporário\n");
    else{
        if (!(arq = fopen("saves.bin", "r+b"))){
            printf("Erro na abertura do arquivo de saves\n");
        }
        else {
            while (!feof(arq)){
                if((fread(&buffer, sizeof(save), 1, arq)) == 1){
                    if (buffer.identificador != jogador.identificador){
                        if ((fwrite(&buffer,sizeof(save), 1,arqTemp)) != 1)
                            printf("Erro ao escrever no arquivo\n");
                    }
                }
            }
            fclose(arq);
            if (!(arq = fopen("saves.bin", "w+b"))){
                printf("Erro na abertura do arquivo de saves em modo de escrita\n");
            }
            else {
                rewind(arqTemp);
                while (!feof(arqTemp)){
                    if((fread(&buffer, sizeof(save), 1, arqTemp)) == 1){
                            buffer.identificador = id;
                            id++;
                        if ((fwrite(&buffer,sizeof(save), 1,arq)) != 1)
                            printf("Erro ao escrever no arquivo\n");
                    }
                }
            }
        fclose(arq);
        fclose(arqTemp);
        }
    }
}



