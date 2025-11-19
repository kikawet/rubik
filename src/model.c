#include "model.h"
#include <raylib.h>
#include <string.h>

/// CUBE section

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

    TraceLog(LOG_FATAL, "Unknown index face: %d", f); // Will abort with exit(EXIT_FAILURE)
    __builtin_unreachable();
}

EColor f2c(const EFace f)
{
    switch (f)
    {
    case F_U: return White;
    case F_L: return Orange;
    case F_F: return Green;
    case F_R: return Red;
    case F_B: return Blue;
    case F_D: return Yellow;
    default: break;
    }

    TraceLog(LOG_FATAL, "Unknown color for face: %d", f); // Will abort with exit(EXIT_FAILURE)
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

void paintCell(Face* f, const uint8_t cell, const EColor color)
{
    assert(ECOLOR_LENGTH <= 6 && "check width is enough to fit all possible colors in a face");
    const int width = 4; // bits required for a color
    int offset = 0;

    Face mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell & F_U && cell & F_L)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell == F_U)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell & F_U && cell & F_R)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell == F_L)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell == F_R)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell & F_D && cell & F_L)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell == F_D)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = 0 | ~(~0U << offset);
    if (cell & F_D && cell & F_R)
    {
        *f = (*f & mask) | color << offset;
    }
}

void setFaceColor(Face* f, const EColor color)
{
    *f = 0;

    paintCell(f, F_U | F_L, color);
    paintCell(f, F_U, color);
    paintCell(f, F_U | F_R, color);

    paintCell(f, F_L, color);
    paintCell(f, F_R, color);

    paintCell(f, F_D | F_L, color);
    paintCell(f, F_D, color);
    paintCell(f, F_D | F_R, color);
}

Cube newCube(void)
{
    Cube c = {0};

    setFaceColor(&c.faces[f2i(F_U)], f2c(F_U));
    setFaceColor(&c.faces[f2i(F_F)], f2c(F_F));
    setFaceColor(&c.faces[f2i(F_B)], f2c(F_B));
    setFaceColor(&c.faces[f2i(F_D)], f2c(F_D));
    setFaceColor(&c.faces[f2i(F_L)], f2c(F_L));
    setFaceColor(&c.faces[f2i(F_R)], f2c(F_R));

    return c;
}

void resetCube(Cube* c)
{
    setFaceColor(&c->faces[f2i(F_U)], f2c(F_U));
    setFaceColor(&c->faces[f2i(F_F)], f2c(F_F));
    setFaceColor(&c->faces[f2i(F_B)], f2c(F_B));
    setFaceColor(&c->faces[f2i(F_D)], f2c(F_D));
    setFaceColor(&c->faces[f2i(F_L)], f2c(F_L));
    setFaceColor(&c->faces[f2i(F_R)], f2c(F_R));
}

/// Movements section

const char* moveToStr(const Move m)
{
    switch (m)
    {
    case U: return "U";
    case Up: return "U'";
    case UU: return "U2";
    case L: return "L";
    case Lp: return "L'";
    case LL: return "L2";
    case F: return "F";
    case Fp: return "F'";
    case FF: return "F2";
    case R: return "R";
    case Rp: return "R'";
    case RR: return "R2";
    case B: return "B";
    case Bp: return "B'";
    case BB: return "B2";
    case D: return "D";
    case Dp: return "D'";
    case DD: return "D2";
    default: return "";
    }
}

void strToMoves(const char* str, Moves* mv)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (strncmp(str + i, "U'", 2) == 0)
        {
            da_append(mv, Up);
            i++;
        }
        else if (strncmp(str + i, "L'", 2) == 0)
        {
            da_append(mv, Lp);
            i++;
        }
        else if (strncmp(str + i, "F'", 2) == 0)
        {
            da_append(mv, Fp);
            i++;
        }
        else if (strncmp(str + i, "R'", 2) == 0)
        {
            da_append(mv, Rp);
            i++;
        }
        else if (strncmp(str + i, "B'", 2) == 0)
        {
            da_append(mv, Bp);
            i++;
        }
        else if (strncmp(str + i, "D'", 2) == 0)
        {
            da_append(mv, Dp);
            i++;
        }
        else if (strncmp(str + i, "U2", 2) == 0)
        {
            da_append(mv, UU);
            i++;
        }
        else if (strncmp(str + i, "L2", 2) == 0)
        {
            da_append(mv, LL);
            i++;
        }
        else if (strncmp(str + i, "F2", 2) == 0)
        {
            da_append(mv, FF);
            i++;
        }
        else if (strncmp(str + i, "R2", 2) == 0)
        {
            da_append(mv, RR);
            i++;
        }
        else if (strncmp(str + i, "B2", 2) == 0)
        {
            da_append(mv, BB);
            i++;
        }
        else if (strncmp(str + i, "D2", 2) == 0)
        {
            da_append(mv, DD);
            i++;
        }
        else if (strncmp(str + i, "U", 1) == 0)
        {
            da_append(mv, U);
        }
        else if (strncmp(str + i, "L", 1) == 0)
        {
            da_append(mv, L);
        }
        else if (strncmp(str + i, "F", 1) == 0)
        {
            da_append(mv, F);
        }
        else if (strncmp(str + i, "R", 1) == 0)
        {
            da_append(mv, R);
        }
        else if (strncmp(str + i, "B", 1) == 0)
        {
            da_append(mv, B);
        }
        else if (strncmp(str + i, "D", 1) == 0)
        {
            da_append(mv, D);
        }

        // Invalid move!
        i++;
    }
}

// degrees angles are reversed bc raylib conversions are hard :(
float moveMaxAngle(const Move m)
{
    switch (m)
    {
    case U:
    case L:
    case F:
    case R:
    case B:
    case D:
        return -90.f;
    case Up:
    case Lp:
    case Fp:
    case Rp:
    case Bp:
    case Dp:
        return 90.f;
    case UU:
    case LL:
    case FF:
    case RR:
    case BB:
    case DD:
        return -180.f;
    default:
        return 0.f;
    }
}

EFace moveToFace(const Move m)
{
    switch (m)
    {
    case NO_MOVE: return 0;
    case U:
    case Up:
    case UU:
        return F_U;
    case L:
    case Lp:
    case LL:
        return F_L;
    case F:
    case Fp:
    case FF:
        return F_F;
    case R:
    case Rp:
    case RR:
        return F_R;
    case B:
    case Bp:
    case BB:
        return F_B;
    case D:
    case Dp:
    case DD:
        return F_D;
    default: break;
    }

    TraceLog(LOG_FATAL, "Invalid move %d", m); // Will abort with exit(EXIT_FAILURE)
    __builtin_unreachable();
}

void copyCell(Face* dst, const uint8_t cellDst, const Face* src, const uint8_t cellSrc)
{
    const EColor color = getCell(src, cellSrc);
    paintCell(dst, cellDst, color);
}

void rotateUpCCW(Cube* cube)
{
    // Update Up face
    const Face up_before = cube->faces[f2i(F_U)];
    Face* up = &cube->faces[f2i(F_U)];
    copyCell(up, F_U | F_L, up, F_U | F_R);
    copyCell(up, F_U, up, F_R);
    copyCell(up, F_U | F_R, up, F_D | F_R);
    copyCell(up, F_R, up, F_D);
    copyCell(up, F_D | F_R, up, F_D | F_L);
    copyCell(up, F_D, up, F_L);
    copyCell(up, F_D | F_L, &up_before, F_U | F_L);
    copyCell(up, F_L, &up_before, F_U);

    // Update adjacent faces
    const Face back_before = cube->faces[f2i(F_B)];
    Face* back = &cube->faces[f2i(F_B)];
    Face* right = &cube->faces[f2i(F_R)];
    Face* front = &cube->faces[f2i(F_F)];
    Face* left = &cube->faces[f2i(F_L)];

    copyCell(back, F_U | F_R, right, F_U | F_R);
    copyCell(back, F_U, right, F_U);
    copyCell(back, F_U | F_L, right, F_U | F_L);

    copyCell(right, F_U | F_R, front, F_U | F_R);
    copyCell(right, F_U, front, F_U);
    copyCell(right, F_U | F_L, front, F_U | F_L);

    copyCell(front, F_U | F_R, left, F_U | F_R);
    copyCell(front, F_U, left, F_U);
    copyCell(front, F_U | F_L, left, F_U | F_L);

    copyCell(left, F_U | F_R, &back_before, F_U | F_R);
    copyCell(left, F_U, &back_before, F_U);
    copyCell(left, F_U | F_L, &back_before, F_U | F_L);
}

void rotateUpCW(Cube* cube)
{
    rotateUpCCW(cube);
    rotateUpCCW(cube);
    rotateUpCCW(cube);
}

void rotateDownCCW(Cube* cube)
{
    // Update Down face
    const Face down_before = cube->faces[f2i(F_D)];
    Face* down = &cube->faces[f2i(F_D)];
    copyCell(down, F_U | F_L, down, F_U | F_R);
    copyCell(down, F_U, down, F_R);
    copyCell(down, F_U | F_R, down, F_D | F_R);
    copyCell(down, F_R, down, F_D);
    copyCell(down, F_D | F_R, down, F_D | F_L);
    copyCell(down, F_D, down, F_L);
    copyCell(down, F_D | F_L, &down_before, F_U | F_L);
    copyCell(down, F_L, &down_before, F_U);

    // Update adjacent faces
    const Face front_before = cube->faces[f2i(F_B)];
    Face* front = &cube->faces[f2i(F_B)];
    Face* left = &cube->faces[f2i(F_L)];
    Face* back = &cube->faces[f2i(F_F)];
    Face* right = &cube->faces[f2i(F_R)];

    copyCell(front, F_D | F_R, left, F_D | F_R);
    copyCell(front, F_D, left, F_D);
    copyCell(front, F_D | F_L, left, F_D | F_L);

    copyCell(left, F_D | F_R, back, F_D | F_R);
    copyCell(left, F_D, back, F_D);
    copyCell(left, F_D | F_L, back, F_D | F_L);

    copyCell(back, F_D | F_R, right, F_D | F_R);
    copyCell(back, F_D, right, F_D);
    copyCell(back, F_D | F_L, right, F_D | F_L);

    copyCell(right, F_D | F_R, &front_before, F_D | F_R);
    copyCell(right, F_D, &front_before, F_D);
    copyCell(right, F_D | F_L, &front_before, F_D | F_L);
}

void rotateDownCW(Cube* cube)
{
    rotateDownCCW(cube);
    rotateDownCCW(cube);
    rotateDownCCW(cube);
}

void rotateFrontCCW(Cube* cube)
{
    // Update Front face
    const Face front_before = cube->faces[f2i(F_F)];
    Face* front = &cube->faces[f2i(F_F)];
    copyCell(front, F_U | F_L, front, F_U | F_R);
    copyCell(front, F_U, front, F_R);
    copyCell(front, F_U | F_R, front, F_D | F_R);
    copyCell(front, F_R, front, F_D);
    copyCell(front, F_D | F_R, front, F_D | F_L);
    copyCell(front, F_D, front, F_L);
    copyCell(front, F_D | F_L, &front_before, F_U | F_L);
    copyCell(front, F_L, &front_before, F_U);

    // Update adjacent faces
    const Face up_before = cube->faces[f2i(F_U)];
    Face* up = &cube->faces[f2i(F_U)];
    Face* right = &cube->faces[f2i(F_R)];
    Face* down = &cube->faces[f2i(F_D)];
    Face* left = &cube->faces[f2i(F_L)];

    copyCell(up, F_D | F_L, right, F_L | F_U);
    copyCell(up, F_D, right, F_L);
    copyCell(up, F_D | F_R, right, F_L | F_D);

    copyCell(right, F_L | F_U, down, F_U | F_R);
    copyCell(right, F_L, down, F_U);
    copyCell(right, F_L | F_D, down, F_U | F_L);

    copyCell(down, F_U | F_R, left, F_R | F_D);
    copyCell(down, F_U, left, F_R);
    copyCell(down, F_U | F_L, left, F_R | F_U);

    copyCell(left, F_R | F_U, &up_before, F_D | F_R);
    copyCell(left, F_R, &up_before, F_D);
    copyCell(left, F_R | F_D, &up_before, F_D | F_L);
}

void rotateFrontCW(Cube* cube)
{
    rotateFrontCCW(cube);
    rotateFrontCCW(cube);
    rotateFrontCCW(cube);
}

void rotateBackCCW(Cube* cube)
{
    // Update Back face
    const Face back_before = cube->faces[f2i(F_B)];
    Face* back = &cube->faces[f2i(F_B)];
    copyCell(back, F_U | F_L, back, F_U | F_R);
    copyCell(back, F_U, back, F_R);
    copyCell(back, F_U | F_R, back, F_D | F_R);
    copyCell(back, F_R, back, F_D);
    copyCell(back, F_D | F_R, back, F_D | F_L);
    copyCell(back, F_D, back, F_L);
    copyCell(back, F_D | F_L, &back_before, F_U | F_L);
    copyCell(back, F_L, &back_before, F_U);

    // Update adjacent faces
    const Face up_before = cube->faces[f2i(F_U)];
    Face* up = &cube->faces[f2i(F_U)];
    Face* right = &cube->faces[f2i(F_R)];
    Face* down = &cube->faces[f2i(F_D)];
    Face* left = &cube->faces[f2i(F_L)];

    copyCell(up, F_U | F_R, left, F_L | F_U);
    copyCell(up, F_U, left, F_L);
    copyCell(up, F_U | F_L, left, F_L | F_D);

    copyCell(left, F_L | F_U, down, F_D | F_L);
    copyCell(left, F_L, down, F_D);
    copyCell(left, F_L | F_D, down, F_D | F_R);

    copyCell(down, F_D | F_R, right, F_R | F_U);
    copyCell(down, F_D, right, F_R);
    copyCell(down, F_D | F_L, right, F_R | F_D);

    copyCell(right, F_R | F_U, &up_before, F_U | F_L);
    copyCell(right, F_R, &up_before, F_U);
    copyCell(right, F_R | F_D, &up_before, F_U | F_R);
}

void rotateBackCW(Cube* cube)
{
    rotateBackCCW(cube);
    rotateBackCCW(cube);
    rotateBackCCW(cube);
}

void rotateLeftCCW(Cube* cube)
{
    // Update Left face
    const Face left_before = cube->faces[f2i(F_L)];
    Face* left = &cube->faces[f2i(F_L)];
    copyCell(left, F_U | F_L, left, F_U | F_R);
    copyCell(left, F_U, left, F_R);
    copyCell(left, F_U | F_R, left, F_D | F_R);
    copyCell(left, F_R, left, F_D);
    copyCell(left, F_D | F_R, left, F_D | F_L);
    copyCell(left, F_D, left, F_L);
    copyCell(left, F_D | F_L, &left_before, F_U | F_L);
    copyCell(left, F_L, &left_before, F_U);

    // Update adjacent faces
    const Face up_before = cube->faces[f2i(F_U)];
    Face* up = &cube->faces[f2i(F_U)];
    Face* front = &cube->faces[f2i(F_F)];
    Face* down = &cube->faces[f2i(F_D)];
    Face* back = &cube->faces[f2i(F_B)];

    copyCell(up, F_L | F_U, front, F_L | F_U);
    copyCell(up, F_L, front, F_L);
    copyCell(up, F_L | F_D, front, F_L | F_D);

    copyCell(front, F_L | F_U, down, F_L | F_U);
    copyCell(front, F_L, down, F_L);
    copyCell(front, F_L | F_D, down, F_L | F_D);

    copyCell(down, F_L | F_U, back, F_R | F_D);
    copyCell(down, F_L, back, F_R);
    copyCell(down, F_L | F_D, back, F_R | F_U);

    copyCell(back, F_R | F_U, &up_before, F_L | F_D);
    copyCell(back, F_R, &up_before, F_L);
    copyCell(back, F_R | F_D, &up_before, F_L | F_U);
}

void rotateLeftCW(Cube* cube)
{
    rotateLeftCCW(cube);
    rotateLeftCCW(cube);
    rotateLeftCCW(cube);
}

void rotateRightCCW(Cube* cube)
{
    // Update Right face
    const Face right_before = cube->faces[f2i(F_R)];
    Face* right = &cube->faces[f2i(F_R)];
    copyCell(right, F_U | F_L, right, F_U | F_R);
    copyCell(right, F_U, right, F_R);
    copyCell(right, F_U | F_R, right, F_D | F_R);
    copyCell(right, F_R, right, F_D);
    copyCell(right, F_D | F_R, right, F_D | F_L);
    copyCell(right, F_D, right, F_L);
    copyCell(right, F_D | F_L, &right_before, F_U | F_L);
    copyCell(right, F_L, &right_before, F_U);

    // Update adjacent faces
    const Face up_before = cube->faces[f2i(F_U)];
    Face* up = &cube->faces[f2i(F_U)];
    Face* back = &cube->faces[f2i(F_B)];
    Face* down = &cube->faces[f2i(F_D)];
    Face* front = &cube->faces[f2i(F_F)];

    copyCell(up, F_R | F_D, back, F_L | F_U);
    copyCell(up, F_R, back, F_L);
    copyCell(up, F_R | F_U, back, F_L | F_D);

    copyCell(back, F_L | F_U, down, F_R | F_D);
    copyCell(back, F_L, down, F_R);
    copyCell(back, F_L | F_D, down, F_R | F_U);

    copyCell(down, F_R | F_U, front, F_R | F_U);
    copyCell(down, F_R, front, F_R);
    copyCell(down, F_R | F_D, front, F_R | F_D);

    copyCell(front, F_R | F_U, &up_before, F_R | F_U);
    copyCell(front, F_R, &up_before, F_R);
    copyCell(front, F_R | F_D, &up_before, F_R | F_D);
}

void rotateRightCW(Cube* cube)
{
    rotateRightCCW(cube);
    rotateRightCCW(cube);
    rotateRightCCW(cube);
}

void rotateCube(Cube* c, const Move m)
{
    if (m == NO_MOVE) return;

    switch (m)
    {
    case UU: rotateUpCW(c); // fallthrough
    case U: rotateUpCW(c);
        break;
    case Up: rotateUpCCW(c);
        break;

    case LL: rotateLeftCW(c); // fallthrough
    case L: rotateLeftCW(c);
        break;
    case Lp: rotateLeftCCW(c);
        break;

    case FF: rotateFrontCW(c); // fallthrough
    case F: rotateFrontCW(c);
        break;
    case Fp: rotateFrontCCW(c);
        break;

    case RR: rotateRightCW(c); // fallthrough
    case R: rotateRightCW(c);
        break;
    case Rp: rotateRightCCW(c);
        break;

    case BB: rotateBackCW(c); // fallthrough
    case B: rotateBackCW(c);
        break;
    case Bp: rotateBackCCW(c);
        break;

    case DD: rotateDownCW(c); // fallthrough
    case D: rotateDownCW(c);
        break;
    case Dp: rotateDownCCW(c);
        break;

    default:
        {
            TraceLog(LOG_FATAL, "Unknown cube rotation %d", m); // Will abort with exit(EXIT_FAILURE)
            __builtin_unreachable();
        }
    }
}
