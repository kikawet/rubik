#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>
#include <time.h>

#include "model.c"
#include "draw.c"

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
        return -90.f;
    case Up:
    case Lp:
    case Fp:
    case Rp:
    case Bp:
    case Dp:
        return 90.f;
    case U2:
    case L2:
    case F2:
    case R2:
    case B2:
    case D2:
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

static inline void rotateUpCCW(Cube* cube)
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

static inline void rotateDownCCW(Cube* cube)
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

static inline void rotateFrontCCW(Cube* cube)
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

static inline void rotateBackCCW(Cube* cube)
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

static inline void rotateLeftCCW(Cube* cube)
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

static inline void rotateRightCCW(Cube* cube)
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

void rotateCube(Cube* cube,const Move m)
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
    if (moves->current >= moves->count && cube->rotation.progress >= 1.f)
        return;

    const float dt = GetFrameTime();
    const float progressSpeed = 0.4f;
    Rotation* r = &cube->rotation;

    if (cube->rotation.face == 0)
    {
        r->move = moves->items[moves->current++];
        r->face = moveToFace(r->move);
        r->progress = 0;
        r->angle = moveMaxAngle(r->move) * r->progress;
        return;
    }

    if (r->progress >= 1.f)
    {
        rotateCube(cube, r->move);
        r->move = moves->items[moves->current++];
        r->face = moveToFace(r->move);
        r->progress = 0;
        r->angle = moveMaxAngle(r->move) * r->progress;
        return;
    }

    r->progress += progressSpeed * dt;
    r->angle = moveMaxAngle(r->move) * r->progress;
}

Cube newCube(void)
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

Moves generateMoves(const size_t amount)
{
    Moves moves = {0};

    if (amount == 0)
        return moves;

    moves.count = amount;
    moves.items = malloc(sizeof(Move)*moves.count);

    SetRandomSeed(time(NULL));
    for (size_t i = 0; i < moves.count; i++)
    {
        Move m = NO_MOVE;
        while (m == NO_MOVE)
        {
            m =  GetRandomValue(NO_MOVE,  MOVE_LENGTH - 1);
        }
        moves.items[i] = m;
    }

    return moves;
}

int main()
{
    Cube cube = newCube();
    Moves moves = generateMoves(2);

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
        const float dt = GetFrameTime();

        if (IsKeyDown(KEY_SPACE))
        {
            camera.position.y += 10.f * dt;
        }

        if (IsKeyDown(KEY_LEFT_CONTROL))
        {
            camera.position.y -= 10.f * dt;
        }

        UpdateCamera(&camera, CAMERA_ORBITAL);
        updateCube(&cube, &moves);

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
