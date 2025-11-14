
#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

// TY TSODING <3 section
#define da_append(da, item)                  \
    do {                                     \
        da_reserve((da), (da)->count + 1);   \
        (da)->items[(da)->count++] = (item); \
    } while (0)

#define da_reserve(da, expected_capacity)                                                  \
    do {                                                                                   \
        if ((expected_capacity) > (da)->capacity) {                                        \
            if ((da)->capacity == 0) {                                                     \
                (da)->capacity = 16;                                                       \
            }                                                                              \
            while ((expected_capacity) > (da)->capacity) {                                 \
                (da)->capacity *= 2;                                                       \
            }                                                                              \
            (da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items));  /* NOLINT Realloc may leak but since we assert below is safe! */   \
            assert((da)->items != NULL && "Buy more RAM lol");                             \
        }                                                                                  \
    } while (0)
// END TY TSODING <3 section


#define Face uint32_t

typedef enum { White, Orange, Green, Red, Blue, Yellow, ECOLOR_LENGTH } EColor;

typedef enum
{
    F_U = 1,
    F_L = 2,
    F_F = 4,
    F_R = 8,
    F_B = 16,
    F_D = 32,
    EFACE_LENGTH
} EFace;

typedef enum
{
    NO_MOVE,
    U, Up, UU,
    L, Lp, LL,
    F, Fp, FF,
    R, Rp, RR,
    B, Bp, BB,
    D, Dp, DD,
    MOVE_LENGTH
} Move;

typedef struct Rotation
{
    EFace face;     // 0 for none; Since many moves rotate the same face this is handy
    Move move;      // clone current move to modify Moves as freely
    float angle;    // degrees
    float progress; // [0, 1)
} Rotation;

typedef struct Cube
{
    Face faces[EFACE_LENGTH];
    Rotation rotation;
} Cube;

typedef struct Moves
{
    Move* items;
    size_t current;
    size_t count;
    size_t capacity;
} Moves;

Cube newCube(void);
void resetCube(Cube* c);
void rotateCube(Cube* c, Move m);

// Helpers for conversions
EFace moveToFace(Move m);
float moveMaxAngle(Move m);
const char* moveToStr(Move m);
void strToMoves(const char* str, Moves* mv); // str is null terminated!!
int f2i(EFace f);    // Face to index
EColor f2c(EFace f); // Get the color of a face
EColor getCell(const Face* f, uint8_t cell);

#endif //MODEL_H
