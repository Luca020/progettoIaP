#include <stdio.h>
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