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
#include <windows.h>
#include "dvector.h"
#define INTENSITY_RESET 0


struct box {
    char symbol;
    int reference;
    short predecessor;
    short gCost;
    short hCost;
    short numCoins;
    short numDrills;
    _Bool extracted;
    _Bool visited;
};
typedef struct box Box;

size_t xSize = 15;
size_t ySize = 20;

void createBoard(Box* board, size_t xSize, size_t ySize){ //crea il campo di gioco con pareti e spazi vuoti
    for(int i = 0; i < (xSize*ySize); i++){
        //board[i].visited=0;
        if ((i>=0 && i<(xSize)) || (i>=((xSize*ySize)-xSize) && i<(xSize*ySize)) || i%xSize==0 || i%xSize==(xSize-1)){
            board[i].symbol='#';
        }
        else {
            board[i].symbol=' ';
        }
    }
}

int randomStart(size_t xSize, size_t ySize){ //genera uno start in posizione casuale
    int start;
    short randomSide = rand()%4; //sceglie a caso uno dei 4 lati
    switch(randomSide) {
        case 0: //lato superiore
            start=(rand()%(xSize-2))+1; //sceglie randomicamente un numero tra 1 e (xSize-2)
            break;
        case 1: //lato inferiore
            start=rand()%(xSize-2) + 1 + ((xSize*ySize)-xSize); //sceglie randomicamente un numero tra ((xSize*ySize)-xSize+1) e (xSize*ySize-1)
            break;
        case 2: //lato sinistro
            start=((rand()%(ySize-2))*xSize)+xSize;//sceglie randomicamente un numero tra xSize e (xSize*ySize)-xSize che modulo xSize dia 0
            break;
        case 3: //lato destro
            start=((rand()%(ySize-2))*xSize)+xSize+xSize-1; //sceglie randomicamente un numero tra xSize+xSize-1 e (xSize*ySize)-xSize-1 che modulo xSize dia (xSize-1)
            break;
        default:
            printf("Errore\n");
            break;
    }
    return start;
}

void randomEnd(int start, int* end, size_t xSize, size_t ySize){ // genera un end in posizione casuale nella parete opposta a quella dello start
    if (start<(xSize-1)){ //se lo start è posizionato sulla parete superiore
        *end = (rand()%(xSize-2) + 1 + ((xSize*ySize)-xSize));
    }
    else if (start>((xSize*ySize)-xSize)){ //se lo start è posizionato sulla parete inferiore
        *end=((rand()%(xSize-2))+1); //sceglie randomicamente un numero tra 1 e (xSize-2)
    }
    else if (start%xSize==0){ //se lo start è posizionato sulla parete sinistra
        *end = (((rand()%(ySize-2))*xSize)+xSize+xSize-1);
    }
    else { //se lo start è posizionato sulla parete destra
        *end=(((rand()%(ySize-2))*xSize)+xSize);
    }
}


void printLabirint(Box* board, int *score, size_t xSize, size_t ySize){ //stampa il labirinto
    //printf("Sono entrato nella printLabirint\n");
    for(int i = 0; i < ySize; i++){
        for(int j = 0; j < xSize; j++){
           /* if (board[i*xSize+j].symbol=='#'){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
                printf("  ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), INTENSITY_RESET);
                printf("\033[1;37m");
            }
            else if (board[i*xSize+j].symbol=='.'||board[i*xSize+j].symbol=='o'){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
                printf("  ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), INTENSITY_RESET);
                printf("\033[1;37m");
            }
            else if (board[i*xSize+j].symbol=='_'){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE);
                printf("  ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), INTENSITY_RESET);
                printf("\033[1;37m");
            }
            else{*/

            printf("%c ", board[i*xSize+j].symbol);
            //}
        }
        //printf("\033[0;37m");
        printf("\n");
    }
    printf("\nPunteggio: %d\n", *score);
    printf(".....................................................................\n");
    printf("\n");
}

void createObjects(Box *board, size_t xSize, size_t ySize, short numOfObjects, char typeObject){

    short position;

    for(int i = 0; i < numOfObjects; i++){
        position = rand()%(xSize*ySize);
        if (board[position].symbol!='#' && board[position].symbol!='_' && board[position].symbol!= 'o' && typeObject == '$'){
            board[position].symbol= '$';
        }
        if(board[position].symbol!= '#' && board[position].symbol != '_' && board[position].symbol!= '$' && board[position].symbol!= 'o' && board[position].symbol != '!' && typeObject == 'T'){
            board[position].symbol = 'T';
        }
        if (board[position].symbol!= '#' && board[position].symbol!= '_' && board[position].symbol!= '$' && board[position].symbol!= 'o' && typeObject == '!') {
            board[position].symbol= '!';
        }
    }

}

void randomCoins(Box* board, size_t xSize, size_t ySize){ //posiziona un numero casuale di monete in posizioni casuali
    char typeObject = '$';
    short numOfCoins = (rand()%(xSize*3))+5; //determina il numero di monete. Minimo 10, massimo (xSize+9)
    createObjects(board, xSize, ySize, numOfCoins, typeObject);
}

void randomDrill(Box* board,size_t xSize, size_t ySize){
    char typeObject = 'T';
    short numOfDrill = (rand()%xSize)+5;
    createObjects(board, xSize, ySize, numOfDrill, typeObject);
}

void randomBombs(Box* board, size_t xSize, size_t ySize) { //posiziona un numero casuale di imprevisti (!) in posizioni casuali
    char typeObject = '!';
    short numOfBombs = (rand()%xSize)+5; //determina il numero di imprevisti. Minimo 5, massimo (xSize+4)
    createObjects(board, xSize, ySize, numOfBombs, typeObject);
}

void randomFlyingWalls(Box *board, size_t xSize, size_t ySize, int difficulty) { //posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale

    int numOfWalls;
    int d; // determina il numero massimo di ripetizioni, serve per non entrare in loop nel caso in cui non ci siano più spazi liberi in cui inserire muri
    int k = (rand() % xSize) +3; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSize/3 *2 +1)
    int base;
    if(difficulty == 1){
        numOfWalls = (rand()%xSize)+2;
    }
    else if(difficulty == 2){
        numOfWalls = (rand()%(xSize*2))+3; //determina randomicamente il numero di muri
    }
    else{
        numOfWalls = (rand()%(xSize*4))+5;
    }
    //printf("il numero di muri volanti e' %d\n", numOfWalls);
    for (int i = 0; i < numOfWalls; i++) {
        d = xSize*ySize*xSize;
        do {
            base = rand() % (xSize * ySize);
            d--;
        } while ((board[base].symbol!=' ' || board[base+1].symbol=='#' || board[base-1].symbol=='#' || board[base+xSize].symbol=='#' || board[base-xSize].symbol=='#'
                  || board[base+xSize+1].symbol=='#' || board[base+xSize-1].symbol=='#' || board[base-xSize+1].symbol=='#' || board[base-xSize-1].symbol=='#') && d>0);
        //se la casella scelta non è uno spazio o se confina con un muro o una parete
        if (d>0){
            board[base].symbol='#';
            short direction = rand()%4;
            switch(direction){
                case 0: //basso
                    while (board[base+xSize-1].symbol!='#' && board[base+xSize].symbol!='#' && board[base+xSize+1].symbol!='#' && board[base+xSize+xSize-1].symbol!='#' && board[base+xSize+xSize].symbol!='#' && board[base+xSize+xSize+1].symbol!='#' &&
                           board[base+xSize-1].symbol!='_' && board[base+xSize].symbol!='_' && board[base+xSize+1].symbol!='_' && board[base+xSize+xSize-1].symbol!='_' && board[base+xSize+xSize].symbol!='_' && board[base+xSize+xSize+1].symbol!='_' &&
                           board[base+xSize-1].symbol!='o' && board[base+xSize].symbol!='o' && board[base+xSize+1].symbol!='o' && board[base+xSize+xSize-1].symbol!='o' && board[base+xSize+xSize].symbol!='o' && board[base+xSize+xSize+1].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                        board[base+xSize].symbol='#';
                        base += xSize;
                        k--;
                    }
                    break;
                case 1: //alto
                    while (board[base-xSize-1].symbol!='#' && board[base-xSize].symbol!='#' && board[base-xSize+1].symbol!='#' && board[base-xSize-xSize-1].symbol!='#' && board[base-xSize-xSize].symbol!='#' && board[base-xSize-xSize+1].symbol!='#' &&
                           board[base-xSize-1].symbol!='_' && board[base-xSize].symbol!='_' && board[base-xSize+1].symbol!='_' && board[base-xSize-xSize-1].symbol!='_' && board[base-xSize-xSize].symbol!='_' && board[base-xSize-xSize+1].symbol!='_' &&
                           board[base-xSize-1].symbol!='o' && board[base-xSize].symbol!='o' && board[base-xSize+1].symbol!='o' && board[base-xSize-xSize-1].symbol!='o' && board[base-xSize-xSize].symbol!='o' && board[base-xSize-xSize+1].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                        board[base-xSize].symbol='#';
                        base -= xSize;
                        k--;
                    }
                    break;
                case 2: //destra
                    while (board[base-xSize+1].symbol!='#' && board[base-xSize+2].symbol!='#' && board[base+1].symbol!='#' && board[base+2].symbol!='#' && board[base+xSize+1].symbol!='#' && board[base+xSize+2].symbol!='#' &&
                           board[base-xSize+1].symbol!='_' && board[base-xSize+2].symbol!='_' && board[base+1].symbol!='_' && board[base+2].symbol!='_' && board[base+xSize+1].symbol!='_' && board[base+xSize+2].symbol!='_' &&
                           board[base-xSize+1].symbol!='o' && board[base-xSize+2].symbol!='o' && board[base+1].symbol!='o' && board[base+2].symbol!='o' && board[base+xSize+1].symbol!='o' && board[base+xSize+2].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                        board[base+1].symbol='#';
                        base++;
                        k--;
                    }
                    break;
                case 3: //sinistra
                    while (board[base-xSize-1].symbol!='#' && board[base-xSize-2].symbol!='#' && board[base-1].symbol!='#' && board[base-2].symbol!='#' && board[base+xSize-1].symbol!='#' && board[base+xSize-2].symbol!='#' &&
                           board[base-xSize-1].symbol!='_' && board[base-xSize-2].symbol!='_' && board[base-1].symbol!='_' && board[base-2].symbol!='_' && board[base+xSize-1].symbol!='_' && board[base+xSize-2].symbol!='_' &&
                           board[base-xSize-1].symbol!='o' && board[base-xSize-2].symbol!='o' && board[base-1].symbol!='o' && board[base-2].symbol!='o' && board[base+xSize-1].symbol!='o' && board[base+xSize-2].symbol!='o' && k>0){
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

void randomWalls(Box *board, size_t xSize, size_t ySize, int difficulty){ //posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale
    int numOfWalls;
    if(difficulty == 1){
        numOfWalls = (rand()%((xSize)));
    }
    else if(difficulty == 2){
        numOfWalls = (rand()%(xSize*2));//determina il numero di muri
    }
    else{
        numOfWalls = (rand()%(xSize*4));
    }

    //printf("il numero di muri e' %d\n", numOfWalls);
    int d;
    int k=(rand()%xSize)+3; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSize/3 *2 +1)
    int base;
    for (int i=0; i< numOfWalls; i++) {
        d = xSize*ySize*xSize; // determina il numero massimo di ripetizioni, serve per non entrare in loop nel caso in cui non ci siano più spazi liberi in cui inserire muri
        do {
            base = rand()%(xSize*ySize);
            d--;
        } while ((board[base].symbol!='#' || base==0 ||  base==(xSize-1) || base==((xSize*ySize)-1) || base==((xSize*ySize)-xSize+1)) && d>0); //se la casella scelta e' un angolo o se non contiene '#'
        if (d>0){
            if (base<(xSize-1)){ //se la casella scelta fa parte della parete superiore
                while (board[base+xSize-1].symbol!='#' && board[base+xSize].symbol!='#' && board[base+xSize+1].symbol!='#' && board[base+xSize+xSize-1].symbol!='#' && board[base+xSize+xSize].symbol!='#' && board[base+xSize+xSize+1].symbol!='#' &&
                       board[base+xSize-1].symbol!='_' && board[base+xSize].symbol!='_' && board[base+xSize+1].symbol!='_' && board[base+xSize+xSize-1].symbol!='_' && board[base+xSize+xSize].symbol!='_' && board[base+xSize+xSize+1].symbol!='_' &&
                       board[base+xSize-1].symbol!='o' && board[base+xSize].symbol!='o' && board[base+xSize+1].symbol!='o' && board[base+xSize+xSize-1].symbol!='o' && board[base+xSize+xSize].symbol!='o' && board[base+xSize+xSize+1].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                    board[base+xSize].symbol='#';
                    base += xSize;
                    k--;
                }
            }
            else if (base>((xSize*ySize)-xSize+1) && base<((xSize*ySize)-1)){ //se la casella scelta fa parte della parete inferiore
                while (board[base-xSize-1].symbol!='#' && board[base-xSize].symbol!='#' && board[base-xSize+1].symbol!='#' && board[base-xSize-xSize-1].symbol!='#' && board[base-xSize-xSize].symbol!='#' && board[base-xSize-xSize+1].symbol!='#' &&
                       board[base-xSize-1].symbol!='_' && board[base-xSize].symbol!='_' && board[base-xSize+1].symbol!='_' && board[base-xSize-xSize-1].symbol!='_' && board[base-xSize-xSize].symbol!='_' && board[base-xSize-xSize+1].symbol!='_' &&
                       board[base-xSize-1].symbol!='o' && board[base-xSize].symbol!='o' && board[base-xSize+1].symbol!='o' && board[base-xSize-xSize-1].symbol!='o' && board[base-xSize-xSize].symbol!='o' && board[base-xSize-xSize+1].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                    board[base-xSize].symbol='#';
                    base -= xSize;
                    k--;
                }
            }
            else if (base%xSize==0){ //se la casella scelta fa parte della parete sinistra
                while (board[base-xSize+1].symbol!='#' && board[base-xSize+2].symbol!='#' && board[base+1].symbol!='#' && board[base+2].symbol!='#' && board[base+xSize+1].symbol!='#' && board[base+xSize+2].symbol!='#' &&
                       board[base-xSize+1].symbol!='_' && board[base-xSize+2].symbol!='_' && board[base+1].symbol!='_' && board[base+2].symbol!='_' && board[base+xSize+1].symbol!='_' && board[base+xSize+2].symbol!='_' &&
                       board[base-xSize+1].symbol!='o' && board[base-xSize+2].symbol!='o' && board[base+1].symbol!='o' && board[base+2].symbol!='o' && board[base+xSize+1].symbol!='o' && board[base+xSize+2].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                    board[base+1].symbol='#';
                    base++;
                    k--;
                }
            }
            else if (base%xSize==(xSize-1)){ //se la casella scelta fa parte della parete destra
                while (board[base-xSize-1].symbol!='#' && board[base-xSize-2].symbol!='#' && board[base-1].symbol!='#' && board[base-2].symbol!='#' && board[base+xSize-1].symbol!='#' && board[base+xSize-2].symbol!='#' &&
                       board[base-xSize-1].symbol!='_' && board[base-xSize-2].symbol!='_' && board[base-1].symbol!='_' && board[base-2].symbol!='_' && board[base+xSize-1].symbol!='_' && board[base+xSize-2].symbol!='_' &&
                       board[base-xSize-1].symbol!='o' && board[base-xSize-2].symbol!='o' && board[base-1].symbol!='o' && board[base-2].symbol!='o' && board[base+xSize-1].symbol!='o' && board[base+xSize-2].symbol!='o' && k>0){
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
                            while (board[base+xSize-1].symbol!='#' && board[base+xSize].symbol!='#' && board[base+xSize+1].symbol!='#' && board[base+xSize+xSize-1].symbol!='#' && board[base+xSize+xSize].symbol!='#' && board[base+xSize+xSize+1].symbol!='#' &&
                                   board[base+xSize-1].symbol!='_' && board[base+xSize].symbol!='_' && board[base+xSize+1].symbol!='_' && board[base+xSize+xSize-1].symbol!='_' && board[base+xSize+xSize].symbol!='_' && board[base+xSize+xSize+1].symbol!='_' &&
                                   board[base+xSize-1].symbol!='o' && board[base+xSize].symbol!='o' && board[base+xSize+1].symbol!='o' && board[base+xSize+xSize-1].symbol!='o' && board[base+xSize+xSize].symbol!='o' && board[base+xSize+xSize+1].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                                board[base+xSize].symbol='#';
                                base += xSize;
                                k--;
                            }
                            break;
                        case 1: //sviluppo un ramo verso l'alto
                            while (board[base-xSize-1].symbol!='#' && board[base-xSize].symbol!='#' && board[base-xSize+1].symbol!='#' && board[base-xSize-xSize-1].symbol!='#' && board[base-xSize-xSize].symbol!='#' && board[base-xSize-xSize+1].symbol!='#' &&
                                   board[base-xSize-1].symbol!='_' && board[base-xSize].symbol!='_' && board[base-xSize+1].symbol!='_' && board[base-xSize-xSize-1].symbol!='_' && board[base-xSize-xSize].symbol!='_' && board[base-xSize-xSize+1].symbol!='_' &&
                                   board[base-xSize-1].symbol!='o' && board[base-xSize].symbol!='o' && board[base-xSize+1].symbol!='o' && board[base-xSize-xSize-1].symbol!='o' && board[base-xSize-xSize].symbol!='o' && board[base-xSize-xSize+1].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                                board[base-xSize].symbol='#';
                                base -= xSize;
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
                            while (board[base-xSize+1].symbol!='#' && board[base-xSize+2].symbol!='#' && board[base+1].symbol!='#' && board[base+2].symbol!='#' && board[base+xSize+1].symbol!='#' && board[base+xSize+2].symbol!='#' &&
                                   board[base-xSize+1].symbol!='_' && board[base-xSize+2].symbol!='_' && board[base+1].symbol!='_' && board[base+2].symbol!='_' && board[base+xSize+1].symbol!='_' && board[base+xSize+2].symbol!='_' &&
                                   board[base-xSize+1].symbol!='o' && board[base-xSize+2].symbol!='o' && board[base+1].symbol!='o' && board[base+2].symbol!='o' && board[base+xSize+1].symbol!='o' && board[base+xSize+2].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                                board[base+1].symbol='#';
                                base++;
                                k--;
                            }
                            break;
                        case 1: //sviluppo un ramo verso destra
                            while (board[base-xSize-1].symbol!='#' && board[base-xSize-2].symbol!='#' && board[base-1].symbol!='#' && board[base-2].symbol!='#' && board[base+xSize-1].symbol!='#' && board[base+xSize-2].symbol!='#' &&
                                   board[base-xSize-1].symbol!='_' && board[base-xSize-2].symbol!='_' && board[base-1].symbol!='_' && board[base-2].symbol!='_' && board[base+xSize-1].symbol!='_' && board[base+xSize-2].symbol!='_' &&
                                   board[base-xSize-1].symbol!='o' && board[base-xSize-2].symbol!='o' && board[base-1].symbol!='o' && board[base-2].symbol!='o' && board[base+xSize-1].symbol!='o' && board[base+xSize-2].symbol!='o' && k>0){
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

int createLabirint(Box* board, int * end, size_t xSize, size_t ySize, int difficulty){
    //printf("Sono entrato nella createLabirint\n");
    int start = randomStart(xSize, ySize); //genera uno start in posizione casuale
    //printf("Questo e' lo start: %d\n", start);
    //printf("Sono tornato dalla randomStart\n");
    board[start].symbol='o';
    //printf("Ho assegnato 'o' allo start\n");
    randomEnd(start, end, xSize, ySize); // genera un end in posizione casuale
    //printf("Sono tornato dalla randomEnd\n");
    board[*end].symbol='_';
    randomFlyingWalls(board, xSize, ySize, difficulty); // posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale
    //printf("Sono tornato dalla randomFlyingWalls\n");
    randomWalls(board, xSize, ySize, difficulty); //posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale
    //printf("Sono tornato dalla randomWalls\n");
    randomCoins(board, xSize, ySize); //posiziona un numero casuale di monete in posizioni casuali
    //printf("Sono tornato dalla randomCoins\n");
    randomBombs(board, xSize, ySize); //posiziona un numero casuale di imprevisti in posizioni casuali
    //printf("Sono tornato dalla randomBombs\n");
    randomDrill(board, xSize, ySize);
    return start;

}

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

void mode(){
    printf("In che modalita' vuoi giocare?\n");
    printf("1. Modalita' utente\n");
    printf("2. Modalita' AI random\n");
    printf("3. Modalita' AI avanzata\n");
}

void death(){ //il giocatore è morto
    printf("Purtroppo sei morto!");
    printf("\n");
    exit(0);
}

void success(int *score){ //il giocatore è giunto a destinazione
    printf("\n");
    printf("Congratulazioni! Sei arrivato a destinazione!\n");
    printf("Il tuo punteggio e': %d\n", *score);
}

void wrongInput(){ //il giocatore ha inserito in input non corretto
    printf("Input non corretto, reinserirlo.\n");
}

void nextMove(Box* board, int currentPosition, int nextPosition, int *score, int* coins, _Bool* endGame, int* drillPoints, size_t xSize, size_t ySize, vector_t* snake){
    (*score)--;
    if (nextPosition < 0 || nextPosition>=xSize*ySize){
        death();
    }
    else {
        if (board[nextPosition].symbol=='#') { //se la posizione sopra la posizione attuale è un muro o una parete
            if(*drillPoints>0){
                v_push_back(snake, nextPosition);
                board[v_get(snake, 0)].symbol = ' ';
                v_pop_front(snake);
                board[nextPosition].symbol = 'o';
                if (v_size(snake)!=1)
                    board[currentPosition].symbol = '.';
                (*drillPoints)--;
            }
            else{
                death();
                *endGame = 1;
            }
        }

        else if (board[nextPosition].symbol=='_') { //se ho completato il percorso
            v_push_back(snake, nextPosition);
            board[v_get(snake, 0)].symbol = ' ';
            v_pop_front(snake);
            board[nextPosition].symbol = 'o';
            if (v_size(snake)!=1)
                board[currentPosition].symbol = '.';
            *endGame = 1;
        }

        else{ //se la posizione sopra la posizione attuale è interna al labirinto

            if (board[nextPosition].symbol == ' '|| board[nextPosition].symbol == 'T' || v_size(snake) == 1) { // se è un trapano o uno spazio
                if (board[nextPosition].symbol == 'T')  // se è un trapano
                    *drillPoints += 3;
                v_push_back(snake, nextPosition);
                board[v_get(snake, 0)].symbol = ' ';
                if (board[nextPosition].symbol != '$')  // se non è una moneta
                    v_pop_front(snake);
                else {
                    *score += 10;
                    (*coins)++;
                }
            }

            else if(board[nextPosition].symbol == '!') { //se incontra un imprevisto
                int snakeLength = v_size(snake);
                int counter = snakeLength;
                //printf("prima di entrare nel ciclo:\ncounter = %d\nsnakeLength/2 = %d\n\n", counter, snakeLength/2);
                while(counter >= snakeLength/2){
                    //printf("counter = %d\nsnakeLength/2 = %d\n\n", counter, snakeLength/2);
                    board[v_get(snake, 0)].symbol = ' ';
                    v_pop_front(snake);
                    counter--;
                    //v_push_back(snake,nextPosition);
                }
                printf("\n");
                v_push_back(snake,nextPosition);
                //printf("dopo il ciclo:\ncounter = %d\nsnakeLength = %d\n", counter, snakeLength/2);
                *score -= (*coins) * 10 / 2; //toglie il punteggio
                *coins = *coins / 2; //dimezza numero di monete
            }

            else if(board[nextPosition].symbol == '.'){

                while (v_get(snake, 0)!= nextPosition){
                    board[v_get(snake, 0)].symbol = ' ';
                    v_pop_front(snake);
                    (*coins)--;
                    *score -=10;
                }
                v_pop_front(snake);
                v_push_back(snake,nextPosition);
            }

            else { // se incontra una moneta
                v_push_back(snake, nextPosition);
                *score += 10;
                (*coins)++;
            }

            board[nextPosition].symbol = 'o';
            if (v_size(snake)!=1)
                board[currentPosition].symbol = '.';

        }
    }
}


void moving (Box* board, int start, int *score, char* movesString, size_t xSize, size_t ySize, vector_t* snake){ //muove il giocatore
    char move;
    int currentPosition = start;
    int nextPosition;
    v_push_back(snake, start);
    v_print(snake);
    printLabirint(board, score, xSize, ySize);
    int* coins = (int*)malloc(sizeof(int)); //monete raccolte
    *coins=0;
    _Bool* endGame=(_Bool*)malloc(sizeof(_Bool));
    *endGame=0;
    int* drillPoints = (int*)malloc(sizeof(int));
    *drillPoints=0;
    int l = 0;
    do{
        scanf(" %c", &move);
        switch(move){
            case 'W':
            case 'w':
                if (currentPosition<xSize){
                    death();
                }
                nextPosition = currentPosition - xSize;
                movesString[l] = 'N';
                l++;
                nextMove(board, currentPosition, nextPosition, score, coins, endGame, drillPoints, xSize, ySize, snake);
                break;

            case 'S':
            case 's':
                if (currentPosition>(xSize*ySize)-xSize){
                    death();
                }
                nextPosition = currentPosition + xSize;
                movesString[l] = 'S';
                l++;
                nextMove(board, currentPosition, nextPosition, score, coins, endGame, drillPoints, xSize, ySize, snake);
                break;

            case 'D':
            case 'd':
                if (currentPosition%xSize == xSize-1){
                    death();
                }
                nextPosition = currentPosition + 1;
                movesString[l] = 'E';
                l++;
                nextMove(board, currentPosition, nextPosition, score, coins, endGame, drillPoints, xSize, ySize, snake);
                break;

            case 'A':
            case 'a':
                if (currentPosition%xSize == 0){
                    death();
                }
                nextPosition = currentPosition - 1;
                movesString[l] = 'O';
                l++;
                nextMove(board, currentPosition, nextPosition, score, coins, endGame, drillPoints, xSize, ySize, snake);
                break;

            default:
                wrongInput();
                break;
        }
        currentPosition = nextPosition; //nuova pos. attuale
        printLabirint(board, score, xSize, ySize);
    }while(!(*endGame));
    movesString[l]='\0';
    success(score);
    free(coins);
    free(endGame);
    free(drillPoints);
}

void randomAlgorithm(Box* board, int start, int *score, char* movesString, size_t xSize, size_t ySize, vector_t* snake){
    int currentPosition = start;
    printLabirint(board, score, xSize, ySize);
    int coins = 0; //monete raccolte
    _Bool endGame=0;
    do{
        int move = rand()%4;
        switch(move){
            case 0:
                if (board[currentPosition - xSize].symbol==' ' || board[currentPosition - xSize].symbol=='.' || board[currentPosition -xSize].symbol=='$' || board[currentPosition -xSize].symbol=='!'){ //se la posizione sopra la posizione attuale è parte del labirinto
                    board[currentPosition].symbol= '.';
                    if (board[currentPosition - xSize].symbol=='!'){ //se incontra un imprevisto
                        *score -= coins*10/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (board[currentPosition - xSize].symbol=='$'){ // se incontra una moneta
                        *score += 10;
                        coins ++;
                    }
                    else { //altrimenti
                        (*score)--;
                    }
                    board[currentPosition - xSize].symbol= 'o';
                    currentPosition = currentPosition - xSize; //nuova pos. attuale
                    //system("clear");
                    *movesString = 'N';
                    movesString++;
                    printLabirint(board, score, xSize, ySize);
                }
                else if (board[currentPosition - xSize].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol= '.';
                    board[currentPosition - xSize].symbol= 'o';
                    (*score)--;
                    //system("clear");
                    printLabirint(board, score, xSize, ySize);
                    success(score);
                    endGame = 1;
                }
                /* else { //se la posizione sotto la posizione attuale è un muro o una parete
                   death();
                   endGame = 1;
               }*/
                break;
            case 1:
                if (board[currentPosition +xSize].symbol==' ' || board[currentPosition + xSize].symbol=='.' || board[currentPosition +xSize].symbol=='$' || board[currentPosition +xSize].symbol=='!'){ //se la posizione sotto la posizione attuale è parte del labirinto
                    board[currentPosition].symbol='.';
                    if (board[currentPosition + xSize].symbol=='!'){ //se incontra un imprevisto
                        *score -= coins*10/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (board[currentPosition + xSize].symbol=='$'){ // se incontra una moneta
                        *score += 10;
                        coins ++;
                    }
                    else { //altrimenti
                        (*score)--;
                    }
                    board[currentPosition + xSize].symbol='o';
                    currentPosition = currentPosition + xSize; //nuova pos. attuale
                    //system("clear");
                    *movesString = 'S';
                    movesString++;
                    printLabirint(board, score, xSize, ySize);
                }
                else if (board[currentPosition + xSize].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition + xSize].symbol='o';
                    (*score)--;
                    //system("clear");
                    printLabirint(board, score, xSize, ySize);
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
                        *score -= coins*10/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (board[currentPosition + 1].symbol=='$'){ // se incontra una moneta
                        *score += 10;
                        coins ++;
                    }
                    else { //altrimenti
                        (*score)--;
                    }
                    board[currentPosition + 1].symbol='o';
                    currentPosition = currentPosition + 1; //nuova pos. attuale
                    *movesString = 'E';
                    movesString++;
                    //system("clear");
                    printLabirint(board, score, xSize, ySize);
                }
                else if (board[currentPosition + 1].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition + 1].symbol='o';
                    (*score)--;
                    //system("clear");
                    printLabirint(board, score, xSize, ySize);
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
                        *score -= coins*10/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (board[currentPosition - 1].symbol=='$'){ // se incontra una moneta
                        *score += 10;
                        coins ++;
                    }
                    else { //altrimenti
                        (*score)--;
                    }
                    board[currentPosition - 1].symbol='o';
                    currentPosition = currentPosition - 1; //nuova pos. attuale
                    //system("clear");
                    *movesString = 'O';
                    movesString++;
                    printLabirint(board, score, xSize, ySize);
                }
                else if (board[currentPosition - 1].symbol=='_') { //se ho completato il percorso
                    board[currentPosition].symbol='.';
                    board[currentPosition - 1].symbol='o';
                    (*score)--;
                    //system("clear");
                    printLabirint(board, score, xSize, ySize);
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

void visitNextNode(Box* board, int currentPos, int nextPos, int* end, size_t xSize, int* xDist, int* yDist, _Bool* arrived){
    board[nextPos].numCoins = board[currentPos].numCoins;
    board[nextPos].numDrills = board[currentPos].numDrills;
    if (board[nextPos].symbol == '#'){
        board[nextPos].numDrills -= 1;
    }
    board[nextPos].gCost = 1 + board[currentPos].gCost;
    *xDist = abs(((nextPos) % xSize) - *end % xSize);
    *yDist = abs(((nextPos) / xSize) - *end / xSize);
    board[nextPos].hCost = *yDist + *xDist;
    if (board[nextPos].symbol == '$'){
        board[nextPos].numCoins = board[currentPos].numCoins + 1;
    }
    else if (board[nextPos].symbol == '!'){
        board[nextPos].numCoins = (board[currentPos].numCoins)/2;
    }
    else if (board[nextPos].symbol == 'T'){
        board[nextPos].numDrills += 3;
    }
    else {
        board[nextPos].numCoins = board[currentPos].numCoins;
    }
    board[nextPos].predecessor = currentPos;
    board[nextPos].reference = nextPos;
    board[nextPos].visited = 1;
    if (board[nextPos].symbol == '_') {
        *arrived = 1;
    }
}

void visitFirstNode(Box* board, int start, int nextPos, int* end, size_t xSize, int* xDist, int* yDist){
    //printf("\n\nSono entrato nella visitFirstNode\n");
    board[nextPos].gCost = 1 + board[start].gCost;
    //printf("board[nextPos].gCost = %d\n", board[nextPos].gCost);
    *xDist = abs(((nextPos) % xSize) - *end % xSize);
    *yDist = abs(((nextPos) / xSize) - *end / xSize);
    board[nextPos].hCost = *yDist + *xDist;
    //printf("board[nextPos].hCost = %d\n", board[nextPos].hCost);
    board[nextPos].predecessor = start;
    board[nextPos].reference = nextPos;
    board[nextPos].visited = 1;
    if (board[nextPos].symbol == '$'){
        board[nextPos].numCoins = board[start].numCoins +1;
        board[nextPos].numDrills = board[start].numDrills;
    }
    else if (board[nextPos].symbol == 'T'){
        board[nextPos].numDrills += 3;
        board[nextPos].numCoins = board[start].numCoins;
    }
    else {
        board[nextPos].numDrills = board[start].numDrills;
        board[nextPos].numCoins = board[start].numCoins;
    }
}

void aStarAlgorithm(Box* board, int start, int* end, int* score, char* movesString, size_t xSize, size_t ySize, vector_t *snake){
    for (int i =0; i<xSize*ySize; i++){
        board[i].visited=0;
        board[i].extracted=0;
    }
    int *xDist = malloc(sizeof(int));
    int *yDist = malloc(sizeof(int));
    printLabirint(board, score, xSize, ySize);
    board[start].gCost = 0;
    *xDist = abs((start%xSize) - *end%xSize);
    printf("start%%xSize: %d\n", start%xSize);
    printf("end%%xSize: %d\n", *end%xSize);
    printf("*xDist: %d,\n", *xDist);
    *yDist = abs((start/xSize) - *end/xSize);
    printf("start/xSize: %d\n", start/xSize);
    printf("end/xSize: %d\n", *end/xSize);
    printf("*yDist: %d,\n", *yDist);
    board[start].hCost = *yDist + *xDist;
    board[start].reference = start;
    board[start].predecessor = -1;
    board[start].numCoins = 0;
    board[start].extracted = 1;
    board[start].visited = 1;
    board[start].numDrills = 0;

    Box candidates[xSize*ySize];
    candidates[0]=board[start];
    printf("candidates[0].symbol: %c,\ncandidates[0].predecessor: %d,\ncandidates[0].fCost: %d,\n",
           candidates[0].symbol, candidates[0].predecessor, candidates[0].gCost + candidates[0].hCost);
    short min;
    short i;
    short size = 1;
    _Bool *arrived = malloc(sizeof(_Bool));
    *arrived=0;
    int nextPos;
    if (start<xSize-1){ //se lo start è posizionato sulla parete superiore
        printf("Sono entrato nell'if basso\n");
        nextPos = start + xSize;
        printf("start = %d, nextPos = %d\n", start, nextPos);
        visitFirstNode(board, start, nextPos, end, xSize, xDist, yDist);
    }
    else if (start>((xSize*ySize)-xSize)){ //se lo start è posizionato sulla parete inferiore
        printf("Sono entrato nell'if alto\n");
        nextPos = start - xSize;
        printf("start = %d, nextPos = %d\n", start, nextPos);
        visitFirstNode(board, start, nextPos, end, xSize, xDist, yDist);
    }

    else if (start%xSize==0){ //se lo start è posizionato sulla parete sinistra
        printf("Sono entrato nell'if destro\n");
        nextPos = start + 1;
        printf("start = %d, nextPos = %d\n", start, nextPos);
        visitFirstNode(board, start, nextPos, end, xSize, xDist, yDist);
    }

    else { //se lo start è posizionato sulla parete destra
        printf("Sono entrato nell'if sinistro\n");
        nextPos = start - 1;
        printf("start = %d, nextPos = %d\n", start, nextPos);
        visitFirstNode(board, start, nextPos, end, xSize, xDist, yDist);
    }

    printf("Sono tornato dalla firstMove e nextPos = %d\n", nextPos);
    printf("board[nextPos].reference = %d\n", board[nextPos].reference);
    candidates[size] = board[nextPos];
    size++;
    candidates[0].extracted=1;

    while (!(*arrived)) {
        min = xSize*ySize;
        printf("\n");
        for (int j = 0; j < size; j++) {/*
             printf("\ncandidates[%d].extracted: %d\n",j, candidates[j].extracted);
             printf("  t: %d\n", candidates[j].reference);
             printf("  candidates[%d].predecessor: %d\n",j, candidates[j].predecessor);
             printf("  candidates[%d].numCoins: %d\n",j, candidates[j].numCoins);
             printf("  candidates[%d].hCost: %d\n",j, candidates[j].hCost);
             printf("  candidates[%d].gCost: %d\n",j, candidates[j].gCost);
             printf("  candidates[%d].fCost: %d\n",j, candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10);*/
             if (candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10 <= min && candidates[j].extracted==0) {
                min = candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10;
                //printf("    MINIMO\n");
                i = j;
             }
        }

        printf("\n\n");
        candidates[i].extracted=1;
        int t = candidates[i].reference; // t rappresenta la posizione nel labirinto della casella estratta tra i candidati
        board[t].predecessor=candidates[i].predecessor;
        board[t].hCost=candidates[i].hCost;
        board[t].gCost=candidates[i].gCost;
/*
            printf("\n\n!board[t - xSize].visited = %d,\nt>=xSize = %d\nboard[t - xSize].symbol = %c\nboard[t].predecessor != t -xSize = %d\n"
                    , !board[t - xSize].visited, t>=xSize,board[t - xSize].symbol, board[t].predecessor != t -xSize);*/


        if (!board[t - xSize].visited && t - xSize > 0 && (board[t].numDrills > 0 || board[t - xSize].symbol != '#') && board[t - xSize].symbol != 'o' && board[t].predecessor != t -xSize) { //casella alta se essa non è un muro e se non è il predecessore del nodo predecessore
            printf("Sono entrato nell'if alto\n");
            nextPos = t - xSize;
            visitNextNode(board, t, nextPos, end, xSize, xDist, yDist, arrived);
            candidates[size] = board[nextPos];
            size++;
        }
/*
          printf("\n\n!board[t +1 ].visited = %d,\nboard[t+1].symbol = %c\nboard[t].predecessor != t + 1 = %d\n"
                , !board[t +1].visited, board[t +1].symbol, board[t].predecessor != t +1);*/

        if (!board[t + 1].visited && (t + 1)%xSize != 0 && (board[t].numDrills > 0 || board[t + 1].symbol != '#') && board[t + 1].symbol != 'o' && board[t].predecessor != t +1) { //casella a destra se essa non è un muro e se non è il predecessore del nodo predecessore
            printf("Sono entrato nell'if destro\n");
            nextPos = t + 1;
            visitNextNode(board, t, nextPos, end, xSize, xDist, yDist, arrived);
            candidates[size] = board[nextPos];
            size++;
        }
/*
          printf("\n\n!board[t + xSize].visited = %d,\nt<xSize*ySize-xSize = %d\nboard[t + xSize].symbol = %c\nboard[t].predecessor != t +xSize = %d\n"
                  , !board[t + xSize].visited, t<xSize*ySize-xSize,board[t + xSize].symbol, board[t].predecessor != t +xSize);*/

        if (!board[t + xSize].visited && t<xSize*ySize && (board[t].numDrills > 0 || board[t + xSize].symbol != '#') && board[t + xSize].symbol != 'o' && board[t].predecessor != t +xSize) { //casella bassa se essa non è un muro e se non è il predecessore del nodo predecessore
            printf("Sono entrato nell'if basso\n");
            nextPos = t + xSize;
            visitNextNode(board, t, nextPos, end, xSize, xDist, yDist, arrived);
            candidates[size] = board[nextPos];
            size++;
        }
/*
           printf("\n\n!board[t -1 ].visited = %d,\nboard[t-1].symbol = %c\nboard[t].predecessor != t -1 = %d\n"
                 , !board[t -1].visited, board[t -1].symbol, board[t].predecessor != t -1);*/

        if (!board[t - 1].visited && (t - 1)%xSize != (xSize-1) && (board[t].numDrills > 0 || board[t - 1].symbol != '#') && board[t - 1].symbol != 'o' && board[t].predecessor != t -1) { //casella a sinistra se essa non è un muro e se non è il predecessore del nodo predecessore
            printf("Sono entrato nell'if sinistro\n");
            nextPos = t - 1;
            visitNextNode(board, t, nextPos, end, xSize, xDist, yDist, arrived);
            candidates[size] = board[nextPos];
            size++;
        }

        printf("   candidates[size].symbol: %c,\n   candidates[size].predecessor: %d,\n   candidates[size].numCoins: %d,\n"
            "   candidates[size].fCost: %d\n   candidates[size].reference: %d\n\n", candidates[size-1].symbol,
            candidates[size-1].predecessor, candidates[size-1].numCoins,
            candidates[size-1].gCost + candidates[size-1].hCost - candidates[size-1].numCoins*10, candidates[size-1].reference);
    }


    int* predecessors = (int*)malloc(sizeof(int)*xSize*ySize);//short predecessors[xSize*ySize]; //vettore che contiene tutti i predecessori
    int j = 0; // posizione nell'array predecessors
    int k = *end; // serve per scorrere a ritroso le posizioni del percorso

    while (k!=start){
        predecessors[j]=k;
        k=board[k].predecessor;
        j++;
    }
    predecessors[j]=k;

    /*printf("\nContenuto di predecessor[]:\n");
    for (int p=0; p<=j; p++) {
        printf("%d ", predecessors[p]);
    }*/

    short predecessorsSize = j;
    short path[j]; //vettore che contiene tutte le posizioni del labirinto facenti parte del percorso migliore
    for(i=0; i<=predecessorsSize; i++){
        path[i]=predecessors[j];
        j--;
    }

    /*printf("\nContenuto di path[]:\n");
    for (int i=0; i<=predecessorsSize; i++){
        printf("%d ", path[i]);
    }
    printf("\n");*/

    int l=0;

    for (int i =1; i<=predecessorsSize; i++){
        if (path[i]==path[i-1]-xSize)//se va verso l'alto
            movesString[l]='N';
        else if (path[i]==path[i-1]+1)//se va a destra
            movesString[l]='E';
        else if (path[i]==path[i-1]+xSize)//se va verso il basso
            movesString[l]='S';
        else
            movesString[l]='O';
        l++;
    }
    movesString[l]='\0';

    char move;
    int currentPosition;
    int nextPosition;
    v_push_back(snake, start);
    v_print(snake);
    printLabirint(board, score, xSize, ySize);
    int* coins = (int*)malloc(sizeof(int)); //monete raccolte
    *coins=0;
    _Bool* endGame=(_Bool*)malloc(sizeof(_Bool));
    *endGame=0;
    int* drillPoints = (int*)malloc(sizeof(int));
    *drillPoints=0;

    for (int i=0; i<predecessorsSize; i++){
        currentPosition = path[i];
        nextPosition = path[i+1];
        nextMove(board, currentPosition, nextPosition, score, coins, endGame, drillPoints, xSize, ySize, snake);
        printLabirint(board,score,xSize,ySize);
    }

    free(predecessors);
    free(arrived);
    free(coins);
    free(endGame);
    free(drillPoints);
}


int inputBoard(Box *board, int start, int* end){
    char* s = (char*)malloc(sizeof(char)*xSize); //stringa di input
    int i;
    for (i=0; i<ySize; i++){
        scanf(" %[^\n]s", s);
        int j;
        for(j=0;j<xSize;j++){
            board[i*xSize+j].symbol=s[j];
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
    return start;
}

void challenge(Box *board, int start, int* end, int *score, char* movesString, vector_t *snake){
    printf("MODALITA' SFIDA!\n");
    scanf(" %d", &xSize);
    scanf(" %d", &ySize);
    start = inputBoard(board, start, end);
    aStarAlgorithm(board, start, end, score, movesString, xSize, ySize, snake);
}

int main(int argc, char *argv[]){
    int* end = (int*)malloc(sizeof(int));
    Box board[xSize*ySize];
    char* moves = (char*) malloc(10000*sizeof(char)); //il vettore nel quale salverò la sequenza di mosse
    vector_t* snake = v_create();
    int* score=malloc(sizeof(int));
    *score = 1000;
    printf("\nBENVENUTO NELLO SNAKE DI LUCA E FRANCO!\n");
    if (argc==2 && strcmp(argv[1], "--challenge")==0){
        challenge(board, 0, end, score, moves, snake);
        printf("%s\n", moves);
        exit(0);
    }
    srand(time(NULL)); //funzione che determina il seme per la randomizzazione
    printf("\nPrima di cominciare inserire le dimensioni del labirinto.\n");

    _Bool wrongChar = 1;
    while (wrongChar){
        printf("Larghezza:");
        scanf(" %d", &xSize);
        if (xSize<3){
            printf("Dimensione troppo piccola, reinserirla.\n");
        }
        else if (xSize>40){
            printf("Dimensione troppo grande, reinserirla.\n");
        }
        else wrongChar=0;
    }
    wrongChar = 1;
    while (wrongChar){
        printf("Altezza:");
        scanf(" %d", &ySize);
        if (ySize<3){
            printf("Dimensione troppo piccola, reinserirla.\n");
        }
        else if (ySize>40){
            printf("Dimensione troppo grande, reinserirla.\n");
        }
        else wrongChar=0;
    }
    printf("\n");
    char choose;
    wrongChar = 1;
    printf("Cosa vuoi utilizzare?\n1. Un labirinto inserito da me\n2. Un labirinto generato automaticamente dal programma\n");
    int start;
    while (wrongChar){
        scanf(" %c", &choose);
        printf("\n");
        if(choose== '1') { //se l'utente vuole inserire il proprio labirinto
            printf("Inserisci il tuo labirinto:\n");
            start = inputBoard(board, start, end);
        }
        else if (choose== '2') { //se l'utente vuole utilizzare il labirinto generato dal programma
            printf("Scegli la difficolta' di gioco:\n1. Facile\n2. Media\n3. Difficile\n");
            unsigned short difficulty;//indice di difficoltà
            scanf(" %hd", &difficulty);
            createBoard(board, xSize, ySize);
            start = createLabirint(board, end, xSize, ySize, difficulty);
        }
        else {
            wrongInput();
        }

        printf("\n");
        mode();

        wrongChar = 1;
        while (wrongChar){
            scanf(" %c", &choose);
            printf("\n");
            if(choose== '1') {
                guide();
                moving(board, start, score, moves, xSize, ySize, snake);
                wrongChar=0;
            }
            else if (choose== '2'){
                randomAlgorithm(board, start, score, moves,xSize, ySize, snake);
                wrongChar=0;
            }

            else if(choose == '3'){
                aStarAlgorithm(board, start, end, score, moves, xSize, ySize, snake);
                wrongChar=0;
            }

            else {
                wrongInput();
            }
        }
        printf("\nSequenza di mosse effettuate per arrivare al traguardo:\n");
        printf("%s\n", moves);
    }

    return 0;
}