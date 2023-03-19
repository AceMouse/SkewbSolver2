#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum faces{FRONT, RIGHT, BACK, LEFT, TOP, BOTTOM, FACES};
enum positions{TOP_LEFT, TOP_RIGHT, MIDDLE, BOTTOM_LEFT, BOTTOM_RIGHT, POSITIONS};
enum colours{ORANGE, GREEN, RED, BLUE, WHITE, YELLOW, COLOURS};
enum pivots{FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT, PIVOTS};

char* FACE_NAME[] = {"FRONT", "RIGHT", "BACK", "LEFT", "TOP", "BOTTOM"};
char* POSITION_NAME[] = {"TOP_LEFT", "TOP_RIGHT", "MIDDLE", "BOTTOM_LEFT", "BOTTOM_RIGHT"};
char* COLOUR_NAME[] = {"ORANGE", "GREEN", "RED", "BLUE", "WHITE", "YELLOW"};

#define TO_IDX(f,p) ((f)*POSITIONS+(p))
#define CLOCKWISE 0
#define ANTI_CLOCKWISE 1

int is_valid(uint8_t* cube);
uint8_t is_solved(uint8_t* cube);
uint8_t diff_from_solved(uint8_t* cube);
void init_cube(uint8_t* cube);
void print_cube(uint8_t* cube);
void turn_fl(uint8_t* cube);
void turn_fr(uint8_t* cube);
void turn_bl(uint8_t* cube);
void turn_br(uint8_t* cube);
void turn(uint8_t* cube, uint8_t pivot, uint8_t anti_clockwise);
void scramble_cube(uint8_t* cube, uint8_t moves);

int main(){
    uint8_t cube[POSITIONS*FACES];
    init_cube(cube);
    printf("%i\n",is_solved(cube));
    scramble_cube(cube, 20);
    printf("%i\n",is_solved(cube));
    print_cube(cube);
    diff_from_solved(cube);
    printf("%i\n",is_valid(cube));
    return 0;
}

int is_valid(uint8_t* cube){
    uint8_t counts[COLOURS] = {0};
    for (int i = 0; i < FACES*POSITIONS; i++){
        counts[cube[i]]++;
    }
    uint8_t ret = 1;
    for (int i = 0; i<COLOURS; i++){
        if (counts[i] != POSITIONS){
            ret = 0;
#ifdef DEBUG
            printf("WRONG NUMBER OF %-7s: %i\n",COLOUR_NAME[i], counts[i]);
#endif /* DEBUG */
        }
    }
    return ret;
}

uint8_t is_solved(uint8_t* cube){
    uint8_t res = is_valid(cube);
    for (int f = 0; f<FACES; f++){
        uint8_t colour_for_face = cube[TO_IDX(f,0)]; 
        for (int p = 0; p<POSITIONS; p++){
            res &= colour_for_face == cube[TO_IDX(f,p)];
        }
    }
    return res;
}

uint8_t diff_from_solved(uint8_t* cube){
    uint8_t res = 0;
    for (int f = 0; f<FACES; f++){
        for (int p = 0; p<POSITIONS; p++){
            if (cube[TO_IDX(f,p)] != f){
#ifdef DEBUG
                printf("(%-6s , %-12s ): %-7s instead of %s\n", FACE_NAME[f], POSITION_NAME[p],COLOUR_NAME[cube[TO_IDX(f,p)]],COLOUR_NAME[f]);
#endif /* DEBUG */
                res++;
            }
        }
    }
    return res;
}

void init_cube(uint8_t* cube){
    for (int f = 0; f<FACES; f++){
        for (int p = 0; p<POSITIONS; p++){
            cube[TO_IDX(f,p)] = f;
        }
    }
}

void print_cube(uint8_t* cube){
    for (int f = 0; f<FACES; f++){
        for (int p = 0; p<POSITIONS; p++){
            printf("(%-6s , %-12s ): %s\n", FACE_NAME[f], POSITION_NAME[p],COLOUR_NAME[cube[TO_IDX(f,p)]]);
        }
    }
    printf("-------------------------------------\n");
}

void turn_fl(uint8_t* cube){
    uint8_t old[FACES*POSITIONS];
    memcpy(old, cube, FACES*POSITIONS);
    
    cube[TO_IDX(FRONT,TOP_LEFT)]    = old[TO_IDX(TOP,BOTTOM_LEFT)];
    cube[TO_IDX(FRONT,TOP_RIGHT)]   = old[TO_IDX(TOP,TOP_LEFT)];
    cube[TO_IDX(FRONT,MIDDLE)]      = old[TO_IDX(TOP,MIDDLE)];
    cube[TO_IDX(FRONT,BOTTOM_LEFT)] = old[TO_IDX(TOP,BOTTOM_RIGHT)];

    cube[TO_IDX(LEFT,TOP_LEFT)]     = old[TO_IDX(FRONT,BOTTOM_LEFT)];
    cube[TO_IDX(LEFT,TOP_RIGHT)]    = old[TO_IDX(FRONT,TOP_LEFT)];
    cube[TO_IDX(LEFT,MIDDLE)]       = old[TO_IDX(FRONT,MIDDLE)];
    cube[TO_IDX(LEFT,BOTTOM_RIGHT)] = old[TO_IDX(FRONT,TOP_RIGHT)];

    cube[TO_IDX(TOP,TOP_LEFT)]      = old[TO_IDX(LEFT,BOTTOM_RIGHT)];
    cube[TO_IDX(TOP,MIDDLE)]        = old[TO_IDX(LEFT,MIDDLE)];
    cube[TO_IDX(TOP,BOTTOM_LEFT)]   = old[TO_IDX(LEFT,TOP_RIGHT)];
    cube[TO_IDX(TOP,BOTTOM_RIGHT)]  = old[TO_IDX(LEFT,TOP_LEFT)];

    cube[TO_IDX(RIGHT,TOP_LEFT)]    = old[TO_IDX(BACK,TOP_RIGHT)];

    cube[TO_IDX(BACK,TOP_RIGHT)]    = old[TO_IDX(BOTTOM,TOP_LEFT)];

    cube[TO_IDX(BOTTOM,TOP_LEFT)]   = old[TO_IDX(RIGHT,TOP_LEFT)];
}

void turn_fr(uint8_t* cube){
    uint8_t old[FACES*POSITIONS];
    memcpy(old, cube, FACES*POSITIONS);
    
    cube[TO_IDX(FRONT,TOP_LEFT)]    = old[TO_IDX(RIGHT,BOTTOM_LEFT)];
    cube[TO_IDX(FRONT,TOP_RIGHT)]   = old[TO_IDX(RIGHT,TOP_LEFT)];
    cube[TO_IDX(FRONT,MIDDLE)]      = old[TO_IDX(RIGHT,MIDDLE)];
    cube[TO_IDX(FRONT,BOTTOM_RIGHT)]= old[TO_IDX(RIGHT,TOP_RIGHT)];

    cube[TO_IDX(RIGHT,TOP_LEFT)]    = old[TO_IDX(TOP,BOTTOM_RIGHT)];
    cube[TO_IDX(RIGHT,TOP_RIGHT)]   = old[TO_IDX(TOP,BOTTOM_LEFT)];
    cube[TO_IDX(RIGHT,MIDDLE)]      = old[TO_IDX(TOP,MIDDLE)];
    cube[TO_IDX(RIGHT,BOTTOM_LEFT)] = old[TO_IDX(TOP,TOP_RIGHT)];

    cube[TO_IDX(TOP,TOP_RIGHT)]     = old[TO_IDX(FRONT,TOP_LEFT)];
    cube[TO_IDX(TOP,MIDDLE)]        = old[TO_IDX(FRONT,MIDDLE)];
    cube[TO_IDX(TOP,BOTTOM_LEFT)]   = old[TO_IDX(FRONT,BOTTOM_RIGHT)];
    cube[TO_IDX(TOP,BOTTOM_RIGHT)]  = old[TO_IDX(FRONT,TOP_RIGHT)];
    
    cube[TO_IDX(LEFT,TOP_RIGHT)]    = old[TO_IDX(BOTTOM,TOP_RIGHT)];

    cube[TO_IDX(BOTTOM,TOP_RIGHT)]  = old[TO_IDX(BACK,TOP_LEFT)];

    cube[TO_IDX(BACK,TOP_LEFT)]     = old[TO_IDX(LEFT,TOP_RIGHT)];
}

void turn_bl(uint8_t* cube){
    uint8_t old[FACES*POSITIONS];
    memcpy(old, cube, FACES*POSITIONS);
    
    cube[TO_IDX(TOP,TOP_LEFT)]      = old[TO_IDX(BACK,TOP_RIGHT)]; 
    cube[TO_IDX(TOP,TOP_RIGHT)]     = old[TO_IDX(BACK,BOTTOM_RIGHT)];
    cube[TO_IDX(TOP,MIDDLE)]        = old[TO_IDX(BACK,MIDDLE)];
    cube[TO_IDX(TOP,BOTTOM_LEFT)]   = old[TO_IDX(BACK,TOP_LEFT)];
   
    cube[TO_IDX(LEFT,TOP_LEFT)]     = old[TO_IDX(TOP,TOP_LEFT)];
    cube[TO_IDX(LEFT,TOP_RIGHT)]    = old[TO_IDX(TOP,TOP_RIGHT)];
    cube[TO_IDX(LEFT,MIDDLE)]       = old[TO_IDX(TOP,MIDDLE)];
    cube[TO_IDX(LEFT,BOTTOM_LEFT)]  = old[TO_IDX(TOP,BOTTOM_LEFT)];
   
    cube[TO_IDX(BACK,TOP_LEFT)]     = old[TO_IDX(LEFT,BOTTOM_LEFT)];
    cube[TO_IDX(BACK,TOP_RIGHT)]    = old[TO_IDX(LEFT,TOP_LEFT)];
    cube[TO_IDX(BACK,MIDDLE)]       = old[TO_IDX(LEFT,MIDDLE)];
    cube[TO_IDX(BACK,BOTTOM_RIGHT)] = old[TO_IDX(LEFT,TOP_RIGHT)];
    
    cube[TO_IDX(RIGHT,TOP_RIGHT)]   = old[TO_IDX(BOTTOM,BOTTOM_LEFT)];

    cube[TO_IDX(BOTTOM,BOTTOM_LEFT)]= old[TO_IDX(FRONT,TOP_LEFT)];

    cube[TO_IDX(FRONT,TOP_LEFT)]    = old[TO_IDX(RIGHT,TOP_RIGHT)];
}

void turn_br(uint8_t* cube){
    uint8_t old[FACES*POSITIONS];
    memcpy(old, cube, FACES*POSITIONS);

    cube[TO_IDX(TOP,TOP_LEFT)]      = old[TO_IDX(RIGHT,TOP_RIGHT)]; 
    cube[TO_IDX(TOP,TOP_RIGHT)]     = old[TO_IDX(RIGHT,BOTTOM_RIGHT)];
    cube[TO_IDX(TOP,MIDDLE)]        = old[TO_IDX(RIGHT,MIDDLE)];
    cube[TO_IDX(TOP,BOTTOM_RIGHT)]  = old[TO_IDX(RIGHT,TOP_LEFT)];
    
    cube[TO_IDX(RIGHT,TOP_LEFT)]    = old[TO_IDX(BACK,BOTTOM_LEFT)];
    cube[TO_IDX(RIGHT,TOP_RIGHT)]   = old[TO_IDX(BACK,TOP_LEFT)];
    cube[TO_IDX(RIGHT,MIDDLE)]      = old[TO_IDX(BACK,MIDDLE)];
    cube[TO_IDX(RIGHT,BOTTOM_RIGHT)]= old[TO_IDX(BACK,TOP_RIGHT)];
    
    cube[TO_IDX(BACK,TOP_LEFT)]     = old[TO_IDX(TOP,TOP_RIGHT)];
    cube[TO_IDX(BACK,TOP_RIGHT)]    = old[TO_IDX(TOP,BOTTOM_RIGHT)];
    cube[TO_IDX(BACK,MIDDLE)]       = old[TO_IDX(TOP,MIDDLE)];
    cube[TO_IDX(BACK,BOTTOM_LEFT)]  = old[TO_IDX(TOP,TOP_LEFT)];
    
    cube[TO_IDX(LEFT,TOP_LEFT)]     = old[TO_IDX(BOTTOM,BOTTOM_RIGHT)];

    cube[TO_IDX(BOTTOM,BOTTOM_RIGHT)]= old[TO_IDX(FRONT,TOP_RIGHT)];

    cube[TO_IDX(FRONT,TOP_RIGHT)]   = old[TO_IDX(LEFT,TOP_LEFT)];
}

void turn(uint8_t* cube, uint8_t pivot, uint8_t anti_clockwise){
    for(int i = 0; i<=anti_clockwise; i++){
        switch(pivot){
            case FRONT_LEFT:
                turn_fl(cube);
                break;
            case FRONT_RIGHT:
                turn_fr(cube);
                break;
            case BACK_LEFT:
                turn_bl(cube);
                break;
            case BACK_RIGHT:
                turn_br(cube);
                break;
        }
    }
}

void scramble_cube(uint8_t* cube, uint8_t moves){
    while(moves--){
        uint8_t pivot = rand()%PIVOTS;
        uint8_t anti_clockwise = rand()%2;
        turn(cube, pivot, anti_clockwise);
    }
}
