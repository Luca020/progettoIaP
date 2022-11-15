//idee e cose da fare:
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
#include <math.h>
//#include "aiAlgorithms.h"

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

short unsigned xSide = 15u; //lunghezza del campo di gioco
short unsigned ySide = 15u; //altezza del campo di gioco



void createBoard(Box* b){ //crea il campo di gioco con pareti e spazi vuoti
    for(int i = 0; i < (xSide*ySide); i++){
        //b[i].visited=0;
        if ((i>=0 && i<(xSide)) || (i>=((xSide*xSide)-xSide) && i<(xSide*xSide)) || i%xSide==0 || i%xSide==(xSide-1)){
            b[i].symbol='#';
        }
        else {
            b[i].symbol=' ';
        }
    }
}

int randomStart(){ //genera uno start in posizione casuale
    int start;
    short randomSide = rand()%4; //sceglie a caso uno dei 4 lati
    switch(randomSide) {
        case 0: //lato superiore
            start=(rand()%(xSide-2))+1; //sceglie randomicamente un numero tra 1 e (xSide-2)
            break;
        case 1: //lato inferiore
            start=rand()%(xSide-2) + 1 + ((xSide*xSide)-xSide); //sceglie randomicamente un numero tra ((xSide*xSide)-xSide+1) e (xSide*xSide-1)
            break;
        case 2: //lato sinistro
            start=((rand()%(xSide-2))*xSide)+xSide;//sceglie randomicamente un numero tra xSide e (xSide*xSide)-xSide che modulo xSide dia 0
            break;
        case 3: //lato destro
            start=((rand()%(xSide-2))*xSide)+xSide+xSide-1; //sceglie randomicamente un numero tra xSide+xSide-1 e (xSide*xSide)-xSide-1 che modulo xSide dia (xSide-1)
            break;
        default:
            printf("Errore\n");
            break;
    }
    return start;
}

int randomEnd(int start){ // genera un end in posizione casuale in una parete diversa da quella dello start
    int end;
    int dir[3];
    if (start<(xSide-1)){ //se lo start è posizionato sulla parete superiore
        dir[0]=1;
        dir[1]=2;
        dir[2]=3;
    }
    else if (start>((xSide*xSide)-xSide)){ //se lo start è posizionato sulla parete inferiore
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
            end=(rand()%(xSide-2))+1; //sceglie randomicamente un numero tra 1 e (xSide-2)
            break;
        case 1: //lato inferiore
            end=rand()%(xSide-2) + 1 + ((xSide*xSide)-xSide); //sceglie randomicamente un numero tra ((xSide*xSide)-xSide+1) e (xSide*xSide-1)
            break;
        case 2: //lato sinistro
            end=((rand()%(xSide-2))*xSide)+xSide;//sceglie randomicamente un numero tra xSide e (xSide*xSide)-xSide che modulo xSide dia 0
            break;
        case 3: //lato destro
            end=((rand()%(xSide-2))*xSide)+xSide+xSide-1; //sceglie randomicamente un numero tra xSide+xSide-1 e (xSide*xSide)-xSide-1 che modulo xSide dia (xSide-1)
            break;
        default:
            printf("Errore\n");
            break;
    }
    return end;
}

void printLabirint(Box* b, int score){ //stampa il labirinto
    printf("Sono entrato nella printLabirint\n");
    for(int i = 0; i < xSide; i++){
        for(int j = 0; j < xSide; j++){
            printf("%c "
                   "", b[i*xSide+j].symbol);
        }
        printf("\n");
    }
    printf("\nPunteggio: %d\n", score);
    printf(".....................................................................\n");
    printf("\n");
}

void randomCoins(Box *b){ //posiziona un numero casuale di monete in posizioni casuali
    int numOfCoins = (rand()%(xSide*3)) +5; //determina il numero di monete. Minimo 10, massimo (xSide+9)
    for (int i=0; i<numOfCoins; i++) {
        int coin = rand()%(xSide*xSide);
        if (b[coin].symbol!='#' && b[coin].symbol!='_' && b[coin].symbol!= 'o'){
            b[coin].symbol= '$';}

    }
}

void randomBombs(Box *b) { //posiziona un numero casuale di imprevisti (!) in posizioni casuali
    int numOfBombs = (rand()%xSide) +5; //determina il numero di imprevisti. Minimo 5, massimo (xSide+4)
    for (int i = 0; i < numOfBombs; i++) {
        int bomb = rand() % (xSide * xSide);
        if (b[bomb].symbol!= '#' && b[bomb].symbol!= '_' && b[bomb].symbol!= '$' && b[bomb].symbol!= 'o') {
            b[bomb].symbol= '!';
        }
    }
}

/*
void randomWallsSimple(char *b){ //genera muri partendo dai lati della campo. Non genera rami figli. Buggato
    int numOfWalls = (rand()%(xSide/4)) +15; //determina il numero di muri
    for (int i=0; i< numOfWalls; i++){
        int base;
        int k=(rand()%((xSide/3)*2))+1; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSide/2 +1)
        short randomSide = rand()%4; //determina il lato dal quale partira' il muro
        switch(randomSide) {
            case 0: //lato superiore
                base=rand()%xSide; //sceglie randomicamente un numero tra 0 e xSide
                if(b[base].symbol!='o' && b[base].symbol!='_'){ //se il punto da cui parte il muro non è lo start o l'end
                    while (b[base+xSide+xSide] != '#' && b[base+xSide+xSide-1] != '#' && b[base+xSide+xSide+1] != '#' && k>0){ //finché non sta per incrociare un altro muro o finché non raggiunge la lunghezza randomica stabilita precedentemente
                        b[base+xSide].symbol='#';
                        base += xSide;
                        k--;
                    }
                }
                break;
            case 1: //lato inferiore
                base=rand()%xSide + ((xSide*xSide)-xSide); //sceglie randomicamente un numero tra ((xSide*xSide)-xSide) e (xSide*xSide)
                if(b[base].symbol!='o' && b[base].symbol!='_'){ //se il punto da cui parte il muro non è lo start o l'end
                    while (b[base-xSide-xSide] != '#' && b[base-xSide-xSide-1] != '#' && b[base-xSide-xSide+1] != '#' && k>0) { //finché non sta per incrociare un altro muro o finché non raggiunge la lunghezza randomica stabilita precedentemente
                        b[base - xSide].symbol='#';
                        base -= xSide;
                        k--;
                    }
                }
                break;
            case 2: //lato sinistro
                base=(rand()%xSide)*xSide; //sceglie randomicamente un numero tra 0 e (xSide*xSide) che modulo xSide dia 0
                if(b[base].symbol!='o' && b[base].symbol!='_') { //se il punto da cui parte il muro non è lo start o l'end
                    while (b[base + 2] != '#' && b[base-xSide+2] != '#' && b[base+xSide+2] != '#' && k>0) { //finché non sta per incrociare un altro muro' o finché non raggiunge la lunghezza randomica stabilita precedentemente
                        b[base + 1].symbol='#';
                        base += 1;
                        k--;
                    }
                }
                break;
            case 3: //lato destro
                base=(rand()%xSide)*xSide-1; //sceglie randomicamente un numero tra 0 e (xSide*xSide) che modulo xSide dia (xSide-1)
                if(b[base].symbol!='o' && b[base].symbol!='_') { //se il punto da cui parte il muro non è lo start o l'end
                    while (b[base - 2] != '#' && b[base-xSide-2] != '#' && b[base+xSide-2] != '#' && k>0) { //finché non sta per incrociare un altro muro o finché non raggiunge la lunghezza randomica stabilita precedentemente
                        b[base - 1].symbol='#';
                        base -= 1;
                        k--;
                    }
                }
                break;
            default:
                printf("Errore\n");
                break;
        }
    }
}
*/

void randomFlyingWalls(Box *b) { //posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale
    int numOfWalls = (rand()%xSide); //determina randomicamente il numero di muri
    int d;
    int k = (rand() % ((xSide/2))) +1; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSide/3 *2 +1)
    int base;
    for (int i = 0; i < numOfWalls; i++) {
        d = xSide*ySide*xSide;
        do {
            base = rand() % (xSide * xSide);
            d--;
        } while ((b[base].symbol!=' ' || b[base+1].symbol=='#' || b[base-1].symbol=='#' || b[base+xSide].symbol=='#' || b[base-xSide].symbol=='#'
                  || b[base+xSide+1].symbol=='#' || b[base+xSide-1].symbol=='#' || b[base-xSide+1].symbol=='#' || b[base-xSide-1].symbol=='#') && d>0);
        //se la casella scelta non è uno spazio o se confina con un muro o una parete
        if (d>0){
            b[base].symbol='#';
            short direction = rand()%4;
            switch(direction){
                case 0: //basso
                    while (b[base+xSide-1].symbol!='#' && b[base+xSide].symbol!='#' && b[base+xSide+1].symbol!='#' && b[base+xSide+xSide-1].symbol!='#' && b[base+xSide+xSide].symbol!='#' && b[base+xSide+xSide+1].symbol!='#' &&
                           b[base+xSide-1].symbol!='_' && b[base+xSide].symbol!='_' && b[base+xSide+1].symbol!='_' && b[base+xSide+xSide-1].symbol!='_' && b[base+xSide+xSide].symbol!='_' && b[base+xSide+xSide+1].symbol!='_' &&
                           b[base+xSide-1].symbol!='o' && b[base+xSide].symbol!='o' && b[base+xSide+1].symbol!='o' && b[base+xSide+xSide-1].symbol!='o' && b[base+xSide+xSide].symbol!='o' && b[base+xSide+xSide+1].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                        b[base+xSide].symbol='#';
                        base += xSide;
                        k--;
                    }
                    break;
                case 1: //alto
                    while (b[base-xSide-1].symbol!='#' && b[base-xSide].symbol!='#' && b[base-xSide+1].symbol!='#' && b[base-xSide-xSide-1].symbol!='#' && b[base-xSide-xSide].symbol!='#' && b[base-xSide-xSide+1].symbol!='#' &&
                           b[base-xSide-1].symbol!='_' && b[base-xSide].symbol!='_' && b[base-xSide+1].symbol!='_' && b[base-xSide-xSide-1].symbol!='_' && b[base-xSide-xSide].symbol!='_' && b[base-xSide-xSide+1].symbol!='_' &&
                           b[base-xSide-1].symbol!='o' && b[base-xSide].symbol!='o' && b[base-xSide+1].symbol!='o' && b[base-xSide-xSide-1].symbol!='o' && b[base-xSide-xSide].symbol!='o' && b[base-xSide-xSide+1].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                        b[base-xSide].symbol='#';
                        base -= xSide;
                        k--;
                    }
                    break;
                case 2: //destra
                    while (b[base-xSide+1].symbol!='#' && b[base-xSide+2].symbol!='#' && b[base+1].symbol!='#' && b[base+2].symbol!='#' && b[base+xSide+1].symbol!='#' && b[base+xSide+2].symbol!='#' &&
                           b[base-xSide+1].symbol!='_' && b[base-xSide+2].symbol!='_' && b[base+1].symbol!='_' && b[base+2].symbol!='_' && b[base+xSide+1].symbol!='_' && b[base+xSide+2].symbol!='_' &&
                           b[base-xSide+1].symbol!='o' && b[base-xSide+2].symbol!='o' && b[base+1].symbol!='o' && b[base+2].symbol!='o' && b[base+xSide+1].symbol!='o' && b[base+xSide+2].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                        b[base+1].symbol='#';
                        base++;
                        k--;
                    }
                    break;
                case 3: //sinistra
                    while (b[base-xSide-1].symbol!='#' && b[base-xSide-2].symbol!='#' && b[base-1].symbol!='#' && b[base-2].symbol!='#' && b[base+xSide-1].symbol!='#' && b[base+xSide-2].symbol!='#' &&
                           b[base-xSide-1].symbol!='_' && b[base-xSide-2].symbol!='_' && b[base-1].symbol!='_' && b[base-2].symbol!='_' && b[base+xSide-1].symbol!='_' && b[base+xSide-2].symbol!='_' &&
                           b[base-xSide-1].symbol!='o' && b[base-xSide-2].symbol!='o' && b[base-1].symbol!='o' && b[base-2].symbol!='o' && b[base+xSide-1].symbol!='o' && b[base+xSide-2].symbol!='o' && k>0){
                        //finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start
                        b[base-1].symbol='#';
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

void randomWalls(Box *b){ //posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale
    int numOfWalls = (rand()%(xSide*4)) +10; //determina il numero di muri
    int d;
    int k=(rand()%((xSide/3)*2))+2; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSide/3 *2 +1)
    int base;
    for (int i=0; i< numOfWalls; i++) {
        d = xSide*ySide*xSide;
        do {
            base = rand()%(xSide*xSide);
            d--;
        } while ((b[base].symbol!='#' || base==0 ||  base==(xSide-1) || base==((xSide*xSide)-1) || base==((xSide*xSide)-xSide+1)) && d>0); //se la casella scelta e' un angolo o se non contiene '#'
        if (d>0){
            if (base<(xSide-1)){ //se la casella scelta fa parte della parete superiore
                while (b[base+xSide-1].symbol!='#' && b[base+xSide].symbol!='#' && b[base+xSide+1].symbol!='#' && b[base+xSide+xSide-1].symbol!='#' && b[base+xSide+xSide].symbol!='#' && b[base+xSide+xSide+1].symbol!='#' &&
                       b[base+xSide-1].symbol!='_' && b[base+xSide].symbol!='_' && b[base+xSide+1].symbol!='_' && b[base+xSide+xSide-1].symbol!='_' && b[base+xSide+xSide].symbol!='_' && b[base+xSide+xSide+1].symbol!='_' &&
                       b[base+xSide-1].symbol!='o' && b[base+xSide].symbol!='o' && b[base+xSide+1].symbol!='o' && b[base+xSide+xSide-1].symbol!='o' && b[base+xSide+xSide].symbol!='o' && b[base+xSide+xSide+1].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                    b[base+xSide].symbol='#';
                    base += xSide;
                    k--;
                }
            }
            else if (base>((xSide*xSide)-xSide+1) && base<((xSide*xSide)-1)){ //se la casella scelta fa parte della parete inferiore
                while (b[base-xSide-1].symbol!='#' && b[base-xSide].symbol!='#' && b[base-xSide+1].symbol!='#' && b[base-xSide-xSide-1].symbol!='#' && b[base-xSide-xSide].symbol!='#' && b[base-xSide-xSide+1].symbol!='#' &&
                       b[base-xSide-1].symbol!='_' && b[base-xSide].symbol!='_' && b[base-xSide+1].symbol!='_' && b[base-xSide-xSide-1].symbol!='_' && b[base-xSide-xSide].symbol!='_' && b[base-xSide-xSide+1].symbol!='_' &&
                       b[base-xSide-1].symbol!='o' && b[base-xSide].symbol!='o' && b[base-xSide+1].symbol!='o' && b[base-xSide-xSide-1].symbol!='o' && b[base-xSide-xSide].symbol!='o' && b[base-xSide-xSide+1].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                    b[base-xSide].symbol='#';
                    base -= xSide;
                    k--;
                }
            }
            else if (base%xSide==0){ //se la casella scelta fa parte della parete sinistra
                while (b[base-xSide+1].symbol!='#' && b[base-xSide+2].symbol!='#' && b[base+1].symbol!='#' && b[base+2].symbol!='#' && b[base+xSide+1].symbol!='#' && b[base+xSide+2].symbol!='#' &&
                       b[base-xSide+1].symbol!='_' && b[base-xSide+2].symbol!='_' && b[base+1].symbol!='_' && b[base+2].symbol!='_' && b[base+xSide+1].symbol!='_' && b[base+xSide+2].symbol!='_' &&
                       b[base-xSide+1].symbol!='o' && b[base-xSide+2].symbol!='o' && b[base+1].symbol!='o' && b[base+2].symbol!='o' && b[base+xSide+1].symbol!='o' && b[base+xSide+2].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                    b[base+1].symbol='#';
                    base++;
                    k--;
                }
            }
            else if (base%xSide==(xSide-1)){ //se la casella scelta fa parte della parete destra
                while (b[base-xSide-1].symbol!='#' && b[base-xSide-2].symbol!='#' && b[base-1].symbol!='#' && b[base-2].symbol!='#' && b[base+xSide-1].symbol!='#' && b[base+xSide-2].symbol!='#' &&
                       b[base-xSide-1].symbol!='_' && b[base-xSide-2].symbol!='_' && b[base-1].symbol!='_' && b[base-2].symbol!='_' && b[base+xSide-1].symbol!='_' && b[base+xSide-2].symbol!='_' &&
                       b[base-xSide-1].symbol!='o' && b[base-xSide-2].symbol!='o' && b[base-1].symbol!='o' && b[base-2].symbol!='o' && b[base+xSide-1].symbol!='o' && b[base+xSide-2].symbol!='o' && k>0){
                    //finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start
                    b[base-1].symbol='#';
                    base--;
                    k--;
                }
            }
            else { //se la casella scelta fa parte di un muro inserito nei cicli precedenti
                k += 5;
                if (b[base+1].symbol=='#' || b[base-1].symbol=='#'){ //se è un muro orizzontale
                    short direction = rand()%2; //scelgo un lato dal quale sviluppare un ulteriore ramo
                    switch(direction){
                        case 0: //sviluppo un ramo verso il basso
                            while (b[base+xSide-1].symbol!='#' && b[base+xSide].symbol!='#' && b[base+xSide+1].symbol!='#' && b[base+xSide+xSide-1].symbol!='#' && b[base+xSide+xSide].symbol!='#' && b[base+xSide+xSide+1].symbol!='#' &&
                                   b[base+xSide-1].symbol!='_' && b[base+xSide].symbol!='_' && b[base+xSide+1].symbol!='_' && b[base+xSide+xSide-1].symbol!='_' && b[base+xSide+xSide].symbol!='_' && b[base+xSide+xSide+1].symbol!='_' &&
                                   b[base+xSide-1].symbol!='o' && b[base+xSide].symbol!='o' && b[base+xSide+1].symbol!='o' && b[base+xSide+xSide-1].symbol!='o' && b[base+xSide+xSide].symbol!='o' && b[base+xSide+xSide+1].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                                b[base+xSide].symbol='#';
                                base += xSide;
                                k--;
                            }
                            break;
                        case 1: //sviluppo un ramo verso l'alto
                            while (b[base-xSide-1].symbol!='#' && b[base-xSide].symbol!='#' && b[base-xSide+1].symbol!='#' && b[base-xSide-xSide-1].symbol!='#' && b[base-xSide-xSide].symbol!='#' && b[base-xSide-xSide+1].symbol!='#' &&
                                   b[base-xSide-1].symbol!='_' && b[base-xSide].symbol!='_' && b[base-xSide+1].symbol!='_' && b[base-xSide-xSide-1].symbol!='_' && b[base-xSide-xSide].symbol!='_' && b[base-xSide-xSide+1].symbol!='_' &&
                                   b[base-xSide-1].symbol!='o' && b[base-xSide].symbol!='o' && b[base-xSide+1].symbol!='o' && b[base-xSide-xSide-1].symbol!='o' && b[base-xSide-xSide].symbol!='o' && b[base-xSide-xSide+1].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                                b[base-xSide].symbol='#';
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
                            while (b[base-xSide+1].symbol!='#' && b[base-xSide+2].symbol!='#' && b[base+1].symbol!='#' && b[base+2].symbol!='#' && b[base+xSide+1].symbol!='#' && b[base+xSide+2].symbol!='#' &&
                                   b[base-xSide+1].symbol!='_' && b[base-xSide+2].symbol!='_' && b[base+1].symbol!='_' && b[base+2].symbol!='_' && b[base+xSide+1].symbol!='_' && b[base+xSide+2].symbol!='_' &&
                                   b[base-xSide+1].symbol!='o' && b[base-xSide+2].symbol!='o' && b[base+1].symbol!='o' && b[base+2].symbol!='o' && b[base+xSide+1].symbol!='o' && b[base+xSide+2].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                                b[base+1].symbol='#';
                                base++;
                                k--;
                            }
                            break;
                        case 1: //sviluppo un ramo verso destra
                            while (b[base-xSide-1].symbol!='#' && b[base-xSide-2].symbol!='#' && b[base-1].symbol!='#' && b[base-2].symbol!='#' && b[base+xSide-1].symbol!='#' && b[base+xSide-2].symbol!='#' &&
                                   b[base-xSide-1].symbol!='_' && b[base-xSide-2].symbol!='_' && b[base-1].symbol!='_' && b[base-2].symbol!='_' && b[base+xSide-1].symbol!='_' && b[base+xSide-2].symbol!='_' &&
                                   b[base-xSide-1].symbol!='o' && b[base-xSide-2].symbol!='o' && b[base-1].symbol!='o' && b[base-2].symbol!='o' && b[base+xSide-1].symbol!='o' && b[base+xSide-2].symbol!='o' && k>0){
                                //finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start
                                b[base-1].symbol='#';
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

int createLabirint(Box* b){
    printf("Sono entrato nella createLabirint\n");
    int start = randomStart(); //genera uno start in posizione casuale
    printf("Questo e' lo start: %d\n", start);
    printf("Sono tornato dalla randomStart\n");
    b[start].symbol='o';
    printf("Ho assegnato 'o' allo start\n");
    int end = 44;//randomEnd(start); // genera un end in posizione casuale
    printf("Sono tornato dalla randomEnd\n");
    b[end].symbol='_';
    randomFlyingWalls(&b[0]); // posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale
    printf("Sono tornato dalla randomFlyingWalls\n");
    randomWalls(&b[0]); //posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale
    printf("Sono tornato dalla randomWalls\n");
    randomCoins(&b[0]); //posiziona un numero casuale di monete in posizioni casuali
    printf("Sono tornato dalla randomCoins\n");
    randomBombs(&b[0]); //posiziona un numero casuale di imprevisti in posizioni casuali
    printf("Sono tornato dalla randomBombs\n");
    return start;

}

char welcome(){
    printf("Benvenuto! In che modalita' vuoi giocare?\n");
    printf("Per modalita' AI premere 'a'\n");
    printf("Per modalita' utente premere 'b'\n");
    printf("\n");
    char mode;
    /*do {
        mode = scanf("%c", &mode);
        printf("%c\n", mode);
    } while (mode != 'a' && mode != 'A' && mode != 'b' && mode != 'B');*/ //finché non viene immesso un carattere valido
    mode = scanf("%c", &mode);
    return mode;
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
    printf("Input non corretto, reinserirlo.");
    printf("\n");
}

void moving(Box* b, int start, int score){ //muove il giocatore
    char move;
    int currentPosition = start;
    printLabirint(&b[0], score);
    int coins = 0; //monete raccolte
    _Bool endGame=0;
    do{
        scanf(" %c", &move);
        //move=getchar();
        switch(move){
            case 'W':
            case 'w':
                if (b[currentPosition -xSide].symbol==' ' || b[currentPosition - xSide].symbol=='.' || b[currentPosition -xSide].symbol=='$' || b[currentPosition -xSide].symbol=='!'){ //se la posizione sopra la posizione attuale è parte del labirinto
                    b[currentPosition].symbol= '.';
                    if (b[currentPosition - xSide].symbol=='!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (b[currentPosition - xSide].symbol=='$'){ // se incontra una moneta
                        score += 10;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    b[currentPosition - xSide].symbol= 'o';
                    currentPosition = currentPosition - xSide; //nuova pos. attuale
                    //system("clear");
                    printLabirint(&b[0], score);
                }
                else if (b[currentPosition - xSide].symbol=='_') { //se ho completato il percorso
                    b[currentPosition].symbol= '.';
                    b[currentPosition - xSide].symbol= 'o';
                    score--;
                    //system("clear");
                    printLabirint(&b[0], score);
                    success(score);
                    endGame = 1;
                }
                else { //se la posizione sopra la posizione attuale è un muro o una parete
                    death();
                    endGame = 1;
                }
                break;

            case 'S':
            case 's':
                if (b[currentPosition +xSide].symbol==' ' || b[currentPosition + xSide].symbol=='.' || b[currentPosition +xSide].symbol=='$' || b[currentPosition +xSide].symbol=='!'){ //se la posizione sotto la posizione attuale è parte del labirinto
                    b[currentPosition].symbol='.';
                    if (b[currentPosition + xSide].symbol=='!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (b[currentPosition + xSide].symbol=='$'){ // se incontra una moneta
                        score += 3;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    b[currentPosition + xSide].symbol='o';
                    currentPosition = currentPosition + xSide; //nuova pos. attuale
                    //system("clear");
                    printLabirint(&b[0], score);
                }

                else if (b[currentPosition + xSide].symbol=='_') { //se ho completato il percorso
                    b[currentPosition].symbol='.';
                    b[currentPosition + xSide].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(&b[0], score);
                    success(score);
                    endGame = 1;
                }
                else { //se la posizione sotto la posizione attuale è un muro o una parete
                    death();
                    endGame = 1;
                }
                break;

            case 'D':
            case 'd':
                if (b[currentPosition +1].symbol==' ' || b[currentPosition + 1].symbol=='.' || b[currentPosition +1].symbol=='$' || b[currentPosition +1].symbol=='!'){ //se la posizione a destra della posizione attuale è parte del labirinto
                    b[currentPosition].symbol='.';
                    if (b[currentPosition + 1].symbol=='!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (b[currentPosition + 1].symbol=='$'){ // se incontra una moneta
                        score += 3;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    b[currentPosition + 1].symbol='o';
                    currentPosition = currentPosition + 1; //nuova pos. attuale
                    //system("clear");
                    printLabirint(&b[0], score);
                }
                else if (b[currentPosition + 1].symbol=='_') { //se ho completato il percorso
                    b[currentPosition].symbol='.';
                    b[currentPosition + 1].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(&b[0], score);
                    success(score);
                    endGame = 1;
                }
                else { //se la posizione a destra della posizione attuale è un muro o una parete
                    death();
                    endGame = 1;
                }
                break;

            case 'A':
            case 'a':
                if  (b[currentPosition - 1].symbol==' ' || b[currentPosition - 1].symbol=='.' || b[currentPosition -1].symbol=='$' || b[currentPosition -1].symbol=='!'){ //se la posizione a sinistra della posizione attuale è parte del labirinto
                    b[currentPosition].symbol='.';
                    if (b[currentPosition - 1].symbol=='!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (b[currentPosition - 1].symbol=='$'){ // se incontra una moneta
                        score += 3;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    b[currentPosition - 1].symbol='o';
                    currentPosition = currentPosition - 1; //nuova pos. attuale
                    //system("clear");
                    printLabirint(&b[0], score);
                }
                else if (b[currentPosition - 1].symbol=='_') { //se ho completato il percorso
                    b[currentPosition].symbol='.';
                    b[currentPosition - 1].symbol='o';
                    score--;
                    //system("clear");
                    printLabirint(&b[0], score);
                    success(score);
                    endGame = 1;
                }
                else { //se la posizione a sinistra della posizione attuale è un muro o una parete
                    death();
                    endGame = 1;
                }
                break;

            default:
                wrongInput();
                break;
        }
    }while(!endGame);

}



void aStarAlgorithm(Box* b, int start, int end){

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
        b[i].visited=0;
        b[i].extracted=0;
    }
    printLabirint(&b[0], 1000);
    int score = 0;
    short xDist, yDist;
    b[start].gCost = 0;
    xDist = abs((start%xSide) - end%xSide);
    printf("start%%xSide: %d\n", start%xSide);
    printf("end%%xSide: %d\n", end%xSide);
    printf("xDist: %d,\n", xDist);
    yDist = abs((start/xSide) - end/xSide);
    printf("start/xSide: %d\n", start/xSide);
    printf("end/xSide: %d\n", end/xSide);
    printf("yDist: %d,\n", yDist);
    b[start].hCost = yDist + xDist;
    b[start].reference = start;
    b[start].predecessor = -1;
    b[start].numCoins = 0;
    b[start].extracted = 1;
    b[start].visited = 1;

    Box candidates[xSide*ySide];
    candidates[0]=b[start];
    printf("candidates[0].symbol: %c,\ncandidates[0].predecessor: %d,\ncandidates[0].fCost: %d,\n",
           candidates[0].symbol, candidates[0].predecessor, candidates[0].gCost + candidates[0].hCost);
    short min;
    short i;
    short size = 1;
    _Bool arrived = 0;
    int k;

    if (start<xSide-1){ //se lo start è posizionato sulla parete superiore
        printf("Sono entrato nell'if basso\n");
        b[start + xSide].gCost = 1 + b[start].gCost;
        xDist = abs(((start + xSide) % xSide) - end % xSide);
        yDist = abs(((start + xSide) / xSide) - end / xSide);
        b[start + xSide].hCost = yDist + xDist;
        b[start + xSide].predecessor = start;
        b[start + xSide].reference = start + xSide;
        b[start + xSide].visited = 1;
        if (b[start + xSide].symbol == '$'){
            b[start + xSide].numCoins = b[start].numCoins +1;
        }
        else {
            b[start + xSide].numCoins = b[start].numCoins;
        }
        candidates[size] = b[start + xSide];
        size++;
    }
    else if (start>((xSide*xSide)-xSide)){ //se lo start è posizionato sulla parete inferiore
        printf("Sono entrato nell'if alto\n");
        b[start - xSide].gCost = 1 + b[start].gCost;
        xDist = abs(((start - xSide) % xSide) - end % xSide);
        yDist = abs(((start - xSide) / xSide) - end / xSide);
        b[start - xSide].hCost = yDist + xDist;
        b[start - xSide].predecessor = start;
        b[start - xSide].reference = start - xSide;
        b[start - xSide].visited = 1;
        if (b[start - xSide].symbol == '$'){
            b[start - xSide].numCoins = b[start].numCoins +1;
        }
        else {
            b[start - xSide].numCoins = b[start].numCoins;
        }
        candidates[size] = b[start - xSide];
        size++;
    }

    else if (start%xSide==0){ //se lo start è posizionato sulla parete sinistra
        printf("Sono entrato nell'if destro\n");
        b[start + 1].gCost = 1 + b[start].gCost;
        xDist = abs(((start + 1) % xSide) - end % xSide);
        yDist = abs(((start + 1) / xSide) - end / xSide);
        b[start + 1].hCost = yDist + xDist;
        b[start + 1].predecessor = start;
        b[start + 1].reference = start + 1;
        b[start + 1].visited = 1;
        if (b[start +1].symbol == '$'){
            b[start + 1].numCoins = b[start].numCoins +1;
        }
        else {
            b[start + 1].numCoins = b[start].numCoins;
        }
        candidates[size] = b[start + 1];
        size++;
    }

    else { //se lo start è posizionato sulla parete destra
        printf("Sono entrato nell'if sinistro\n");
        b[start - 1].gCost = 1 + b[start].gCost;
        xDist = abs(((start - 1) % xSide) - end % xSide);
        yDist = abs(((start - 1) / xSide) - end / xSide);
        b[start - 1].hCost = yDist + xDist;
        b[start - 1].predecessor = start;
        b[start - 1].reference = start - 1;
        b[start - 1].visited = 1;
        if (b[start -1].symbol == '$'){
            b[start - 1].numCoins = b[start].numCoins +1;
        }
        else {
            b[start - 1].numCoins = b[start].numCoins;
        }
        candidates[size] = b[start - 1];
        size++;
    }

    candidates[0].extracted=1;

    while (!arrived) {
        min = xSide*ySide;
        printf("\n");
        for (int j = 0; j < size; j++) {
            printf("\ncandidates[%d].extracted: %d\n",j, candidates[j].extracted);
            printf("  t: %d\n", candidates[j].reference);
            printf("  candidates[%d].predecessor: %d\n",j, candidates[j].predecessor);
            printf("  candidates[%d].numCoins: %d\n",j, candidates[j].numCoins);
            printf("  candidates[%d].hCost: %d\n",j, candidates[j].hCost);
            printf("  candidates[%d].gCost: %d\n",j, candidates[j].gCost);
            printf("  candidates[%d].fCost: %d\n",j, candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10);
            if (candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10 <= min && candidates[j].extracted==0) {
                min = candidates[j].gCost + candidates[j].hCost - candidates[j].numCoins*10;
                printf("    MINIMO\n");
                i = j;
            }
        }

        printf("\n\n");
        candidates[i].extracted=1;
        int t = candidates[i].reference;
        b[t].predecessor=candidates[i].predecessor;
        b[t].hCost=candidates[i].hCost;
        b[t].gCost=candidates[i].gCost;

        printf("\n\n!b[t - xSide].visited = %d,\nt>=xSide = %d\nb[t - xSide].symbol = %c\nb[t].predecessor != t -xSide = %d\n\n"
                , !b[t - xSide].visited, t>=xSide,b[t - xSide].symbol, b[t].predecessor != t -xSide);

        if (!b[t - xSide].visited && t>=xSide && b[t - xSide].symbol != '#' && b[t - xSide].symbol != 'o' && b[t].predecessor != t -xSide) { //casella alta se essa non è un muro e se non è il predecessore del nodo predecessore
            printf("Sono entrato nell'if alto\n");
            b[t - xSide].numCoins = b[t].numCoins;
            b[t - xSide].gCost = 1 + b[t].gCost;
            xDist = abs(((t - xSide) % xSide) - end % xSide);
            yDist = abs(((t - xSide) / xSide) - end / xSide);
            b[t - xSide].hCost = yDist + xDist;
            if (b[t - xSide].symbol == '$'){
                b[t - xSide].numCoins = b[t].numCoins + 1;
            }
            else if (b[t - xSide].symbol == '!'){
                b[t - xSide].numCoins = (b[t].numCoins)/2;
            }
            else {
                b[t - xSide].numCoins = b[t].numCoins;
            }
            b[t - xSide].predecessor = t;
            b[t - xSide].reference = t - xSide;
            b[t - xSide].visited = 1;
            candidates[size] = b[t - xSide];

            printf("   candidates[size].symbol: %c,\n   candidates[size].predecessor: %d,\n   candidates[size].numCoins: %d,\n"
                   "   candidates[size].fCost: %d\n   candidates[size].reference: %d\n\n", candidates[size].symbol,
                   candidates[size].predecessor, candidates[size].numCoins,
                   candidates[size].gCost + candidates[size].hCost - candidates[size].numCoins*10, candidates[size].reference);
            size++;
            if (b[t - xSide].symbol == '_') {
                arrived = 1;
                k=t-xSide;
            }
        }

        printf("\n\n!b[t +1 ].visited = %d,\nb[t+1].symbol = %c\nb[t].predecessor != t + 1 = %d\n\n"
                , !b[t +1].visited, b[t +1].symbol, b[t].predecessor != t +1);

        if (!b[t + 1].visited && b[t + 1].symbol != '#' && b[t + 1].symbol != 'o' && b[t].predecessor != t +1) { //casella a destra se essa non è un muro e se non è il predecessore del nodo predecessore
            printf("Sono entrato nell'if destro\n");
            b[t + 1].numCoins = b[t].numCoins;
            b[t + 1].gCost = 1 + b[t].gCost;
            xDist = abs(((t + 1) % xSide) - end % xSide);
            yDist = abs(((t + 1) / xSide) - end / xSide);
            b[t + 1].hCost = yDist + xDist;
            if (b[t + 1].symbol == '$'){
                b[t + 1].numCoins = b[t].numCoins + 1;
            }
            else if (b[t + 1].symbol == '!'){
                b[t + 1].numCoins = (b[t].numCoins)/2;
            }
            else {
                b[t + 1].numCoins = b[t].numCoins;
            }
            b[t + 1].predecessor = t;
            b[t + 1].reference = t + 1;
            b[t + 1].visited = 1;
            candidates[size] = b[t + 1];

            printf("   candidates[size].symbol: %c,\n   candidates[size].predecessor: %d,\n   candidates[size].numCoins: %d,\n"
                   "   candidates[size].fCost: %d\n   candidates[size].reference: %d\n\n", candidates[size].symbol,
                   candidates[size].predecessor, candidates[size].numCoins,
                   candidates[size].gCost + candidates[size].hCost - candidates[size].numCoins*10, candidates[size].reference);
            size++;
            if (b[t + 1].symbol == '_'){
                printf("Sono entrato nell'if finale\n");
                arrived = 1;
                k=t+1;
                printf("k=%d\n", k);
            }
        }

        printf("\n\n!b[t + xSide].visited = %d,\nt<xSide*ySide-xSide = %d\nb[t + xSide].symbol = %c\nb[t].predecessor != t +xSide = %d\n\n"
                , !b[t + xSide].visited, t<xSide*ySide-xSide,b[t + xSide].symbol, b[t].predecessor != t +xSide);

        if (!b[t + xSide].visited && t<xSide*ySide-xSide && b[t + xSide].symbol != 'o' && b[t + xSide].symbol != '#' && b[t].predecessor != t +xSide) { //casella bassa se essa non è un muro e se non è il predecessore del nodo predecessore
            printf("Sono entrato nell'if basso\n");
            b[t + xSide].numCoins = b[t].numCoins;
            b[t + xSide].gCost = 1 + b[t].gCost;
            xDist = abs(((t + xSide) % xSide) - end % xSide);
            yDist = abs(((t + xSide) / xSide) - end / xSide);
            b[t + xSide].hCost = yDist + xDist;
            if (b[t + xSide].symbol == '$'){
                b[t + xSide].numCoins = b[t].numCoins + 1;
            }
            else if (b[t + xSide].symbol == '!'){
                b[t + xSide].numCoins = (b[t].numCoins)/2;
            }
            else {
                b[t + xSide].numCoins = b[t].numCoins;
            }
            b[t + xSide].predecessor = t;
            b[t + xSide].reference = t + xSide;
            b[t + xSide].visited = 1;
            candidates[size] = b[t + xSide];

            printf("candidates[size].symbol: %c,\n   candidates[size].predecessor: %d,\n   candidates[size].numCoins: %d,\n   candidates[size].fCost: %d,"
                   "\n   candidates[size].reference: %d\n\n", candidates[size].symbol,
                   candidates[size].predecessor, candidates[size].numCoins,
                   candidates[size].gCost + candidates[size].hCost - candidates[size].numCoins*10, candidates[size].reference);
            size++;
            if (b[t + xSide].symbol == '_'){
                arrived = 1;
                k=t+xSide;
            }
        }

        printf("\n\n!b[t -1 ].visited = %d,\nb[t-1].symbol = %c\nb[t].predecessor != t -1 = %d\n\n"
                , !b[t -1].visited, b[t -1].symbol, b[t].predecessor != t -1);

        if (!b[t - 1].visited && b[t - 1].symbol != '#' && b[t - 1].symbol != 'o' && b[t].predecessor != t -1) { //casella a sinistra se essa non è un muro e se non è il predecessore del nodo predecessore
            printf("Sono entrato nell'if sinistro\n");
            b[t - 1].numCoins = b[t].numCoins;
            b[t - 1].gCost = 1 + b[t].gCost;
            xDist = abs(((t - 1) % xSide) - end % xSide);
            yDist = abs(((t - 1) / xSide) - end / xSide);
            b[t - 1].hCost = yDist + xDist;
            if (b[t - 1].symbol == '$'){
                b[t - 1].numCoins = b[t].numCoins + 1;
            }
            else if (b[t - 1].symbol == '!'){
                b[t - 1].numCoins = (b[t].numCoins)/2;
            }
            else {
                b[t - 1].numCoins = b[t].numCoins;
            }
            b[t - 1].predecessor = t;
            b[t - 1].reference = t - 1;
            b[t - 1].visited = 1;
            candidates[size] = b[t - 1];

            printf("candidates[size].symbol: %c,\n   candidates[size].predecessor: %d,\n   candidates[size].numCoins: %d,\n   candidates[size].fCost: %d,\n"
                   "   candidates[size].reference: %d\n\n", candidates[size].symbol,
                   candidates[size].predecessor, candidates[size].numCoins,
                   candidates[size].gCost + candidates[size].hCost - candidates[size].numCoins*10, candidates[size].reference);
            size++;
            if (b[t - 1].symbol == '_') {
                arrived = 1;
                k=t-1;
            }
        }
    }

    short predecessors[xSide*ySide/2];
    int j = 0;
    printf("   b[k].predecessor prima del ciclo while = %d \n", b[k].predecessor);
    while (k!=start){
        printf("Sono entrato nel while finale\n");
        printf("   b[k].predecessor prima =%d\n", b[k].predecessor);

        predecessors[j]=k;
        printf("   predecessors[%d] =%d\n", j, predecessors[j]);
        k=b[k].predecessor;
        printf("   b[k].predecessor dopo =%d\n\n", b[k].predecessor);
        j++;
    }
    printf("j=%d",j);
    predecessors[j]=k;
    printf("\nContenuto di predecessor[]:\n");
    for (int p=0; p<j; p++){
        printf("%d ", predecessors[p]);
    }

    short path[j-1];
    short r = j;
    for(i=0; i<=j; i++){
        path[i]=predecessors[r];
        r--;
    }

    printf("\nContenuto di path[]:\n");
    for (int p=0; p<j; p++){
        printf("%d ", path[p]);
    }

    printLabirint(&b[0],10);
    printf("\n");

    for (int h = 1; h<j; h++ ){
        printf("Sono entrato nell'ultimo for\n");
        b[path[h]].symbol='.';
    }

    printLabirint(&b[0], 10);
}



int main(int argc, char *argv[]){
    srand(time(NULL)); //funzione che determina il seme per la randomizzazione
    int score=1000;
    Box board[xSide*xSide];
    printf("Step 1\n");
    createBoard(&board[0]);
    printf("Step 2\n");
    int start = createLabirint(&board[0]);
    printf("Step 3\n");
    aStarAlgorithm(&board[0], start, 44);
    char mode = welcome();
    mode='b';
    printf("Step 4\n");
    if (mode=='b' || mode=='B'){
        guide();
        moving(&board[0], start, score);
    }
    else if(mode=='a' || mode=='A'){
        //to be defined
    }

    return 0;
}