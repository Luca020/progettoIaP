//892607 891480


/** Ciao
 * porcoddio
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "boardOperations.h"
#include "aIAlgorithms.h"

void moving (Box* board, int start, int score, char* movesString, size_t xSide, size_t ySide);

#define INTENSITY_RESET 0

int xSide;
int ySide;

void guide(){ //informazioni per il giocatore
    printf("\nSEI IN MODALITA' UTENTE.\n\n");
    printf("Per muoversi all'interno del labirinto utilizzare:\n'w' per muoversi verso l'alto\n"
           "'s' per muoversi verso il basso\n'a' per per muoversi verso sinistra\n'd' per muoversi verso destra\n\n");
    printf("Partendo dallo start (o) raggiungere il traguardo (_) con il punteggio piu' alto possibile. \n");
    printf("Ogni mossa costa un punto. \nOgni moneta ($) concede 10 punti extra.\n"
           "Ogni imprevisto (!) dimezza il numero di monete raccolte fino a quel momento.\n"
           "Ogni trapano (T) permette di perforare 3 muri.\n\n");
    printf("Buona partita! \n\n\n");
}

void welcome(){
    printf("Benvenuto! In che modalita' vuoi giocare?\n");
    printf("1. Modalita' utente\n");
    printf("2. Modalita' AI random\n");
    printf("3. Modalita' AI avanzata\n");
}

void success(int score){ //il giocatore è giunto a destinazione
    printf("\n");
    printf("Congratulazioni! Sei arrivato a destinazione!\n");
    printf("Il tuo punteggio e': %d\n", score);
}

void wrongInput(){ //il giocatore ha inserito in input non corretto
    printf("Input non corretto, reinserirlo.\n");
}

void death(){ //il giocatore è morto
    printf("Purtroppo sei morto!");
    printf("\n");
}

void challenge(Box *board, int start, int* end, char* movesString){
    printf("\nSEI IN MODALITA' SFIDA!\n");
    scanf(" %d", &xSide);
    scanf(" %d", &ySide);
    int i;
    char* s = (char*)malloc(sizeof(char)*xSide); //stringa di input
    for (i=0; i<ySide; i++){
        scanf(" %[^\n]s", s);
        int j;
        for(j=0;j<xSide;j++){
            board[i*xSide+j].symbol=s[j];
        }
    }
    free(s);

    _Bool found =0;
    i=0;

    while (!found){ //cerco lo start
        if (board[i].symbol=='o'){
            start = i;
            found =1;
        }
        i++;
    }

    found = 0;
    i=0;

    while (!found){ //cerco l'end
        if (board[i].symbol=='_'){
            *end = i;
            found =1;
        }
        i++;
    }
    aStarAlgorithm(board, start, end, movesString, xSide, ySide);
}

int main(int argc, char *argv[]){
    int* end = (int*)malloc(sizeof(int));
    Box board[xSide*ySide];
    char* moves = (char*) malloc(3000*sizeof(char)); //il vettore nel quale salverò la sequenza di mosse
    if (argc==2 && strcmp(argv[1], "--challenge")==0){
        challenge(board, 0, end, moves);
        printf("%s\n", moves);
        exit(0);
    }
    srand(time(NULL)); //funzione che determina il seme per la randomizzazione
    int score=1000;
    printf("Prima di cominciare inserisci le dimensioni del labirinto\n");

    _Bool wrongChar = 1;
    while (wrongChar){
        printf("Larghezza:");
        scanf(" %d", &xSide);
        if (xSide<3){
            wrongInput();
        }
        else wrongChar=0;
    }

    wrongChar = 1;
    while (wrongChar){
        printf("Altezza:");
        scanf(" %d", &ySide);
        if (ySide<3){
            wrongInput();
        }
        else wrongChar=0;
    }

    printf("\n");

    char choose;
    wrongChar = 1;
    printf("Voglio utilizzare:\n1. Un labirinto inserito da me\n2. Il labirinto generato dal programma\n");
    int start;
    while (wrongChar){
        scanf(" %c", &choose);
        printf("\n");
        if(choose== '1') { //se l'utente vuole inserire il proprio labirinto
            char* s = (char*)malloc(sizeof(char)*xSide); //stringa di input
            int i;
            printf("Inserisci il tuo labirinto:\n");
            for (i=0; i<ySide; i++){
                scanf(" %[^\n]s", s);
                int j;
                for(j=0;j<xSide;j++){
                    board[i*xSide+j].symbol=s[j];
                }
            }
            free(s);

            _Bool found =0;
            i=0;

            while (!found){ //cerco lo start
                if (board[i].symbol=='o'){
                    start = i;
                    found =1;
                }
                i++;
            }

            found = 0;
            i=0;

            while (!found){ //cerco l'end
                if (board[i].symbol=='_'){
                    *end = i;
                    found =1;
                }
                i++;
            }
        }
        else if (choose== '2') { //se l'utente vuole utilizzare il labirinto generato dal programma
            printf("Scegli la difficolta' di gioco:\n1. Facile\n2. Media\n3. Difficile\n");
            unsigned short difficulty = 3;//indice di difficoltà
            scanf(" %hd", &difficulty);
            printf("\n");

            createBoard(board, xSide, ySide);

            start = createLabirint(board, end, xSide, ySide, difficulty);
        }
        else {
            wrongInput();
        }

        printf("\n");
        welcome();

        wrongChar = 1;
        while (wrongChar){
            scanf(" %c", &choose);
            printf("\n");
            if(choose== '1') {
                guide();
                moving(board, start, score, moves, xSide, ySide);
                wrongChar=0;
            }
            else if (choose== '2'){
                randomAlgorithm(board, start, score, moves,xSide, ySide);
                wrongChar=0;
            }

            else if(choose == '3'){
                aStarAlgorithm(board, start, end, moves, xSide, ySide);
                wrongChar=0;
            }

            else {
                wrongInput();
            }
        }
        printf("%s\n", moves);
    }

    return 0;
}