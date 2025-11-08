// Credit: https://github.com/hkociemba/RubiksCube-OptimalSolver/tree/master#

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

void search(size_t UD_flip, size_t RL_flip, size_t FB_flip,
            size_t UD_twist, size_t RL_twist, size_t FB_twist,
            size_t UD_slice_sorted, size_t RL_slice_sorted, size_t FB_slice_sorted,
            size_t corners,
            size_t UD_dist, size_t RL_dist, size_t FB_dist,
            size_t togo)
{
	// printf("togo == %ld\n", togo);
    if (solfound) return;
    if (togo == 0)
    {
    	// printf("togo == 0 exit\n");
        if (corners == 0) // slicesorte should all be 0
        {
        	// printf("corners == 0 exit\n");
        	solfound = true;
        }

        return;
    }

	for (int m = 0; m < FACE_TURN_MOVE_LENGTH; ++m)
    {
        if (sofar.count > 0)
        {
            size_t diff = sofar.items[sofar.count - 1] / 3 - m / 3;
            if (diff == 0 || diff == 3) // successive moves on same face or on same axis with wrong order
            {
            	// printf("diff exit\n");
            	continue;
            }
        }

        nodecount++;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const size_t corners1 = corners_move[N_MOVE * corners + m];
        const size_t co_dist1 = corner_depth[corners1];
        if (co_dist1 >= togo) // impossible to reach subgroup H in togo - 1 moves
        {
        	//printf("co_dist1 exit %ld >= %ld %s\n", co_dist1, togo, faceTurnMoveToStr(m));
        	continue;
        }    
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        size_t UD_twist1 = twist_move[N_MOVE * UD_twist + m];
        size_t UD_flip1 = flip_move[N_MOVE * UD_flip + m];
        size_t UD_slice_sorted1 = slice_sorted_move[N_MOVE * UD_slice_sorted + m];

        size_t fs = N_FLIP * UD_slice_sorted1 + UD_flip1; // raw new flip_slicesorted coordinate
        // now representation as representant-symmetry pair
        size_t fs_idx = flipslicesorted_classidx[fs]; // index of representant
        size_t fs_sym = flipslicesorted_sym[fs]; // symmetry

        size_t UD_dist1_mod3 = get_flipslicesorted_twist_depth3(
            N_TWIST * fs_idx + twist_conj[(UD_twist1 << 4) + fs_sym]
        );
        size_t UD_dist1 = distance[3*UD_dist + UD_dist1_mod3];

        if (UD_dist1 >= togo)
        {
        	// printf("UD_dist1 exit %ld >= %ld \n", UD_dist1, togo);
        	continue;
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const unsigned short mrl = conj_move[N_MOVE * 16 + m]; // move viewed from 120° rotated position

        size_t RL_twist1 = twist_move[N_MOVE * RL_twist + mrl];
        size_t RL_flip1 = flip_move[N_MOVE * RL_flip + mrl];
        size_t RL_slice_sorted1 = slice_sorted_move[N_MOVE * RL_slice_sorted + mrl];

        fs = N_FLIP * RL_slice_sorted1 + RL_flip1;
        fs_idx = flipslicesorted_classidx[fs];
        fs_sym = flipslicesorted_sym[fs];

        size_t RL_dist1_mod3 = get_flipslicesorted_twist_depth3(
        	N_TWIST * fs_idx + twist_conj[(RL_twist1 << 4) + fs_sym]
        );
		size_t RL_dist1 = distance[3 * RL_dist + RL_dist1_mod3];

		if (RL_dist1 >= togo)
		{
        	// printf("RL_dist1 exit\n");
        	continue;
        }

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		const unsigned short mfb = conj_move[N_MOVE * 32 + m]; // move viewed from 240° rotated position

        size_t FB_twist1 = twist_move[N_MOVE * FB_twist + mfb];
        size_t FB_flip1 = flip_move[N_MOVE * FB_flip + mfb];
        size_t FB_slice_sorted1 = slice_sorted_move[N_MOVE * FB_slice_sorted + mfb];

        fs = N_FLIP * FB_slice_sorted1 + FB_flip1;
        fs_idx = flipslicesorted_classidx[fs];
        fs_sym = flipslicesorted_sym[fs];

        size_t FB_dist1_mod3 = get_flipslicesorted_twist_depth3(
        	N_TWIST * fs_idx + twist_conj[(FB_twist1 << 4) + fs_sym]
        );
        size_t FB_dist1 = distance[3 * FB_dist + FB_dist1_mod3];

        if (FB_dist1 >= togo)
        {
        	// printf("FB_dist1 exit\n");
        	continue;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (UD_dist1 != 0 && UD_dist1 == RL_dist1 && RL_dist1 == FB_dist1)
        {
        	// printf("UD_dist1 != 0 exit\n");
           	if (UD_dist1 + 1 >= togo)
           	{
           		// printf("UD_dist1 + 1 >= togo exit\n");
           		continue; // due to design of coordinate
           	}
        }

		//printf("Testing: %s\n", faceTurnMoveToStr(m));
		
        da_append(&sofar, m);
        search(
        	UD_flip1,         RL_flip1,         FB_flip1,
        	UD_twist1,        RL_twist1,       	FB_twist1,
        	UD_slice_sorted1, RL_slice_sorted1, FB_slice_sorted1,
        	corners1,
        	UD_dist1,         RL_dist1,			FB_dist1,
        	togo -1
        );

        if (solfound)
        	return;
        	
        sofar.count--;
    }

	// This is just to debug remove it later dummy dum dum
	// printf("FORCED exit\n");
    // solfound = true;
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

    // printf("Solution has %d moves\n", sofar.count);

    for(int i = 0; i < sofar.count; i++){
    	FaceTurnMove ftm = sofar.items[i];
    	// printf("%s ", faceTurnMoveToStr(ftm));
    	da_append(queue, faceTurnMoveToMove(ftm));
    }
    //printf("\n");

    return solfound;
}
