
#include <stdint.h>

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
    U, Up, U2,
    L, Lp, L2,
    F, Fp, F2,
    R, Rp, R2,
    B, Bp, B2,
    D, Dp, D2,
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

// Face to index
int f2i(const EFace f)
{
    switch (f)
    {
    case F_U: return 0;
    case F_L: return 1;
    case F_F: return 2;
    case F_R: return 3;
    case F_B: return 4;
    case F_D: return 5;
    default: break;
    }

    TraceLog(LOG_FATAL, "Unknown face %d\n", f); // Will abort with exit(EXIT_FAILURE)
    __builtin_unreachable();
}

EColor getCell(const Face* f, const uint8_t cell)
{
    const int width = 4; // bits required for a color
    const int mask = 0xF;
    int offset = 0;

    if (cell & F_U && cell & F_L)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell == F_U)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell & F_U && cell & F_R)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell == F_L)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell == F_R)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell & F_D && cell & F_L)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell == F_D)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell & F_D && cell & F_R)
    {
        return (*f >> offset) & mask;
    }

    return White;
}

const char* moveToStr(const Move m)
{
    switch (m)
    {
    case U:  return "U";
    case Up: return "U'";
    case U2: return "U2";
    case L:  return "L";
    case Lp: return "L'";
    case L2: return "L2";
    case F:  return "F";
    case Fp: return "F'";
    case F2: return "F2";
    case R:  return "R";
    case Rp: return "R'";
    case R2: return "R2";
    case B:  return "B";
    case Bp: return "B'";
    case B2: return "B2";
    case D:  return "D";
    case Dp: return "D'";
    case D2: return "D2";
    default: return "";
    }
}
