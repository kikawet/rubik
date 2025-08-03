#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>

#include "model.c"
#include "draw.c"

Cube newCube()
{
    Cube c = {0};

    setFaceColor(&c.faces[f2i(F_U)], White);
    setFaceColor(&c.faces[f2i(F_F)], Green);
    setFaceColor(&c.faces[f2i(F_B)], Blue);
    setFaceColor(&c.faces[f2i(F_D)], Yellow);
    setFaceColor(&c.faces[f2i(F_L)], Orange);
    setFaceColor(&c.faces[f2i(F_R)], Red);

    return c;
}

// degrees
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
        return 90.f;
    case Up:
    case Lp:
    case Fp:
    case Rp:
    case Bp:
    case Dp:
        return -90.f;
    case U2:
    case L2:
    case F2:
    case R2:
    case B2:
    case D2:
        return 180.f;
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
    case U2:
        return F_U;
    case L:
    case Lp:
    case L2:
        return F_L;
    case F:
    case Fp:
    case F2:
        return F_F;
    case R:
    case Rp:
    case R2:
        return F_R;
    case B:
    case Bp:
    case B2:
        return F_B;
    case D:
    case Dp:
    case D2:
        return F_D;
    }

    TraceLog(LOG_FATAL, "Invalid move %d", m); // Will abort with exit(EXIT_FAILURE)
    __builtin_unreachable();
}


void copyCell(Face* dst, const uint8_t cellDst, Face* src, const uint8_t cellSrc)
{
    const EColor color = getCell(src, cellSrc);
    paintCell(dst, cellDst, color);
}

static inline void rotateUpCCW(Cube* cube)
{
    Face up_before = cube->faces[f2i(F_U)];
    Face* up = &cube->faces[f2i(F_U)];
    copyCell(up, F_U | F_L, up, F_U | F_R);
    copyCell(up, F_U, up, F_R);
    copyCell(up, F_U | F_R, up, F_D | F_R);
    copyCell(up, F_R, up, F_D);
    copyCell(up, F_D | F_R, up, F_D | F_L);
    copyCell(up, F_D, up, F_L);
    copyCell(up, F_D | F_L, &up_before, F_U | F_L);
    copyCell(up, F_L, &up_before, F_U);

    // Adjacent faces
    Face back_before = cube->faces[f2i(F_B)];
}

void rotateUpCW(Cube* cube)
{
    rotateUpCCW(cube);
    rotateUpCCW(cube);
    rotateUpCCW(cube);
}

void rotateDownCW(Cube* cube)
{
    fprintf(stderr, "Not implemented %s:%d\n", __FILE__, __LINE__);
    exit(1);
}

void rotateDownCCW(Cube* cube)
{
    fprintf(stderr, "Not implemented %s:%d\n", __FILE__, __LINE__);
    exit(1);
}

void rotateFrontCW(Cube* cube)
{
    fprintf(stderr, "Not implemented %s:%d\n", __FILE__, __LINE__);
    exit(1);
}

void rotateFrontCCW(Cube* cube)
{
    fprintf(stderr, "Not implemented %s:%d\n", __FILE__, __LINE__);
    exit(1);
}

void rotateBackCW(Cube* cube)
{
    fprintf(stderr, "Not implemented %s:%d\n", __FILE__, __LINE__);
    exit(1);
}

void rotateBackCCW(Cube* cube)
{
    fprintf(stderr, "Not implemented %s:%d\n", __FILE__, __LINE__);
    exit(1);
}

void rotateLeftCW(Cube* cube)
{
    fprintf(stderr, "Not implemented %s:%d\n", __FILE__, __LINE__);
    exit(1);
}

void rotateLeftCCW(Cube* cube)
{
    fprintf(stderr, "Not implemented %s:%d\n", __FILE__, __LINE__);
    exit(1);
}

void rotateRightCW(Cube* cube)
{
    fprintf(stderr, "Not implemented %s:%d\n", __FILE__, __LINE__);
    exit(1);
}

void rotateRightCCW(Cube* cube)
{
    fprintf(stderr, "Not implemented %s:%d\n", __FILE__, __LINE__);
    exit(1);
}

void rotateCube(Cube* cube, Move m)
{
    if (m == NO_MOVE) return;

    switch (m)
    {
    case U2: rotateUpCW(cube); // fallthrough
    case U: rotateUpCW(cube); break;
    case Up: rotateUpCCW(cube); break;

    case L2: rotateLeftCW(cube); // fallthrough
    case L: rotateLeftCW(cube); break;
    case Lp: rotateLeftCCW(cube); break;

    case F2: rotateFrontCW(cube); // fallthrough
    case F: rotateFrontCW(cube); break;
    case Fp: rotateFrontCCW(cube); break;

    case R2: rotateRightCW(cube); // fallthrough
    case R: rotateRightCW(cube); break;
    case Rp: rotateRightCCW(cube); break;

    case B2: rotateBackCW(cube); // fallthrough
    case B: rotateBackCW(cube); break;
    case Bp: rotateBackCCW(cube); break;

    case D2: rotateDownCW(cube); // fallthrough
    case D: rotateDownCW(cube); break;
    case Dp: rotateDownCCW(cube); break;

    default:
        {
            TraceLog(LOG_FATAL, "Unknown cube rotation %d\n", m); // Will abort with exit(EXIT_FAILURE)
            __builtin_unreachable();
        }
    }

}

void updateCube(Cube* cube, Moves* moves)
{
    if (moves->current >= moves->count)
        return;

    const float dt = GetFrameTime();
    const float progressSpeed = 0.4f;
    Rotation* r = &cube->rotation;

    if (r->progress >= 1.f || cube->rotation.face == 0)
    {
        r->face = moveToFace(moves->items[moves->current++]);
        r->progress = 0;
        r->angle = moveMaxAngle(moves->items[moves->current]) * r->progress;
        return;
    }

    r->progress += progressSpeed * dt;
    r->angle = moveMaxAngle(moves->items[moves->current]) * r->progress;
    TraceLog(LOG_INFO, "Updating cube Face: %d", r->face);
}

Moves generateMoves()
{
    Moves moves = {0};

    moves.count = 4;
    moves.items = malloc(sizeof(Move)*moves.count);

    int i = 0;
    moves.items[i++] = U;
    moves.items[i++] = F;
    moves.items[i++] = Fp;
    moves.items[i++] = Up;

    return moves;
}

int main()
{
    Cube cube = newCube();
    Moves moves = {0};// generateMoves();

    paintCell(&cube.faces[f2i(F_U)], F_D, Green);
    paintCell(&cube.faces[f2i(F_U)], F_D | F_L, Green);
    paintCell(&cube.faces[f2i(F_U)], F_L, Orange);
    paintCell(&cube.faces[f2i(F_U)], F_L | F_U, Orange);
    paintCell(&cube.faces[f2i(F_U)], F_U, Blue);
    paintCell(&cube.faces[f2i(F_U)], F_U | F_R, Blue);
    paintCell(&cube.faces[f2i(F_U)], F_R, Red);
    paintCell(&cube.faces[f2i(F_U)], F_R | F_D, Red);

    rotateCube(&cube, U);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    InitWindow(800, 600, "Rubik's cube");

    SetExitKey(KEY_Q);

    Camera3D camera = {0};
    camera.position = (Vector3){10.0f, 10.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose())
    {
        if (IsKeyReleased(KEY_UP))
        {
            camera.position.y += 1.f;
        }

        UpdateCamera(&camera, CAMERA_ORBITAL);
        //updateCube(&cube, &moves);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawFPS(10, 10);
            BeginMode3D(camera);
            {
                DrawRubik(&cube);

                DrawGrid(10, 1.f);
            }
            EndMode3D();
        }
        EndDrawing();
    }

    CloseWindow();
    free(moves.items);

    return EXIT_SUCCESS;
}
