#include <assert.h>
#include <dlfcn.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <raylib.h>
#include <rcamera.h>


#define ASYNC_IMPLEMENTATION
#include "solver/async.h"

#include "cube.h"
#include "draw.h"
#include "solver/solver.h"
#include "../resources/MozillaTextFont.h"

// TODO: Global state looked like a good idea at the start but after chaging the code looks like add friction, remove?
typedef struct state
{
    Cube cube;
    Moves solution;
    Animation animation;

    void* dlsolver;
    solve_t* solver;
} State;

void nextMove(State* s)
{
    if (s->solution.items == NULL) return;

    Animation* a = &s->animation;
    if (a->current_move >= s->solution.count)
    {
        // Reset rotation
        a->move = NO_MOVE;
        a->face = 0;
        a->progress = 0;
        a->angle = 0;

        // Reset queue
        a->current_move = 0;
        s->solution.count = 0;

        TraceLog(LOG_DEBUG, "Resetting state");
        return;
    }

    // Advance current
    a->move = s->solution.items[a->current_move++];
    a->face = moveToFace(a->move);
    a->progress = 0;
    a->angle = moveMaxAngle(a->move) * a->progress;

    // TODO: after a buffer of (10?) moves has been advanced "free/reset Moves"
}

void updateCube(State* s)
{
    if (s->animation.current_move >= s->solution.count && s->animation.face == 0)
        return;

    const float dt = GetFrameTime();
    const float progressSpeed = 0.4f;
    Animation* a = &s->animation;

    if (a->face == 0)
    {
        nextMove(s);
        return;
    }

    if (a->progress >= 1.f)
    {
        rotateCube(&s->cube, a->move);
        nextMove(s);
        return;
    }

    if (a->is_paused) return;

    a->progress += progressSpeed * dt;
    a->angle = moveMaxAngle(a->move) * a->progress;
}

Moves generateMoves(const size_t amount)
{
    Moves moves = {0};

    if (amount == 0)
        return moves;

    moves.count = amount;
    moves.items = malloc(sizeof(Move) * moves.count);

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
    {
        // Scramble
        // TODO: accept Moves*
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

void thistlethwaite_suffle(Cube* cube)
{
    const Move m[] = {UU, BB, Rp, FF, Rp, UU, LL, BB, Rp, BB, RR, UU, BB, Up, L, RR, U, L, F, DD, Rp, Fp};
    const size_t size = sizeof(m) / sizeof(m[0]);

    for (size_t i = 0; i < size; i++)
    {
        rotateCube(cube, m[i]);
    }
}

bool reload_libsolver(State* s)
{
    if (s->dlsolver != NULL) dlclose(s->dlsolver);

    const char* libsolver_file_name = "./libsolver.so";
    s->dlsolver = dlopen(libsolver_file_name, RTLD_NOW);

    if (s->dlsolver == NULL)
    {
        TraceLog(LOG_ERROR, "HOTRELOAD: could not load %s: %s", libsolver_file_name, dlerror());
        return false;
    }

    // Cast it to prt to remove a warning
    s->solver = (solve_t*)(intptr_t)dlsym(s->dlsolver, "solve");
    if (s->solver == NULL)
    {
        TraceLog(LOG_ERROR, "HOTRELOAD: could not find solve symbol in %s: %s", libsolver_file_name, dlerror());
        return false;
    }

    return true;
}

void parse_args(const int argc, const char** argv, Cube* cube)
{
    if (argc != 2) return;

    Moves rotations = {0};
    strToMoves(argv[1], &rotations);

    for (size_t i = 0; i < rotations.count; i++)
    {
        rotateCube(cube, rotations.items[i]);
    }

    if (rotations.items != NULL) free(rotations.items);
}

int main(const int argc, const char** argv)
{
    State state = {0};
    Cube copy = {0};
    AsyncSolver as = {0};

    if (!reload_libsolver(&state)) return EXIT_FAILURE;

    resetCube(&state.cube);

    parse_args(argc, argv, &state.cube);
    //thistlethwaite_suffle(&cube);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    InitWindow(800, 600, "Rubik's cube");

    SetTraceLogLevel(LOG_ALL);

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
        handleKeys(&state.cube, &state.solution);
        updateCube(&state); // This is more like update animation + cube

        as_solve(&as, &state.solution);

        if (IsKeyPressed(KEY_F1))
        {
            if (state.animation.current_move < state.solution.count)
            {
                TraceLog(LOG_INFO, "Cannot request a new solve while rotating!");
                continue;
            }
            memcpy(&copy, &state.cube, sizeof(Cube));
            TraceLog(LOG_INFO, "Cloned cube state before solving");
            as_new_custom(&as, state.cube, state.solver);

            if (IsKeyDown(KEY_LEFT_SHIFT))
            {
                as_solve(&as, &state.solution);
                for (size_t i = state.animation.current_move; i < state.solution.count; i++)
                {
                    rotateCube(&state.cube, state.solution.items[i]);
                }

                state.animation.current_move = state.solution.count;
            }
        }

        if (IsKeyPressed(KEY_F5))
        {
            if (!reload_libsolver(&state)) return EXIT_FAILURE;
            TraceLog(LOG_INFO, "Solver reloaded successfully");
        }

        if (IsKeyPressed(KEY_F7))
        {
            memcpy(&state.cube, &copy, sizeof(Cube));
            state.animation.current_move = 0;
            state.solution.count = 0;
            TraceLog(LOG_INFO, "Restored cube state");
        }

        if (IsKeyPressed(KEY_F8))
        {
            memcpy(&copy, &state.cube, sizeof(Cube));
            TraceLog(LOG_INFO, "Cloned cube state");
        }

        if (IsKeyPressed(KEY_N))
        {
            state.animation.progress = 1.f;
            TraceLog(LOG_INFO, "Skyped movement %s", moveToStr(state.animation.move));
        }

        if (IsKeyPressed(KEY_P))
        {
            state.animation.is_paused = !state.animation.is_paused;
            TraceLog(LOG_INFO, "Skyped movement %s", moveToStr(state.animation.move));
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawFPS(10, 10);

            BeginMode3D(camera);
            {
                DrawRubik(&state.cube, &state.animation);
                DrawGrid(10, 1.f);
            }
            EndMode3D();

            drawCurrentMovement(&state.animation, &mozillaFont);
        }
        EndDrawing();
    }

    CloseWindow();

    if (state.solution.capacity) free(state.solution.items);
    as_free(&as);

    return EXIT_SUCCESS;
}
