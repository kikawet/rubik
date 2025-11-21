
#ifndef SOLVER_H
#define SOLVER_H

#include <stdbool.h>

#include "../cube.h"

// Call once to push into queue all the required moves to solve the cube
// Need to pass cube by value so changes don't affect it
// Returns true if success :)
bool solve(Cube cube, Moves* queue);

typedef bool (solve_t)(Cube cube, Moves* queue);

#endif //SOLVER_H
