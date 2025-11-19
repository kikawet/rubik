// Credit: https://github.com/hkociemba/RubiksCube-OptimalSolver/tree/master#
// This code is a subset from the one linked; basically only the code required to solve a cube lol

#include <stdio.h>

#include "hkociemba.c"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// TODO: move this to a common place
void run_asserts(void)
{
#ifdef NOT_A_PUSSY
    TraceLog(LOG_TRACE, "Using generated data from include files");
#else
    TraceLog(LOG_TRACE, "Using generated data from object files");
#endif


    // TODO: run this only once :)
    assert(flipslicesorted_twist_depth3_start != NULL);
    assert(flipslicesorted_twist_depth3_end != NULL);
    assert(&flipslicesorted_twist_depth3_start[0] < &flipslicesorted_twist_depth3_end[0]);
    assert(
        flipslicesorted_twist_depth3_end - flipslicesorted_twist_depth3_start == FLIPSLICESORTED_TWIST_DEPTH3_LENGTH);

    assert(
        corner_depth[0] == 0 && corner_depth[1] == 5 &&
        "You must compile using define -DNOT_A_PUSSY (also use TCC if you don't want to compile for +30sec)");

    assert(get_twist(&(CubieCube){.co = {2, 0, 0, 1, 1, 0, 0, 2}}) == 1494);
}


typedef struct SolutionMoves
{
    FaceTurnMove* items;
    size_t count;
    size_t capacity;
} SolutionMoves;

typedef struct SearchContext
{
    bool solfound;
    size_t nodecount;
    SolutionMoves sofar;
} SearchContext;

bool calculate_next_coords(CoordCube* ncc, const FaceTurnMove m, const CoordCube cc, const int32_t togo)
{
    const int32_t corners1 = corners_move[N_MOVE * cc.corners + m];
    const int32_t co_dist1 = corner_depth[corners1];
    if (co_dist1 >= togo) // impossible to reach subgroup H in togo - 1 moves
    {
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const int32_t UD_twist1 = twist_move[N_MOVE * cc.UD_twist + m];
    const int32_t UD_flip1 = flip_move[N_MOVE * cc.UD_flip + m];
    const int32_t UD_slice_sorted1 = slice_sorted_move[N_MOVE * cc.UD_slice_sorted + m];

    int32_t fs = N_FLIP * UD_slice_sorted1 + UD_flip1; // raw new flip_slicesorted coordinate
    // now representation as representant-symmetry pair
    uint32_t fs_idx = flipslicesorted_classidx[fs]; // index of representant
    int32_t fs_sym = flipslicesorted_sym[fs]; // symmetry

    const int32_t UD_dist1_mod3 = get_flipslicesorted_twist_depth3(
        N_TWIST * fs_idx + twist_conj[(UD_twist1 << 4) + fs_sym]
    );
    const int32_t UD_dist1 = (int32_t)distance[3 * cc.UD_phasex24_depth + UD_dist1_mod3];

    if (UD_dist1 >= togo)
    {
        return false;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const unsigned short mrl = conj_move[N_MOVE * 16 + m]; // move viewed from 120° rotated position

    const int32_t RL_twist1 = twist_move[N_MOVE * cc.RL_twist + mrl];
    const int32_t RL_flip1 = flip_move[N_MOVE * cc.RL_flip + mrl];
    const int32_t RL_slice_sorted1 = slice_sorted_move[N_MOVE * cc.RL_slice_sorted + mrl];

    fs = N_FLIP * RL_slice_sorted1 + RL_flip1;
    fs_idx = flipslicesorted_classidx[fs];
    fs_sym = flipslicesorted_sym[fs];

    const int32_t RL_dist1_mod3 = get_flipslicesorted_twist_depth3(
        N_TWIST * fs_idx + twist_conj[(RL_twist1 << 4) + fs_sym]
    );
    const int32_t RL_dist1 = (int32_t)distance[3 * cc.RL_phasex24_depth + RL_dist1_mod3];

    if (RL_dist1 >= togo)
    {
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const unsigned short mfb = conj_move[N_MOVE * 32 + m]; // move viewed from 240° rotated position

    const int32_t FB_twist1 = twist_move[N_MOVE * cc.FB_twist + mfb];
    const int32_t FB_flip1 = flip_move[N_MOVE * cc.FB_flip + mfb];
    const int32_t FB_slice_sorted1 = slice_sorted_move[N_MOVE * cc.FB_slice_sorted + mfb];

    fs = N_FLIP * FB_slice_sorted1 + FB_flip1;
    fs_idx = flipslicesorted_classidx[fs];
    fs_sym = flipslicesorted_sym[fs];

    const int32_t FB_dist1_mod3 = get_flipslicesorted_twist_depth3(
        N_TWIST * fs_idx + twist_conj[(FB_twist1 << 4) + fs_sym]
    );
    const int32_t FB_dist1 = (int32_t)distance[3 * cc.FB_phasex24_depth + FB_dist1_mod3];

    if (FB_dist1 >= togo)
    {
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (UD_dist1 != 0 && UD_dist1 == RL_dist1 && RL_dist1 == FB_dist1)
    {
        if (UD_dist1 + 1 >= togo)
        {
            return false; // due to design of coordinate
        }
    }

    ncc->UD_flip = UD_flip1;
    ncc->UD_twist = UD_twist1;
    ncc->UD_slice_sorted = UD_slice_sorted1;
    ncc->UD_phasex24_depth = UD_dist1;
    ncc->RL_flip = RL_flip1;
    ncc->RL_twist = RL_twist1;
    ncc->RL_slice_sorted = RL_slice_sorted1;
    ncc->RL_phasex24_depth = RL_dist1;
    ncc->FB_flip = FB_flip1;
    ncc->FB_twist = FB_twist1;
    ncc->FB_slice_sorted = FB_slice_sorted1;
    ncc->FB_phasex24_depth = FB_dist1;
    ncc->corners = corners1;

    return true;
}

void search(SearchContext* ctx, const CoordCube cc, const int32_t togo)
{
    if (ctx->solfound) return;
    if (togo == 0)
    {
        if (cc.corners == 0) // slicesorte should all be 0
        {
            ctx->solfound = true;
        }

        return;
    }

    CoordCube ncc[FACE_TURN_MOVE_LENGTH] = {0};
    bool hasNext[FACE_TURN_MOVE_LENGTH] = {0};
    //FIXME: [PERFORMANCE] I separated this two for loops because it looked like this one can be parallelized
    //       BUT it adds to much overhead (using openmp) that it takes about x5 longer to run.
    for (int m = 0; m < FACE_TURN_MOVE_LENGTH; ++m)
    {
        if (ctx->sofar.count > 0)
        {
            const int16_t diff = ctx->sofar.items[ctx->sofar.count - 1] / 3 - m / 3;
            if (diff == 0 || diff == 3) // successive moves on same face or on same axis with wrong order
            {
                continue;
            }
        }

        ctx->nodecount++;
        hasNext[m] = calculate_next_coords(&ncc[m], m, cc, togo);
    }

    for (int m = 0; m < FACE_TURN_MOVE_LENGTH; ++m)
    {
        if (!hasNext[m]) continue;
        da_append(&ctx->sofar, m);

        search(ctx, ncc[m], togo - 1);

        if (ctx->solfound)
            return;

        ctx->sofar.count--;
    }
}

bool solve(const Cube cube, Moves* queue)
{
    run_asserts();
    TraceLog(LOG_DEBUG, "RUNNING OPTIMAL SOLVER");

    const FaceCube fc = cubeToFaceCube(&cube);
    const CubieCube cc2 = faceToCubie(&fc);
    const CoordCube coc = cubieToCoordCube(&cc2);
    // F rotation in FaceCube cords: UUUUUULLLURRURRURRFFFFFFFFFRRRDDDDDDLLDLLDLLDBBBBBBBBB
    // dumpFaceCube(&fc);
    // printf("UUUUUULLLURRURRURRFFFFFFFFFRRRDDDDDDLLDLLDLLDBBBBBBBBB\n");

    //debug_cordcube(&coc);

    // lower bound for distance to solved
    int32_t togo = MAX(coc.UD_phasex24_depth, MAX(coc.RL_phasex24_depth, coc.FB_phasex24_depth));
    size_t totnodes = 0;
    SearchContext ctx = {0};

    while (!ctx.solfound)
    {
        ctx.sofar.count = 0;
        totnodes += ctx.nodecount;
        ctx.nodecount = 0;
        search(&ctx, coc, togo);
        togo++;
    }

    TraceLog(LOG_TRACE, "Solution has %d moves", ctx.sofar.count);
    TraceLog(LOG_TRACE, "Searched %d nodes", totnodes);

    for (size_t i = 0; i < ctx.sofar.count; i++)
    {
        const FaceTurnMove ftm = ctx.sofar.items[i];
        da_append(queue, faceTurnMoveToMove(ftm));
    }

    // TODO: if this variable is global no free nor realloc should be required
    if (ctx.sofar.items) free(ctx.sofar.items);

    return ctx.solfound;
}
