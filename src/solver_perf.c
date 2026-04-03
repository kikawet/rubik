#include <stdio.h>
#include <raylib.h>

#include "solver/solver.h"

bool is_face_solved(const Face* f, const EColor color)
{
    return
        getCell(f, F_U | F_L) == color &&
        getCell(f, F_U) == color &&
        getCell(f, F_U | F_R) == color &&

        getCell(f, F_L) == color &&
        getCell(f, F_R) == color &&

        getCell(f, F_D | F_L) == color &&
        getCell(f, F_D) == color &&
        getCell(f, F_D | F_R) == color;
}

bool is_solved(const Cube* cube)
{
    const Face* up = &cube->faces[f2i(F_U)];
    const Face* down = &cube->faces[f2i(F_D)];
    const Face* front = &cube->faces[f2i(F_F)];
    const Face* back = &cube->faces[f2i(F_B)];
    const Face* left = &cube->faces[f2i(F_L)];
    const Face* right = &cube->faces[f2i(F_R)];

    return
        is_face_solved(up, f2c(F_U)) &&
        is_face_solved(down, f2c(F_D)) &&
        is_face_solved(front, f2c(F_F)) &&
        is_face_solved(back, f2c(F_B)) &&
        is_face_solved(left, f2c(F_L)) &&
        is_face_solved(right, f2c(F_R));
}

void applyMoves(Cube* cube, const Moves* queue)
{
    for (size_t i = 0; i < queue->count; i++)
        rotateCube(cube, queue->items[i]);
}

void dump_moves(const Moves* moves)
{
    for (size_t i = 0; i < moves->count; i++)
        fprintf(stderr, "%s", moveToStr(moves->items[i]));

    fprintf(stderr, "\n");
}

void parse_args(const int argc, const char** argv, Moves* moves)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s \"[moves]\"\n", argv[0]);
        exit(1);
    }

    strToMoves(argv[1], moves);
}

int main(const int argc, const char** argv)
{
    SetTraceLogLevel(LOG_NONE);

    Cube cube;
    resetCube(&cube);
    Moves shuffle = {0};
    Moves solution = {0};

    parse_args(argc, argv, &shuffle);
    applyMoves(&cube, &shuffle);

    if (!solve(cube, &solution))
    {
        fprintf(stderr, "Error solving cube!\n");
        dump_moves(&shuffle);
        abort();
    }

    applyMoves(&cube, &solution);

    if (!is_solved(&cube))
    {
        dump_moves(&shuffle);
        abort();
    }

    dump_moves(&solution);

    if (shuffle.items) free(shuffle.items);
    if (solution.items) free(solution.items);

    return 0;
}
