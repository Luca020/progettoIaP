#include <stdlib.h>
#include <string.h>
#include "boardOperations.h"

void moving (Box* board, int start, int score, char* movesString, size_t xSide, size_t ySide);
/*
void firstMove(Box* board, int start, int* end, size_t xSide, size_t ySide){
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
        else if (board[start+xSide].symbol == 'T'){
            board[start + xSide].numDrills += 3;
        }
        else {
            board[start + xSide].numCoins = board[start].numCoins;
            board[start + xSide].numDrills = board[start].numDrills;
        }
        candidates[size] = board[start + xSide];
        size++;
    }
}
*/
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
    int xDist = malloc(sizeof(int));
    int yDist = malloc(sizeof(int));
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
    board[start].numDrills = 0;

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
        else if (board[start+xSide].symbol == 'T'){
            board[start + xSide].numDrills += 3;
        }
        else {
            board[start + xSide].numCoins = board[start].numCoins;
            board[start + xSide].numDrills = board[start].numDrills;
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
        else if (board[start - xSide].symbol == 'T'){
            board[start - xSide].numDrills += 3;
        }
        else {
            board[start - xSide].numCoins = board[start].numCoins;
            board[start - xSide].numDrills = board[start].numDrills;
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
        else if (board[start + 1].symbol == 'T'){
            board[start + 1].numDrills += 3;
        }
        else {
            board[start + 1].numCoins = board[start].numCoins;
            board[start + 1].numDrills = board[start].numDrills;
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
        if (board[start - 1].symbol == '$'){
            board[start - 1].numCoins = board[start].numCoins +1;
        }
        else if (board[start - 1].symbol == 'T'){
            board[start - 1].numDrills += 3;
        }
        else {
            board[start - 1].numCoins = board[start].numCoins;
            board[start - 1].numDrills = board[start].numDrills;
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
        int t = candidates[i].reference; // t rappresenta la posizione nel labirinto della casella estratta tra i candidati
        board[t].predecessor=candidates[i].predecessor;
        board[t].hCost=candidates[i].hCost;
        board[t].gCost=candidates[i].gCost;

        /*    printf("\n\n!board[t - xSide].visited = %d,\nt>=xSide = %d\nboard[t - xSide].symbol = %c\nboard[t].predecessor != t -xSide = %d\n\n"
                    , !board[t - xSide].visited, t>=xSide,board[t - xSide].symbol, board[t].predecessor != t -xSide);*/

        if (!board[t - xSide].visited && t - xSide > 0 && (board[t].numDrills > 0 || board[t - xSide].symbol != '#') && board[t - xSide].symbol != 'o' && board[t].predecessor != t -xSide) { //casella alta se essa non è un muro e se non è il predecessore del nodo predecessore
            //printf("Sono entrato nell'if alto\n");
            board[t - xSide].numCoins = board[t].numCoins;
            board[t - xSide].numDrills = board[t].numDrills;
            if (board[t - xSide].symbol == '#'){
                board[t - xSide].numDrills -= 1;
            }
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
            else if (board[t - xSide].symbol == 'T'){
                board[t - xSide].numDrills += 3;
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

        if (!board[t + 1].visited && (t + 1)%xSide != 0 && (board[t].numDrills > 0 || board[t + 1].symbol != '#') && board[t + 1].symbol != 'o' && board[t].predecessor != t +1) { //casella a destra se essa non è un muro e se non è il predecessore del nodo predecessore
            //     printf("Sono entrato nell'if destro\n");
            board[t + 1].numCoins = board[t].numCoins;
            board[t + 1].numDrills = board[t].numDrills;
            if (board[t + 1].symbol == '#'){
                board[t + 1].numDrills -= 1;
            }
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
            else if (board[t + 1].symbol == 'T'){
                board[t + 1].numDrills += 3;
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

        if (!board[t + xSide].visited && t<xSide*ySide && (board[t].numDrills > 0 || board[t + xSide].symbol != '#') && board[t + xSide].symbol != 'o' && board[t].predecessor != t +xSide) { //casella bassa se essa non è un muro e se non è il predecessore del nodo predecessore
            //     printf("Sono entrato nell'if basso\n");
            board[t + xSide].numCoins = board[t].numCoins;
            board[t + xSide].numDrills = board[t].numDrills;
            if (board[t + xSide].symbol == '#'){
                board[t + xSide].numDrills -= 1;
            }
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
            else if (board[t + xSide].symbol == 'T'){
                board[t + xSide].numDrills += 3;
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

        if (!board[t - 1].visited && (t - 1)%xSide != (xSide-1) && (board[t].numDrills > 0 || board[t - 1].symbol != '#') && board[t - 1].symbol != 'o' && board[t].predecessor != t -1) { //casella a sinistra se essa non è un muro e se non è il predecessore del nodo predecessore
            //      printf("Sono entrato nell'if sinistro\n");
            board[t - 1].numCoins = board[t].numCoins;
            board[t - 1].numDrills = board[t].numDrills;
            if (board[t - 1].symbol == '#'){
                board[t - 1].numDrills -= 1;
            }
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
            else if (board[t - 1].symbol == 'T'){
                board[t - 1].numDrills += 3;
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
        //printf("board[%d].numDrills = %d\n", k, board[k].numDrills);
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

    //printLabirint(board,score, xSide, ySide);
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
    //printLabirint(board, score, xSide, ySide);
    //printf("Numero di monete al termine della partita: %d\n", board[*end].numCoins);
    //printf("Numero di mosse della partita: %d\n", j);
}