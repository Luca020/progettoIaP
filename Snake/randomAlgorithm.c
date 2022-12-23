#include <stdlib.h>
#include <string.h>
#include "boardOperations.h"

#ifndef MESSAGES
#define MESSAGES
    void death();
    void success(int score);
    void wrongInput();
#endif //MESSAGES


void moving (Box* board, int start, int score, char* movesString, size_t xSide, size_t ySide);

void randomAlgorithm(Box* board, int start, int score, char* movesString, size_t xSide, size_t ySide){
    int currentPosition = start;
    printLabirint(board, score, xSide, ySide);
    int coins = 0; //monete raccolte
    _Bool endGame=0;
    do{
        int move = rand()%4;
        switch(move){
            case 0:
                if (board[currentPosition -xSide].symbol==' ' || board[currentPosition - xSide].symbol=='.' || board[currentPosition -xSide].symbol=='$' || board[currentPosition -xSide].symbol=='!'){ //se la posizione sopra la posizione attuale è parte del labirinto
                    board[currentPosition].symbol= '.';
                    if (board[currentPosition - xSide].symbol=='!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (board[currentPosition - xSide].symbol=='$'){ // se incontra una moneta
                        score += 10;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    board[currentPosition - xSide].symbol= 'o';
                    currentPosition = currentPosition - xSide; //nuova pos. attuale
                    //system("clear");
                    *movesString = 'N';
                    movesString++;
                    printLabirint(board, score, xSide, ySide);
                }
                else if (board[currentPosition - xSide].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol= '.';
                    board[currentPosition - xSide].symbol= 'o';
                    score--;
                    //system("clear");
                    printLabirint(board, score, xSide, ySide);
                    success(score);
                    endGame = 1;
                }
                /* else { //se la posizione sotto la posizione attuale è un muro o una parete
                   death();
                   endGame = 1;
               }*/
                break;
            case 1:
                if (board[currentPosition +xSide].symbol==' ' || board[currentPosition + xSide].symbol=='.' || board[currentPosition +xSide].symbol=='$' || board[currentPosition +xSide].symbol=='!'){ //se la posizione sotto la posizione attuale è parte del labirinto
                    board[currentPosition].symbol='.';
                    if (board[currentPosition + xSide].symbol=='!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (board[currentPosition + xSide].symbol=='$'){ // se incontra una moneta
                        score += 10;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    board[currentPosition + xSide].symbol='o';
                    currentPosition = currentPosition + xSide; //nuova pos. attuale
                    //system("clear");
                    *movesString = 'S';
                    movesString++;
                    printLabirint(board, score, xSide, ySide);
                }
                else if (board[currentPosition + xSide].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition + xSide].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(board, score, xSide, ySide);
                    success(score);
                    endGame = 1;
                }
                /* else { //se la posizione sotto la posizione attuale è un muro o una parete
                     death();
                     endGame = 1;
                 }*/
                break;
            case 2:
                if (board[currentPosition +1].symbol==' ' || board[currentPosition + 1].symbol=='.' || board[currentPosition +1].symbol=='$' || board[currentPosition +1].symbol=='!'){ //se la posizione a destra della posizione attuale è parte del labirinto
                    board[currentPosition].symbol='.';
                    if (board[currentPosition + 1].symbol=='!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (board[currentPosition + 1].symbol=='$'){ // se incontra una moneta
                        score += 10;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    board[currentPosition + 1].symbol='o';
                    currentPosition = currentPosition + 1; //nuova pos. attuale
                    *movesString = 'E';
                    movesString++;
                    //system("clear");
                    printLabirint(board, score, xSide, ySide);
                }
                else if (board[currentPosition + 1].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition + 1].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(board, score, xSide, ySide);
                    success(score);
                    endGame = 1;
                }
                /*else { //se la posizione a destra della posizione attuale è un muro o una parete
                    death();
                    endGame = 1;
                }*/
                break;
            case 3:
                if  (board[currentPosition - 1].symbol==' ' || board[currentPosition - 1].symbol=='.' || board[currentPosition -1].symbol=='$' || board[currentPosition -1].symbol=='!'){ //se la posizione a sinistra della posizione attuale è parte del labirinto
                    board[currentPosition].symbol='.';
                    if (board[currentPosition - 1].symbol=='!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (board[currentPosition - 1].symbol=='$'){ // se incontra una moneta
                        score += 10;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    board[currentPosition - 1].symbol='o';
                    currentPosition = currentPosition - 1; //nuova pos. attuale
                    //system("clear");
                    *movesString = 'O';
                    movesString++;
                    printLabirint(board, score, xSide, ySide);
                }
                else if (board[currentPosition - 1].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition - 1].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(board, score, xSide, ySide);
                    success(score);
                    endGame = 1;
                }
                /*else { //se la posizione a sinistra della posizione attuale è un muro o una parete
                    death();
                    endGame = 1;
                }*/
                break;
            default:
                wrongInput();
                break;
        }
    }
    while(!endGame);
    *movesString = '\0';
}