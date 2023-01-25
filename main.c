/**
 * @file main.c
 * @author Luca e Franco
 * @brief il codice seguente realizza lo snake 
 * @version 1.0
 * @date 2023-01-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "dvector.h"
/**
 * @brief Box data type
 * 
 */
struct box {
    /** Rappresenta uno dei caratteri presenti nella board */
    char symbol;
    /** Rappresenta il numero della casella in cui mi trovo */
    int reference;
    /** Nodo che nel percorso precede quello in cui mi trovo */
    short predecessor;
    /** Distanza dallo start */
    short gCost;
    /** Distanza euclidea dal traguardo */
    short hCost; 
    /** Rappresenta il numero di monete */
    short numCoins;
    /** Rappresenta il numero di trapani */
    short numDrills;
    /** Valore che indica se ho visitato i 4 nodi (sopra, sotto, destra e sinistra) */
    _Bool extracted;
    /** Valore che indica l'esistenza del nodo */
    _Bool visited;
};
/**
 * @brief typedef of Box data type
 * 
 */
typedef struct box Box;

/**
 * @brief Crea il campo di gioco vuoto con le pareti esterne
 * 
 * @param board Campo di gioco 
 * @param xSize Lunghezza
 * @param ySize Altezza
 */
void createBoard(Box* board, int xSize, int ySize){ 
    for(int i = 0; i < (xSize*ySize); i++){
        if ((i>=0 && i<(xSize)) || (i>=((xSize*ySize)-xSize) && i<(xSize*ySize)) || i%xSize==0 || i%xSize==(xSize-1)){ 
            board[i].symbol='#';
        }
        else {
            board[i].symbol=' ';
        }
    }
}
/**
 * @brief Genera lo start in posizione casuale
 * 
 * @param xSize Lunghezza
 * @param ySize Altezza
 * @return start 
 */
int randomStart(int xSize, int ySize){ 
    int start;
    short randomSide = rand()%4; /*sceglie a caso uno dei 4 lati*/
    switch(randomSide) {
        case 0: /*lato superiore*/
            start=(rand()%(xSize-2))+1; /*sceglie randomicamente un numero tra 1 e (xSize-2)*/
            break;
        case 1: /*lato inferiore*/
            start=rand()%(xSize-2) + 1 + ((xSize*ySize)-xSize); /*sceglie randomicamente un numero tra ((xSize*ySize)-xSize+1) e (xSize*ySize-1)*/
            break;
        case 2: /*lato sinistro*/
            start=((rand()%(ySize-2))*xSize)+xSize;/*sceglie randomicamente un numero tra xSize e (xSize*ySize)-xSize che modulo xSize dia 0*/
            break;
        case 3: /*lato destro*/
            start=((rand()%(ySize-2))*xSize)+xSize+xSize-1; /*sceglie randomicamente un numero tra xSize+xSize-1 e (xSize*ySize)-xSize-1 che modulo xSize dia (xSize-1)*/
            break;
        default:
            printf("Errore\n");
            break;
    }
    return start;
}

/**
 * @brief Genera l'end in posizione casuale
 * 
 * @param start Inizio labirinto 
 * @param end Fine labirinto 
 * @param xSize Lunghezza
 * @param ySize Altezza
 */
void randomEnd(int start, int* end, int xSize, int ySize){
    if (start<(xSize-1)){ /*se lo start è posizionato sulla parete superiore*/
        *end = (rand()%(xSize-2) + 1 + ((xSize*ySize)-xSize));
    }
    else if (start>((xSize*ySize)-xSize)){ /*se lo start è posizionato sulla parete inferiore*/
        *end=((rand()%(xSize-2))+1);
    }
    else if (start%xSize==0){ /*se lo start è posizionato sulla parete sinistra*/
        *end = (((rand()%(ySize-2))*xSize)+xSize+xSize-1);
    }
    else { /*se lo start è posizionato sulla parete destra*/
        *end=(((rand()%(ySize-2))*xSize)+xSize);
    }
}

/**
 * @brief stampa il labirinto e aggiorna il punteggio
 * 
 * @param board Campo di gioco
 * @param score Punteggio
 * @param xSize Larghezza
 * @param ySize Altezza
 * @return * void 
 */
void printLabirint(Box* board, int *score, int xSize, int ySize){
    for(int i = 0; i < ySize; i++){
        for(int j = 0; j < xSize; j++){
            printf("%c ", board[i*xSize+j].symbol);
        }
        printf("\n");
    }
    printf("\nPunteggio: %d\n", *score);
    printf(".....................................................................\n");
    printf("\n");
}
/**
 * @brief crea gli oggetti all'interno del labirinto
 * 
 * @param board Campo di gioco 
 * @param xSize Larghezza
 * @param ySize Altezza
 * @param numOfObjects Numero di oggetti 
 * @param objectType Tipo degli oggetti ($, !, T);
 */
void createObjects(Box *board, int xSize, int ySize, short numOfObjects, char objectType){
    int counter = xSize*ySize; /*fissa un limite di iterazioni del ciclo, per evitare il loop quando non c'è più spazio per posizionare oggetti*/
    short position = rand()%(xSize*ySize); /*trova una posizione casuale all'interno del labirinto in cui posizionare l'oggetto*/
    for(int i = 0; i < numOfObjects; i++){
        while (board[position].symbol!=' ' && counter>0){ /*finché non trova una posizione che sia libera*/
            position = rand()%(xSize*ySize); /*continua a cercare una posizione*/
            counter--;
        }
        if (board[position].symbol == ' ')
            board[position].symbol= objectType;
    }

}

/**
 * @brief Posiziona un numero casuale di monete in posizioni casuali ($)
 * 
 * @param board Campo di gioco 
 * @param xSize Larghezza
 * @param ySize Altezza
 */
void randomCoins(Box* board, int xSize, int ySize){ 
    char objectType = '$';
    short numOfCoins = rand()%(xSize*2); /*determina il numero di monete*/
    createObjects(board, xSize, ySize, numOfCoins, objectType);
}
/**
 * @brief Posiziona un numero casuale di trapani in posizioni casuali (T)
 * 
 * @param board Campo di gioco 
 * @param xSize Larghezza
 * @param ySize Altezza
 * @return * void 
 */
void randomDrill(Box* board,int xSize, int ySize){ 
    char objectType = 'T';
    short numOfDrill = rand()%xSize; /*determina il numero di trapani*/
    createObjects(board, xSize, ySize, numOfDrill, objectType);
}

/**
 * @brief Posiziona un numero casuale di imprevisti in posizioni casuali (!)
 * 
 * @param board Campo di gioco 
 * @param xSize Larghezza
 * @param ySize Altezza
 * @return * void 
 */
void randomBombs(Box* board, int xSize, int ySize) { 
    char objectType = '!';
    short numOfBombs = rand()%xSize; 
    createObjects(board, xSize, ySize, numOfBombs, objectType);
}

/**
 * @brief Posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale
 * 
 * @param board Campo di gioco
 * @param xSize Larghezza
 * @param ySize Altezza
 * @param difficulty Indice di difficoltà
 */
void randomFlyingWalls(Box *board, int xSize, int ySize, int difficulty) {
    int numOfWalls;
    int baseOfWall;
    if(difficulty == 1){
        numOfWalls = (rand()%(xSize/4))+2; /* determina randomicamente il numero di muri*/
    }
    else if(difficulty == 2){
        numOfWalls = (rand()%(xSize/2))+4;
    }
    else{
        numOfWalls = (rand()%xSize)+10;
    }
    for (int i = 0; i < numOfWalls; i++) {
        int wallLength = (rand() % xSize) +4; /*determina randomicamente la lunghezza del muro*/
        int counter = xSize*ySize*xSize; /*fissa un limite di iterazioni del ciclo, per evitare il loop quando non c'è più spazio per posizionare muri*/
        do {
            baseOfWall= rand() % (xSize * ySize); /*cerca un posto libero in cui costruire il muro*/
            counter--;
        } while ((board[baseOfWall].symbol!=' ' || board[baseOfWall+1].symbol=='#' || board[baseOfWall-1].symbol=='#' || board[baseOfWall+xSize].symbol=='#' || board[baseOfWall-xSize].symbol=='#'
                  || board[baseOfWall+xSize+1].symbol=='#' || board[baseOfWall+xSize-1].symbol=='#' || board[baseOfWall-xSize+1].symbol=='#' || board[baseOfWall-xSize-1].symbol=='#') && counter>0);
        if (counter>0){
            board[baseOfWall].symbol='#';
            short direction = rand()%4; /*determina randomicamente in quale direzione si svilupperà il muro*/
            switch(direction){
                case 0: /*basso*/
                    while (board[baseOfWall+xSize-1].symbol!='#' && board[baseOfWall+xSize].symbol!='#' && board[baseOfWall+xSize+1].symbol!='#' && board[baseOfWall+xSize+xSize-1].symbol!='#' && board[baseOfWall+xSize+xSize].symbol!='#' && board[baseOfWall+xSize+xSize+1].symbol!='#' &&
                           board[baseOfWall+xSize-1].symbol!='_' && board[baseOfWall+xSize].symbol!='_' && board[baseOfWall+xSize+1].symbol!='_' && board[baseOfWall+xSize+xSize-1].symbol!='_' && board[baseOfWall+xSize+xSize].symbol!='_' && board[baseOfWall+xSize+xSize+1].symbol!='_' &&
                           board[baseOfWall+xSize-1].symbol!='o' && board[baseOfWall+xSize].symbol!='o' && board[baseOfWall+xSize+1].symbol!='o' && board[baseOfWall+xSize+xSize-1].symbol!='o' && board[baseOfWall+xSize+xSize].symbol!='o' && board[baseOfWall+xSize+xSize+1].symbol!='o' && wallLength>0){
                        /*finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start*/
                        board[baseOfWall+xSize].symbol='#';
                        baseOfWall+= xSize;
                        wallLength--;
                    }
                    break;
                case 1: /*alto*/
                    while (board[baseOfWall-xSize-1].symbol!='#' && board[baseOfWall-xSize].symbol!='#' && board[baseOfWall-xSize+1].symbol!='#' && board[baseOfWall-xSize-xSize-1].symbol!='#' && board[baseOfWall-xSize-xSize].symbol!='#' && board[baseOfWall-xSize-xSize+1].symbol!='#' &&
                           board[baseOfWall-xSize-1].symbol!='_' && board[baseOfWall-xSize].symbol!='_' && board[baseOfWall-xSize+1].symbol!='_' && board[baseOfWall-xSize-xSize-1].symbol!='_' && board[baseOfWall-xSize-xSize].symbol!='_' && board[baseOfWall-xSize-xSize+1].symbol!='_' &&
                           board[baseOfWall-xSize-1].symbol!='o' && board[baseOfWall-xSize].symbol!='o' && board[baseOfWall-xSize+1].symbol!='o' && board[baseOfWall-xSize-xSize-1].symbol!='o' && board[baseOfWall-xSize-xSize].symbol!='o' && board[baseOfWall-xSize-xSize+1].symbol!='o' && wallLength>0){
                        /*finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start*/
                        board[baseOfWall-xSize].symbol='#';
                        baseOfWall-= xSize;
                        wallLength--;
                    }
                    break;
                case 2: /*destra*/
                    while (board[baseOfWall-xSize+1].symbol!='#' && board[baseOfWall-xSize+2].symbol!='#' && board[baseOfWall+1].symbol!='#' && board[baseOfWall+2].symbol!='#' && board[baseOfWall+xSize+1].symbol!='#' && board[baseOfWall+xSize+2].symbol!='#' &&
                           board[baseOfWall-xSize+1].symbol!='_' && board[baseOfWall-xSize+2].symbol!='_' && board[baseOfWall+1].symbol!='_' && board[baseOfWall+2].symbol!='_' && board[baseOfWall+xSize+1].symbol!='_' && board[baseOfWall+xSize+2].symbol!='_' &&
                           board[baseOfWall-xSize+1].symbol!='o' && board[baseOfWall-xSize+2].symbol!='o' && board[baseOfWall+1].symbol!='o' && board[baseOfWall+2].symbol!='o' && board[baseOfWall+xSize+1].symbol!='o' && board[baseOfWall+xSize+2].symbol!='o' && wallLength>0){
                        /*finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start*/
                        board[baseOfWall+1].symbol='#';
                        baseOfWall++;
                        wallLength--;
                    }
                    break;
                case 3: /*sinistra*/
                    while (board[baseOfWall-xSize-1].symbol!='#' && board[baseOfWall-xSize-2].symbol!='#' && board[baseOfWall-1].symbol!='#' && board[baseOfWall-2].symbol!='#' && board[baseOfWall+xSize-1].symbol!='#' && board[baseOfWall+xSize-2].symbol!='#' &&
                           board[baseOfWall-xSize-1].symbol!='_' && board[baseOfWall-xSize-2].symbol!='_' && board[baseOfWall-1].symbol!='_' && board[baseOfWall-2].symbol!='_' && board[baseOfWall+xSize-1].symbol!='_' && board[baseOfWall+xSize-2].symbol!='_' &&
                           board[baseOfWall-xSize-1].symbol!='o' && board[baseOfWall-xSize-2].symbol!='o' && board[baseOfWall-1].symbol!='o' && board[baseOfWall-2].symbol!='o' && board[baseOfWall+xSize-1].symbol!='o' && board[baseOfWall+xSize-2].symbol!='o' && wallLength>0){
                        /*finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start*/
                        board[baseOfWall-1].symbol='#';
                        baseOfWall--;
                        wallLength--;
                    }
                    break;
                default:
                    printf("Errore\n");
                    break;
            }
        }
    }
}


/**
 * @brief posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale. Questi muri partono da pareti o da muri già esistenti
 * 
 * @param board Campo di gioco
 * @param xSize Larghezza
 * @param ySize Altezza
 * @param difficulty Indice di difficoltà
 */
void randomWalls(Box *board, int xSize, int ySize, int difficulty){ 
    int numOfWalls;
    int baseOfWall;
    if(difficulty == 1){ /*determina randomicamente il numero di muri*/
        numOfWalls = (rand()%(xSize/2)+2);
    }
    else if(difficulty == 2){
        numOfWalls = (rand()%(xSize*2)+10);
    }
    else{
        numOfWalls = (rand()%(xSize*4)+30);
    }
    for (int i=0; i< numOfWalls; i++) {
        int wallLength=(rand()%xSize)+7; /*determina randomicamente la lunghezza del muro*/
        int counter = xSize*ySize*xSize; /*fissa un limite di iterazioni del ciclo, per evitare il loop quando non c'è più spazio per posizionare muri*/
        do {
            baseOfWall= rand()%(xSize*ySize); /*cerca un posto libero in cui costruire il muro*/
            counter--;
        } while ((board[baseOfWall].symbol!='#' || baseOfWall==0 ||  baseOfWall==(xSize-1) || baseOfWall==((xSize*ySize)-1) || baseOfWall==((xSize*ySize)-xSize+1)) && counter>0); //se la casella scelta e' un angolo o se non contiene '#'
        if (counter>0){
            if (baseOfWall<(xSize-1)){ /*se la casella scelta fa parte della parete superiore*/
                while (board[baseOfWall+xSize-1].symbol!='#' && board[baseOfWall+xSize].symbol!='#' && board[baseOfWall+xSize+1].symbol!='#' && board[baseOfWall+xSize+xSize-1].symbol!='#' && board[baseOfWall+xSize+xSize].symbol!='#' && board[baseOfWall+xSize+xSize+1].symbol!='#' &&
                       board[baseOfWall+xSize-1].symbol!='_' && board[baseOfWall+xSize].symbol!='_' && board[baseOfWall+xSize+1].symbol!='_' && board[baseOfWall+xSize+xSize-1].symbol!='_' && board[baseOfWall+xSize+xSize].symbol!='_' && board[baseOfWall+xSize+xSize+1].symbol!='_' &&
                       board[baseOfWall+xSize-1].symbol!='o' && board[baseOfWall+xSize].symbol!='o' && board[baseOfWall+xSize+1].symbol!='o' && board[baseOfWall+xSize+xSize-1].symbol!='o' && board[baseOfWall+xSize+xSize].symbol!='o' && board[baseOfWall+xSize+xSize+1].symbol!='o' && wallLength>0){
                    /*finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start*/
                    board[baseOfWall+xSize].symbol='#';
                    baseOfWall+= xSize;
                    wallLength--;
                }
            }
            else if (baseOfWall>((xSize*ySize)-xSize+1) && baseOfWall<((xSize*ySize)-1)){ /**se la casella scelta fa parte della parete inferiore*/
                while (board[baseOfWall-xSize-1].symbol!='#' && board[baseOfWall-xSize].symbol!='#' && board[baseOfWall-xSize+1].symbol!='#' && board[baseOfWall-xSize-xSize-1].symbol!='#' && board[baseOfWall-xSize-xSize].symbol!='#' && board[baseOfWall-xSize-xSize+1].symbol!='#' &&
                       board[baseOfWall-xSize-1].symbol!='_' && board[baseOfWall-xSize].symbol!='_' && board[baseOfWall-xSize+1].symbol!='_' && board[baseOfWall-xSize-xSize-1].symbol!='_' && board[baseOfWall-xSize-xSize].symbol!='_' && board[baseOfWall-xSize-xSize+1].symbol!='_' &&
                       board[baseOfWall-xSize-1].symbol!='o' && board[baseOfWall-xSize].symbol!='o' && board[baseOfWall-xSize+1].symbol!='o' && board[baseOfWall-xSize-xSize-1].symbol!='o' && board[baseOfWall-xSize-xSize].symbol!='o' && board[baseOfWall-xSize-xSize+1].symbol!='o' && wallLength>0){
                    /*finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start*/
                    board[baseOfWall-xSize].symbol='#';
                    baseOfWall-= xSize;
                    wallLength--;
                }
            }
            else if (baseOfWall%xSize==0){ /*se la casella scelta fa parte della parete sinistra*/
                while (board[baseOfWall-xSize+1].symbol!='#' && board[baseOfWall-xSize+2].symbol!='#' && board[baseOfWall+1].symbol!='#' && board[baseOfWall+2].symbol!='#' && board[baseOfWall+xSize+1].symbol!='#' && board[baseOfWall+xSize+2].symbol!='#' &&
                       board[baseOfWall-xSize+1].symbol!='_' && board[baseOfWall-xSize+2].symbol!='_' && board[baseOfWall+1].symbol!='_' && board[baseOfWall+2].symbol!='_' && board[baseOfWall+xSize+1].symbol!='_' && board[baseOfWall+xSize+2].symbol!='_' &&
                       board[baseOfWall-xSize+1].symbol!='o' && board[baseOfWall-xSize+2].symbol!='o' && board[baseOfWall+1].symbol!='o' && board[baseOfWall+2].symbol!='o' && board[baseOfWall+xSize+1].symbol!='o' && board[baseOfWall+xSize+2].symbol!='o' && wallLength>0){
                    /*finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start*/
                    board[baseOfWall+1].symbol='#';
                    baseOfWall++;
                    wallLength--;
                }
            }
            else if (baseOfWall%xSize==(xSize-1)){ /*se la casella scelta fa parte della parete destra*/
                while (board[baseOfWall-xSize-1].symbol!='#' && board[baseOfWall-xSize-2].symbol!='#' && board[baseOfWall-1].symbol!='#' && board[baseOfWall-2].symbol!='#' && board[baseOfWall+xSize-1].symbol!='#' && board[baseOfWall+xSize-2].symbol!='#' &&
                       board[baseOfWall-xSize-1].symbol!='_' && board[baseOfWall-xSize-2].symbol!='_' && board[baseOfWall-1].symbol!='_' && board[baseOfWall-2].symbol!='_' && board[baseOfWall+xSize-1].symbol!='_' && board[baseOfWall+xSize-2].symbol!='_' &&
                       board[baseOfWall-xSize-1].symbol!='o' && board[baseOfWall-xSize-2].symbol!='o' && board[baseOfWall-1].symbol!='o' && board[baseOfWall-2].symbol!='o' && board[baseOfWall+xSize-1].symbol!='o' && board[baseOfWall+xSize-2].symbol!='o' && wallLength>0){
                    /*finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start*/
                    board[baseOfWall-1].symbol='#';
                    baseOfWall--;
                    wallLength--;
                }
            }
            else { /*se la casella scelta fa parte di un muro inserito nei cicli precedenti*/
                wallLength += 5;
                if (board[baseOfWall+1].symbol=='#' || board[baseOfWall-1].symbol=='#'){ /*se è un muro orizzontale*/
                    short direction = rand()%2; /*scelgo un lato dal quale sviluppare un ulteriore ramo*/
                    switch(direction){
                        case 0: /*sviluppo un ramo verso il basso*/
                            while (board[baseOfWall+xSize-1].symbol!='#' && board[baseOfWall+xSize].symbol!='#' && board[baseOfWall+xSize+1].symbol!='#' && board[baseOfWall+xSize+xSize-1].symbol!='#' && board[baseOfWall+xSize+xSize].symbol!='#' && board[baseOfWall+xSize+xSize+1].symbol!='#' &&
                                   board[baseOfWall+xSize-1].symbol!='_' && board[baseOfWall+xSize].symbol!='_' && board[baseOfWall+xSize+1].symbol!='_' && board[baseOfWall+xSize+xSize-1].symbol!='_' && board[baseOfWall+xSize+xSize].symbol!='_' && board[baseOfWall+xSize+xSize+1].symbol!='_' &&
                                   board[baseOfWall+xSize-1].symbol!='o' && board[baseOfWall+xSize].symbol!='o' && board[baseOfWall+xSize+1].symbol!='o' && board[baseOfWall+xSize+xSize-1].symbol!='o' && board[baseOfWall+xSize+xSize].symbol!='o' && board[baseOfWall+xSize+xSize+1].symbol!='o' && wallLength>0){
                                /*finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start*/
                                board[baseOfWall+xSize].symbol='#';
                                baseOfWall+= xSize;
                                wallLength--;
                            }
                            break;
                        case 1: /*sviluppo un ramo verso l'alto*/
                            while (board[baseOfWall-xSize-1].symbol!='#' && board[baseOfWall-xSize].symbol!='#' && board[baseOfWall-xSize+1].symbol!='#' && board[baseOfWall-xSize-xSize-1].symbol!='#' && board[baseOfWall-xSize-xSize].symbol!='#' && board[baseOfWall-xSize-xSize+1].symbol!='#' &&
                                   board[baseOfWall-xSize-1].symbol!='_' && board[baseOfWall-xSize].symbol!='_' && board[baseOfWall-xSize+1].symbol!='_' && board[baseOfWall-xSize-xSize-1].symbol!='_' && board[baseOfWall-xSize-xSize].symbol!='_' && board[baseOfWall-xSize-xSize+1].symbol!='_' &&
                                   board[baseOfWall-xSize-1].symbol!='o' && board[baseOfWall-xSize].symbol!='o' && board[baseOfWall-xSize+1].symbol!='o' && board[baseOfWall-xSize-xSize-1].symbol!='o' && board[baseOfWall-xSize-xSize].symbol!='o' && board[baseOfWall-xSize-xSize+1].symbol!='o' && wallLength>0){
                                /*finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start*/
                                board[baseOfWall-xSize].symbol='#';
                                baseOfWall-= xSize;
                                wallLength--;
                            }
                            break;
                        default:
                            printf("Errore!\n");
                            break;
                    }
                }
                else{ /* se è un muro verticale*/
                    int direction = rand()%2; /*scelgo un lato dal quale sviluppare un ulteriore ramo*/
                    switch(direction){
                        case 0: /*sviluppo un ramo verso sinistra*/
                            while (board[baseOfWall-xSize+1].symbol!='#' && board[baseOfWall-xSize+2].symbol!='#' && board[baseOfWall+1].symbol!='#' && board[baseOfWall+2].symbol!='#' && board[baseOfWall+xSize+1].symbol!='#' && board[baseOfWall+xSize+2].symbol!='#' &&
                                   board[baseOfWall-xSize+1].symbol!='_' && board[baseOfWall-xSize+2].symbol!='_' && board[baseOfWall+1].symbol!='_' && board[baseOfWall+2].symbol!='_' && board[baseOfWall+xSize+1].symbol!='_' && board[baseOfWall+xSize+2].symbol!='_' &&
                                   board[baseOfWall-xSize+1].symbol!='o' && board[baseOfWall-xSize+2].symbol!='o' && board[baseOfWall+1].symbol!='o' && board[baseOfWall+2].symbol!='o' && board[baseOfWall+xSize+1].symbol!='o' && board[baseOfWall+xSize+2].symbol!='o' && wallLength>0){
                                /*finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start*/
                                board[baseOfWall+1].symbol='#';
                                baseOfWall++;
                                wallLength--;
                            }
                            break;
                        case 1: /*sviluppo un ramo verso destra*/
                            while (board[baseOfWall-xSize-1].symbol!='#' && board[baseOfWall-xSize-2].symbol!='#' && board[baseOfWall-1].symbol!='#' && board[baseOfWall-2].symbol!='#' && board[baseOfWall+xSize-1].symbol!='#' && board[baseOfWall+xSize-2].symbol!='#' &&
                                   board[baseOfWall-xSize-1].symbol!='_' && board[baseOfWall-xSize-2].symbol!='_' && board[baseOfWall-1].symbol!='_' && board[baseOfWall-2].symbol!='_' && board[baseOfWall+xSize-1].symbol!='_' && board[baseOfWall+xSize-2].symbol!='_' &&
                                   board[baseOfWall-xSize-1].symbol!='o' && board[baseOfWall-xSize-2].symbol!='o' && board[baseOfWall-1].symbol!='o' && board[baseOfWall-2].symbol!='o' && board[baseOfWall+xSize-1].symbol!='o' && board[baseOfWall+xSize-2].symbol!='o' && wallLength>0){
                                /*finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start*/
                                board[baseOfWall-1].symbol='#';
                                baseOfWall--;
                                wallLength--;
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
/**
 * @brief Create il labirinto completo
 * 
 * @param board Campo di gioco
 * @param end Fine labirinto
 * @param xSize Lunghezza
 * @param ySize Altezza
 * @param difficulty Indice di difficoltà
 * @return * int 
 */
int createLabirint(Box* board, int * end, int xSize, int ySize, int difficulty){ 
    int start = randomStart(xSize, ySize); /*genera uno start in posizione casuale*/
    board[start].symbol='o';
    randomEnd(start, end, xSize, ySize); /* genera un end in posizione casuale*/
    board[*end].symbol='_';
    randomFlyingWalls(board, xSize, ySize, difficulty); /*posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza e direzione casuale*/
    randomWalls(board, xSize, ySize, difficulty); /*posiziona un numero casuale di muri in posizioni casuali e di lunghezza e direzione casuale*/
    randomCoins(board, xSize, ySize); /*posiziona un numero casuale di monete in posizioni casuali*/
    randomBombs(board, xSize, ySize); /*posiziona un numero casuale di imprevisti in posizioni casuali*/
    randomDrill(board, xSize, ySize); /*posiziona un numero casuale di trapani in posizioni casuali*/
    return start;

}

/**
 * @brief Guida per l'utente
 * 
 */
void guide(){ 
    printf("\nSEI IN MODALITA' UTENTE.\n\n");
    printf("Per muoversi all'interno del labirinto utilizzare:\n'w' per muoversi verso l'alto\n"
           "'s' per muoversi verso il basso\n'a' per per muoversi verso sinistra\n'd' per muoversi verso destra\n\n");
    printf("Partendo dallo start (o) raggiungere il traguardo (_) con il punteggio piu' alto possibile. \n");
    printf("Ogni mossa costa un punto. \nOgni moneta ($) concede 10 punti extra.\n"
           "Ogni imprevisto (!) dimezza il numero di monete raccolte fino a quel momento.\n"
           "Ogni trapano (T) permette di perforare 3 muri.\n\n");
    printf("Buona partita! \n\n\n");
}

/**
 * @brief Permette di scegliere la modalità di gioco 
 * 
 */
void mode(){ 
    printf("In che modalita' vuoi giocare?\n");
    printf("1. Modalita' utente\n");
    printf("2. Modalita' AI random\n");
    printf("3. Modalita' AI avanzata\n");
}

/**
 * @brief Il giocatore è morto
 * 
 */
void death(){ 
    printf("Purtroppo sei morto!");
    printf("\n");
    exit(0);
}

/**
 * @brief Il giocatore è giunto al traguardo e viene stampato il punteggio
 * 
 * @param score Punteggio
 */
void success(int *score){ 
    printf("\n");
    printf("Congratulazioni! Sei arrivato a destinazione!\n");
    printf("Il tuo punteggio e': %d\n", *score);
}

/**
 * @brief Il giocatore ha inserito un input corretto
 * 
 */
void wrongInput(){ 
    printf("Input non corretto, reinserirlo.\n");
}

/**
 * @brief Prossima mossa della moving che stabilisce in base all'oggetto che l'utente incontra l'allungamento e l'accorciamento del serpente e l'incrementare o il decrementare del punteggio o la morte
 * 
 * @param board Campo di gioco
 * @param currentPosition Posizione corrente 
 * @param nextPosition Posizione successiva
 * @param score Punteggio
 * @param coins Monete
 * @param endGame Fine del gioco
 * @param drillPoints Trapano
 * @param xSize Larghezza
 * @param ySize Altezza
 * @param snake Vettore che memorizza le posizioni del campo di gioco in cui si trova snake
 */
void nextMove(Box* board, int currentPosition, int nextPosition, int *score, int* coins, _Bool* endGame, int* drillPoints, int xSize, int ySize, vector_t* snake){ 
    (*score)--;
    if (nextPosition < 0 || nextPosition>=xSize*ySize){ /*se esce dalla parete superiore o inferiore*/
        death();
    }
    else {
        if (board[nextPosition].symbol=='#') { /*se la nextPosition è un muro o una parete*/
            if(*drillPoints>0){ /*se ho drillPoints*/
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
        else if (board[nextPosition].symbol=='_') { /*se sono arrivato al traguardo*/
            v_push_back(snake, nextPosition);
            board[v_get(snake, 0)].symbol = ' ';
            v_pop_front(snake);
            board[nextPosition].symbol = 'o';
            if (v_size(snake)!=1)
                board[currentPosition].symbol = '.';
            *endGame = 1;
        }
        else{ /*se la nextPosition non è un muro*/
            if (board[nextPosition].symbol == ' '|| board[nextPosition].symbol == 'T' || v_size(snake) == 1) { /*se è un trapano o uno spazio o se la lunghezza del serpente è 1*/
                if (board[nextPosition].symbol == 'T')  /* se è un trapano*/
                    *drillPoints += 3;
                v_push_back(snake, nextPosition);
                board[v_get(snake, 0)].symbol = ' ';
                if (board[nextPosition].symbol != '$')  /* se non è una moneta*/
                    v_pop_front(snake);
                else { /*se è una moneta*/
                    *score += 10;
                    (*coins)++;
                }
            }
            else if(board[nextPosition].symbol == '!') { /*se è un imprevisto*/
                int snakeLength = v_size(snake);
                int counter = snakeLength;
                while(counter >= snakeLength/2){
                    board[v_get(snake, 0)].symbol = ' ';
                    v_pop_front(snake);
                    counter--;
                }
                v_push_back(snake,nextPosition);
                *score -= (*coins) * 10 / 2; /*toglie il punteggio*/
                *coins = *coins / 2; /*dimezza il numero di monete*/
            }
            else if(board[nextPosition].symbol == '.'){ /*se è un pezzo del serpente stesso*/
                while (v_get(snake, 0)!= nextPosition){
                    board[v_get(snake, 0)].symbol = ' ';
                    v_pop_front(snake);
                    (*coins)--;
                    *score -=10;
                }
                v_pop_front(snake);
                v_push_back(snake,nextPosition);
            }
            else { /**se è una moneta*/
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

/**
 * @brief Muove il serpente
 * 
 * @param board Campo di gioco
 * @param start Inizio labirinto
 * @param score Punteggio
 * @param movesString Array che memorizza le mosse dell'utente
 * @param xSize Larghezza
 * @param ySize Altezza
 * @param snake Vettore che memorizza le posizioni del campo di gioco in cui si trova snake
 */
void moving (Box* board, int start, int *score, char* movesString, int xSize, int ySize, vector_t* snake){ //muove il serpente
    char move;
    int currentPosition = start;
    int nextPosition;
    v_push_back(snake, start);
    printLabirint(board, score, xSize, ySize);
    int* coins = (int*)malloc(sizeof(int)); //monete raccolte
    *coins=0;
    _Bool* endGame=(_Bool*)malloc(sizeof(_Bool));
    *endGame=0;
    int* drillPoints = (int*)malloc(sizeof(int));
    *drillPoints=0;
    int l = 0; //scorre l'array di mosse movesString
    do{
        scanf(" %c", &move);

        switch(move){
            case 'W': //se il giocatore va verso l'alto
            case 'w':
                if (currentPosition<xSize){
                    death();
                }
                nextPosition = currentPosition - xSize;
                movesString[l] = 'N';
                l++;
                nextMove(board, currentPosition, nextPosition, score, coins, endGame, drillPoints, xSize, ySize, snake);
                break;

            case 'S': //se il giocatore va verso il basso
            case 's':
                if (currentPosition>(xSize*ySize)-xSize){
                    death();
                }
                nextPosition = currentPosition + xSize;
                movesString[l] = 'S';
                l++;
                nextMove(board, currentPosition, nextPosition, score, coins, endGame, drillPoints, xSize, ySize, snake);
                break;

            case 'D': //se il giocatore va a destra
            case 'd':
                if (currentPosition%xSize == xSize-1){
                    death();
                }
                nextPosition = currentPosition + 1;
                movesString[l] = 'E';
                l++;
                nextMove(board, currentPosition, nextPosition, score, coins, endGame, drillPoints, xSize, ySize, snake);
                break;

            case 'A': //se il giocatore va a sinistra
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
    movesString = realloc(movesString, l);
}

/**
 * @brief Algoritmo che muove randomicamente il serpente fino alla fine
 * 
 * @param board Campo di gioco
 * @param start Inizio labirinto
 * @param score Punteggio 
 * @param movesString Array che memorizza le mosse dell'utente
 * @param xSize Larghezza
 * @param ySize Altezza
 * @param snake Vettore che memorizza le posizioni del campo di gioco in cui si trova lo snake
 */
void randomAlgorithm(Box* board, int start, int* score, char* movesString, int xSize, int ySize, vector_t* snake) {
    int currentPosition = start;
    v_push_back(snake, start);
    int coins = 0; 
    _Bool endGame = 0;
    int drillPoints = 0;
    int l = 0; 
    printLabirint(board, score, xSize, ySize);
    /*inizializza gli array di movimenti e direzioni*/
    int moves[] = {-xSize, xSize, -1, 1};
    int directions[] = {'N', 'S', 'O', 'E'};
    

    /*esegui il movimento finché non si raggiunge la fine del gioco*/
    for (int i = 0; !endGame; i++) {
        int move = rand() % 4; /*Scegli una mossa a caso*/
        int nextPosition = currentPosition + moves[move]; /*Calcola la prossima posizione*/

        /*verifica se la mossa è valida e esegui il movimento*/
        if (board[nextPosition].symbol != '#' && nextPosition > 0 && nextPosition < xSize * ySize) {
            movesString[l] = directions[move];
            l++;
            nextMove(board, currentPosition, nextPosition, score, &coins, &endGame, &drillPoints, xSize, ySize, snake);
            currentPosition = nextPosition; /*aggiorna la posizione corrente*/
            printLabirint(board, score, xSize, ySize);
        }
    }
    movesString[l]='\0';
    movesString = realloc(movesString, l);

}

/**
 * @brief Realizza la visita di un nodo del labirinto
 * 
 * @param board Campo di gioco 
 * @param currentPos Posizione corrente
 * @param nextPos Posizione successiva
 * @param end Fine labirinto
 * @param xSize Larghezza
 * @param xDist Distanza delle x dallo start
 * @param yDist Distanza delle y dallo start
 * @param arrived Indica la fine del percorso
 */
void visitNextNode(Box* board, int currentPos, int nextPos, int* end, int xSize, int* xDist, int* yDist, _Bool* arrived){ 
    board[nextPos].numCoins = board[currentPos].numCoins;
    board[nextPos].numDrills = board[currentPos].numDrills;
    if (board[nextPos].symbol == '#'){ /*se è un muro o una parete*/
        board[nextPos].numDrills -= 1;
    }
    board[nextPos].gCost = 1 + board[currentPos].gCost; /*aggiorna il gCost*/
    *xDist = abs(((nextPos) % xSize) - *end % xSize);
    *yDist = abs(((nextPos) / xSize) - *end / xSize);
    board[nextPos].hCost = *yDist + *xDist; /*aggiorna l'hCost*/
    if (board[nextPos].symbol == '$'){ /*se è una moneta*/
        board[nextPos].numCoins = board[currentPos].numCoins + 1;
    }
    else if (board[nextPos].symbol == '!'){ /*se è un imprevisto*/
        board[nextPos].numCoins = (board[currentPos].numCoins)/2;
    }
    else if (board[nextPos].symbol == 'T'){ /*se è un trapano*/
        board[nextPos].numDrills += 3;
    }
    board[nextPos].predecessor = currentPos; /*assegna il nodo predecessore*/
    board[nextPos].reference = nextPos;
    board[nextPos].visited = 1;
    if (board[nextPos].symbol == '_') { /*se è l'arrivo*/
        *arrived = 1;
    }
}

/**
 * @brief Realizza la visita del primo nodo dopo lo start
 * 
 * @param board Campo di gioco 
 * @param start Inizio labirinto
 * @param nextPos Posizione successiva
 * @param end Fine labirinto
 * @param xSize Larghezza
 * @param xDist Distanza delle x dallo start
 * @param yDist Distanza delle y dallo start
 */
void visitFirstNode(Box* board, int start, int nextPos, int* end, int xSize, int* xDist, int* yDist){ 
    board[nextPos].numDrills = board[start].numDrills;
    board[nextPos].numCoins = board[start].numCoins;
    board[nextPos].gCost = 1 + board[start].gCost; /*aggiorna il gCost (distanza dallo start)*/
    *xDist = abs(((nextPos) % xSize) - *end % xSize);
    *yDist = abs(((nextPos) / xSize) - *end / xSize);
    board[nextPos].hCost = *yDist + *xDist; /*aggiorna l'hCost */
    board[nextPos].predecessor = start; /*assegna come nodo predecessore lo start*/
    board[nextPos].reference = nextPos;
    board[nextPos].visited = 1;
    if (board[nextPos].symbol == '$'){ /*se è una moneta*/
        board[nextPos].numCoins = board[start].numCoins +1;
    }
    else if (board[nextPos].symbol == 'T'){ /*se è un trapano*/
        board[nextPos].numDrills += 3;
    }
}

/**
 * @brief Inizializza tutti i nodi come non visitati e non esplorati
 * 
 * @param board Campo di gioco
 * @param start Inizio labirinto
 * @param end Fine labirinto
 * @param score Punteggio
 * @param movesString Array che memorizza le mosse dell'utente
 * @param xSize Larghezza
 * @param ySize Altezza
 * @param snake Vettore che memorizza le posizioni del campo di gioco in cui si trova lo snake 
 */
void aStarAlgorithm(Box* board, int start, int* end, int* score, char* movesString, int xSize, int ySize, vector_t *snake){
    for (int i =0; i<xSize*ySize; i++){ 
        board[i].visited=0;
        board[i].extracted=0;
    }
    int *xDist = malloc(sizeof(int));
    int *yDist = malloc(sizeof(int));
    board[start].gCost = 0; /*aggiorna il gCost*/
    *xDist = abs((start%xSize) - *end%xSize);
    *yDist = abs((start/xSize) - *end/xSize);
    board[start].hCost = *yDist + *xDist; /*aggiorna l'hCost*/
    board[start].reference = start;
    board[start].predecessor = -1;
    board[start].numCoins = 0;
    board[start].extracted = 1;
    board[start].visited = 1;
    board[start].numDrills = 0;
    Box candidates[xSize*ySize]; /*vettore che conterrà tutti i nodi visitati*/
    candidates[0]=board[start];
    short size = 1; /*permette di scorrere candidates*/
    short min;
    short i;
    _Bool *arrived = malloc(sizeof(_Bool));
    *arrived=0;
    int nextPos;
    
    if (start<xSize-1){ /*se lo start è posizionato sulla parete superiore*/
        nextPos = start + xSize;
        visitFirstNode(board, start, nextPos, end, xSize, xDist, yDist);
    }
    else if (start>((xSize*ySize)-xSize)){ /*se lo start è posizionato sulla parete inferiore*/
        nextPos = start - xSize;
        visitFirstNode(board, start, nextPos, end, xSize, xDist, yDist);
    }
    else if (start%xSize==0){ /*se lo start è posizionato sulla parete sinistra*/
        nextPos = start + 1;
        visitFirstNode(board, start, nextPos, end, xSize, xDist, yDist);
    }
    else { /*se lo start è posizionato sulla parete destra*/
        nextPos = start - 1;
        visitFirstNode(board, start, nextPos, end, xSize, xDist, yDist);
    }
    candidates[size] = board[nextPos];
    size++;
    candidates[0].extracted=1;

    while (!(*arrived)) {
        min = xSize*ySize;
        for (int j = 0; j < size; j++) {/*scorre candidates per trovare il nodo con fCost minore che non è ancora stato estratto*/
            if (candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10 <= min && candidates[j].extracted==0) {
                min = candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10;         
                i = j;
             }
        }
        candidates[i].extracted=1;
        int extractedPosition = candidates[i].reference; /*extractedPosition rappresenta la posizione nel labirinto della casella estratta tra i candidati*/
        board[extractedPosition].predecessor=candidates[i].predecessor;
        board[extractedPosition].hCost=candidates[i].hCost;
        board[extractedPosition].gCost=candidates[i].gCost;

        if (!board[extractedPosition - xSize].visited && extractedPosition - xSize > 0 && (board[extractedPosition].numDrills > 0 || board[extractedPosition - xSize].symbol != '#') && board[extractedPosition - xSize].symbol != 'o' && board[extractedPosition].predecessor != extractedPosition -xSize) { //casella alta se essa non è un muro e se non è il predecessore del nodo in questione
            nextPos = extractedPosition - xSize;
            visitNextNode(board, extractedPosition, nextPos, end, xSize, xDist, yDist, arrived);
            candidates[size] = board[nextPos];
            size++;
        }
        if (!board[extractedPosition + 1].visited && (extractedPosition + 1)%xSize != 0 && (board[extractedPosition].numDrills > 0 || board[extractedPosition + 1].symbol != '#') && board[extractedPosition + 1].symbol != 'o' && board[extractedPosition].predecessor != extractedPosition +1) { //casella a destra se essa non è un muro e se non è il predecessore del nodo in questione
            nextPos = extractedPosition + 1;
            visitNextNode(board, extractedPosition, nextPos, end, xSize, xDist, yDist, arrived);
            candidates[size] = board[nextPos];
            size++;
        }
        if (!board[extractedPosition + xSize].visited && extractedPosition<xSize*ySize && (board[extractedPosition].numDrills > 0 || board[extractedPosition + xSize].symbol != '#') && board[extractedPosition + xSize].symbol != 'o' && board[extractedPosition].predecessor != extractedPosition +xSize) { //casella bassa se essa non è un muro e se non è il predecessore del nodo in questione
            nextPos = extractedPosition + xSize;
            visitNextNode(board, extractedPosition, nextPos, end, xSize, xDist, yDist, arrived);
            candidates[size] = board[nextPos];
            size++;
        }
        if (!board[extractedPosition - 1].visited && (extractedPosition - 1)%xSize != (xSize-1) && (board[extractedPosition].numDrills > 0 || board[extractedPosition - 1].symbol != '#') && board[extractedPosition - 1].symbol != 'o' && board[extractedPosition].predecessor != extractedPosition -1) { //casella a sinistra se essa non è un muro e se non è il predecessore del nodo in questione
            nextPos = extractedPosition - 1;
            visitNextNode(board, extractedPosition, nextPos, end, xSize, xDist, yDist, arrived);
            candidates[size] = board[nextPos];
            size++;
        }
    }
     /*vettore che contiene tutti i predecessori*/
    int* predecessors = (int*)malloc(sizeof(int)*xSize*ySize);
    int j = 0; /* posizione nell'array predecessors*/
    int k = *end; /*k serve per scorrere a ritroso le posizioni del percorso*/

    while (k!=start){
        predecessors[j]=k;
        k=board[k].predecessor;
        j++;
    }
    predecessors[j]=k;

    short predecessorsSize = j;
    short path[j]; /*vettore che contiene tutte le posizioni del labirinto facenti parte del percorso migliore, dallo start al traguardo*/
    for(i=0; i<=predecessorsSize; i++){
        path[i]=predecessors[j];
        j--;
    }

    int l=0; /*permettere di scorrere il vettore movesString*/
    for (int i =1; i<=predecessorsSize; i++){
        if (path[i]==path[i-1]-xSize)/*se va verso l'alto*/
            movesString[l]='N';
        else if (path[i]==path[i-1]+1)/**se va a destra*/
            movesString[l]='E';
        else if (path[i]==path[i-1]+xSize)/*se va verso il basso*/
            movesString[l]='S';
        else /*se va a sinistra*/
            movesString[l]='O';
        l++;
    }
    movesString[l]='\0'; /*carattere di terminazione*/

    char move;
    int currentPosition;
    int nextPosition;
    v_push_back(snake, start);
    printLabirint(board, score, xSize, ySize);
    int* coins = (int*)malloc(sizeof(int)); /*monete raccolte*/
    *coins=0;
    _Bool* endGame=(_Bool*)malloc(sizeof(_Bool));
    *endGame=0;
    int* drillPoints = (int*)malloc(sizeof(int));
    *drillPoints=0;

    for (int i=0; i<predecessorsSize; i++){ /*crea il serpente e mostra la sua evoluzione all'interno del percorso*/
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
    movesString = realloc(movesString, l);
}

/**
 * @brief Permette di prendere in input un labirinto 
 * 
 * @param board Campo di gioco
 * @param start Inizio labirinto
 * @param end Fine labirinto
 * @param xSize Larghezza
 * @param ySize Altezza
 * @return start
 */
int inputBoard(Box *board, int start, int* end, int xSize, int ySize){ 
    char* string = (char*)malloc(sizeof(char)*xSize); /*stringa di input*/
    int i;
    for (i=0; i<ySize; i++){
        scanf(" %[^\n]s", string);
        int j;
        for(j=0;j<xSize;j++){
            board[i*xSize+j].symbol=string[j];
        }
    }
    free(string);

    _Bool found =0;
    i=0;
    while (!found){ /*cerco lo start*/
        if (board[i].symbol=='o'){
            start = i;
            found =1;
        }
        i++;
    }

    found = 0;
    i=0;
    while (!found){ /*cerco l'end*/
        if (board[i].symbol=='_'){
            *end = i;
            found =1;
        }
        i++;
    }
    return start;
}

/**
 * @brief permette di svolgere le challenge assegnate dal professore durante il corso
 * 
 * @param board 
 * @param start 
 * @param end 
 * @param score 
 * @param xSize 
 * @param ySize 
 * @param movesString 
 * @param snake 
 */
void challenge(Box *board, int start, int* end, int *score, int xSize, int ySize, char* movesString, vector_t *snake){ 
    printf("MODALITA' SFIDA!\n");
    scanf(" %d", &xSize);
    scanf(" %d", &ySize);
    start = inputBoard(board, start, end, xSize, ySize);
    aStarAlgorithm(board, start, end, score, movesString, xSize, ySize, snake);
}

/**
 * @brief main
 * 
 * @param argc 
 * @param argv 
 * @return 0
 */
int main(int argc, char *argv[]){
    int* end = (int*)malloc(sizeof(int));
    char* moves = (char*) malloc(10000*sizeof(char)); /*il vettore nel quale salverò la sequenza di mosse*/
    vector_t* snake = v_create();
    int* score=malloc(sizeof(int));
    *score = 1000;
    printf("\nBENVENUTO NELLO SNAKE DI LUCA E FRANCO!\n");
    int xSize, ySize;

    srand(time(NULL)); /*funzione che determina il seme per la randomizzazione*/
    printf("\nPrima di cominciare inserire le dimensioni del labirinto.\n");
    _Bool wrongChar = 1;
    while (wrongChar){ /*finché l'input dato è fuori dal range 3-40*/
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
    while (wrongChar){ /*finché l'input dato è fuori dal range 3-40*/
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
    //Box board[xSize*ySize];
    Box* board = (Box*) malloc(sizeof(Box)*xSize*ySize);
    wrongChar = 1;
    printf("Cosa vuoi utilizzare?\n1. Un labirinto inserito da me\n2. Un labirinto generato automaticamente dal programma\n");
    int start; 
    char choose;
    while (wrongChar){
        scanf(" %c", &choose);
        printf("\n");
        if(choose== '1') { /*se l'utente vuole inserire il proprio labirinto*/
            printf("Inserisci il tuo labirinto:\n");
            start = inputBoard(board, start, end, xSize, ySize);
        }
        else if (choose== '2') { /*se l'utente vuole utilizzare il labirinto generato dal programma*/
            printf("Scegli la difficolta' di gioco:\n1. Facile\n2. Media\n3. Difficile\n");
            unsigned short difficulty;
            scanf(" %hd", &difficulty);
            createBoard(board, xSize, ySize);
            start = createLabirint(board, end, xSize, ySize, difficulty);
        }
        else {
            wrongInput();
        }

        printf("\n");
        mode(); /*chiede all'utente in che modalità vuole giocare*/
        wrongChar = 1;
        while (wrongChar){
            scanf(" %c", &choose);
            printf("\n");
            if(choose== '1') { /*se vuole utilizzare la modalità utente*/
                guide();
                moving(board, start, score, moves, xSize, ySize, snake);
                wrongChar=0;
            }
            else if (choose== '2'){ /*se vuole utilizzare l'AI randomico*/
                randomAlgorithm(board, start, score, moves,xSize, ySize, snake);
                wrongChar=0;
            }
            else if(choose == '3'){ /*se vuole utilizzare l'AI avanzato*/
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
    v_free(snake);
    return 0;
}