#include <string.h>

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