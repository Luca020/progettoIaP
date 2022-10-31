//idee e cose da fare:
//creare un funzione per ogni direzione di sviluppo dei muri, in modo da non ripetere il codice
//trasformare int in unsigned int quando necessario
//utilizzare gli short quando necessario
//debugging e testing
//dividere in librerie
//implementare comandi con frecce
//implementare lettura comando senza premere enter
//implementare timer che permetta al gioco di procedere anche se non viene dato input
//implementare livelli
//implementare snake che abbia dimensione fisica variabile
//implementare grafica
//documentazione
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
struct box {
    char symbol;
    short predecessor;
    short gCost;
    short hCost;
};
typedef struct box Box;
short unsigned xSide = 20u; //lunghezza del campo di gioco
short unsigned ySide = 20u; //altezza del campo di gioco
/*
void aiAlgorithm(Box* b, int start, int end){
    //gCost= distanza da start
    //hCost= distanza da end
    //fCost= hCost+gCost
    b[start].gCost = 0;
    xDist = abs(((start - xSide) % xSide) - end % xSide);
    yDist = abs(((start - xSide) / xSide) - end / xSide);
    b[start].hCost = yDist + xDist;
    creo struct node con campi int hCost, int f
    Inizializzo una lista di struct in cui terrò dentro le caselle candidate
    Prendo lo start
    Controllo le caselle attorno escluso il predecessore. Se non sono già state controllate:
    ricerca del box b[i] con campo fCost minore. Una volta trovato ne studio i box adiacenti
    Box candidates[500];
    candidates[0]=b[start];
    short min=xSize*ySize;
    short i=0;
    _Bool arrived = 0;
    while (!arrived){
        for (int j =0; j<size; j++){
            if (candidates[j].gCost+candidates[j].hCost<=min){
                min = candidates[j].gCost+candidates[j].hCost;
                i=j;
            }
        }
        if(b[i-xSide].symbol!='#' && b[i].predecessor!=i-xSide) { //casella alta se essa non è un muro e se non è il predecessore del nodo predecessore
            b[i-xSide].gCost = 1+b[predecessor].gCost;
            xDist = abs(((i-xSide)%xSide)-end%xSide);
            yDist = abs(((i-xSide)/xSide)-end/xSide);
            b[i-xSide].hCost = yDist + xDist;
            b[i-xSide].predecessor = i;
            candidates[size]=b[i-xSide];
            size++;
            if (b[i-xSide].symbol='_')
                arrived = 1;
        }
        if(b[i+1].symbol!='#' && b[i].predecessor!=i+1){// casella a destra
            b[i+1].gCost = 1 + b[predecessor].gCost;
            xDist = abs(((i+1)%xSide)-end%xSide);
            yDist = abs(((i+1)/xSide)-end/xSide);
            b[i+1].hCost = yDist+xDist;
            b[i+1].predecessor = i;
            candidates[size]=b[i+1];
            size++;
            if (b[i-xSide].symbol='_')
                arrived = 1;
        }
        if(b[i+xSide].symbol!='#' && b[i].predecessor!=i+xSide){ //casella bassa
            b[i+xSide].gCost = 1 + b[predecessor].gCost;
            xDist = abs(((i+xSide)%xSide)-end%xSide);
            yDist = abs(((i+xSide)/xSide)-end/xSide);
            b[i+xSide].hCost = yDist+xDist;
            b[i+xSide].fCost = b[i+xSide].hCost + b[i+xSide].gCost
            b[i+xSide].predecessor = i;
            candidates[size]=b[i+xSide];
            size++;
            if (b[i-xSide].symbol='_')
                arrived = 1;
        }
        if(b[i-1].symbol!='#' && b[i].predecessor!=i-1){ //casella a sinistra
            b[i-1].gCost = 1 + b[predecessor].gCost;
            xDist = abs(((i-1)%xSide)-end%xSide);
            yDist = abs(((i-1)/xSide)-end/xSide);
            b[i-1].hCost = yDist+xDist;
            b[i-1].fCost = b[i-1].hCost + b[i-1].gCost
            b[i-1].predecessor = i;
            candidates[size]=b[i-1];
            size++;
            if (b[i-xSide].symbol='_')
                arrived = 1;
        }
    }
    
    
    Inserisco nella lista le caselle che rispettavano la condizione dell'if
    Pesco la casella che ha l'fCost minore. Se due sono a parimerito ne scelgo una casualmente.
    Torno alla riga 52 e ciclo finché una delle caselle attorno non è l'end
}*/
void createBoard(Box* b){ //crea il campo di gioco con pareti e spazi vuoti
    for(int i = 0; i < (xSide*ySide); i++){
        if ((i>=0 && i<(xSide)) || (i>=((xSide*ySide)-xSide) && i<(xSide*ySide)-1) || i%xSide==0 || i%xSide==(xSide-1)){
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
            start=rand()%(xSide-2) + 1 + ((xSide*ySide)-xSide); //sceglie randomicamente un numero tra ((xSide*xSide)-xSide+1) e (xSide*xSide-1)
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
            end=(rand()%(xSide-2))+1; //sceglie randomicamente un numero tra 1 e (xSide-2)
            break;
        case 1: //lato inferiore
            end=rand()%(xSide-2) + 1 + ((xSide*ySide)-xSide); //sceglie randomicamente un numero tra ((xSide*xSide)-xSide+1) e (xSide*xSide-1)
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
void randomCoins(Box *b){ //posiziona un numero casuale di monete in posizioni casuali
    int numOfCoins = (rand()%(xSide*3)) +5; //determina il numero di monete. Minimo 10, massimo (xSide+9)
    for (int i=0; i<numOfCoins; i++) {
        int coin = rand()%(xSide*ySide);
        if (b[coin].symbol!='#' && b[coin].symbol!='_' && b[coin].symbol!= 'o'){
            b[coin].symbol= '$';}
    }
}
void randomBombs(Box *b) { //posiziona un numero casuale di imprevisti (!) in posizioni casuali
    int numOfBombs = (rand()%xSide) +5; //determina il numero di imprevisti. Minimo 5, massimo (xSide+4)
    for (int i = 0; i < numOfBombs; i++) {
        int bomb = rand() % (xSide * ySide);
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
    //srand(time(0));
    int numOfWalls = (rand()%xSide); //determina randomicamente il numero di muri
    for (int i = 0; i < numOfWalls; i++) {
        int k = (rand() % ((xSide/2))) +1; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSide/3 *2 +1)
        int base;
        do {
            base = rand() % (xSide * ySide);
        } while (b[base].symbol!=' ' || b[base+1].symbol=='#' || b[base-1].symbol=='#' || b[base+xSide].symbol=='#' || b[base-xSide].symbol=='#'
                 || b[base+xSide+1].symbol=='#' || b[base+xSide-1].symbol=='#' || b[base-xSide+1].symbol=='#' || b[base-xSide-1].symbol=='#');
        //se la casella scelta non è uno spazio o se confina con un muro o una parete
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
                    //finché nelle 6 caselle poste a destra della casella scelta non ci sono muri o end o start
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
                    //finché nelle 6 caselle poste a sinistra della casella scelta non ci sono muri o end o start
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
void randomWalls(Box *b){ //posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale
    int numOfWalls = (rand()%(xSide*4)) +10; //determina il numero di muri
    for (int i=0; i< numOfWalls; i++) {
        int k=(rand()%(xSide/2))+2; //determina randomicamente la lunghezza del muro, minimo 1, massimo (xSide/3 *2 +1)
        int base;
        do {
            base = rand()%(xSide*ySide);
        } while (b[base].symbol!='#' || base==0 ||  base==(xSide-1) || base==((xSide*ySide)-1) || base==((xSide*ySide)-xSide+1)); //se la casella scelta e' un angolo o se non contiene '#'
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
        else if (base>((xSide*ySide)-xSide+1) && base<((xSide*ySide)-1)){ //se la casella scelta fa parte della parete inferiore
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
                //finché nelle 6 caselle poste a destra della casella scelta non ci sono muri o end o start
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
                //finché nelle 6 caselle poste a sinistra della casella scelta non ci sono muri o end o start
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
                            //finché nelle 6 caselle poste a destra della casella scelta non ci sono muri o end o start
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
                            //finché nelle 6 caselle poste a sinistra della casella scelta non ci sono muri o end o start
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
int createLabirint(Box* b){
    int start = randomStart(); //genera uno start in posizione casuale
    b[start].symbol='o';
    int end = randomEnd(start); // genera un end in posizione casuale
    b[end].symbol='_';
    randomFlyingWalls(&b[0]); // posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale
    randomWalls(&b[0]); //posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale
    randomCoins(&b[0]); //posiziona un numero casuale di monete in posizioni casuali
    randomBombs(&b[0]); //posiziona un numero casuale di imprevisti in posizioni casuali
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
void printLabirint(Box* b, int score){ //stampa il labirinto
    for(int i = 0; i < ySide; i++){
        for(int j = 0; j < xSide; j++){
            printf("%c ", b[i*xSide+j].symbol);
        }
        printf("\n");
    }
    printf("\nPunteggio: %d\n", score);
    printf(".....................................................................\n");
    printf("\n");
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
                        score += 3;
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
int main(){
    srand(time(0)); //funzione che determina il seme per la randomizzazione
    //aiAlgorithm();
    int score=0;
    Box board[xSide*ySide];
    createBoard(&board[0]);
    int start = createLabirint(&board[0]);
    char mode = welcome();
    mode='b';
    if (mode=='b' || mode=='B'){
        guide();
        moving(&board[0], start, score);
    }
    else if(mode=='a' || mode=='A'){
        //to be defined
    }
    return 0;
}