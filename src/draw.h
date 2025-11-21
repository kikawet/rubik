

#ifndef DRAW_H
#define DRAW_H

#include "cube.h"

typedef struct animation
{
    bool is_paused;
    size_t current_move;

    EFace face;     // 0 for none; Since many moves rotate the same face this is handy
    Move move;      // clone current move to modify Moves as freely
    float angle;    // degrees
    float progress; // [0, 1)
} Animation;

void DrawRubik(const Cube* cube, const Animation* a);
void drawCurrentMovement(const Animation* a, const Font* f);

#endif //DRAW_H
