#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>
#include <rcamera.h>
#include <time.h>

#include "model.c"
#include "draw.c"
#include "resources/MozillaTextFont.h"

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

// degrees angles are reversed bc conversions are hard
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

void nextMove(Rotation* r, Moves* queue)
{
    if (queue->items == NULL) return;

    if (queue->current >= queue->count)
    {
        // Reset rotation
        r->move = NO_MOVE;
        r->face = 0;
        r->progress = 0;
        r->angle = 0;

        // Reset queue
        queue->current = 0;
        queue->count = 0;

        TraceLog(LOG_DEBUG, "Resetting state");
        return;
    }

    // Advance current
    r->move = queue->items[queue->current++];
    r->face = moveToFace(r->move);
    r->progress = 0;
    r->angle = moveMaxAngle(r->move) * r->progress;

    // TODO: after a buffer of (10?) moves has been advanced "free/reset Moves"
}

void updateCube(Cube* cube, Moves* moves)
{
    if (moves->current >= moves->count && cube->rotation.face == 0)
        return;

    const float dt = GetFrameTime();
    const float progressSpeed = 0.4f;
    Rotation* r = &cube->rotation;

    if (cube->rotation.face == 0)
    {
        nextMove(r, moves);
        return;
    }

    if (r->progress >= 1.f)
    {
        rotateCube(cube, r->move);
        nextMove(r, moves);
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
        const Move m = GetRandomValue(NO_MOVE + 1, MOVE_LENGTH - 1);
        moves.items[i] = m;
    }

    return moves;
}

void updateCamera(Camera* camera)
{
    const float cameraSpeed = 20.f;
    const bool moveInWorldPlane = false;

    camera->target = (Vector3){0.0f, 0.0f, 0.0f};

    // Camera speeds based on frame time
    const float cameraDistance = cameraSpeed * GetFrameTime();

    // Keyboard support
    if (IsKeyDown(KEY_UP)) CameraMoveForward(camera, cameraDistance, moveInWorldPlane);
    if (IsKeyDown(KEY_LEFT)) CameraMoveRight(camera, -cameraDistance, moveInWorldPlane);
    if (IsKeyDown(KEY_DOWN)) CameraMoveForward(camera, -cameraDistance, moveInWorldPlane);
    if (IsKeyDown(KEY_RIGHT)) CameraMoveRight(camera, cameraDistance, moveInWorldPlane);

    if (IsKeyDown(KEY_SPACE)) CameraMoveUp(camera, cameraDistance);
    if (IsKeyDown(KEY_LEFT_CONTROL)) CameraMoveUp(camera, -cameraDistance);

    // Zoom target distance
    CameraMoveToTarget(camera, -GetMouseWheelMove());
    if (IsKeyPressed(KEY_KP_SUBTRACT)) CameraMoveToTarget(camera, 2.0f);
    if (IsKeyPressed(KEY_KP_ADD)) CameraMoveToTarget(camera, -2.0f);
}

void handleKeys(Cube* cube, Moves* queue)
{
    if (IsKeyPressed(KEY_S))
    {   // Scramble
        const Moves moves = generateMoves(15);
        for (size_t i = 0; i < moves.count; i++)
            rotateCube(cube, moves.items[i]);
        free(moves.items);
    }

    const bool ccw = IsKeyDown(KEY_LEFT_SHIFT);

    if (IsKeyPressed(KEY_U))
        da_append(queue, ccw ? Up : U);

    if (IsKeyPressed(KEY_L))
        da_append(queue, ccw ? Lp : L);

    if (IsKeyPressed(KEY_F))
        da_append(queue, ccw ? Fp : F);

    if (IsKeyPressed(KEY_R))
        da_append(queue, ccw ? Rp : R);

    if (IsKeyPressed(KEY_B))
        da_append(queue, ccw ? Bp : B);

    if (IsKeyPressed(KEY_D))
        da_append(queue, ccw ? Dp : D);
}

void drawCurrentMovement(const Cube* cube, const Font* f)
{
    if (cube->rotation.face == 0 || cube->rotation.move == NO_MOVE)
        return;

    const char* movementText = moveToStr(cube->rotation.move);
    const int fontSize = f->baseSize;
    const float spacing = 2.f;

    const int width = GetScreenWidth();
    const int height = GetScreenHeight();
    const Vector2 textSize = MeasureTextEx(*f, movementText, (float)fontSize, spacing);
    const Vector2 movementTextPos = (Vector2){
        ((float)width - textSize.x) / 2.f, (float)height * 0.1f
    };

    DrawTextEx(*f, movementText, movementTextPos, (float)fontSize, spacing, BLACK);
}

int main()
{
    Cube cube = newCube();
    Moves queue = {0};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    InitWindow(800, 600, "Rubik's cube");

    const Font mozillaFont = LoadFont_MozillaTextFont();
    SetExitKey(KEY_Q);

    Camera3D camera = {0};
    camera.position = (Vector3){10.0f, 10.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose())
    {
        updateCamera(&camera);
        handleKeys(&cube, &queue);
        updateCube(&cube, &queue);

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

            drawCurrentMovement(&cube, &mozillaFont);
        }
        EndDrawing();
    }

    if (queue.capacity) free(queue.items);

    CloseWindow();

    return EXIT_SUCCESS;
}
