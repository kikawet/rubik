// Credit: https://github.com/hkociemba/RubiksCube-OptimalSolver/tree/master#
// This is just a C implementation of their Python code

#include <sys/param.h>

#include "hkociemba.c"


void run_asserts()
{
    // TODO: run this only once :)
    assert(flipslicesorted_twist_depth3_start != NULL);
    assert(flipslicesorted_twist_depth3_end != NULL);
    assert(flipslicesorted_twist_depth3_start < flipslicesorted_twist_depth3_end);
    assert(flipslicesorted_twist_depth3_size != 0);
    assert(
        flipslicesorted_twist_depth3_end - flipslicesorted_twist_depth3_start == FLIPSLICESORTED_TWIST_DEPTH3_LENGTH);

    assert(get_twist(&(CubieCube){ .co = {2,0,0,1,1,0,0,2}}) == 1494);
}

void debug_coordCube(const CoordCube* coc)
{
    printf("corners: %ld\n", coc->corners);
    printf("\n");
    printf("UD_twist: %ld\n", coc->UD_twist);
    printf("UD_flip: %ld\n", coc->UD_flip);
    printf("UD_slice_sorted: %ld\n", coc->UD_slice_sorted);
    printf("\n");
    printf("RL_twist: %ld\n", coc->RL_twist);
    printf("RL_flip: %ld\n", coc->RL_flip);
    printf("RL_slice_sorted: %ld\n", coc->RL_slice_sorted);
    printf("\n");
    printf("FB_twist: %ld\n", coc->FB_twist);
    printf("FB_twist: %ld\n", coc->FB_twist);
    printf("FB_slice_sorted: %ld\n", coc->FB_slice_sorted);
    printf("\n");
    printf("UD_flipslicesorted_clsidx: %d\n", coc->UD_flipslicesorted_clsidx);
    printf("UD_flipslicesorted_sym: %d\n", coc->UD_flipslicesorted_sym);
    printf("UD_flipslicesorted_rep: %d\n", coc->UD_flipslicesorted_rep);
    printf("\n");
    printf("RL_flipslicesorted_clsidx: %d\n", coc->RL_flipslicesorted_clsidx);
    printf("RL_flipslicesorted_sym: %d\n", coc->RL_flipslicesorted_sym);
    printf("RL_flipslicesorted_rep: %d\n", coc->RL_flipslicesorted_rep);
    printf("\n");
    printf("FB_flipslicesorted_clsidx: %d\n", coc->FB_flipslicesorted_clsidx);
    printf("FB_flipslicesorted_sym: %d\n", coc->FB_flipslicesorted_sym);
    printf("FB_flipslicesorted_rep: %d\n", coc->FB_flipslicesorted_rep);
    printf("\n");
    printf("UD_phasex24_depth: %ld\n", coc->UD_phasex24_depth);
    printf("RL_phasex24_depth: %ld\n", coc->RL_phasex24_depth);
    printf("FB_phasex24_depth: %ld\n", coc->FB_phasex24_depth);
    printf("\n");
    printf("corner_depth: %ld\n", coc->corner_depth);
}

typedef struct SolutionMoves
{
    FaceTurnMove* items;
    size_t count;
    size_t capacity;
} SolutionMoves;

bool solfound = false;
size_t nodecount = 0;
SolutionMoves sofar = {0};

void search(const size_t UD_flip, const size_t RL_flip, const size_t FB_flip,
            const size_t UD_twist, const size_t RL_twist, const size_t FB_twist,
            const size_t UD_slice_sorted, const size_t RL_slice_sorted, const size_t FB_slice_sorted,
            const size_t corners,
            const size_t UD_dist, const size_t RL_dist, const size_t FB_dist,
            const size_t togo)
{
    if (solfound) return;
    if (togo == 0)
    {
        if (corners == 0) // slicesorte should all be 0
        {
            solfound = true;
        }

        return;
    }

    for (int m = 0; m < FACE_TURN_MOVE_LENGTH; ++m)
    {
        if (sofar.count > 0)
        {
            const size_t diff = sofar.items[sofar.count - 1] / 3 - m / 3;
            if (diff == 0 || diff == 3) // successive moves on same face or on same axis with wrong order
            {
                continue;
            }
        }

        nodecount++;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const size_t corners1 = corners_move[N_MOVE * corners + m];
        const size_t co_dist1 = corner_depth[corners1];
        if (co_dist1 >= togo) // impossible to reach subgroup H in togo - 1 moves
        {
            continue;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const size_t UD_twist1 = twist_move[N_MOVE * UD_twist + m];
        const size_t UD_flip1 = flip_move[N_MOVE * UD_flip + m];
        const size_t UD_slice_sorted1 = slice_sorted_move[N_MOVE * UD_slice_sorted + m];

        size_t fs = N_FLIP * UD_slice_sorted1 + UD_flip1; // raw new flip_slicesorted coordinate
        // now representation as representant-symmetry pair
        size_t fs_idx = flipslicesorted_classidx[fs]; // index of representant
        size_t fs_sym = flipslicesorted_sym[fs]; // symmetry

        const size_t UD_dist1_mod3 = get_flipslicesorted_twist_depth3(
            N_TWIST * fs_idx + twist_conj[(UD_twist1 << 4) + fs_sym]
        );
        const size_t UD_dist1 = distance[3 * UD_dist + UD_dist1_mod3];

        if (UD_dist1 >= togo)
        {
            continue;
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const unsigned short mrl = conj_move[N_MOVE * 16 + m]; // move viewed from 120° rotated position

        const size_t RL_twist1 = twist_move[N_MOVE * RL_twist + mrl];
        const size_t RL_flip1 = flip_move[N_MOVE * RL_flip + mrl];
        const size_t RL_slice_sorted1 = slice_sorted_move[N_MOVE * RL_slice_sorted + mrl];

        fs = N_FLIP * RL_slice_sorted1 + RL_flip1;
        fs_idx = flipslicesorted_classidx[fs];
        fs_sym = flipslicesorted_sym[fs];

        const size_t RL_dist1_mod3 = get_flipslicesorted_twist_depth3(
            N_TWIST * fs_idx + twist_conj[(RL_twist1 << 4) + fs_sym]
        );
        const size_t RL_dist1 = distance[3 * RL_dist + RL_dist1_mod3];

        if (RL_dist1 >= togo)
        {
            continue;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const unsigned short mfb = conj_move[N_MOVE * 32 + m]; // move viewed from 240° rotated position

        const size_t FB_twist1 = twist_move[N_MOVE * FB_twist + mfb];
        const size_t FB_flip1 = flip_move[N_MOVE * FB_flip + mfb];
        const size_t FB_slice_sorted1 = slice_sorted_move[N_MOVE * FB_slice_sorted + mfb];

        fs = N_FLIP * FB_slice_sorted1 + FB_flip1;
        fs_idx = flipslicesorted_classidx[fs];
        fs_sym = flipslicesorted_sym[fs];

        const size_t FB_dist1_mod3 = get_flipslicesorted_twist_depth3(
            N_TWIST * fs_idx + twist_conj[(FB_twist1 << 4) + fs_sym]
        );
        const size_t FB_dist1 = distance[3 * FB_dist + FB_dist1_mod3];

        if (FB_dist1 >= togo)
        {
            continue;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (UD_dist1 != 0 && UD_dist1 == RL_dist1 && RL_dist1 == FB_dist1)
        {
            if (UD_dist1 + 1 >= togo)
            {
                continue; // due to design of coordinate
            }
        }

        da_append(&sofar, m);
        search(
            UD_flip1, RL_flip1, FB_flip1,
            UD_twist1, RL_twist1, FB_twist1,
            UD_slice_sorted1, RL_slice_sorted1, FB_slice_sorted1,
            corners1,
            UD_dist1, RL_dist1, FB_dist1,
            togo - 1
        );

        if (solfound)
            return;

        sofar.count--;
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
    size_t togo = MAX(coc.UD_phasex24_depth, MAX(coc.RL_phasex24_depth, coc.FB_phasex24_depth));
    solfound = false;
    size_t totnodes = 0;
    nodecount = 0;

    while (!solfound)
    {
        sofar.count = 0;
        totnodes += nodecount;
        nodecount = 0;
        search(coc.UD_flip, coc.RL_flip, coc.FB_flip,
               coc.UD_twist, coc.RL_twist, coc.FB_twist,
               coc.UD_slice_sorted, coc.RL_slice_sorted, coc.FB_slice_sorted,
               coc.corners,
               coc.UD_phasex24_depth, coc.RL_phasex24_depth, coc.FB_phasex24_depth,
               togo);
        togo++;
    }

    TraceLog(LOG_TRACE, "Solution has %d moves", sofar.count);

    for (int i = 0; i < sofar.count; i++)
    {
        const FaceTurnMove ftm = sofar.items[i];
        da_append(queue, faceTurnMoveToMove(ftm));
    }

    return solfound;
}
