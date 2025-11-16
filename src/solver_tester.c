#include <stdio.h>
#include <time.h>
#include <raylib.h>

#include "solver/solver.h"

void generate_moves(Moves* moves, const size_t amount)
{
    if (amount == 0) return;

    for (size_t i = 0; i < amount; i++)
    {
        const Move m = GetRandomValue(NO_MOVE + 1, MOVE_LENGTH - 1);
        da_append(moves, m);
    }
}

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

int main(const int argc, char** argv)
{
    SetRandomSeed(time(NULL));
    // SetRandomSeed(2);
    SetTraceLogLevel(LOG_NONE);

    Cube cube = newCube();
    Moves shuffle = {0};
    Moves solution = {0};

    const long rounds = argc < 2 ? 100: strtol(argv[1], NULL, 10);

    for (long i = 1; i <= rounds; i++)
    {
        const long checks = 10000;
        printf("%ld/%ld\n", i, rounds);
        for (long j = 0; j < checks; j++)
        {
            resetCube(&cube);
            generate_moves(&shuffle, i);
            applyMoves(&cube, &shuffle);

            solve(cube, &solution);
            applyMoves(&cube, &solution);

            if (!is_solved(&cube))
            {
                dump_moves(&shuffle);
                abort();
            }

            shuffle.count = 0;
            solution.count = 0;
        }
    }

    if (shuffle.items) free(shuffle.items);
    if (solution.items) free(solution.items);

    return 0;
}
