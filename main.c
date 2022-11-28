//892607 891480


//cose da fare:
//creare un funzione per ogni direzione di sviluppo dei muri, in modo da non ripetere il codice
//trasformare int in unsigned int quando necessario
//utilizzare gli short quando necessario
//debugging e testing
//dividere in librerie
//implementare livelli
//documentazione
//cambiare dimensioni array con size_t come tipo


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//#include "aiAlgorithms.h"

#define INTENSITY_RESET 0

/* come non includere più volte la stessa libreria. Da mettere nella libreria
#ifndef __PART1_H__
#define __PART1_H__

Dichiarazione funzioni
 void f1();
 void f2();
 void g1(time_t x);

#endif
*/

struct box {
    char symbol;
    int reference;
    short predecessor;
    short gCost;
    short hCost;
    short numCoins;
    _Bool extracted;
    _Bool visited;
};
typedef struct box Box;


void createBoard(Box* board, size_t xSide, size_t ySide){ //crea il campo di gioco con pareti e spazi vuoti
    for(int i = 0; i < (xSide*ySide); i++){
        //board[i].visited=0;
        if ((i>=0 && i<(xSide)) || (i>=((xSide*ySide)-xSide) && i<(xSide*ySide)) || i%xSide==0 || i%xSide==(xSide-1)){
            board[i].symbol='#';
        }
        else {
            board[i].symbol=' ';
        }
    }
}

int randomStart(size_t xSide, size_t ySide){ //genera uno start in posizione casuale
    int start;
    short randomSide = rand()%4; //sceglie a caso uno dei 4 lati
    switch(randomSide) {
        case 0: //lato superiore
            start=(rand()%(xSide-2))+1; //sceglie randomicamente un numero tra 1 e (xSide-2)
            break;
        case 1: //lato inferiore
            start=rand()%(xSide-2) + 1 + ((xSide*ySide)-xSide); //sceglie randomicamente un numero tra ((xSide*ySide)-xSide+1) e (xSide*ySide-1)
            break;
        case 2: //lato sinistro
            start=((rand()%(ySide-2))*xSide)+xSide;//sceglie randomicamente un numero tra xSide e (xSide*ySide)-xSide che modulo xSide dia 0
            break;
        case 3: //lato destro
            start=((rand()%(ySide-2))*xSide)+xSide+xSide-1; //sceglie randomicamente un numero tra xSide+xSide-1 e (xSide*ySide)-xSide-1 che modulo xSide dia (xSide-1)
            break;
        default:
            printf("Errore\n");
            break;
    }
    return start;
}

void randomEnd(int start, int* end, size_t xSide, size_t ySide){ // genera un end in posizione casuale in una parete diversa da quella dello start
    int a;
    int dir[3];
    if (start<(xSide-1)){ //se lo start è posizionato sulla parete superiore
        dir[0]=1;
        dir[1]=2;
        dir[2]=3;
    }
    else if (start>((xSide*ySide)-xSide)){ //se lo start è posizionato sulla parete inferiore
        dir[0]=0;
        dir[1]=2;
        dir[2]=3;
    }
    else if (start%xSide==0){ //se lo start è posizionato sulla parete sinistra
        dir[0]=0;
        dir[1]=1;
        dir[2]=3;
    }
    else { //se lo start è posizionato sulla parete destra
        dir[0]=0;
        dir[1]=1;
        dir[2]=2;
    }
    short draft = rand()%3; //sceglie a caso uno num compreso tra 0 e 2
    switch(dir[draft]) {
        case 0: //lato superiore
            a = ((rand()%(xSide-2))+1);
            *end=a; //sceglie randomicamente un numero tra 1 e (xSide-2)
            break;
        case 1: //lato inferiore
            a = (rand()%(xSide-2) + 1 + ((xSide*ySide)-xSide));
            *end=a; //sceglie randomicamente un numero tra ((xSide*ySide)-xSide+1) e (xSide*ySide-1)
            break;
        case 2: //lato sinistro
            a = (((rand()%(ySide-2))*xSide)+xSide);
            *end=a;//sceglie randomicamente un numero tra xSide e (xSide*ySide)-xSide che modulo xSide dia 0
            break;
        case 3: //lato destro
            a = (((rand()%(ySide-2))*xSide)+xSide+xSide-1);
            *end=a; //sceglie randomicamente un numero tra xSide+xSide-1 e (xSide*ySide)-xSide-1 che modulo xSide dia (xSide-1)
            break;
        default:
            printf("Errore\n");
            break;
    }
}

void printLabirint(Box* board, int score, size_t xSide, size_t ySide){ //stampa il labirinto
    //printf("Sono entrato nella printLabirint\n");
    for(int i = 0; i < ySide; i++){
        for(int j = 0; j < xSide; j++){
            /*if (board[i*xSide+j].symbol=='#'){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
                printf("  ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), INTENSITY_RESET);
                printf("\033[1;37m");
            }
            else if (board[i*xSide+j].symbol=='.'||board[i*xSide+j].symbol=='o'){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
                printf("  ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), INTENSITY_RESET);
                printf("\033[1;37m");
            }
            else if (board[i*xSide+j].symbol=='_'){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE);
                printf("  ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), INTENSITY_RESET);
                printf("\033[1;37m");
            }

            else{*/
            printf("%c ", board[i*xSide+j].symbol);
            //}
        }
        //printf("\033[0;37m");
        printf("\n");
    }
    printf("\nPunteggio: %d\n", score);
    printf(".....................................................................\n");
    printf("\n");
}

void randomCoins(Box *board, size_t xSide, size_t ySide, int difficulty){ //posiziona un numero casuale di monete in posizioni casuali
    
    short numOfCoins;
    numOfCoins = (rand()%(xSide*3))+5; //determina il numero di monete. Minimo 10, massimo (xSide+9)

    for (int i=0; i<numOfCoins; i++) {
        int coin = rand()%(xSide*ySide);
        if (board[coin].symbol!='#' && board[coin].symbol!='_' && board[coin].symbol!= 'o'){
            board[coin].symbol= '$';}

    }

}

void randomBombs(Box *board, size_t xSide, size_t ySide, int difficulty) { //posiziona un numero casuale di imprevisti (!) in posizioni casuali
    
    short numOfBombs;

    numOfBombs = (rand()%xSide)+5; //determina il numero di imprevisti. Minimo 5, massimo (xSide+4)
    for (int i = 0; i < numOfBombs; i++) {
            int bomb = rand() % (xSide * ySide);
            if (board[bomb].symbol!= '#' && board[bomb].symbol!= '_' && board[bomb].symbol!= '$' && board[bomb].symbol!= 'o') {
                board[bomb].symbol= '!';
            }
        } 
}

void randomFlyingWalls(Box *board, size_t xSide, size_t ySide, int difficulty) { //posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale

    int numOfWalls;
    int d; // determina il numero massimo di ripetizioni, serve per non entrare in loop nel caso in cui non ci siano più spazi liberi in cui inserire muri
    int k = (rand() % ((xSide/2))) +1; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSide/3 *2 +1)
    int base;
    if(difficulty == 1){
        numOfWalls = (rand()%((xSide/2)));
    }else if(difficulty == 2){
        numOfWalls = (rand()%xSide); //determina randomicamente il numero di muri
    }else if(difficulty == 3){
        numOfWalls = (rand()%((xSide*2)))+5;
    }
    //printf("il numero di muri volanti e' %d\n", numOfWalls);
    for (int i = 0; i < numOfWalls; i++) {
        d = xSide*ySide*xSide;
        do {
            base = rand() % (xSide * ySide);
            d--;
        } while ((board[base].symbol!=' ' || board[base+1].symbol=='#' || board[base-1].symbol=='#' || board[base+xSide].symbol=='#' || board[base-xSide].symbol=='#'
                  || board[base+xSide+1].symbol=='#' || board[base+xSide-1].symbol=='#' || board[base-xSide+1].symbol=='#' || board[base-xSide-1].symbol=='#') && d>0);
        //se la casella scelta non è uno spazio o se confina con un muro o una parete
        if (d>0){
            board[base].symbol='#';
            short direction = rand()%4;
            switch(direction){
                case 0: //basso
                    while (board[base+xSide-1].symbol!='#' && board[base+xSide].symbol!='#' && board[base+xSide+1].symbol!='#' && board[base+xSide+xSide-1].symbol!='#' && board[base+xSide+xSide].symbol!='#' && board[base+xSide+xSide+1].symbol!='#' &&
                           board[base+xSide-1].symbol!='_' && board[base+xSide].symbol!='_' && board[base+xSide+1].symbol!='_' && board[base+xSide+xSide-1].symbol!='_' && board[base+xSide+xSide].symbol!='_' && board[base+xSide+xSide+1].symbol!='_' &&
                           board[base+xSide-1].symbol!='o' && board[base+xSide].symbol!='o' && board[base+xSide+1].symbol!='o' && board[base+xSide+xSide-1].symbol!='o' && board[base+xSide+xSide].symbol!='o' && board[base+xSide+xSide+1].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                        board[base+xSide].symbol='#';
                        base += xSide;
                        k--;
                    }
                    break;
                case 1: //alto
                    while (board[base-xSide-1].symbol!='#' && board[base-xSide].symbol!='#' && board[base-xSide+1].symbol!='#' && board[base-xSide-xSide-1].symbol!='#' && board[base-xSide-xSide].symbol!='#' && board[base-xSide-xSide+1].symbol!='#' &&
                           board[base-xSide-1].symbol!='_' && board[base-xSide].symbol!='_' && board[base-xSide+1].symbol!='_' && board[base-xSide-xSide-1].symbol!='_' && board[base-xSide-xSide].symbol!='_' && board[base-xSide-xSide+1].symbol!='_' &&
                           board[base-xSide-1].symbol!='o' && board[base-xSide].symbol!='o' && board[base-xSide+1].symbol!='o' && board[base-xSide-xSide-1].symbol!='o' && board[base-xSide-xSide].symbol!='o' && board[base-xSide-xSide+1].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                        board[base-xSide].symbol='#';
                        base -= xSide;
                        k--;
                    }
                    break;
                case 2: //destra
                    while (board[base-xSide+1].symbol!='#' && board[base-xSide+2].symbol!='#' && board[base+1].symbol!='#' && board[base+2].symbol!='#' && board[base+xSide+1].symbol!='#' && board[base+xSide+2].symbol!='#' &&
                           board[base-xSide+1].symbol!='_' && board[base-xSide+2].symbol!='_' && board[base+1].symbol!='_' && board[base+2].symbol!='_' && board[base+xSide+1].symbol!='_' && board[base+xSide+2].symbol!='_' &&
                           board[base-xSide+1].symbol!='o' && board[base-xSide+2].symbol!='o' && board[base+1].symbol!='o' && board[base+2].symbol!='o' && board[base+xSide+1].symbol!='o' && board[base+xSide+2].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                        board[base+1].symbol='#';
                        base++;
                        k--;
                    }
                    break;
                case 3: //sinistra
                    while (board[base-xSide-1].symbol!='#' && board[base-xSide-2].symbol!='#' && board[base-1].symbol!='#' && board[base-2].symbol!='#' && board[base+xSide-1].symbol!='#' && board[base+xSide-2].symbol!='#' &&
                           board[base-xSide-1].symbol!='_' && board[base-xSide-2].symbol!='_' && board[base-1].symbol!='_' && board[base-2].symbol!='_' && board[base+xSide-1].symbol!='_' && board[base+xSide-2].symbol!='_' &&
                           board[base-xSide-1].symbol!='o' && board[base-xSide-2].symbol!='o' && board[base-1].symbol!='o' && board[base-2].symbol!='o' && board[base+xSide-1].symbol!='o' && board[base+xSide-2].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start
                        board[base-1].symbol='#';
                        base--;
                        k--;
                    }
                    break;
                default:
                    printf("Errore\n");
                    break;
            }
        }
    }
}

void randomWalls(Box *board, size_t xSide, size_t ySide, int difficulty){ //posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale
    int numOfWalls;
    if(difficulty == 1){
        numOfWalls = (rand()%((xSide/2)));
    }else if(difficulty == 2){
        numOfWalls = (rand()%(xSide));//determina il numero di muri
    }else if(difficulty == 3){
        numOfWalls = (rand()%(xSide*2));
    }

    //printf("il numero di muri e' %d\n", numOfWalls);
    int d;
    int k=(rand()%((xSide/3)*2))+2; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSide/3 *2 +1)
    int base;
    for (int i=0; i< numOfWalls; i++) {
        d = xSide*ySide*xSide; // determina il numero massimo di ripetizioni, serve per non entrare in loop nel caso in cui non ci siano più spazi liberi in cui inserire muri
        do {
            base = rand()%(xSide*ySide);
            d--;
        } while ((board[base].symbol!='#' || base==0 ||  base==(xSide-1) || base==((xSide*ySide)-1) || base==((xSide*ySide)-xSide+1)) && d>0); //se la casella scelta e' un angolo o se non contiene '#'
        if (d>0){
            if (base<(xSide-1)){ //se la casella scelta fa parte della parete superiore
                while (board[base+xSide-1].symbol!='#' && board[base+xSide].symbol!='#' && board[base+xSide+1].symbol!='#' && board[base+xSide+xSide-1].symbol!='#' && board[base+xSide+xSide].symbol!='#' && board[base+xSide+xSide+1].symbol!='#' &&
                       board[base+xSide-1].symbol!='_' && board[base+xSide].symbol!='_' && board[base+xSide+1].symbol!='_' && board[base+xSide+xSide-1].symbol!='_' && board[base+xSide+xSide].symbol!='_' && board[base+xSide+xSide+1].symbol!='_' &&
                       board[base+xSide-1].symbol!='o' && board[base+xSide].symbol!='o' && board[base+xSide+1].symbol!='o' && board[base+xSide+xSide-1].symbol!='o' && board[base+xSide+xSide].symbol!='o' && board[base+xSide+xSide+1].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                    board[base+xSide].symbol='#';
                    base += xSide;
                    k--;
                }
            }
            else if (base>((xSide*ySide)-xSide+1) && base<((xSide*ySide)-1)){ //se la casella scelta fa parte della parete inferiore
                while (board[base-xSide-1].symbol!='#' && board[base-xSide].symbol!='#' && board[base-xSide+1].symbol!='#' && board[base-xSide-xSide-1].symbol!='#' && board[base-xSide-xSide].symbol!='#' && board[base-xSide-xSide+1].symbol!='#' &&
                       board[base-xSide-1].symbol!='_' && board[base-xSide].symbol!='_' && board[base-xSide+1].symbol!='_' && board[base-xSide-xSide-1].symbol!='_' && board[base-xSide-xSide].symbol!='_' && board[base-xSide-xSide+1].symbol!='_' &&
                       board[base-xSide-1].symbol!='o' && board[base-xSide].symbol!='o' && board[base-xSide+1].symbol!='o' && board[base-xSide-xSide-1].symbol!='o' && board[base-xSide-xSide].symbol!='o' && board[base-xSide-xSide+1].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                    board[base-xSide].symbol='#';
                    base -= xSide;
                    k--;
                }
            }
            else if (base%xSide==0){ //se la casella scelta fa parte della parete sinistra
                while (board[base-xSide+1].symbol!='#' && board[base-xSide+2].symbol!='#' && board[base+1].symbol!='#' && board[base+2].symbol!='#' && board[base+xSide+1].symbol!='#' && board[base+xSide+2].symbol!='#' &&
                       board[base-xSide+1].symbol!='_' && board[base-xSide+2].symbol!='_' && board[base+1].symbol!='_' && board[base+2].symbol!='_' && board[base+xSide+1].symbol!='_' && board[base+xSide+2].symbol!='_' &&
                       board[base-xSide+1].symbol!='o' && board[base-xSide+2].symbol!='o' && board[base+1].symbol!='o' && board[base+2].symbol!='o' && board[base+xSide+1].symbol!='o' && board[base+xSide+2].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                    board[base+1].symbol='#';
                    base++;
                    k--;
                }
            }
            else if (base%xSide==(xSide-1)){ //se la casella scelta fa parte della parete destra
                while (board[base-xSide-1].symbol!='#' && board[base-xSide-2].symbol!='#' && board[base-1].symbol!='#' && board[base-2].symbol!='#' && board[base+xSide-1].symbol!='#' && board[base+xSide-2].symbol!='#' &&
                       board[base-xSide-1].symbol!='_' && board[base-xSide-2].symbol!='_' && board[base-1].symbol!='_' && board[base-2].symbol!='_' && board[base+xSide-1].symbol!='_' && board[base+xSide-2].symbol!='_' &&
                       board[base-xSide-1].symbol!='o' && board[base-xSide-2].symbol!='o' && board[base-1].symbol!='o' && board[base-2].symbol!='o' && board[base+xSide-1].symbol!='o' && board[base+xSide-2].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start
                    board[base-1].symbol='#';
                    base--;
                    k--;
                }
            }
            else { //se la casella scelta fa parte di un muro inserito nei cicli precedenti
                k += 5;
                if (board[base+1].symbol=='#' || board[base-1].symbol=='#'){ //se è un muro orizzontale
                    short direction = rand()%2; //scelgo un lato dal quale sviluppare un ulteriore ramo
                    switch(direction){
                        case 0: //sviluppo un ramo verso il basso
                            while (board[base+xSide-1].symbol!='#' && board[base+xSide].symbol!='#' && board[base+xSide+1].symbol!='#' && board[base+xSide+xSide-1].symbol!='#' && board[base+xSide+xSide].symbol!='#' && board[base+xSide+xSide+1].symbol!='#' &&
                                   board[base+xSide-1].symbol!='_' && board[base+xSide].symbol!='_' && board[base+xSide+1].symbol!='_' && board[base+xSide+xSide-1].symbol!='_' && board[base+xSide+xSide].symbol!='_' && board[base+xSide+xSide+1].symbol!='_' &&
                                   board[base+xSide-1].symbol!='o' && board[base+xSide].symbol!='o' && board[base+xSide+1].symbol!='o' && board[base+xSide+xSide-1].symbol!='o' && board[base+xSide+xSide].symbol!='o' && board[base+xSide+xSide+1].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                                board[base+xSide].symbol='#';
                                base += xSide;
                                k--;
                            }
                            break;
                        case 1: //sviluppo un ramo verso l'alto
                            while (board[base-xSide-1].symbol!='#' && board[base-xSide].symbol!='#' && board[base-xSide+1].symbol!='#' && board[base-xSide-xSide-1].symbol!='#' && board[base-xSide-xSide].symbol!='#' && board[base-xSide-xSide+1].symbol!='#' &&
                                   board[base-xSide-1].symbol!='_' && board[base-xSide].symbol!='_' && board[base-xSide+1].symbol!='_' && board[base-xSide-xSide-1].symbol!='_' && board[base-xSide-xSide].symbol!='_' && board[base-xSide-xSide+1].symbol!='_' &&
                                   board[base-xSide-1].symbol!='o' && board[base-xSide].symbol!='o' && board[base-xSide+1].symbol!='o' && board[base-xSide-xSide-1].symbol!='o' && board[base-xSide-xSide].symbol!='o' && board[base-xSide-xSide+1].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                                board[base-xSide].symbol='#';
                                base -= xSide;
                                k--;
                            }
                            break;
                        default:
                            printf("Errore!\n");
                            break;
                    }
                }
                else{ // se è un muro verticale
                    int direction = rand()%2; //scelgo un lato dal quale sviluppare un ulteriore ramo
                    switch(direction){
                        case 0: //sviluppo un ramo verso sinistra
                            while (board[base-xSide+1].symbol!='#' && board[base-xSide+2].symbol!='#' && board[base+1].symbol!='#' && board[base+2].symbol!='#' && board[base+xSide+1].symbol!='#' && board[base+xSide+2].symbol!='#' &&
                                   board[base-xSide+1].symbol!='_' && board[base-xSide+2].symbol!='_' && board[base+1].symbol!='_' && board[base+2].symbol!='_' && board[base+xSide+1].symbol!='_' && board[base+xSide+2].symbol!='_' &&
                                   board[base-xSide+1].symbol!='o' && board[base-xSide+2].symbol!='o' && board[base+1].symbol!='o' && board[base+2].symbol!='o' && board[base+xSide+1].symbol!='o' && board[base+xSide+2].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                                board[base+1].symbol='#';
                                base++;
                                k--;
                            }
                            break;
                        case 1: //sviluppo un ramo verso destra
                            while (board[base-xSide-1].symbol!='#' && board[base-xSide-2].symbol!='#' && board[base-1].symbol!='#' && board[base-2].symbol!='#' && board[base+xSide-1].symbol!='#' && board[base+xSide-2].symbol!='#' &&
                                   board[base-xSide-1].symbol!='_' && board[base-xSide-2].symbol!='_' && board[base-1].symbol!='_' && board[base-2].symbol!='_' && board[base+xSide-1].symbol!='_' && board[base+xSide-2].symbol!='_' &&
                                   board[base-xSide-1].symbol!='o' && board[base-xSide-2].symbol!='o' && board[base-1].symbol!='o' && board[base-2].symbol!='o' && board[base+xSide-1].symbol!='o' && board[base+xSide-2].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start
                                board[base-1].symbol='#';
                                base--;
                                k--;
                            }
                            break;
                        default:
                            printf("Errore!\n");
                            break;
                    }
                }
            }
        }
    }
}

int createLabirint(Box* board, int * end, size_t xSide, size_t ySide, int difficulty){
    //printf("Sono entrato nella createLabirint\n");
    int start = randomStart(xSide, ySide); //genera uno start in posizione casuale
    //printf("Questo e' lo start: %d\n", start);
    //printf("Sono tornato dalla randomStart\n");
    board[start].symbol='o';
    //printf("Ho assegnato 'o' allo start\n");
    randomEnd(start, end, xSide, ySide); // genera un end in posizione casuale
    //printf("Sono tornato dalla randomEnd\n");
    board[*end].symbol='_';
    randomFlyingWalls(&board[0], xSide, ySide, difficulty); // posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale
    //printf("Sono tornato dalla randomFlyingWalls\n");
    randomWalls(&board[0], xSide, ySide, difficulty); //posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale
    //printf("Sono tornato dalla randomWalls\n");
    randomCoins(&board[0], xSide, ySide, difficulty); //posiziona un numero casuale di monete in posizioni casuali
    //printf("Sono tornato dalla randomCoins\n");
    randomBombs(&board[0], xSide, ySide, difficulty); //posiziona un numero casuale di imprevisti in posizioni casuali
    //printf("Sono tornato dalla randomBombs\n");
    return start;

}

char welcome(){
    printf("Benvenuto! In che modalita' vuoi giocare?\n");
    printf("Per modalita' utente premere 'a'\n");
    printf("Per modalita' AI random premere 'b'\n");
    printf("Per modalita' AI avanzata premere 'c'");
    printf("\n");
}

void guide(){ //informazioni per il giocatore
    printf("SEI IN MODALITA' UTENTE.\n\n");
    printf("Per muoversi all'interno del labirinto utilizzare:\n'w' per muoversi verso l'alto\n"
           "'s' per muoversi verso il basso\n'a' per per muoversi verso sinistra\n'd' per muoversi verso destra\n\n");
    printf("Partendo dallo start (o) raggiungere il traguardo (_) con il punteggio piu' alto possibile. \n");
    printf("Ogni mossa costa un punto. \nOgni moneta ($) concede 3 punti extra.\n"
           "Ogni imprevisto (!) dimezza il numero di monete raccolte fino a quel momento.\n\n");
    printf("BUONA PARTITA! \n\n\n");
}



void death(){ //il giocatore è morto
    printf("Purtroppo sei morto!");
    printf("\n");
}

void success(int score){ //il giocatore è giunto a destinazione
    printf("\n");
    printf("Congratulazioni! Sei arrivato a destinazione!\n");
    printf("Il tuo punteggio e': %d\n", score);
}

void wrongInput(){ //il giocatore ha inserito in input non corretto
    printf("Input non corretto, reinserirlo.\n");
}


int nextMove(Box* board, int nextPosition, int score, int* coins, _Bool* endGame, char* movesString, int* drillPoints){
    printf("sono entrato nella nextMove\n");
    movesString++;
    score--;

    if (board[nextPosition].symbol=='#') { //se la posizione sopra la posizione attuale è un muro o una parete
        if (drillPoints>0){
            board[nextPosition].symbol= 'o';
        }
        else{
            death();
            *endGame = 1;
        }
    }

    else if (board[nextPosition].symbol=='_') { //se ho completato il percorso
        board[nextPosition].symbol= 'o';
        //system("clear");
        success(score);
        *endGame = 1;
    }
    
   else { //se la posizione sopra la posizione attuale è interna al labirinto

        if (board[nextPosition].symbol == '!') { //se incontra un imprevisto
            score -= *coins * 3 / 2; //toglie il punteggio
            *coins = *coins / 2; //dimezza numero di monete
        } else if (board[nextPosition].symbol == '$') { // se incontra una moneta
            score += 10;
            *coins++;
        } else if (board[nextPosition].symbol == 'T') {// se incontro un trapano
            *drillPoints += 3;
        }
        board[nextPosition].symbol = 'o';

        //system("clear");
    }
    
    return score;
}


void moving (Box* board, int start, int score, char* movesString, size_t xSide, size_t ySide){ //muove il giocatore
    char move;
    int currentPosition = start;
    printLabirint(&board[0], score, xSide, ySide);
    int* coins = (int*)malloc(sizeof(int)); //monete raccolte
    _Bool* endGame=(_Bool*)malloc(sizeof(_Bool));
    *endGame=0;
    int nextPosition;
    int* drillPoints = (int*)malloc(sizeof(int));
    do{
        scanf(" %c", &move);
        switch(move){
            case 'W':
            case 'w':
                nextPosition = currentPosition - xSide;
                *movesString = 'N';
                score = nextMove(&board[0], nextPosition, score, coins, endGame, movesString, drillPoints);
                board[currentPosition].symbol= '.';
                currentPosition = currentPosition - xSide; //nuova pos. attuale
                printLabirint(&board[0], score, xSide, ySide);
                /* *movesString = 'N';
                movesString++;
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
                    printLabirint(&board[0], score, xSide, ySide);
                }
                else if (board[currentPosition - xSide].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol= '.';
                    board[currentPosition - xSide].symbol= 'o';
                    score--;
                    //system("clear");
                    printLabirint(&board[0], score,xSide,ySide);
                    success(score);
                    endGame = 1;
                }
                else { //se la posizione sopra la posizione attuale è un muro o una parete
                    death();
                    endGame = 1;
                }*/
                break;

            case 'S':
            case 's':
                nextPosition = currentPosition + xSide;
                *movesString = 'S';
                score = nextMove(&board[0], nextPosition, score, coins, endGame, movesString, drillPoints);
                board[currentPosition].symbol= '.';
                currentPosition = currentPosition + xSide; //nuova pos. attuale
                printLabirint(&board[0], score, xSide, ySide);
                /*
                *movesString = 'S';
                movesString++;
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
                    printLabirint(&board[0], score, xSide, ySide);
                }

                else if (board[currentPosition + xSide].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition + xSide].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(&board[0], score, xSide,ySide);
                    success(score);
                    endGame = 1;
                }
                else { //se la posizione sotto la posizione attuale è un muro o una parete
                    death();
                    endGame = 1;
                }*/
                break;

            case 'D':
            case 'd':
                nextPosition = currentPosition + 1;
                *movesString = 'E';
                score = nextMove(&board[0], nextPosition, score, coins, endGame, movesString, drillPoints);
                board[currentPosition].symbol= '.';
                currentPosition = currentPosition + 1; //nuova pos. attuale
                printLabirint(&board[0], score, xSide, ySide);/*
                *movesString = 'E';
                movesString++;
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
                    //system("clear");
                    printLabirint(&board[0], score, xSide,ySide);
                }
                else if (board[currentPosition + 1].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition + 1].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(&board[0], score, xSide, ySide);
                    success(score);
                    endGame = 1;
                }
                else { //se la posizione a destra della posizione attuale è un muro o una parete
                    death();
                    endGame = 1;
                }*/
                break;

            case 'A':
            case 'a':
                nextPosition = currentPosition - 1;
                *movesString = 'O';
                score = nextMove(&board[0], nextPosition, score, coins, endGame, movesString, drillPoints);
                board[currentPosition].symbol= '.';
                currentPosition = currentPosition - 1; //nuova pos. attuale
                printLabirint(&board[0], score, xSide, ySide);/*
                *movesString = 'O';
                movesString++;
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
                    printLabirint(&board[0], score, xSide, ySide);
                }
                else if (board[currentPosition - 1].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition - 1].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(&board[0], score, xSide, ySide);
                    success(score);
                    endGame = 1;
                }
                else { //se la posizione a sinistra della posizione attuale è un muro o una parete
                    death();
                    endGame = 1;
                }*/
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
}

void randomAlgorithm(Box* board, int start, int score, char* movesString, size_t xSide, size_t ySide){
    int currentPosition = start;
    printLabirint(&board[0], score, xSide, ySide);
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
                    printLabirint(&board[0], score, xSide, ySide);
                }
                else if (board[currentPosition - xSide].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol= '.';
                    board[currentPosition - xSide].symbol= 'o';
                    score--;
                    //system("clear");
                    printLabirint(&board[0], score, xSide, ySide);
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
                    printLabirint(&board[0], score, xSide, ySide);
                }
                else if (board[currentPosition + xSide].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition + xSide].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(&board[0], score, xSide, ySide);
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
                    printLabirint(&board[0], score, xSide, ySide);
                }
                else if (board[currentPosition + 1].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition + 1].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(&board[0], score, xSide, ySide);
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
                    printLabirint(&board[0], score, xSide, ySide);
                }
                else if (board[currentPosition - 1].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition - 1].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(&board[0], score, xSide, ySide);
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



void aStarAlgorithm(Box* board, int start, int* end, char* movesString, size_t xSide, size_t ySide){

    //printf("Sono dentro la aStarLabirint\n");
    /* Prendo in input il campo di gioco, il punto di start e quello di end.
     * Inizializzo il nodo start con gCost (distanza dallo start) è = 0 mentre hCost (distanza da end) è = xDist+yDist (distanza sulle ascisse + distanza sulle ordinate).
     * La reference di start è pari a start.
     * Il predecessor di start è -1.
     * Il nodo start non è estratto (extracted = 0) e non è visitato (visited = 0)
     * Creo un array di struct candidates al cui interno andrò ad inserire i nodi candidati, cioè i nodi che visito man mano
     * In prima posizione di questo array posiziono start
     * Estraggo start
     * Inizializzo un boolean arrived = 0
     * Entro in un ciclo while e non esco finché arrived != 1, cioè finché non sono arrivato alla fine (end)
     *      Inizializzo la variabile min con il valore fittizio xSide*ySide (lato lungo per lato corto del campo)
     *      Entro in un ciclo for in cui cerco il nodo non estratto con valore fCost (gCost + hCost) minimo all'interno dell'array candidates
     *      Estraggo il nodo trovato (extracted = 1)
     *      Analizzo i nodi adiacenti al nodo estratto (in alto, a destra, a sinistra, in basso)
     *      Per ciascuna delle 4 casistiche, se il nodo adiacente non è un muro, se non è il predecessore del nodo estratto e se non è al di fuori dei boundaries dell'array lo inserisco all'interno dell'array candidates
     *      Per ciascuno di questi nodi inizializzo extracted a 0, predecessor con il nodo che lo ha generato, reference con il nodo all'interno del campo iniziale al quale fa riferimento, e fCost con la somma della distanza da start e da end
     *      Se uno di questi nodi ha hCost=0 so di poter terminare la ricerca del path e pertanto setto arrived a 1, in modo da uscire dal while
     * inizializzo un array di short predecessors di dimensione xSide*ySide/2
     * entro in un while nel quale inserisco i nodi del path da end fino a start
     * creo un array di short dentro al quale ricopio tutti i nodi presenti in predecessors ma al contrario, cioè da start fino ad end
     * con un ciclo for inizializzo il campo 'symbol' dei nodi appartenti al path trovato con '.'
     * stampo il campo di gioco */
    for (int i =0; i<xSide*ySide; i++){
        board[i].visited=0;
        board[i].extracted=0;
    }
    int score = 0;
    short xDist, yDist;
    board[start].gCost = 0;
    xDist = abs((start%xSide) - *end%xSide);/*
    printf("start%%xSide: %d\n", start%xSide);
    printf("end%%xSide: %d\n", end%xSide);
    printf("xDist: %d,\n", xDist);*/
    yDist = abs((start/xSide) - *end/xSide);/*
    printf("start/xSide: %d\n", start/xSide);
    printf("end/xSide: %d\n", end/xSide);
    printf("yDist: %d,\n", yDist);*/
    board[start].hCost = yDist + xDist;
    board[start].reference = start;
    board[start].predecessor = -1;
    board[start].numCoins = 0;
    board[start].extracted = 1;
    board[start].visited = 1;

    Box candidates[xSide*ySide];
    candidates[0]=board[start];
    /*printf("candidates[0].symbol: %c,\ncandidates[0].predecessor: %d,\ncandidates[0].fCost: %d,\n",
           candidates[0].symbol, candidates[0].predecessor, candidates[0].gCost + candidates[0].hCost);*/
    short min;
    short i;
    short size = 1;
    _Bool arrived = 0;
    int k;

    if (start<xSide-1){ //se lo start è posizionato sulla parete superiore
        //printf("Sono entrato nell'if basso\n");
        board[start + xSide].gCost = 1 + board[start].gCost;
        xDist = abs(((start + xSide) % xSide) - *end % xSide);
        yDist = abs(((start + xSide) / xSide) - *end / xSide);
        board[start + xSide].hCost = yDist + xDist;
        board[start + xSide].predecessor = start;
        board[start + xSide].reference = start + xSide;
        board[start + xSide].visited = 1;
        if (board[start + xSide].symbol == '$'){
            board[start + xSide].numCoins = board[start].numCoins +1;
        }
        else {
            board[start + xSide].numCoins = board[start].numCoins;
        }
        candidates[size] = board[start + xSide];
        size++;
    }
    else if (start>((xSide*ySide)-xSide)){ //se lo start è posizionato sulla parete inferiore
        //printf("Sono entrato nell'if alto\n");
        board[start - xSide].gCost = 1 + board[start].gCost;
        xDist = abs(((start - xSide) % xSide) - *end % xSide);
        yDist = abs(((start - xSide) / xSide) - *end / xSide);
        board[start - xSide].hCost = yDist + xDist;
        board[start - xSide].predecessor = start;
        board[start - xSide].reference = start - xSide;
        board[start - xSide].visited = 1;
        if (board[start - xSide].symbol == '$'){
            board[start - xSide].numCoins = board[start].numCoins +1;
        }
        else {
            board[start - xSide].numCoins = board[start].numCoins;
        }
        candidates[size] = board[start - xSide];
        size++;
    }

    else if (start%xSide==0){ //se lo start è posizionato sulla parete sinistra
        //printf("Sono entrato nell'if destro\n");
        board[start + 1].gCost = 1 + board[start].gCost;
        xDist = abs(((start + 1) % xSide) - *end % xSide);
        yDist = abs(((start + 1) / xSide) - *end / xSide);
        board[start + 1].hCost = yDist + xDist;
        board[start + 1].predecessor = start;
        board[start + 1].reference = start + 1;
        board[start + 1].visited = 1;
        if (board[start +1].symbol == '$'){
            board[start + 1].numCoins = board[start].numCoins +1;
        }
        else {
            board[start + 1].numCoins = board[start].numCoins;
        }
        candidates[size] = board[start + 1];
        size++;
    }

    else { //se lo start è posizionato sulla parete destra
        //printf("Sono entrato nell'if sinistro\n");
        board[start - 1].gCost = 1 + board[start].gCost;
        xDist = abs(((start - 1) % xSide) - *end % xSide);
        yDist = abs(((start - 1) / xSide) - *end / xSide);
        board[start - 1].hCost = yDist + xDist;
        board[start - 1].predecessor = start;
        board[start - 1].reference = start - 1;
        board[start - 1].visited = 1;
        if (board[start -1].symbol == '$'){
            board[start - 1].numCoins = board[start].numCoins +1;
        }
        else {
            board[start - 1].numCoins = board[start].numCoins;
        }
        candidates[size] = board[start - 1];
        size++;
    }

    candidates[0].extracted=1;

    while (!arrived) {
        min = xSide*ySide;
        //printf("\n");
        for (int j = 0; j < size; j++) {
            /* printf("\ncandidates[%d].extracted: %d\n",j, candidates[j].extracted);
             printf("  t: %d\n", candidates[j].reference);
             printf("  candidates[%d].predecessor: %d\n",j, candidates[j].predecessor);
             printf("  candidates[%d].numCoins: %d\n",j, candidates[j].numCoins);
             printf("  candidates[%d].hCost: %d\n",j, candidates[j].hCost);
             printf("  candidates[%d].gCost: %d\n",j, candidates[j].gCost);
             printf("  candidates[%d].fCost: %d\n",j, candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10);
            */ if (candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10 <= min && candidates[j].extracted==0) {
                min = candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10;
                //printf("    MINIMO\n");
                i = j;
            }
        }

        //printf("\n\n");
        candidates[i].extracted=1;
        int t = candidates[i].reference;
        board[t].predecessor=candidates[i].predecessor;
        board[t].hCost=candidates[i].hCost;
        board[t].gCost=candidates[i].gCost;

        /*    printf("\n\n!board[t - xSide].visited = %d,\nt>=xSide = %d\nboard[t - xSide].symbol = %c\nboard[t].predecessor != t -xSide = %d\n\n"
                    , !board[t - xSide].visited, t>=xSide,board[t - xSide].symbol, board[t].predecessor != t -xSide);*/

        if (!board[t - xSide].visited && t>=xSide && board[t - xSide].symbol != '#' && board[t - xSide].symbol != 'o' && board[t].predecessor != t -xSide) { //casella alta se essa non è un muro e se non è il predecessore del nodo predecessore
            //printf("Sono entrato nell'if alto\n");
            board[t - xSide].numCoins = board[t].numCoins;
            board[t - xSide].gCost = 1 + board[t].gCost;
            xDist = abs(((t - xSide) % xSide) - *end % xSide);
            yDist = abs(((t - xSide) / xSide) - *end / xSide);
            board[t - xSide].hCost = yDist + xDist;
            if (board[t - xSide].symbol == '$'){
                board[t - xSide].numCoins = board[t].numCoins + 1;
            }
            else if (board[t - xSide].symbol == '!'){
                board[t - xSide].numCoins = (board[t].numCoins)/2;
            }
            else {
                board[t - xSide].numCoins = board[t].numCoins;
            }
            board[t - xSide].predecessor = t;
            board[t - xSide].reference = t - xSide;
            board[t - xSide].visited = 1;
            candidates[size] = board[t - xSide];

            /* printf("   candidates[size].symbol: %c,\n   candidates[size].predecessor: %d,\n   candidates[size].numCoins: %d,\n"
                    "   candidates[size].fCost: %d\n   candidates[size].reference: %d\n\n", candidates[size].symbol,
                    candidates[size].predecessor, candidates[size].numCoins,
                    candidates[size].gCost + candidates[size].hCost - candidates[size].numCoins*10, candidates[size].reference);
            */
            size++;
            if (board[t - xSide].symbol == '_') {
                arrived = 1;
                k=t-xSide;
            }
        }

        //  printf("\n\n!board[t +1 ].visited = %d,\nboard[t+1].symbol = %c\nboard[t].predecessor != t + 1 = %d\n\n"
        //        , !board[t +1].visited, board[t +1].symbol, board[t].predecessor != t +1);

        if (!board[t + 1].visited && board[t + 1].symbol != '#' && board[t + 1].symbol != 'o' && board[t].predecessor != t +1) { //casella a destra se essa non è un muro e se non è il predecessore del nodo predecessore
            //     printf("Sono entrato nell'if destro\n");
            board[t + 1].numCoins = board[t].numCoins;
            board[t + 1].gCost = 1 + board[t].gCost;
            xDist = abs(((t + 1) % xSide) - *end % xSide);
            yDist = abs(((t + 1) / xSide) - *end / xSide);
            board[t + 1].hCost = yDist + xDist;
            if (board[t + 1].symbol == '$'){
                board[t + 1].numCoins = board[t].numCoins + 1;
            }
            else if (board[t + 1].symbol == '!'){
                board[t + 1].numCoins = (board[t].numCoins)/2;
            }
            else {
                board[t + 1].numCoins = board[t].numCoins;
            }
            board[t + 1].predecessor = t;
            board[t + 1].reference = t + 1;
            board[t + 1].visited = 1;
            candidates[size] = board[t + 1];

            /*    printf("   candidates[size].symbol: %c,\n   candidates[size].predecessor: %d,\n   candidates[size].numCoins: %d,\n"
                       "   candidates[size].fCost: %d\n   candidates[size].reference: %d\n\n", candidates[size].symbol,
                       candidates[size].predecessor, candidates[size].numCoins,
                       candidates[size].gCost + candidates[size].hCost - candidates[size].numCoins*10, candidates[size].reference);
               */
            size++;
            if (board[t + 1].symbol == '_'){
                //    printf("Sono entrato nell'if finale\n");
                arrived = 1;
                k=t+1;
                //printf("k=%d\n", k);
            }
        }

        //  printf("\n\n!board[t + xSide].visited = %d,\nt<xSide*ySide-xSide = %d\nboard[t + xSide].symbol = %c\nboard[t].predecessor != t +xSide = %d\n\n"
        //          , !board[t + xSide].visited, t<xSide*ySide-xSide,board[t + xSide].symbol, board[t].predecessor != t +xSide);

        if (!board[t + xSide].visited && t<xSide*ySide-xSide && board[t + xSide].symbol != 'o' && board[t + xSide].symbol != '#' && board[t].predecessor != t +xSide) { //casella bassa se essa non è un muro e se non è il predecessore del nodo predecessore
            //     printf("Sono entrato nell'if basso\n");
            board[t + xSide].numCoins = board[t].numCoins;
            board[t + xSide].gCost = 1 + board[t].gCost;
            xDist = abs(((t + xSide) % xSide) - *end % xSide);
            yDist = abs(((t + xSide) / xSide) - *end / xSide);
            board[t + xSide].hCost = yDist + xDist;
            if (board[t + xSide].symbol == '$'){
                board[t + xSide].numCoins = board[t].numCoins + 1;
            }
            else if (board[t + xSide].symbol == '!'){
                board[t + xSide].numCoins = (board[t].numCoins)/2;
            }
            else {
                board[t + xSide].numCoins = board[t].numCoins;
            }
            board[t + xSide].predecessor = t;
            board[t + xSide].reference = t + xSide;
            board[t + xSide].visited = 1;
            candidates[size] = board[t + xSide];

            /*    printf("candidates[size].symbol: %c,\n   candidates[size].predecessor: %d,\n   candidates[size].numCoins: %d,\n   candidates[size].fCost: %d,"
                       "\n   candidates[size].reference: %d\n\n", candidates[size].symbol,
                       candidates[size].predecessor, candidates[size].numCoins,
                       candidates[size].gCost + candidates[size].hCost - candidates[size].numCoins*10, candidates[size].reference);
             */
            size++;
            if (board[t + xSide].symbol == '_'){
                arrived = 1;
                k=t+xSide;
            }
        }

        //   printf("\n\n!board[t -1 ].visited = %d,\nboard[t-1].symbol = %c\nboard[t].predecessor != t -1 = %d\n\n"
        //         , !board[t -1].visited, board[t -1].symbol, board[t].predecessor != t -1);

        if (!board[t - 1].visited && board[t - 1].symbol != '#' && board[t - 1].symbol != 'o' && board[t].predecessor != t -1) { //casella a sinistra se essa non è un muro e se non è il predecessore del nodo predecessore
            //      printf("Sono entrato nell'if sinistro\n");
            board[t - 1].numCoins = board[t].numCoins;
            board[t - 1].gCost = 1 + board[t].gCost;
            xDist = abs(((t - 1) % xSide) - *end % xSide);
            yDist = abs(((t - 1) / xSide) - *end / xSide);
            board[t - 1].hCost = yDist + xDist;
            if (board[t - 1].symbol == '$'){
                board[t - 1].numCoins = board[t].numCoins + 1;
            }
            else if (board[t - 1].symbol == '!'){
                board[t - 1].numCoins = (board[t].numCoins)/2;
            }
            else {
                board[t - 1].numCoins = board[t].numCoins;
            }
            board[t - 1].predecessor = t;
            board[t - 1].reference = t - 1;
            board[t - 1].visited = 1;
            candidates[size] = board[t - 1];

            /*    printf("candidates[size].symbol: %c,\n   candidates[size].predecessor: %d,\n   candidates[size].numCoins: %d,\n   candidates[size].fCost: %d,\n"
                       "   candidates[size].reference: %d\n\n", candidates[size].symbol,
                       candidates[size].predecessor, candidates[size].numCoins,
                       candidates[size].gCost + candidates[size].hCost - candidates[size].numCoins*10, candidates[size].reference);
             */
            size++;
            if (board[t - 1].symbol == '_') {
                arrived = 1;
                k=t-1;
            }
        }
    }

    int* predecessors = (int*)malloc(sizeof(int)*xSide*ySide);//short predecessors[xSide*ySide]; //vettore che contiene tutti i predecessori
    int j = 0;
    //printf("   board[k].predecessor prima del ciclo while = %d \n", board[k].predecessor);
    while (k!=start){
        //printf("Sono entrato nel while finale\n");
        //printf("   board[k].predecessor prima =%d\n", board[k].predecessor);

        predecessors[j]=k;
        //printf("   predecessors[%d] =%d\n", j, predecessors[j]);
        k=board[k].predecessor;
        //printf("   board[k].predecessor dopo =%d\n\n", board[k].predecessor);
        j++;
    }
    //printf("j=%d",j);
    predecessors[j]=k;
    /*printf("\nContenuto di predecessor[]:\n");
    for (int p=0; p<j; p++){
        printf("%d ", predecessors[p]);
    }*/

    short path[j-1]; //vettore che contiene tutte le posizioni del labirinto facenti parte del percorso migliore
    short r = j;
    for(i=0; i<=j; i++){
        path[i]=predecessors[r];
        r--;
    }

    /*printf("\nContenuto di path[]:\n");
    for (int p=0; p<j; p++){
        printf("%d ", path[p]);
    }*/

    //printLabirint(&board[0],score);
    //printf("\n");

    for (int h = 0; h<j; h++ ){
        //printf("Sono entrato nell'ultimo for\n");
        board[path[h]].symbol='.';
    }
    board[predecessors[0]].symbol='o';
    k=0;

    for (int i =1; i<=j; i++){
        if (path[i]==path[i-1]-xSide)//se va verso l'alto
            movesString[k]='N';
        else if (path[i]==path[i-1]+1)//se va a destra
            movesString[k]='E';
        else if (path[i]==path[i-1]+xSide)//se va verso il basso
            movesString[k]='S';
        else
            movesString[k]='O';
        k++;
    }
    movesString[k]='\0';
    score = 1000-j+(board[*end].numCoins)*10;
    free(predecessors);
    //printLabirint(&board[0], score);
    //printf("Numero di monete al termine della partita: %d\n", board[*end].numCoins);
    //printf("Numero di mosse della partita: %d\n", j);
}

void challenge(Box *board, int start, int* end, char* movesString, size_t xSide, size_t ySide){
    printf("MODALITA' SFIDA!\n");
    int i;
    char* s = (char*)malloc(sizeof(char)*xSide); //stringa di input
    for (i=0; i<ySide; i++){
        //scanf("%[^\n]s", s);
        scanf(" %[^\n]s", s);
        //printf("s = %s\n", s);
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

    aStarAlgorithm(&board[0], start, end, movesString, xSide, ySide);
}

int main(int argc, char *argv[]){
    srand(time(NULL)); //funzione che determina il seme per la randomizzazione
    int score=1000;
    int level;
    size_t xSide = 15;
    size_t ySide = 10;
    printf("PRIMA DI COMINCIARE SCEGLI LE DIMENSIONI DEL LABIRINTO:\n");
    scanf(" %ld", &xSide);
    scanf(" %ld", &ySide);
    char choose;
    printf("Scegli la difficolta' di gioco:\n1. EASY\n2. MEDIUM\n3. HARD\n");
    unsigned short difficulty;//indice di difficoltà
    scanf(" %hd", &difficulty);
    char* moves = (char*) malloc(300*sizeof(char)); //il vettore nel quale salverò la sequenza di mosse
    Box board[xSide*ySide];
    //printf("Step 1\n");
    createBoard(&board[0], xSide, ySide);
    //printf("Step 2\n");
    int* end = (int*)malloc(sizeof(int));
    int start = createLabirint(&board[0], end, xSide, ySide, difficulty);
    if (argc==2 && strcmp(argv[1], "--challenge")==0){
        challenge(&board[0], start, end, moves, xSide, ySide);
        printf("%s\n", moves);
        exit(0);
    }
    //printf("Step 3\n");
    char mode = welcome();
    //printf("Step 4\n");
    _Bool wrongChar = 1;
    while (wrongChar){
        scanf(" %c", &choose);

        if(choose== 'a' || choose=='A') {
            guide();
            moving(&board[0], start, score, moves, xSide, ySide);
            wrongChar=0;
        }
        else if (choose== 'b' || choose=='B'){
            randomAlgorithm(&board[0], start, score, moves,xSide, ySide);
            wrongChar=0;
        }

        else if(choose == 'c' || choose == 'C'){
            aStarAlgorithm(&board[0], start, end, moves, xSide, ySide);
            wrongChar=0;
        }

        else {
            wrongInput();
        }
    }

    printf("%s\n", moves);
    free(moves);
    free(end);

    return 0;
}