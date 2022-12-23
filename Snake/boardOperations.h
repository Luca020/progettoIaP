/** Ciao
 * porcoddio sono dentro la libreria
*/

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

void printLabirint(Box* board, int score, size_t xSide, size_t ySide);
void createBoard(Box* board, size_t xSide, size_t ySide);
int createLabirint(Box* board, int * end, size_t xSide, size_t ySide, int difficulty);