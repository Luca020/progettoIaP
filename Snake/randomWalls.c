#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void randomFlyingWalls(Box *board, size_t xSide, size_t ySide, int difficulty) { //posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale

    int numOfWalls;
    int d; // determina il numero massimo di ripetizioni, serve per non entrare in loop nel caso in cui non ci siano più spazi liberi in cui inserire muri
    int k = (rand() % xSide) +3; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSide/3 *2 +1)
    int base;
    if(difficulty == 1){
        numOfWalls = (rand()%xSide)+2;
    }
    else if(difficulty == 2){
        numOfWalls = (rand()%(xSide*2))+3; //determina randomicamente il numero di muri
    }
    else{
        numOfWalls = (rand()%(xSide*4))+5;
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
        numOfWalls = (rand()%((xSide)));
    }
    else if(difficulty == 2){
        numOfWalls = (rand()%(xSide*2));//determina il numero di muri
    }
    else{
        numOfWalls = (rand()%(xSide*4));
    }

    //printf("il numero di muri e' %d\n", numOfWalls);
    int d;
    int k=(rand()%xSide)+3; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSide/3 *2 +1)
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