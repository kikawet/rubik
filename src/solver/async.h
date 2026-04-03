// At this point why not create a header only library? lol
// Use: #define ASYNC_IMPLEMENTATION

// Header library to calculate solution in another thread, note that solution will be cached once found
#ifndef ASYNC_H
#define ASYNC_H

#define __USE_GNU
#include <pthread.h>
#include <errno.h>

#include "solver.h"
#include "../cube.h"

typedef struct async_solver_t AsyncSolver;

// Start solver in a new thread immediately
void as_new(AsyncSolver* as, Cube cube);

// Use a custom solver; hopefully solver is not unloaded during solving lol
void as_new_custom(AsyncSolver* as, Cube cube, solve_t* solver);

// True if solution is ready, return true and insert solution into queue only the first time even if true is returned.
bool as_solve(AsyncSolver* as, Moves* queue);

// Free as in free of charge, you can call ass many times as you want baby
void as_free(AsyncSolver* as);

bool as_completed(const AsyncSolver* as);

#ifdef ASYNC_IMPLEMENTATION

typedef struct as_thread_params
{
    Cube cube;
    solve_t* solver;
} ASThreadParams;

struct async_solver_t
{
    pthread_t thread;
    ASThreadParams* params;
    Moves* solution;
};


void* async_wrapper(void* data)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    const ASThreadParams* params = data;
    // Freed on as_new or as_free
    Moves* solution = calloc(1, sizeof(Moves));
    params->solver(params->cube, solution); // This somehow leaks check valgrind
    pthread_exit(solution);
}

inline void as_new_custom(AsyncSolver* as, const Cube cube, solve_t* solver)
{
    as_free(as);

    as->params = calloc(1, sizeof(ASThreadParams));
    as->params->cube = cube;
    as->params->solver = solver;

    pthread_create(&as->thread, NULL, async_wrapper, as->params);
}

inline bool as_solve(AsyncSolver* as, Moves* queue)
{
    if (as->solution) return true;
    if (!as->thread) return true; // This is just a message to stop calling when there is not even a thread waiting

    void* sol;
    const int status = pthread_tryjoin_np(as->thread, &sol);
    if (status == EBUSY) return false;
    if (status != 0)
    {
        // unlucky :P
        fprintf(stderr, "[ERROR] as_solve: error joining thread, %s\n", strerror(status));
        abort();
    }

    as->solution = sol;
    for (size_t i = 0; i< as->solution->count;i++)
        da_append(queue, as->solution->items[i]);

    return true;
}

inline void as_free(AsyncSolver* as)
{
    if (as->thread && !as->solution) pthread_cancel(as->thread);
    if (as->params) free(as->params);
    if (as->solution) free(as->solution);

    as->thread = 0;
    as->params = NULL;
    as->solution = NULL;
}

inline bool as_completed(const AsyncSolver* as)
{
    return as->solution;
}

#endif // ASYNC_IMPLEMENTATION

#endif // ASYNC_H
