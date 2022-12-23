#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef BOX_STRUCT
#define BOX_STRUCT
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
#endif //BOX_STRUCT


void randomFlyingWalls(Box *board, size_t xSide, size_t ySide, int difficulty);
void randomWalls(Box *board, size_t xSide, size_t ySide, int difficulty);


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

void createObjects(Box *board, size_t xSide, size_t ySide, short numOfObjects, char objectType){

    short position;

    for(int i = 0; i < numOfObjects; i++){
        position = rand()%(xSide*ySide);
        if (board[position].symbol!='#' && board[position].symbol!='_' && board[position].symbol!= 'o' && objectType == '$'){
            board[position].symbol= '$';
        }
        if(board[position].symbol!= '#' && board[position].symbol != '_' && board[position].symbol!= '$' && board[position].symbol!= 'o' && board[position].symbol != '!' && objectType == 'T'){
            board[position].symbol = 'T';
        }
        if (board[position].symbol!= '#' && board[position].symbol!= '_' && board[position].symbol!= '$' && board[position].symbol!= 'o' && objectType == '!') {
            board[position].symbol= '!';
        }
    }
}

void randomCoins(Box* board, size_t xSide, size_t ySide){ //posiziona un numero casuale di monete in posizioni casuali
    char objectType = '$';
    short numOfCoins = (rand()%(xSide*3))+5; //determina il numero di monete. Minimo 10, massimo (xSide+9)
    createObjects(board, xSide, ySide, numOfCoins, objectType);
}

void randomBombs(Box* board, size_t xSide, size_t ySide) { //posiziona un numero casuale di imprevisti (!) in posizioni casuali
    char objectType = '!';
    short numOfBombs = (rand()%xSide)+5; //determina il numero di imprevisti. Minimo 5, massimo (xSide+4)
    createObjects(board, xSide, ySide, numOfBombs, objectType);
}

void randomDrill(Box* board,size_t xSide, size_t ySide){
    char objectType = 'T';
    short numOfDrill = (rand()%xSide)+5;
    createObjects(board, xSide, ySide, numOfDrill, objectType);
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
    randomFlyingWalls(board, xSide, ySide, difficulty); // posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale
    //printf("Sono tornato dalla randomFlyingWalls\n");
    randomWalls(board, xSide, ySide, difficulty); //posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale
    //printf("Sono tornato dalla randomWalls\n");
    randomCoins(board, xSide, ySide); //posiziona un numero casuale di monete in posizioni casuali
    //printf("Sono tornato dalla randomCoins\n");
    randomBombs(board, xSide, ySide); //posiziona un numero casuale di imprevisti in posizioni casuali
    //printf("Sono tornato dalla randomBombs\n");
    randomDrill(board, xSide, ySide);
    return start;
}