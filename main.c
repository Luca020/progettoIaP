//da fare: creare un funzione per ogni direzione di sviluppo dei muri, in modo da non ripetere il codice
            //trasformare int in unsigned int quando necessario
            //utilizzare gli short quando necessario
            //debugging e testing
            //dividere in librerie
            //implemtare comandi con frecce
            //implementare lettura comando senza premere enter
            //implementare timer
            //implementare lunghezza snake
            //implementare grafica

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int side = 20; //lato del quadrato di gioco

void createBoard(char* b){ //crea il campo di gioco con pareti e spazi vuoti
    for(int i = 0; i < (side*side); i++){
        if ((i>=0 && i<(side)) || (i>=((side*side)-side) && i<(side*side)) || i%side == 0 || i%side == (side-1)){
            b[i] = '#';
        }
        else {
            b[i] = ' ';
        }
    }
}

int randomStart(){ //genera uno start in posizione casuale
    int start;
    short randomSide = rand()%4; //sceglie a caso uno dei 4 lati
    switch(randomSide) {
        case 0: //lato superiore
            start=(rand()%(side-2))+1; //sceglie randomicamente un numero tra 1 e (side-1)
            break;
        case 1: //lato inferiore
            start=rand()%(side-2) + 1 + ((side*side)-side); //sceglie randomicamente un numero tra ((side*side)-side+1) e (side*side-1)
            break;
        case 2: //lato sinistro
            start=((rand()%(side-2))*side)+side;//sceglie randomicamente un numero tra side e (side*side)-side che modulo side dia 0
            break;
        case 3: //lato destro
            start=((rand()%(side-2))*side)+side+side-1; //sceglie randomicamente un numero tra side+side-1 e (side*side)-side-1 che modulo side dia (side-1)
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
    if (start<(side-1)){ //se lo start è posizionato sulla parete superiore
        dir[0]=1;
        dir[1]=2;
        dir[2]=3;
    }
    else if (start>((side*side)-side)){ //se lo start è posizionato sulla parete inferiore
        dir[0]=0;
        dir[1]=2;
        dir[2]=3;
    }
    else if (start%side==0){ //se lo start è posizionato sulla parete sinistra
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
                end=(rand()%(side-2))+1; //sceglie randomicamente un numero tra 1 e (side-2)
                break;
            case 1: //lato inferiore
                end=rand()%(side-2) + 1 + ((side*side)-side); //sceglie randomicamente un numero tra ((side*side)-side+1) e (side*side-1)
                break;
            case 2: //lato sinistro
                end=((rand()%(side-2))*side)+side;//sceglie randomicamente un numero tra side e (side*side)-side che modulo side dia 0
                break;
            case 3: //lato destro
                end=((rand()%(side-2))*side)+side+side-1; //sceglie randomicamente un numero tra side+side-1 e (side*side)-side-1 che modulo side dia (side-1)
                break;
            default:
                printf("Errore\n");
                break;
    }
    return end;
}

void randomCoins(char *b){ //posiziona un numero casuale di monete in posizioni casuali
    int numOfCoins = (rand()%(side*3)) +5; //determina il numero di monete. Minimo 10, massimo (side+9)
    for (int i=0; i<numOfCoins; i++) {
        int coin = rand()%(side*side);
        if (b[coin]!='#' && b[coin]!='_' && b[coin] != 'o'){
            b[coin]= '$';}
    }
}

void randomBombs(char *b) { //posiziona un numero casuale di imprevisti (!) in posizioni casuali
    int numOfBombs = (rand()%side) +5; //determina il numero di imprevisti. Minimo 5, massimo (side+4)
    for (int i = 0; i < numOfBombs; i++) {
        int bomb = rand() % (side * side);
        if (b[bomb] != '#' && b[bomb] != '_' && b[bomb] != '$' && b[bomb] != 'o') {
            b[bomb] = '!';
        }
    }
}

/*
void randomWallsSimple(char *b){ //genera muri partendo dai lati della campo. Non genera rami figli. Buggato
    int numOfWalls = (rand()%(side/4)) +15; //determina il numero di muri
    for (int i=0; i< numOfWalls; i++){
        int base;
        int k=(rand()%((side/3)*2))+1; //determina randomicamente la lunghezza del muro, minimo 1, massimo (side/2 +1)
        short randomSide = rand()%4; //determina il lato dal quale partira' il muro
        switch(randomSide) {
            case 0: //lato superiore
                base=rand()%side; //sceglie randomicamente un numero tra 0 e side
                if(b[base]!='o' && b[base]!='_'){ //se il punto da cui parte il muro non è lo start o l'end
                    while (b[base+side+side] != '#' && b[base+side+side-1] != '#' && b[base+side+side+1] != '#' && k>0){ //finché non sta per incrociare un altro muro o finché non raggiunge la lunghezza randomica stabilita precedentemente
                        b[base+side]='#';
                        base += side;
                        k--;
                    }
                }
                break;
            case 1: //lato inferiore
                base=rand()%side + ((side*side)-side); //sceglie randomicamente un numero tra ((side*side)-side) e (side*side)
                if(b[base]!='o' && b[base]!='_'){ //se il punto da cui parte il muro non è lo start o l'end
                    while (b[base-side-side] != '#' && b[base-side-side-1] != '#' && b[base-side-side+1] != '#' && k>0) { //finché non sta per incrociare un altro muro o finché non raggiunge la lunghezza randomica stabilita precedentemente
                        b[base - side] = '#';
                        base -= side;
                        k--;
                    }
                }
                break;
            case 2: //lato sinistro
                base=(rand()%side)*side; //sceglie randomicamente un numero tra 0 e (side*side) che modulo side dia 0
                if(b[base]!='o' && b[base]!='_') { //se il punto da cui parte il muro non è lo start o l'end
                    while (b[base + 2] != '#' && b[base-side+2] != '#' && b[base+side+2] != '#' && k>0) { //finché non sta per incrociare un altro muro' o finché non raggiunge la lunghezza randomica stabilita precedentemente
                        b[base + 1] = '#';
                        base += 1;
                        k--;
                    }
                }
                break;
            case 3: //lato destro
                base=(rand()%side)*side-1; //sceglie randomicamente un numero tra 0 e (side*side) che modulo side dia (side-1)
                if(b[base]!='o' && b[base]!='_') { //se il punto da cui parte il muro non è lo start o l'end
                    while (b[base - 2] != '#' && b[base-side-2] != '#' && b[base+side-2] != '#' && k>0) { //finché non sta per incrociare un altro muro o finché non raggiunge la lunghezza randomica stabilita precedentemente
                        b[base - 1] = '#';
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

void randomFlyingWalls(char *b) { //posiziona un numero casuale di muri 'volanti' in posizioni casuali e di lunghezza casuale
    srand(time(0));
    int numOfWalls = (rand()%side); //determina randomicamente il numero di muri
    for (int i = 0; i < numOfWalls; i++) {
        int k = (rand() % ((side/2))) +1; //determina randomicamente la lunghezza del muro, minimo 1, massimo (side/3 *2 +1)
        int base;
        do {
            base = rand() % (side * side);
        } while (b[base]!=' ' || b[base+1]=='#' || b[base-1]=='#' || b[base+side]=='#' || b[base-side]=='#'
        || b[base+side+1]=='#' || b[base+side-1]=='#' || b[base-side+1]=='#' || b[base-side-1]=='#');
        //se la casella scelta non è uno spazio o se confina con un muro o una parete
        b[base]='#';
        short direction = rand()%4;
        switch(direction){
            case 0: //basso
                while (b[base+side-1]!='#' && b[base+side]!='#' && b[base+side+1]!='#' && b[base+side+side-1]!='#' && b[base+side+side]!='#' && b[base+side+side+1]!='#' &&
                       b[base+side-1]!='_' && b[base+side]!='_' && b[base+side+1]!='_' && b[base+side+side-1]!='_' && b[base+side+side]!='_' && b[base+side+side+1]!='_' &&
                       b[base+side-1]!='o' && b[base+side]!='o' && b[base+side+1]!='o' && b[base+side+side-1]!='o' && b[base+side+side]!='o' && b[base+side+side+1]!='o' && k>0){
                    //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                    b[base+side]='#';
                    base += side;
                    k--;
                }
                break;
            case 1: //alto
                while (b[base-side-1]!='#' && b[base-side]!='#' && b[base-side+1]!='#' && b[base-side-side-1]!='#' && b[base-side-side]!='#' && b[base-side-side+1]!='#' &&
                       b[base-side-1]!='_' && b[base-side]!='_' && b[base-side+1]!='_' && b[base-side-side-1]!='_' && b[base-side-side]!='_' && b[base-side-side+1]!='_' &&
                       b[base-side-1]!='o' && b[base-side]!='o' && b[base-side+1]!='o' && b[base-side-side-1]!='o' && b[base-side-side]!='o' && b[base-side-side+1]!='o' && k>0){
                    //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                    b[base-side]='#';
                    base -= side;
                    k--;
                }
                break;
            case 2: //destra
                while (b[base-side+1]!='#' && b[base-side+2]!='#' && b[base+1]!='#' && b[base+2]!='#' && b[base+side+1]!='#' && b[base+side+2]!='#' &&
                       b[base-side+1]!='_' && b[base-side+2]!='_' && b[base+1]!='_' && b[base+2]!='_' && b[base+side+1]!='_' && b[base+side+2]!='_' &&
                       b[base-side+1]!='o' && b[base-side+2]!='o' && b[base+1]!='o' && b[base+2]!='o' && b[base+side+1]!='o' && b[base+side+2]!='o' && k>0){
                    //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                    b[base+1]='#';
                    base++;
                    k--;
                }
                break;
            case 3: //sinistra
                while (b[base-side-1]!='#' && b[base-side-2]!='#' && b[base-1]!='#' && b[base-2]!='#' && b[base+side-1]!='#' && b[base+side-2]!='#' &&
                       b[base-side-1]!='_' && b[base-side-2]!='_' && b[base-1]!='_' && b[base-2]!='_' && b[base+side-1]!='_' && b[base+side-2]!='_' &&
                       b[base-side-1]!='o' && b[base-side-2]!='o' && b[base-1]!='o' && b[base-2]!='o' && b[base+side-1]!='o' && b[base+side-2]!='o' && k>0){
                    //finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start
                    b[base-1]='#';
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

void randomWalls(char *b){ //posiziona un numero casuale di muri in posizioni casuali e di lunghezza casuale
    srand(time(0));
    int numOfWalls = (rand()%(side*4)) +10; //determina il numero di muri
    for (int i=0; i< numOfWalls; i++) {
        int k=(rand()%(side/2))+2; //determina randomicamente la lunghezza del muro, minimo 1, massimo (side/3 *2 +1)
        int base;
        do {
            base = rand()%(side*side);
        } while (b[base]!='#' || base==0 ||  base==(side-1) || base==((side*side)-1) || base==((side*side)-side+1)); //se la casella scelta e' un angolo o se non contiene '#'

        if (base<(side-1)){ //se la casella scelta fa parte della parete superiore
            while (b[base+side-1]!='#' && b[base+side]!='#' && b[base+side+1]!='#' && b[base+side+side-1]!='#' && b[base+side+side]!='#' && b[base+side+side+1]!='#' &&
                   b[base+side-1]!='_' && b[base+side]!='_' && b[base+side+1]!='_' && b[base+side+side-1]!='_' && b[base+side+side]!='_' && b[base+side+side+1]!='_' &&
                   b[base+side-1]!='o' && b[base+side]!='o' && b[base+side+1]!='o' && b[base+side+side-1]!='o' && b[base+side+side]!='o' && b[base+side+side+1]!='o' && k>0){
                   //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                b[base+side]='#';
                base += side;
                k--;
            }
        }
        else if (base>((side*side)-side+1) && base<((side*side)-1)){ //se la casella scelta fa parte della parete inferiore
            while (b[base-side-1]!='#' && b[base-side]!='#' && b[base-side+1]!='#' && b[base-side-side-1]!='#' && b[base-side-side]!='#' && b[base-side-side+1]!='#' &&
                   b[base-side-1]!='_' && b[base-side]!='_' && b[base-side+1]!='_' && b[base-side-side-1]!='_' && b[base-side-side]!='_' && b[base-side-side+1]!='_' &&
                   b[base-side-1]!='o' && b[base-side]!='o' && b[base-side+1]!='o' && b[base-side-side-1]!='o' && b[base-side-side]!='o' && b[base-side-side+1]!='o' && k>0){
                //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                b[base-side]='#';
                base -= side;
                k--;
            }
        }
        else if (base%side==0){ //se la casella scelta fa parte della parete sinistra
            while (b[base-side+1]!='#' && b[base-side+2]!='#' && b[base+1]!='#' && b[base+2]!='#' && b[base+side+1]!='#' && b[base+side+2]!='#' &&
                   b[base-side+1]!='_' && b[base-side+2]!='_' && b[base+1]!='_' && b[base+2]!='_' && b[base+side+1]!='_' && b[base+side+2]!='_' &&
                   b[base-side+1]!='o' && b[base-side+2]!='o' && b[base+1]!='o' && b[base+2]!='o' && b[base+side+1]!='o' && b[base+side+2]!='o' && k>0){
                //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                b[base+1]='#';
                base++;
                k--;
            }
        }
        else if (base%side==(side-1)){ //se la casella scelta fa parte della parete destra
            while (b[base-side-1]!='#' && b[base-side-2]!='#' && b[base-1]!='#' && b[base-2]!='#' && b[base+side-1]!='#' && b[base+side-2]!='#' &&
                   b[base-side-1]!='_' && b[base-side-2]!='_' && b[base-1]!='_' && b[base-2]!='_' && b[base+side-1]!='_' && b[base+side-2]!='_' &&
                   b[base-side-1]!='o' && b[base-side-2]!='o' && b[base-1]!='o' && b[base-2]!='o' && b[base+side-1]!='o' && b[base+side-2]!='o' && k>0){
                //finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start
                b[base-1]='#';
                base--;
                k--;
            }
        }
        else { //se la casella scelta fa parte di un muro inserito nei cicli precedenti
            k += 5;
            if (b[base+1]=='#' || b[base-1]=='#'){ //se è un muro orizzontale
                short direction = rand()%2; //scelgo un lato dal quale sviluppare un ulteriore ramo
                switch(direction){
                    case 0: //sviluppo un ramo verso il basso
                        while (b[base+side-1]!='#' && b[base+side]!='#' && b[base+side+1]!='#' && b[base+side+side-1]!='#' && b[base+side+side]!='#' && b[base+side+side+1]!='#' &&
                               b[base+side-1]!='_' && b[base+side]!='_' && b[base+side+1]!='_' && b[base+side+side-1]!='_' && b[base+side+side]!='_' && b[base+side+side+1]!='_' &&
                               b[base+side-1]!='o' && b[base+side]!='o' && b[base+side+1]!='o' && b[base+side+side-1]!='o' && b[base+side+side]!='o' && b[base+side+side+1]!='o' && k>0){
                            //finché nelle 6 caselle poste sotto la casella scelta non ci sono muri o end o start
                            b[base+side]='#';
                            base += side;
                            k--;
                        }
                        break;
                    case 1: //sviluppo un ramo verso l'alto
                        while (b[base-side-1]!='#' && b[base-side]!='#' && b[base-side+1]!='#' && b[base-side-side-1]!='#' && b[base-side-side]!='#' && b[base-side-side+1]!='#' &&
                               b[base-side-1]!='_' && b[base-side]!='_' && b[base-side+1]!='_' && b[base-side-side-1]!='_' && b[base-side-side]!='_' && b[base-side-side+1]!='_' &&
                               b[base-side-1]!='o' && b[base-side]!='o' && b[base-side+1]!='o' && b[base-side-side-1]!='o' && b[base-side-side]!='o' && b[base-side-side+1]!='o' && k>0){
                            //finché nelle 6 caselle poste sopra la casella scelta non ci sono muri o end o start
                            b[base-side]='#';
                            base -= side;
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
                        while (b[base-side+1]!='#' && b[base-side+2]!='#' && b[base+1]!='#' && b[base+2]!='#' && b[base+side+1]!='#' && b[base+side+2]!='#' &&
                               b[base-side+1]!='_' && b[base-side+2]!='_' && b[base+1]!='_' && b[base+2]!='_' && b[base+side+1]!='_' && b[base+side+2]!='_' &&
                               b[base-side+1]!='o' && b[base-side+2]!='o' && b[base+1]!='o' && b[base+2]!='o' && b[base+side+1]!='o' && b[base+side+2]!='o' && k>0){
                            //finché nelle 6 caselle poste a destra la casella scelta non ci sono muri o end o start
                            b[base+1]='#';
                            base++;
                            k--;
                        }
                        break;
                    case 1: //sviluppo un ramo verso destra
                        while (b[base-side-1]!='#' && b[base-side-2]!='#' && b[base-1]!='#' && b[base-2]!='#' && b[base+side-1]!='#' && b[base+side-2]!='#' &&
                               b[base-side-1]!='_' && b[base-side-2]!='_' && b[base-1]!='_' && b[base-2]!='_' && b[base+side-1]!='_' && b[base+side-2]!='_' &&
                               b[base-side-1]!='o' && b[base-side-2]!='o' && b[base-1]!='o' && b[base-2]!='o' && b[base+side-1]!='o' && b[base+side-2]!='o' && k>0){
                            //finché nelle 6 caselle poste a sinistra la casella scelta non ci sono muri o end o start
                            b[base-1]='#';
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

int createLabirint(char* b){
    int start = randomStart(); //genera uno start in posizione casuale
    b[start]='o';
    int end = randomEnd(start); // genera un end in posizione casuale
    b[end]='_';
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

void printLabirint(char* b, int score){ //stampa il labirinto
    for(int i = 0; i < side; i++){
        for(int j = 0; j < side; j++){
            printf("%c ", b[i*side+j]);
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

void moving(char* b, int start, int score){ //muove il giocatore
    char move;
    int currentPosition = start;
    printLabirint(&b[0], score);
    int coins = 0; //monete raccolte
    _Bool endGame=0;
    do{
        scanf(" %c", &move);
        switch(move){
            case 'W':
            case 'w':
                if (b[currentPosition -side] == ' ' || b[currentPosition - side] == '.' || b[currentPosition -side] == '$' || b[currentPosition -side] == '!'){ //se la posizione sopra la posizione attuale è parte del labirinto
                    b[currentPosition] = '.';
                    if (b[currentPosition - side] == '!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (b[currentPosition - side] == '$'){ // se incontra una moneta
                        score += 3;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    b[currentPosition - side] = 'o';
                    currentPosition = currentPosition - side; //nuova pos. attuale
                    system("clear");
                    printLabirint(&b[0], score);
                }
                else if (b[currentPosition - side] == '_') { //se ho completato il percorso
                    b[currentPosition] = '.';
                    b[currentPosition - side] = 'o';
                    score--;
                    system("clear");
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
                if (b[currentPosition +side] == ' ' || b[currentPosition + side] == '.' || b[currentPosition +side] == '$' || b[currentPosition +side] == '!'){ //se la posizione sotto la posizione attuale è parte del labirinto
                    b[currentPosition] = '.';
                    if (b[currentPosition + side] == '!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (b[currentPosition + side] == '$'){ // se incontra una moneta
                        score += 3;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    b[currentPosition + side] = 'o';
                    currentPosition = currentPosition + side; //nuova pos. attuale
                    system("clear");
                    printLabirint(&b[0], score);
                }

                else if (b[currentPosition + side] == '_') { //se ho completato il percorso
                    b[currentPosition] = '.';
                    b[currentPosition + side] = 'o';
                    score--;
                    system("clear");
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
                if (b[currentPosition +1] == ' ' || b[currentPosition + 1] == '.' || b[currentPosition +1] == '$' || b[currentPosition +1] == '!'){ //se la posizione a destra della posizione attuale è parte del labirinto
                    b[currentPosition] = '.';
                    if (b[currentPosition + 1] == '!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (b[currentPosition + 1] == '$'){ // se incontra una moneta
                        score += 3;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    b[currentPosition + 1] = 'o';
                    currentPosition = currentPosition + 1; //nuova pos. attuale
                    system("clear");
                    printLabirint(&b[0], score);
                }
                else if (b[currentPosition + 1] == '_') { //se ho completato il percorso
                    b[currentPosition] = '.';
                    b[currentPosition + 1] = 'o';
                    score--;
                    system("clear");
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
                if  (b[currentPosition - 1] == ' ' || b[currentPosition - 1] == '.' || b[currentPosition -1] == '$' || b[currentPosition -1] == '!'){ //se la posizione a sinistra della posizione attuale è parte del labirinto
                    b[currentPosition] = '.';
                    if (b[currentPosition - 1] == '!'){ //se incontra un imprevisto
                        score -= coins*3/2; //toglie il punteggio
                        coins = coins/2; //dimezza numero di monete
                    }
                    else if (b[currentPosition - 1] == '$'){ // se incontra una moneta
                        score += 3;
                        coins ++;
                    }
                    else { //altrimenti
                        score--;
                    }
                    b[currentPosition - 1] = 'o';
                    currentPosition = currentPosition - 1; //nuova pos. attuale
                    system("clear");
                    printLabirint(&b[0], score);
                }
                else if (b[currentPosition - 1] == '_') { //se ho completato il percorso
                    b[currentPosition] = '.';
                    b[currentPosition - 1] = 'o';
                    score--;
                    system("clear");
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
    int score=0;
    char board[side*side];
    createBoard(&board[0]);
    int start = createLabirint(&board[0]);
    char mode = welcome();
    mode = 'b';
    if (mode == 'b' || mode == 'B'){
        guide();
        moving(&board[0], start, score);
    }
    else if(mode == 'a' || mode == 'A'){
        //to be defined
    }
    return 0;
}
