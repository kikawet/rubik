// TODO: Maybe add one of these solvers? https://rosettacode.org/wiki/Solve_a_Rubik's_cube#thistlethwaite

#include "./solver.h"

#include <raylib.h>
#include <stdio.h>

void updateCube(Cube* cube, Moves* queue)
{
    for (size_t i = queue->current; i < queue->count; i++)
    {
        rotateCube(cube, queue->items[i]);
    }

    queue->current = queue->count;
}

bool solveWhiteGreenEdge(Cube* cube, Moves* moves)
{
    updateCube(cube, moves); // This should do nothing since solver can only be called with empty moves queue

    // TODO: replace static colors with variables :/
    const Face* up = &cube->faces[f2i(F_U)];
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Search Front + Up
    // check if White in on top
    if (getCell(up, F_U) == White && getCell(back, F_U) == Green)
    {
        da_append(moves, UU);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_D) == White && getCell(front, F_U) == Green)
    {
        // Do nothing :)

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_R) == White && getCell(right, F_U) == Green)
    {
        da_append(moves, U);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_L) == White && getCell(left, F_U) == Green)
    {
        da_append(moves, Up);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_U) == White && getCell(up, F_U) == Green)
    {
        da_append(moves, U);
        da_append(moves, Rp);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_U) == White && getCell(up, F_D) == Green)
    {
        da_append(moves, Up);
        da_append(moves, Rp);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_U) == White && getCell(up, F_R) == Green)
    {
        da_append(moves, Rp);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_U) == White && getCell(up, F_L) == Green)
    {
        da_append(moves, L);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // check if White is on front side
    else if (getCell(front, F_L) == White && getCell(left, F_R) == Green)
    {
        da_append(moves, Lp);
        da_append(moves, Up);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_R) == White && getCell(right, F_L) == Green)
    {
        da_append(moves, R);
        da_append(moves, U);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_L) == White && getCell(front, F_R) == Green)
    {
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_R) == White && getCell(front, F_L) == Green)
    {
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // check if White is on back
    else if (getCell(back, F_L) == White && getCell(right, F_R) == Green)
    {
        da_append(moves, Rp);
        da_append(moves, U);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_R) == White && getCell(left, F_L) == Green)
    {
        da_append(moves, L);
        da_append(moves, Up);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_R) == White && getCell(back, F_L) == Green)
    {
        da_append(moves, RR);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_L) == White && getCell(back, F_R) == Green)
    {
        da_append(moves, LL);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if white is at the bottom
    if (getCell(down, F_U) == White && getCell(front, F_D) == Green)
    {
        da_append(moves, FF);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == White && getCell(back, F_D) == Green)
    {
        da_append(moves, DD);
        da_append(moves, FF);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == White && getCell(right, F_D) == Green)
    {
        da_append(moves, Dp);
        da_append(moves, FF);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == White && getCell(left, F_D) == Green)
    {
        da_append(moves, D);
        da_append(moves, FF);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if green is at the bottom
    if (getCell(front, F_D) == White && getCell(down, F_U) == Green)
    {
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Fp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_D) == White && getCell(down, F_D) == Green)
    {
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_D) == White && getCell(down, F_R) == Green)
    {
        da_append(moves, R);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_D) == White && getCell(down, F_L) == Green)
    {
        da_append(moves, Lp);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find edge piece", __func__);
        return false;
    }

    return true;
}

bool solveWhiteBlueEdge(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* up = &cube->faces[f2i(F_U)];
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if White is on top
    if (getCell(up, F_U) == White && getCell(back, F_U) == Blue)
    {
        // Do nothing :)

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_R) == White && getCell(right, F_U) == Blue)
    {
        da_append(moves, RR);
        da_append(moves, D);
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_L) == White && getCell(left, F_U) == Blue)
    {
        da_append(moves, LL);
        da_append(moves, Dp);
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_U) == White && getCell(up, F_U) == Blue)
    {
        da_append(moves, U);
        da_append(moves, R);
        da_append(moves, Up);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_U) == White && getCell(up, F_R) == Blue)
    {
        da_append(moves, R);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_U) == White && getCell(up, F_L) == Blue)
    {
        da_append(moves, Lp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check edge on front
    else if (getCell(front, F_R) == White && getCell(right, F_L) == Blue)
    {
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_L) == White && getCell(left, F_R) == Blue)
    {
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_L) == White && getCell(front, F_R) == Blue)
    {
        da_append(moves, RR);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_R) == White && getCell(front, F_L) == Blue)
    {
        da_append(moves, LL);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check edge on back face
    else if (getCell(left, F_L) == White && getCell(back, F_R) == Blue)
    {
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_R) == White && getCell(back, F_L) == Blue)
    {
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_R) == White && getCell(left, F_L) == Blue)
    {
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_L) == White && getCell(right, F_R) == Blue)
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check edge on bottom face
    else if (getCell(down, F_U) == White && getCell(front, F_D) == Blue)
    {
        da_append(moves, DD);
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == White && getCell(right, F_D) == Blue)
    {
        da_append(moves, D);
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == White && getCell(back, F_D) == Blue)
    {
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == White && getCell(left, F_D) == Blue)
    {
        da_append(moves, Dp);
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_D) == White && getCell(down, F_U) == Blue)
    {
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_D) == White && getCell(down, F_R) == Blue)
    {
        da_append(moves, Rp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_D) == White && getCell(down, F_D) == Blue)
    {
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_D) == White && getCell(down, F_L) == Blue)
    {
        da_append(moves, L);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find edge piece", __func__);
        return false;
    }

    return true;
}

bool solveWhiteRedEdge(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* up = &cube->faces[f2i(F_U)];
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if edge is up
    if (getCell(up, F_R) == White && getCell(right, F_U) == Red)
    {
        // Do nothing :)

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_R) == Red && getCell(right, F_U) == White)
    {
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_L) == White && getCell(left, F_U) == Red)
    {
        da_append(moves, LL);
        da_append(moves, DD);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_U) == White && getCell(up, F_L) == Red)
    {
        da_append(moves, LL);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, R);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // check if edge is in front face
    else if (getCell(front, F_R) == White && getCell(right, F_L) == Red)
    {
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_L) == White && getCell(left, F_R) == Red)
    {
        da_append(moves, FF);
        da_append(moves, R);
        da_append(moves, FF);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_L) == White && getCell(front, F_R) == Red)
    {
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_R) == White && getCell(front, F_L) == Red)
    {
        da_append(moves, Fp);
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if edge is on back
    else if (getCell(back, F_L) == White && getCell(right, F_R) == Red)
    {
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_R) == White && getCell(left, F_L) == Red)
    {
        da_append(moves, Lp);
        da_append(moves, DD);
        da_append(moves, L);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_R) == White && getCell(back, F_L) == Red)
    {
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_L) == White && getCell(back, F_R) == Red)
    {
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if edge is at bottom
    else if (getCell(down, F_U) == White && getCell(front, F_D) == Red)
    {
        da_append(moves, D);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == White && getCell(right, F_D) == Red)
    {
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == White && getCell(back, F_D) == Red)
    {
        da_append(moves, Dp);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == White && getCell(left, F_D) == Red)
    {
        da_append(moves, DD);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_D) == White && getCell(down, F_U) == Red)
    {
        da_append(moves, Fp);
        da_append(moves, R);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_D) == White && getCell(down, F_R) == Red)
    {
        da_append(moves, Dp);
        da_append(moves, Fp);
        da_append(moves, R);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_D) == White && getCell(down, F_D) == Red)
    {
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_D) == White && getCell(down, F_L) == Red)
    {
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find edge piece", __func__);
        return false;
    }

    return true;
}

bool solveWhiteOrangeEdge(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* up = &cube->faces[f2i(F_U)];
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if edge is up
    if (getCell(up, F_L) == White && getCell(left, F_U) == Orange)
    {
        // Do nothing :)

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_U) == White && getCell(up, F_L) == Orange)
    {
        da_append(moves, Up);
        da_append(moves, Fp);
        da_append(moves, U);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if edge is front
    else if (getCell(front, F_L) == White && getCell(left, F_R) == Orange)
    {
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_R) == White && getCell(right, F_L) == Orange)
    {
        da_append(moves, Rp);
        da_append(moves, DD);
        da_append(moves, R);
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_R) == White && getCell(front, F_L) == Orange)
    {
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_L) == White && getCell(front, F_R) == Orange)
    {
        da_append(moves, F);
        da_append(moves, Dp);
        da_append(moves, Fp);
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check back face
    else if (getCell(back, F_R) == White && getCell(left, F_L) == Orange)
    {
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_L) == White && getCell(right, F_R) == Orange)
    {
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_L) == White && getCell(back, F_R) == Orange)
    {
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_R) == White && getCell(back, F_L) == Orange)
    {
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check down
    else if (getCell(down, F_U) == White && getCell(front, F_D) == Orange)
    {
        da_append(moves, Dp);
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == White && getCell(right, F_D) == Orange)
    {
        da_append(moves, DD);
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == White && getCell(back, F_D) == Orange)
    {
        da_append(moves, D);
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == White && getCell(left, F_D) == Orange)
    {
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_D) == White && getCell(down, F_U) == Orange)
    {
        da_append(moves, F);
        da_append(moves, Lp);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_D) == White && getCell(down, F_R) == Orange)
    {
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, Lp);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_D) == White && getCell(down, F_D) == Orange)
    {
        da_append(moves, Bp);
        da_append(moves, L);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_D) == White && getCell(down, F_L) == Orange)
    {
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, Lp);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find edge piece", __func__);
        return false;
    }

    return true;
}

bool solveCross(Cube* cube, Moves* queue)
{
    return solveWhiteGreenEdge(cube, queue) && solveWhiteBlueEdge(cube, queue) && solveWhiteRedEdge(cube, queue) &&
        solveWhiteOrangeEdge(cube, queue);
}

bool f2lGreenRedCorner(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* up = &cube->faces[f2i(F_U)];
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if corner is in place (down right) (white side)
    if (getCell(up, F_D | F_R) == White && getCell(right, F_U | F_L) == Red && getCell(front, F_U | F_R) == Green)
    {
        // Do nothing

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_D | F_R) == Red && getCell(right, F_U | F_L) == Green && getCell(front, F_U | F_R) == White)
    {
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, DD);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_D | F_R) == Green && getCell(right, F_U | F_L) == White && getCell(front, F_U | F_R) == Red)
    {
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, F);
        da_append(moves, Dp);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in up right (white side)
    else if (getCell(up, F_U | F_R) == White && getCell(right, F_U | F_R) == Green && getCell(back, F_U | F_L) == Red)
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, F);
        da_append(moves, DD);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_R) == Green && getCell(right, F_U | F_R) == Red && getCell(back, F_U | F_L) == White)
    {
        da_append(moves, Bp);
        da_append(moves, F);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_R) == Red && getCell(right, F_U | F_R) == White && getCell(back, F_U | F_L) == Green)
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, RR);
        da_append(moves, DD);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is up left (white side)
    else if (getCell(up, F_U | F_L) == White && getCell(left, F_U | F_L) == Red && getCell(back, F_U | F_R) == Green)
    {
        da_append(moves, B);
        da_append(moves, F);
        da_append(moves, DD);
        da_append(moves, Fp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_L) == Red && getCell(left, F_U | F_L) == Green && getCell(back, F_U | F_R) == White)
    {
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, DD);
        da_append(moves, R);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_L) == Green && getCell(left, F_U | F_L) == White && getCell(back, F_U | F_R) == Red)
    {
        da_append(moves, Lp);
        da_append(moves, F);
        da_append(moves, DD);
        da_append(moves, Fp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if coner is left down (white side)
    else if (getCell(up, F_D | F_L) == White && getCell(left, F_U | F_R) == Green && getCell(front, F_U | F_L) == Red)
    {
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_D | F_L) == Red && getCell(left, F_U | F_R) == White && getCell(front, F_U | F_L) == Green)
    {
        da_append(moves, L);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_D | F_L) == Green && getCell(left, F_U | F_R) == Red && getCell(front, F_U | F_L) == White)
    {
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, FF);
        da_append(moves, DD);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if coner is up right (yellow side)
    else if (getCell(down, F_U | F_R) == White && getCell(front, F_D | F_R) == Red && getCell(right, F_D | F_L) ==
        Green)
    {
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, F);
        da_append(moves, DD);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_R) == Green && getCell(front, F_D | F_R) == White && getCell(right, F_D | F_L) ==
        Red)
    {
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_R) == Red && getCell(front, F_D | F_R) == Green && getCell(right, F_D | F_L) ==
        White)
    {
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, Dp);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if coner is down right (yellow side)
    else if (getCell(down, F_D | F_R) == White && getCell(right, F_D | F_R) == Red && getCell(back, F_D | F_L) == Green)
    {
        da_append(moves, Bp);
        da_append(moves, F);
        da_append(moves, DD);
        da_append(moves, Fp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_R) == Green && getCell(right, F_D | F_R) == White && getCell(back, F_D | F_L) == Red)
    {
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, DD);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_R) == Red && getCell(right, F_D | F_R) == Green && getCell(back, F_D | F_L) == White)
    {
        da_append(moves, F);
        da_append(moves, Dp);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if coner is down left (yellow side)
    else if (getCell(down, F_D | F_L) == White && getCell(left, F_D | F_L) == Green && getCell(back, F_D | F_R) == Red)
    {
        da_append(moves, F);
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_L) == Red && getCell(left, F_D | F_L) == White && getCell(back, F_D | F_R) == Green)
    {
        da_append(moves, F);
        da_append(moves, DD);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_L) == Green && getCell(left, F_D | F_L) == Red && getCell(back, F_D | F_R) == White)
    {
        da_append(moves, Rp);
        da_append(moves, DD);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if coner is up left (yellow side)
    else if (getCell(down, F_U | F_L) == White && getCell(front, F_D | F_L) == Green && getCell(left, F_D | F_R) == Red)
    {
        da_append(moves, L);
        da_append(moves, Rp);
        da_append(moves, DD);
        da_append(moves, R);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_L) == Red && getCell(front, F_D | F_L) == White && getCell(left, F_D | F_R) == Green)
    {
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, DD);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_L) == Green && getCell(front, F_D | F_L) == Red && getCell(left, F_D | F_R) == White)
    {
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find corner piece", __func__);
        return false;
    }

    return true;
}

bool f2lGreenRedEdge(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if edge is on front
    if (getCell(front, F_R) == Green && getCell(right, F_L) == Red)
    {
        // Do nothing

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_R) == Red && getCell(right, F_L) == Green)
    {
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, DD);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, DD);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_L) == Green && getCell(left, F_R) == Red)
    {
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_L) == Red && getCell(left, F_R) == Green)
    {
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if edge is on back
    else if (getCell(back, F_L) == Green && getCell(right, F_R) == Red)
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_L) == Red && getCell(right, F_R) == Green)
    {
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_R) == Green && getCell(left, F_L) == Red)
    {
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_R) == Red && getCell(left, F_L) == Green)
    {
        da_append(moves, Lp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, Dp);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check on the bottom
    else if (getCell(down, F_U) == Green && getCell(front, F_D) == Red)
    {
        da_append(moves, DD);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U) == Red && getCell(front, F_D) == Green)
    {
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == Green && getCell(right, F_D) == Red)
    {
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == Red && getCell(right, F_D) == Green)
    {
        da_append(moves, DD);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == Green && getCell(back, F_D) == Red)
    {
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == Red && getCell(back, F_D) == Green)
    {
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == Green && getCell(left, F_D) == Red)
    {
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == Red && getCell(left, F_D) == Green)
    {
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, Fp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find edge piece", __func__);
        return false;
    }

    return true;
}

bool f2lGreenOrangeCorner(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* up = &cube->faces[f2i(F_U)];
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if corner is in place (down right) (white side)
    if (getCell(up, F_D | F_L) == White && getCell(left, F_U | F_R) == Orange && getCell(front, F_U | F_L) == Green)
    {
        // Do nothing :)

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_D | F_L) == Green && getCell(left, F_U | F_R) == White && getCell(front, F_U | F_L) ==
        Orange)
    {
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);
        da_append(moves, DD);
        da_append(moves, Fp);
        da_append(moves, D);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_D | F_L) == Orange && getCell(left, F_U | F_R) == Green && getCell(front, F_U | F_L) ==
        White)
    {
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, DD);
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in up right (white side)
    else if (getCell(up, F_U | F_R) == White && getCell(right, F_U | F_R) == Orange && getCell(back, F_U | F_L) ==
        Green)
    {
        da_append(moves, R);
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, Rp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_R) == Green && getCell(right, F_U | F_R) == White && getCell(back, F_U | F_L) ==
        Orange)
    {
        da_append(moves, R);
        da_append(moves, Fp);
        da_append(moves, DD);
        da_append(moves, F);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_R) == Orange && getCell(right, F_U | F_R) == Green && getCell(back, F_U | F_L) ==
        White)
    {
        da_append(moves, Bp);
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, Lp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is up left (white side)
    else if (getCell(up, F_U | F_L) == White && getCell(left, F_U | F_L) == Green && getCell(back, F_U | F_R) == Orange)
    {
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, DD);
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_L) == Orange && getCell(left, F_U | F_L) == White && getCell(back, F_U | F_R) == Green)
    {
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, LL);
        da_append(moves, DD);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_L) == Green && getCell(left, F_U | F_L) == Orange && getCell(back, F_U | F_R) == White)
    {
        da_append(moves, B);
        da_append(moves, Fp);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in up right (yellow side)
    else if (getCell(down, F_U | F_R) == White && getCell(front, F_D | F_R) == Green && getCell(right, F_D | F_L) ==
        Orange)
    {
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, Fp);
        da_append(moves, DD);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_R) == Orange && getCell(front, F_D | F_R) == White && getCell(right, F_D | F_L) ==
        Green)
    {
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, DD);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_R) == Green && getCell(front, F_D | F_R) == Orange && getCell(right, F_D | F_L) ==
        White)
    {
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in down right (yellow side)
    else if (getCell(down, F_D | F_R) == White && getCell(right, F_D | F_R) == Green && getCell(back, F_D | F_L) ==
        Orange)
    {
        da_append(moves, L);
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_R) == Orange && getCell(right, F_D | F_R) == White && getCell(back, F_D | F_L) ==
        Green)
    {
        da_append(moves, Fp);
        da_append(moves, DD);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_R) == Green && getCell(right, F_D | F_R) == Orange && getCell(back, F_D | F_L) ==
        White)
    {
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in down left (yellow side)
    else if (getCell(down, F_D | F_L) == White && getCell(back, F_D | F_R) == Green && getCell(left, F_D | F_L) ==
        Orange)
    {
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, LL);
        da_append(moves, Dp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_L) == Orange && getCell(back, F_D | F_R) == White && getCell(left, F_D | F_L) ==
        Green)
    {
        da_append(moves, Fp);
        da_append(moves, D);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_L) == Green && getCell(back, F_D | F_R) == Orange && getCell(left, F_D | F_L) ==
        White)
    {
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in up left (yellow side)
    else if (getCell(down, F_U | F_L) == White && getCell(left, F_D | F_R) == Green && getCell(front, F_D | F_L) ==
        Orange)
    {
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, Fp);
        da_append(moves, DD);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_L) == Orange && getCell(left, F_D | F_R) == White && getCell(front, F_D | F_L) ==
        Green)
    {
        da_append(moves, Dp);
        da_append(moves, Fp);
        da_append(moves, D);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_L) == Green && getCell(left, F_D | F_R) == Orange && getCell(front, F_D | F_L) ==
        White)
    {
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find corner piece", __func__);
        return false;
    }

    return true;
}

bool f2lGreenOrangeEdge(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if edge is on front
    if (getCell(front, F_L) == Green && getCell(left, F_R) == Orange)
    {
        // Do nothing

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(front, F_L) == Orange && getCell(left, F_R) == Green)
    {
        da_append(moves, Fp);
        da_append(moves, D);
        da_append(moves, F);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if edge is on back
    else if (getCell(back, F_R) == Green && getCell(left, F_L) == Orange)
    {
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_R) == Orange && getCell(left, F_L) == Green)
    {
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_L) == Green && getCell(right, F_R) == Orange)
    {
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_L) == Orange && getCell(right, F_R) == Green)
    {
        da_append(moves, Bp);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if edge is on bottom
    else if (getCell(down, F_U) == Orange && getCell(front, F_D) == Green)
    {
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U) == Green && getCell(front, F_D) == Orange)
    {
        da_append(moves, DD);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == Orange && getCell(right, F_D) == Green)
    {
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == Green && getCell(right, F_D) == Orange)
    {
        da_append(moves, D);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == Orange && getCell(back, F_D) == Green)
    {
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == Green && getCell(back, F_D) == Orange)
    {
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == Orange && getCell(left, F_D) == Green)
    {
        da_append(moves, DD);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == Green && getCell(left, F_D) == Orange)
    {
        da_append(moves, Dp);
        da_append(moves, Fp);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find edge piece", __func__);
        return false;
    }

    return true;
}

bool f2lBlueRedCorner(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* up = &cube->faces[f2i(F_U)];
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if corner is in place (up right) (white side)
    if (getCell(up, F_U | F_R) == White && getCell(right, F_U | F_R) == Red && getCell(back, F_U | F_L) == Blue)
    {
        // Do nothing :)

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_R) == Blue && getCell(right, F_U | F_R) == White && getCell(back, F_U | F_L) == Red)
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, DD);
        da_append(moves, Bp);
        da_append(moves, DD);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_R) == Red && getCell(right, F_U | F_R) == Blue && getCell(back, F_U | F_L) == White)
    {
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, DD);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in up left (white side)
    else if (getCell(up, F_U | F_L) == White && getCell(back, F_U | F_R) == Red && getCell(left, F_U | F_L) == Blue)
    {
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_L) == Blue && getCell(back, F_U | F_R) == White && getCell(left, F_U | F_L) == Red)
    {
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, BB);
        da_append(moves, DD);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_L) == Red && getCell(back, F_U | F_R) == Blue && getCell(left, F_U | F_L) == White)
    {
        da_append(moves, Lp);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in up right (yellow side)
    else if (getCell(down, F_U | F_R) == White && getCell(front, F_D | F_R) == Blue && getCell(right, F_D | F_L) == Red)
    {
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_R) == Red && getCell(front, F_D | F_R) == White && getCell(right, F_D | F_L) == Blue)
    {
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_R) == Blue && getCell(front, F_D | F_R) == Red && getCell(right, F_D | F_L) == White)
    {
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in down right (yellow side)
    else if (getCell(down, F_D | F_R) == White && getCell(right, F_D | F_R) == Blue && getCell(back, F_D | F_L) == Red)
    {
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, DD);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_R) == Red && getCell(right, F_D | F_R) == White && getCell(back, F_D | F_L) == Blue)
    {
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_R) == Blue && getCell(right, F_D | F_R) == Red && getCell(back, F_D | F_L) == White)
    {
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in down left (yellow side)
    else if (getCell(down, F_D | F_L) == White && getCell(back, F_D | F_R) == Blue && getCell(left, F_D | F_L) == Red)
    {
        da_append(moves, Lp);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, L);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_L) == Red && getCell(back, F_D | F_R) == White && getCell(left, F_D | F_L) == Blue)
    {
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, DD);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_L) == Blue && getCell(back, F_D | F_R) == Red && getCell(left, F_D | F_L) == White)
    {
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is in up left (yellow side)
    else if (getCell(down, F_U | F_L) == White && getCell(left, F_D | F_R) == Blue && getCell(front, F_D | F_L) == Red)
    {
        da_append(moves, DD);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, DD);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_L) == Red && getCell(left, F_D | F_R) == White && getCell(front, F_D | F_L) == Blue)
    {
        da_append(moves, Bp);
        da_append(moves, DD);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_L) == Blue && getCell(left, F_D | F_R) == Red && getCell(front, F_D | F_L) == White)
    {
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }


    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find corner piece", __func__);
        return false;
    }

    return true;
}

bool f2lBlueRedEdge(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if edge is on back
    if (getCell(back, F_L) == Blue && getCell(right, F_R) == Red)
    {
        // Do nothing :)

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_L) == Red && getCell(right, F_R) == Blue)
    {
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, BB);
        da_append(moves, DD);
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_R) == Blue && getCell(left, F_L) == Red)
    {
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_R) == Red && getCell(left, F_L) == Blue)
    {
        da_append(moves, Lp);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check up yellow side
    else if (getCell(down, F_U) == Red && getCell(front, F_D) == Blue)
    {
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U) == Blue && getCell(front, F_D) == Red)
    {
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == Red && getCell(right, F_D) == Blue)
    {
        da_append(moves, DD);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == Blue && getCell(right, F_D) == Red)
    {
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == Red && getCell(back, F_D) == Blue)
    {
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == Blue && getCell(back, F_D) == Red)
    {
        da_append(moves, DD);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == Red && getCell(left, F_D) == Blue)
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == Blue && getCell(left, F_D) == Red)
    {
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }


    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find edge piece", __func__);
        return false;
    }

    return true;
}

bool f2lBlueOrangeCorner(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* up = &cube->faces[f2i(F_U)];
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if corner is in place (up right) (white side)
    if (getCell(up, F_U | F_L) == White && getCell(back, F_U | F_R) == Blue && getCell(left, F_U | F_L) == Orange)
    {
        // Do nothing :)

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_L) == Orange && getCell(back, F_U | F_R) == White && getCell(left, F_U | F_L) == Blue)
    {
        da_append(moves, B);
        da_append(moves, DD);
        da_append(moves, Bp);
        da_append(moves, Lp);
        da_append(moves, DD);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(up, F_U | F_L) == Blue && getCell(back, F_U | F_R) == Orange && getCell(left, F_U | F_L) == White)
    {
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is up right (yellow side)
    else if (getCell(down, F_U | F_R) == White && getCell(front, F_D | F_R) == Orange && getCell(right, F_D | F_L) ==
        Blue)
    {
        da_append(moves, DD);
        da_append(moves, B);
        da_append(moves, DD);
        da_append(moves, Bp);
        da_append(moves, DD);
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_R) == Blue && getCell(front, F_D | F_R) == White && getCell(right, F_D | F_L) ==
        Orange)
    {
        da_append(moves, Lp);
        da_append(moves, DD);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_R) == Orange && getCell(front, F_D | F_R) == Blue && getCell(right, F_D | F_L) ==
        White)
    {
        da_append(moves, B);
        da_append(moves, DD);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is down right (yellow side)
    else if (getCell(down, F_D | F_R) == White && getCell(right, F_D | F_R) == Orange && getCell(back, F_D | F_L) ==
        Blue)
    {
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, Lp);
        da_append(moves, DD);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_R) == Blue && getCell(right, F_D | F_R) == White && getCell(back, F_D | F_L) ==
        Orange)
    {
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_R) == Orange && getCell(right, F_D | F_R) == Blue && getCell(back, F_D | F_L) ==
        White)
    {
        da_append(moves, DD);
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is down left (yellow side)
    else if (getCell(down, F_D | F_L) == White && getCell(back, F_D | F_R) == Orange && getCell(left, F_D | F_L) ==
        Blue)
    {
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, Lp);
        da_append(moves, DD);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_L) == Blue && getCell(back, F_D | F_R) == White && getCell(left, F_D | F_L) ==
        Orange)
    {
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D | F_L) == Orange && getCell(back, F_D | F_R) == Blue && getCell(left, F_D | F_L) ==
        White)
    {
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check if corner is up left (yellow side)
    else if (getCell(down, F_U | F_L) == White && getCell(left, F_D | F_R) == Orange && getCell(front, F_D | F_L) ==
        Blue)
    {
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, DD);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_L) == Blue && getCell(left, F_D | F_R) == White && getCell(front, F_D | F_L) ==
        Orange)
    {
        da_append(moves, D);
        da_append(moves, Lp);
        da_append(moves, DD);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U | F_L) == Orange && getCell(left, F_D | F_R) == Blue && getCell(front, F_D | F_L) ==
        White)
    {
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find corner piece", __func__);
        return false;
    }

    return true;
}

bool f2lBlueOrangeEdge(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    // Check if edge is on back
    if (getCell(back, F_R) == Blue && getCell(left, F_L) == Orange)
    {
        // Do nothing :)

        success = true;
        TraceLog(LOG_TRACE, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_R) == Orange && getCell(left, F_L) == Blue)
    {
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check yellow side up
    else if (getCell(down, F_U) == Blue && getCell(front, F_D) == Orange)
    {
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_U) == Orange && getCell(front, F_D) == Blue)
    {
        da_append(moves, D);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check yellow side right
    else if (getCell(down, F_R) == Blue && getCell(right, F_D) == Orange)
    {
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_R) == Orange && getCell(right, F_D) == Blue)
    {
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check yellow side down
    else if (getCell(down, F_D) == Blue && getCell(back, F_D) == Orange)
    {
        da_append(moves, DD);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_D) == Orange && getCell(back, F_D) == Blue)
    {
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Check yellow side left
    else if (getCell(down, F_L) == Blue && getCell(left, F_D) == Orange)
    {
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(down, F_L) == Orange && getCell(left, F_D) == Blue)
    {
        da_append(moves, DD);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_ERROR, "[%s] Unable to find edge piece", __func__);
        return false;
    }

    return true;
}

bool solveF2L(Cube* cube, Moves* queue)
{
    return f2lGreenRedCorner(cube, queue) && f2lGreenRedEdge(cube, queue) &&
        f2lGreenOrangeCorner(cube, queue) && f2lGreenOrangeEdge(cube, queue) &&
        f2lBlueRedCorner(cube, queue) && f2lBlueRedEdge(cube, queue) &&
        f2lBlueOrangeCorner(cube, queue) && f2lBlueOrangeEdge(cube, queue);
}

bool genericOLL(Cube* cube, Moves* moves)
{
    // reference for the moves: https://jperm.net/algs/oll
    // Since all the movements are from the top also this helper required: https://cube.rider.biz/algtrans.html
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    if (
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_L) == Yellow && getCell(down, F_D) == Yellow && getCell(down, F_D | F_R) == Yellow
    )
    {
        // Do nothing :)

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 1
        getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D) == Yellow && getCell(left, F_D | F_R) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D) == Yellow && getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, RR);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, DD);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 2
        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D) == Yellow && getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D) == Yellow &&
        getCell(right, F_D) == Yellow && getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Lp);
        da_append(moves, DD);
        da_append(moves, L);
        da_append(moves, BB);
        da_append(moves, Rp);
        da_append(moves, BB);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 3
        getCell(down, F_D | F_L) == Yellow &&
        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow &&
        getCell(left, F_D) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, Lp);
        da_append(moves, RR);
        da_append(moves, F);
        da_append(moves, Rp);
        da_append(moves, F);
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, Lp);
        da_append(moves, F);
        da_append(moves, Rp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 4
        getCell(down, F_D | F_R) == Yellow &&
        getCell(front, F_D) == Yellow && getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow && getCell(left, F_D | F_R) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, Lp);
        da_append(moves, Fp);
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, Lp);
        da_append(moves, Fp);
        da_append(moves, R);
        da_append(moves, Fp);
        da_append(moves, RR);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 5
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U) == Yellow && getCell(down, F_L) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, BB);
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Lp);
        da_append(moves, B);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 6
        getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow && getCell(left, F_D | F_R) == Yellow &&
        getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, BB);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 7
        getCell(down, F_U) == Yellow && getCell(down, F_L) == Yellow && getCell(down, F_D | F_L) == Yellow &&
        getCell(front, F_D | F_L) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, BB);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 8
        getCell(down, F_U) == Yellow && getCell(down, F_R) == Yellow && getCell(down, F_D | F_R) == Yellow &&
        getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow && getCell(left, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, L);
        da_append(moves, Bp);
        da_append(moves, Lp);
        da_append(moves, BB);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 9
        getCell(down, F_U) == Yellow && getCell(down, F_L) == Yellow && getCell(down, F_D | F_R) == Yellow &&
        getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D | F_R) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, RR);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 10
        getCell(down, F_U | F_R) == Yellow && getCell(down, F_L) == Yellow && getCell(down, F_D) == Yellow &&
        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D) == Yellow &&
        getCell(back, F_D | F_L) == Yellow &&
        getCell(left, F_D | F_L) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 11
        getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow && getCell(down, F_L) == Yellow &&
        getCell(front, F_D | F_L) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, U);
        da_append(moves, R);
        da_append(moves, Up);
        da_append(moves, R);
        da_append(moves, BB);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 12
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow &&
        getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, L);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, Rp);
        da_append(moves, BB);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 13
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow && getCell(down, F_D | F_L) == Yellow &&
        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow &&
        getCell(right, F_D | F_L) == Yellow
    )
    {
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, RR);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 14
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow && getCell(down, F_D | F_R) == Yellow &&
        getCell(front, F_D) == Yellow && getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 15
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(front, F_D) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow &&
        getCell(right, F_D | F_L) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 16
        getCell(down, F_U | F_R) == Yellow && getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D | F_R) == Yellow &&
        getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Lp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, Bp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 17
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_D | F_R) == Yellow &&
        getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, RR);
        da_append(moves, Lp);
        da_append(moves, F);
        da_append(moves, R);
        da_append(moves, Fp);
        da_append(moves, Rp);
        da_append(moves, Fp);
        da_append(moves, Rp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 18
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(front, F_D) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, BB);
        da_append(moves, LL);
        da_append(moves, Fp);
        da_append(moves, R);
        da_append(moves, Fp);
        da_append(moves, Rp);
        da_append(moves, FF);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 19
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D) == Yellow &&
        getCell(right, F_D) == Yellow && getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Lp);
        da_append(moves, R);
        da_append(moves, F);
        da_append(moves, R);
        da_append(moves, F);
        da_append(moves, Rp);
        da_append(moves, Fp);
        da_append(moves, Rp);
        da_append(moves, L);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 20
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U | F_R) == Yellow && getCell(down, F_D | F_L) == Yellow
        && getCell(down, F_D | F_R) == Yellow &&
        getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(left, F_D) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, Lp);
        da_append(moves, R);
        da_append(moves, Lp);
        da_append(moves, F);
        da_append(moves, R);
        da_append(moves, Fp);
        da_append(moves, Rp);
        da_append(moves, Fp);
        da_append(moves, Rp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 21
        getCell(down, F_U) == Yellow && getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D) == Yellow &&
        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D | F_R) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 22
        getCell(down, F_U) == Yellow && getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D) == Yellow &&
        getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D | F_L) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D | F_R) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, RR);
        da_append(moves, Dp);
        da_append(moves, RR);
        da_append(moves, Dp);
        da_append(moves, RR);
        da_append(moves, DD);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 23
        getCell(down, F_U) == Yellow && getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_L) == Yellow && getCell(down, F_D) == Yellow && getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D | F_R) == Yellow
    )
    {
        da_append(moves, RR);
        da_append(moves, Up);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);
        da_append(moves, U);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 24
        getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D) == Yellow && getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D | F_L) == Yellow &&
        getCell(back, F_D | F_R) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, Lp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 25
        getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_L) == Yellow && getCell(down, F_D) == Yellow &&

        getCell(back, F_D | F_L) == Yellow &&
        getCell(left, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Bp);
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, Lp);
        da_append(moves, B);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 26
        getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D) == Yellow &&

        getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D | F_R) == Yellow &&
        getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 27
        getCell(down, F_U) == Yellow &&
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_L) == Yellow && getCell(down, F_D) == Yellow &&

        getCell(front, F_D | F_L) == Yellow &&
        getCell(back, F_D | F_L) == Yellow &&
        getCell(right, F_D | F_L) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 28
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow &&
        getCell(down, F_D | F_L) == Yellow && getCell(down, F_D | F_R) == Yellow &&

        getCell(back, F_D) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, Lp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 29
        getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow &&
        getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D | F_L) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 30
        getCell(down, F_U) == Yellow &&
        getCell(down, F_L) == Yellow &&
        getCell(down, F_D | F_L) == Yellow && getCell(down, F_D | F_R) == Yellow &&

        getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_R) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, RR);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, BB);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 31
        getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D | F_L) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 32
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U) == Yellow &&
        getCell(down, F_L) == Yellow &&
        getCell(down, F_D | F_L) == Yellow &&

        getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 33
        getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 34
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_L) == Yellow && getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_R) == Yellow &&
        getCell(right, F_D | F_L) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, RR);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 35
        getCell(down, F_U | F_L) == Yellow &&
        getCell(down, F_R) == Yellow &&
        getCell(down, F_D) == Yellow && getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D) == Yellow &&
        getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow &&
        getCell(right, F_D | F_L) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, RR);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 36
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U) == Yellow &&
        getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D) == Yellow
    )
    {
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, Bp);
        da_append(moves, Lp);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 37
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U) == Yellow &&
        getCell(down, F_L) == Yellow &&
        getCell(down, F_D | F_R) == Yellow &&

        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 38
        getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow &&
        getCell(down, F_D | F_L) == Yellow &&

        getCell(front, F_D | F_L) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(right, F_D) == Yellow && getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 39
        getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_L) == Yellow &&

        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, Bp);
        da_append(moves, Lp);
        da_append(moves, Dp);
        da_append(moves, L);
        da_append(moves, D);
        da_append(moves, B);
        da_append(moves, Dp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 40
        getCell(down, F_U | F_L) == Yellow &&
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D) == Yellow && getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 41
        getCell(down, F_U) == Yellow &&
        getCell(down, F_L) == Yellow &&
        getCell(down, F_D | F_L) == Yellow && getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, DD);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 42
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow &&
        getCell(down, F_D) == Yellow &&

        getCell(front, F_D) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, DD);
        da_append(moves, R);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 43
        getCell(down, F_U) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_R) == Yellow &&

        getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D) == Yellow && getCell(left, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Bp);
        da_append(moves, Dp);
        da_append(moves, Lp);
        da_append(moves, D);
        da_append(moves, L);
        da_append(moves, B);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 44
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U) == Yellow &&
        getCell(down, F_L) == Yellow &&
        getCell(down, F_D | F_L) == Yellow &&

        getCell(back, F_D) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D) == Yellow && getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 45
        getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D | F_R) == Yellow
    )
    {
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 46
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U) == Yellow &&
        getCell(down, F_D | F_L) == Yellow && getCell(down, F_D) == Yellow &&

        getCell(left, F_D) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D) == Yellow && getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 47
        getCell(down, F_U) == Yellow &&
        getCell(down, F_R) == Yellow &&

        getCell(front, F_D | F_L) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, D);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 48
        getCell(down, F_U) == Yellow &&
        getCell(down, F_L) == Yellow &&

        getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D | F_R) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 49
        getCell(down, F_U) == Yellow &&
        getCell(down, F_R) == Yellow &&

        getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D) == Yellow && getCell(left, F_D | F_R) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, Bp);
        da_append(moves, LL);
        da_append(moves, F);
        da_append(moves, LL);
        da_append(moves, B);
        da_append(moves, LL);
        da_append(moves, Fp);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 50
        getCell(down, F_R) == Yellow &&
        getCell(down, F_D) == Yellow &&

        getCell(front, F_D) == Yellow && getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D | F_L) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D) == Yellow && getCell(left, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Lp);
        da_append(moves, F);
        da_append(moves, LL);
        da_append(moves, Bp);
        da_append(moves, LL);
        da_append(moves, Fp);
        da_append(moves, LL);
        da_append(moves, B);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 51
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&

        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, B);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 52
        getCell(down, F_U) == Yellow && getCell(down, F_D) == Yellow &&

        getCell(front, F_D | F_L) == Yellow &&
        getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D) == Yellow && getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, F);
        da_append(moves, Dp);
        da_append(moves, Fp);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 53
        getCell(down, F_U) == Yellow && getCell(down, F_R) == Yellow &&

        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(left, F_D) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, BB);
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Lp);
        da_append(moves, Bp);
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, Lp);
        da_append(moves, B);
        da_append(moves, R);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 54
        getCell(down, F_U) == Yellow && getCell(down, F_L) == Yellow &&

        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow &&
        getCell(right, F_D) == Yellow
    )
    {
        da_append(moves, L);
        da_append(moves, BB);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, B);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 55
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&

        getCell(front, F_D | F_L) == Yellow && getCell(front, F_D) == Yellow && getCell(front, F_D | F_R) == Yellow &&
        getCell(back, F_D | F_L) == Yellow && getCell(back, F_D) == Yellow && getCell(back, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, RR);
        da_append(moves, Bp);
        da_append(moves, RR);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 56
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&

        getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow &&
        getCell(left, F_D | F_L) == Yellow && getCell(left, F_D | F_R) == Yellow &&
        getCell(right, F_D | F_L) == Yellow && getCell(right, F_D | F_R) == Yellow
    )
    {
        da_append(moves, Lp);
        da_append(moves, Fp);
        da_append(moves, L);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Lp);
        da_append(moves, F);
        da_append(moves, L);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if ( // 57
        getCell(down, F_U | F_L) == Yellow && getCell(down, F_U | F_R) == Yellow &&
        getCell(down, F_L) == Yellow && getCell(down, F_R) == Yellow &&
        getCell(down, F_D | F_L) == Yellow && getCell(down, F_D | F_R) == Yellow &&

        getCell(front, F_D) == Yellow &&
        getCell(back, F_D) == Yellow
    )
    {
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, L);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);
        da_append(moves, Lp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_WARNING, "[%s] Unable to find OLL pattern", __func__);
        return false;
    }

    return true;
}

bool solveOLL(Cube* cube, Moves* queue)
{
    // Try all possible combinations because genericOLL only checks one orientation
    if (genericOLL(cube, queue)) return true;

    TraceLog(LOG_INFO, "[%s] Rotating down face to try find OLL pattern", __func__);
    da_append(queue, D);
    if (genericOLL(cube, queue)) return true;

    TraceLog(LOG_INFO, "[%s] Rotating down face to try find OLL pattern", __func__);
    rotateCube(cube, Dp);
    queue->count--;
    queue->current--;
    da_append(queue, DD);
    if (genericOLL(cube, queue)) return true;

    TraceLog(LOG_INFO, "[%s] Rotating down face to try find OLL pattern", __func__);
    queue->count--;
    queue->current--;
    rotateCube(cube, DD);
    da_append(queue, Dp);
    if (genericOLL(cube, queue)) return true;

    queue->count--;
    queue->current--;
    rotateCube(cube, D);
    TraceLog(LOG_ERROR, "[%s] Unable to find any OLL pattern", __func__);
    return false;
}

bool pllGreenRedCorner(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    if (getCell(front, F_D | F_R) == Green && getCell(right, F_D | F_L) == Red)
    {
        // do nothing :)

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(right, F_D | F_R) == Green && getCell(back, F_D | F_L) == Red)
    {
        da_append(moves, Dp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_D | F_R) == Green && getCell(left, F_D | F_L) == Red)
    {
        da_append(moves, DD);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_D | F_R) == Green && getCell(front, F_D | F_L) == Red)
    {
        da_append(moves, D);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_WARNING, "[%s] Unable to find PLL pattern", __func__);
        return false;
    }

    return true;
}

bool pllBlueRedCorner(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    bool success = false;
    if (getCell(right, F_D | F_R) == Red && getCell(back, F_D | F_L) == Blue)
    {
        // do nothing :)

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(back, F_D | F_R) == Red && getCell(left, F_D | F_L) == Blue)
    {
        da_append(moves, Dp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, RR);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_D | F_R) == Red && getCell(front, F_D | F_L) == Blue)
    {
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Bp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_WARNING, "[%s] Unable to find PLL pattern", __func__);
        return false;
    }

    return true;
}

bool pllBlueOrangeCorner(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];

    bool success = false;
    if (getCell(back, F_D | F_R) == Blue && getCell(left, F_D | F_L) == Orange)
    {
        // do nothing :)

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    else if (getCell(left, F_D | F_R) == Blue && getCell(front, F_D | F_L) == Orange)
    {
        da_append(moves, DD);
        da_append(moves, B);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, RR);
        da_append(moves, Bp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, DD);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_WARNING, "[%s] Unable to find PLL pattern", __func__);
        return false;
    }

    return true;
}

bool pllEdges(Cube* cube, Moves* moves)
{
    updateCube(cube, moves);

    // TODO: replace static colors with variables :/
    const EColor front = getCell(&cube->faces[f2i(F_F)], F_D);
    const EColor back = getCell(&cube->faces[f2i(F_B)], F_D);
    const EColor left = getCell(&cube->faces[f2i(F_L)], F_D);
    const EColor right = getCell(&cube->faces[f2i(F_R)], F_D);

    bool success = false;
    if (front == Green && right == Red && back == Blue && left == Orange)
    {
        // do nothing :)

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // H
    else if (front == Blue && right == Orange && back == Green && left == Red)
    {
        da_append(moves, RR);
        da_append(moves, LL);
        da_append(moves, U);
        da_append(moves, RR);
        da_append(moves, LL);
        da_append(moves, DD);
        da_append(moves, RR);
        da_append(moves, LL);
        da_append(moves, U);
        da_append(moves, RR);
        da_append(moves, LL);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Ua green side
    else if (front == Green && right == Orange && back == Red && left == Blue)
    {
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, RR);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Ua red side
    else if (front == Orange && right == Red && back == Green && left == Blue)
    {
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, RR);
        da_append(moves, D);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Ua blue side
    else if (front == Orange && right == Green && back == Blue && left == Red)
    {
        da_append(moves, DD);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, RR);
        da_append(moves, DD);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Ua orange side
    else if (front == Blue && right == Green && back == Red && left == Orange)
    {
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, RR);
        da_append(moves, Dp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Ub green side
    else if (front == Green && right == Blue && back == Orange && left == Red)
    {
        da_append(moves, RR);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, Rp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Ub red side
    else if (front == Blue && right == Red && back == Orange && left == Green)
    {
        da_append(moves, Dp);
        da_append(moves, RR);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, D);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Ub blue side
    else if (front == Red && right == Orange && back == Blue && left == Green)
    {
        da_append(moves, DD);
        da_append(moves, RR);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, DD);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Ub Orange side
    else if (front == Red && right == Blue && back == Green && left == Orange)
    {
        da_append(moves, D);
        da_append(moves, RR);
        da_append(moves, D);
        da_append(moves, R);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, D);
        da_append(moves, Rp);
        da_append(moves, Dp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Z
    else if (front == Red && right == Green && back == Orange && left == Blue)
    {
        da_append(moves, RR);
        da_append(moves, LL);
        da_append(moves, Up);
        da_append(moves, RR);
        da_append(moves, LL);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, L);
        da_append(moves, BB);
        da_append(moves, RR);
        da_append(moves, LL);
        da_append(moves, FF);
        da_append(moves, Rp);
        da_append(moves, L);
        da_append(moves, DD);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }
    // Z left
    else if (front == Orange && right == Blue && back == Red && left == Green)
    {
        da_append(moves, D);
        da_append(moves, RR);
        da_append(moves, LL);
        da_append(moves, Up);
        da_append(moves, RR);
        da_append(moves, LL);
        da_append(moves, Dp);
        da_append(moves, Rp);
        da_append(moves, L);
        da_append(moves, BB);
        da_append(moves, RR);
        da_append(moves, LL);
        da_append(moves, FF);
        da_append(moves, Rp);
        da_append(moves, L);
        da_append(moves, DD);
        da_append(moves, Dp);

        success = true;
        TraceLog(LOG_DEBUG, "[%s] Running case %d", __func__, __LINE__);
    }

    if (!success)
    {
        TraceLog(LOG_WARNING, "[%s] Unable to find PLL pattern", __func__);
        return false;
    }

    return true;
}

bool solvePLL(Cube* cube, Moves* queue)
{
    // reference for the moves: https://jperm.net/algs/2look/pll
    // Since all the movements are from the top also this helper required: https://cube.rider.biz/algtrans.html
    return pllGreenRedCorner(cube, queue) && pllBlueRedCorner(cube, queue) && pllBlueOrangeCorner(cube, queue) &&
        pllEdges(cube, queue);
}

#define return_defer(value) do { result = (value); goto defer; } while(0)

bool solve(Cube cube, Moves* queue)
{
    bool result = true;
    // To solve the cube we need to perform the moves so copy the current here to later restore it!
    const size_t current = queue->current;

    if (!solveCross(&cube, queue))
        return_defer(false);
    if (!solveF2L(&cube, queue))
        return_defer(false);
    if (!solveOLL(&cube, queue))
        return_defer(false);
    if (!solvePLL(&cube, queue))
        return_defer(false);

defer:
    queue->current = current;
    return result;
}
