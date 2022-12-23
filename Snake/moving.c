#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boardOperations.h"

#ifndef MESSAGES
#define MESSAGES
    void death();
    void success(int score);
    void wrongInput();
#endif //MESSAGES

int nextMove(Box* board, int nextPosition, int score, int* coins, _Bool* endGame, char* movesString, int* drillPoints, size_t xSide, size_t ySide){
    movesString++;
    score--;

    if (nextPosition < 0 || nextPosition>=xSide*ySide){
        death();
    }

    else {
        if (board[nextPosition].symbol=='#') { //se la posizione sopra la posizione attuale è un muro o una parete

            if (*drillPoints>0){
                board[nextPosition].symbol= 'o';
                printLabirint(board, score, xSide, ySide);
                (*drillPoints)--;
            }
            else{
                death();
                *endGame = 1;
            }
        }

        else if (board[nextPosition].symbol=='_') { //se ho completato il percorso
            board[nextPosition].symbol= 'o';
            //system("clear");
            printLabirint(board, score, xSide, ySide);
            success(score);
            *endGame = 1;
        }

        else { //se la posizione sopra la posizione attuale è interna al labirinto
            if (board[nextPosition].symbol == '!') { //se incontra un imprevisto
                score -= (*coins) * 3 / 2; //toglie il punteggio
                *coins = *coins / 2; //dimezza numero di monete
            } else if (board[nextPosition].symbol == '$') { // se incontra una moneta
                score += 10;
                *coins++;
            } else if (board[nextPosition].symbol == 'T') {// se incontro un trapano
                *drillPoints += 3;
            }
            board[nextPosition].symbol = 'o';
            printLabirint(board, score, xSide, ySide);
            //system("clear");
        }
    }
    return score;
}

void moving (Box* board, int start, int score, char* movesString, size_t xSide, size_t ySide){ //muove il giocatore
    char move;
    int currentPosition = start;
    printLabirint(board, score, xSide, ySide);
    int* coins = (int*)malloc(sizeof(int)); //monete raccolte
    *coins=0;
    _Bool* endGame=(_Bool*)malloc(sizeof(_Bool));
    *endGame=0;
    int nextPosition;
    int* drillPoints = (int*)malloc(sizeof(int));
    *drillPoints=0;
    do{
        scanf(" %c", &move);
        switch(move){
            case 'W':
            case 'w':
                nextPosition = currentPosition - xSide;
                *movesString = 'N';
                score = nextMove(board, nextPosition, score, coins, endGame, movesString, drillPoints, xSide, ySide);
                board[currentPosition].symbol= '.';
                currentPosition = currentPosition - xSide; //nuova pos. attuale
                break;

            case 'S':
            case 's':
                nextPosition = currentPosition + xSide;
                *movesString = 'S';
                score = nextMove(board, nextPosition, score, coins, endGame, movesString, drillPoints, xSide, ySide);
                board[currentPosition].symbol= '.';
                currentPosition = currentPosition + xSide; //nuova pos. attuale
                break;

            case 'D':
            case 'd':
                nextPosition = currentPosition + 1;
                *movesString = 'E';
                score = nextMove(board, nextPosition, score, coins, endGame, movesString, drillPoints, xSide, ySide);
                board[currentPosition].symbol= '.';
                currentPosition = currentPosition + 1; //nuova pos. attuale
                break;

            case 'A':
            case 'a':
                nextPosition = currentPosition - 1;
                *movesString = 'O';
                score = nextMove(board, nextPosition, score, coins, endGame, movesString, drillPoints, xSide, ySide);
                board[currentPosition].symbol= '.';
                currentPosition = currentPosition - 1; //nuova pos. attuale
                break;

            default:
                wrongInput();
                break;
        }
    }while(!(*endGame));
    free(coins);
    free(endGame);
    free(drillPoints);
    //printf("endGame == %d\n", *endGame);
    *movesString = '\0';
    //printf("Sono arrivato alla fine della moving\n");
}