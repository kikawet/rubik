#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>
#include <rcamera.h>
#include <time.h>

#include "model.h"
#include "draw.h"
#include "resources/MozillaTextFont.h"

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

int main()
{
    // Moves m = thistlethwaite_suffle();



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
