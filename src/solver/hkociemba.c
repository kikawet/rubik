// Credit: https://github.com/hkociemba/RubiksCube-OptimalSolver/tree/master#

// I created this file just in case I make more hkociemba solvers but it should be embeded :)

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <raylib.h>

#include "solver.h"

#include "../../generated/hkociemba_generated.h"

typedef enum
{
    URF,
    UFL,
    ULB,
    UBR,
    DFR,
    DLF,
    DBL,
    DRB,
    CORNER_LENGTH
} Corner;

typedef enum
{
    UR,
    UF,
    UL,
    UB,
    DR,
    DF,
    DL,
    DB,
    FR,
    FL,
    BL,
    BR,
    EDGE_LENGTH
} Edge;

// U, R, F, D, L and B denote the Up, Right, Front, Down, Left and Back face of the cube. 1, 2, and 3 denote a 90°, 180°
// and 270° clockwise rotation of the corresponding face.
typedef enum
{
    MU1, MU2, MU3,
    MR1, MR2, MR3,
    MF1, MF2, MF3,
    MD1, MD2, MD3,
    ML1, ML2, ML3,
    MB1, MB2, MB3,
    FACE_TURN_MOVE_LENGTH
} FaceTurnMove;

typedef enum { CU, CR, CF, CD, CL, CB, COLOR_CUBE_LENGTH } ColorCube;

typedef enum
{
    U1, U2, U3, U4, U5, U6, U7, U8, U9,
    R1, R2, R3, R4, R5, R6, R7, R8, R9,
    F1, F2, F3, F4, F5, F6, F7, F8, F9,
    D1, D2, D3, D4, D5, D6, D7, D8, D9,
    L1, L2, L3, L4, L5, L6, L7, L8, L9,
    B1, B2, B3, B4, B5, B6, B7, B8, B9,
    FACELET_LENGTH
} Facelet;

typedef struct CubieCube
{
    Corner cp[CORNER_LENGTH]; // corner permutation
    uint8_t co[CORNER_LENGTH]; // corner orientation
    Edge ep[EDGE_LENGTH]; // edge permutation
    uint8_t eo[EDGE_LENGTH]; // edge orientation
} CubieCube;

typedef struct CoordCube
{
    uint16_t corners;

    int32_t UD_twist;
    int32_t UD_flip;
    int32_t UD_slice_sorted;

    int32_t RL_twist;
    int32_t RL_flip;
    int32_t RL_slice_sorted;

    int32_t FB_twist;
    int32_t FB_flip;
    int32_t FB_slice_sorted;

    int32_t UD_phasex24_depth;
    int32_t RL_phasex24_depth;
    int32_t FB_phasex24_depth;
} CoordCube;

typedef struct FaceCube
{
    ColorCube f[FACELET_LENGTH];
} FaceCube;

const Facelet cornerFacelet[CORNER_LENGTH][3] = {
    {U9, R1, F3},
    {U7, F1, L3},
    {U1, L1, B3},
    {U3, B1, R3},
    {D3, F9, R7},
    {D1, L9, F7},
    {D7, B9, L7},
    {D9, R9, B7}
};

const Facelet edgeFacelet[EDGE_LENGTH][2] = {
    {U6, R2},
    {U8, F2},
    {U4, L2},
    {U2, B2},
    {D6, R8},
    {D2, F8},
    {D4, L8},
    {D8, B8},
    {F6, R4},
    {F4, L6},
    {B6, L4},
    {B4, R6}
};

const ColorCube cornerColor[CORNER_LENGTH][3] = {
    {CU, CR, CF},
    {CU, CF, CL},
    {CU, CL, CB},
    {CU, CB, CR},
    {CD, CF, CR},
    {CD, CL, CF},
    {CD, CB, CL},
    {CD, CR, CB}
};

const ColorCube edgeColor[EDGE_LENGTH][2] = {
    {CU, CR},
    {CU, CF},
    {CU, CL},
    {CU, CB},
    {CD, CR},
    {CD, CF},
    {CD, CL},
    {CD, CB},
    {CF, CR},
    {CF, CL},
    {CB, CL},
    {CB, CR}
};

// 0 is index of the solved state
#define SOLVED 0

// number of possible face moves
#define N_MOVE 18
// 2^11 possible edge orientations
#define N_FLIP 2048
// 3^7 possible corner orientations
#define N_TWIST 2187

const CubieCube basicSymCube[4] = {
    // rot_urf3 - 120° clockwise rotation around the long diagonal URF-DBL
    {
        .cp = {URF, DFR, DLF, UFL, UBR, DRB, DBL, ULB},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {UF, FR, DF, FL, UB, BR, DB, BL, UR, DR, DL, UL},
        .eo = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    },
    // rot_f2 - 180° rotation around the axis through the F and B centers
    {
        .cp = {DLF, DFR, DRB, DBL, UFL, URF, UBR, ULB},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {DL, DF, DR, DB, UL, UF, UR, UB, FL, FR, BR, BL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    // rot_u4 - 90° clockwise rotation around the axis through the U and D centers
    {
        .cp = {UBR, URF, UFL, ULB, DRB, DFR, DLF, DBL},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {UB, UR, UF, UL, DB, DR, DF, DL, BR, FR, FL, BL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    // mirr_lr2 - reflection at the plane through the U, D, F, B centers
    {
        .cp = {UFL, URF, UBR, ULB, DLF, DFR, DRB, DBL},
        .co = {3, 3, 3, 3, 3, 3, 3, 3},
        .ep = {UL, UF, UR, UB, DL, DF, DR, DB, FL, FR, BR, BL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    }
};

#define SYMCUBE_LENGTH 48

const CubieCube symCube[SYMCUBE_LENGTH] = {
    {
        .cp = {URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {UFL, URF, UBR, ULB, DLF, DFR, DRB, DBL},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {UL, UF, UR, UB, DL, DF, DR, DB, FL, FR, BR, BL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {UBR, URF, UFL, ULB, DRB, DFR, DLF, DBL},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {UB, UR, UF, UL, DB, DR, DF, DL, BR, FR, FL, BL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {URF, UBR, ULB, UFL, DFR, DRB, DBL, DLF},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {UF, UR, UB, UL, DF, DR, DB, DL, FR, BR, BL, FL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {ULB, UBR, URF, UFL, DBL, DRB, DFR, DLF},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {UL, UB, UR, UF, DL, DB, DR, DF, BL, BR, FR, FL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {UBR, ULB, UFL, URF, DRB, DBL, DLF, DFR},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {UR, UB, UL, UF, DR, DB, DL, DF, BR, BL, FL, FR},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {UFL, ULB, UBR, URF, DLF, DBL, DRB, DFR},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {UF, UL, UB, UR, DF, DL, DB, DR, FL, BL, BR, FR},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {ULB, UFL, URF, UBR, DBL, DLF, DFR, DRB},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {UB, UL, UF, UR, DB, DL, DF, DR, BL, FL, FR, BR},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {DLF, DFR, DRB, DBL, UFL, URF, UBR, ULB},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {DL, DF, DR, DB, UL, UF, UR, UB, FL, FR, BR, BL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {DFR, DLF, DBL, DRB, URF, UFL, ULB, UBR},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {DR, DF, DL, DB, UR, UF, UL, UB, FR, FL, BL, BR},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {DBL, DLF, DFR, DRB, ULB, UFL, URF, UBR},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {DB, DL, DF, DR, UB, UL, UF, UR, BL, FL, FR, BR},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {DLF, DBL, DRB, DFR, UFL, ULB, UBR, URF},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {DF, DL, DB, DR, UF, UL, UB, UR, FL, BL, BR, FR},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {DRB, DBL, DLF, DFR, UBR, ULB, UFL, URF},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {DR, DB, DL, DF, UR, UB, UL, UF, BR, BL, FL, FR},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {DBL, DRB, DFR, DLF, ULB, UBR, URF, UFL},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {DL, DB, DR, DF, UL, UB, UR, UF, BL, BR, FR, FL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {DFR, DRB, DBL, DLF, URF, UBR, ULB, UFL},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {DF, DR, DB, DL, UF, UR, UB, UL, FR, BR, BL, FL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {DRB, DFR, DLF, DBL, UBR, URF, UFL, ULB},
        .co = {0, 0, 0, 0, 0, 0, 0, 0},
        .ep = {DB, DR, DF, DL, UB, UR, UF, UL, BR, FR, FL, BL},
        .eo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        .cp = {URF, DFR, DLF, UFL, UBR, DRB, DBL, ULB},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {UF, FR, DF, FL, UB, BR, DB, BL, UR, DR, DL, UL},
        .eo = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    },
    {
        .cp = {DFR, URF, UFL, DLF, DRB, UBR, ULB, DBL},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {DF, FR, UF, FL, DB, BR, UB, BL, DR, UR, UL, DL},
        .eo = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    },
    {
        .cp = {UFL, URF, DFR, DLF, ULB, UBR, DRB, DBL},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {FL, UF, FR, DF, BL, UB, BR, DB, UL, UR, DR, DL},
        .eo = {0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    },
    {
        .cp = {URF, UFL, DLF, DFR, UBR, ULB, DBL, DRB},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {FR, UF, FL, DF, BR, UB, BL, DB, UR, UL, DL, DR},
        .eo = {0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    },
    {
        .cp = {DLF, UFL, URF, DFR, DBL, ULB, UBR, DRB},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {DF, FL, UF, FR, DB, BL, UB, BR, DL, UL, UR, DR},
        .eo = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    },
    {
        .cp = {UFL, DLF, DFR, URF, ULB, DBL, DRB, UBR},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {UF, FL, DF, FR, UB, BL, DB, BR, UL, DL, DR, UR},
        .eo = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    },
    {
        .cp = {DFR, DLF, UFL, URF, DRB, DBL, ULB, UBR},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {FR, DF, FL, UF, BR, DB, BL, UB, DR, DL, UL, UR},
        .eo = {0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    },
    {
        .cp = {DLF, DFR, URF, UFL, DBL, DRB, UBR, ULB},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {FL, DF, FR, UF, BL, DB, BR, UB, DL, DR, UR, UL},
        .eo = {0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    },
    {
        .cp = {DRB, UBR, ULB, DBL, DFR, URF, UFL, DLF},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {DB, BR, UB, BL, DF, FR, UF, FL, DR, UR, UL, DL},
        .eo = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    },
    {
        .cp = {UBR, DRB, DBL, ULB, URF, DFR, DLF, UFL},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {UB, BR, DB, BL, UF, FR, DF, FL, UR, DR, DL, UL},
        .eo = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    },
    {
        .cp = {DBL, DRB, UBR, ULB, DLF, DFR, URF, UFL},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {BL, DB, BR, UB, FL, DF, FR, UF, DL, DR, UR, UL},
        .eo = {0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    },
    {
        .cp = {DRB, DBL, ULB, UBR, DFR, DLF, UFL, URF},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {BR, DB, BL, UB, FR, DF, FL, UF, DR, DL, UL, UR},
        .eo = {0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    },
    {
        .cp = {ULB, DBL, DRB, UBR, UFL, DLF, DFR, URF},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {UB, BL, DB, BR, UF, FL, DF, FR, UL, DL, DR, UR},
        .eo = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    },
    {
        .cp = {DBL, ULB, UBR, DRB, DLF, UFL, URF, DFR},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {DB, BL, UB, BR, DF, FL, UF, FR, DL, UL, UR, DR},
        .eo = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    },
    {
        .cp = {UBR, ULB, DBL, DRB, URF, UFL, DLF, DFR},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {BR, UB, BL, DB, FR, UF, FL, DF, UR, UL, DL, DR},
        .eo = {0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    },
    {
        .cp = {ULB, UBR, DRB, DBL, UFL, URF, DFR, DLF},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {BL, UB, BR, DB, FL, UF, FR, DF, UL, UR, DR, DL},
        .eo = {0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    },
    {
        .cp = {URF, UBR, DRB, DFR, UFL, ULB, DBL, DLF},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {FR, UR, BR, DR, FL, UL, BL, DL, UF, UB, DB, DF},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {UBR, URF, DFR, DRB, ULB, UFL, DLF, DBL},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {BR, UR, FR, DR, BL, UL, FL, DL, UB, UF, DF, DB},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {DFR, URF, UBR, DRB, DLF, UFL, ULB, DBL},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {DR, FR, UR, BR, DL, FL, UL, BL, DF, UF, UB, DB},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {URF, DFR, DRB, UBR, UFL, DLF, DBL, ULB},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {UR, FR, DR, BR, UL, FL, DL, BL, UF, DF, DB, UB},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {DRB, DFR, URF, UBR, DBL, DLF, UFL, ULB},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {BR, DR, FR, UR, BL, DL, FL, UL, DB, DF, UF, UB},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {DFR, DRB, UBR, URF, DLF, DBL, ULB, UFL},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {FR, DR, BR, UR, FL, DL, BL, UL, DF, DB, UB, UF},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {UBR, DRB, DFR, URF, ULB, DBL, DLF, UFL},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {UR, BR, DR, FR, UL, BL, DL, FL, UB, DB, DF, UF},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {DRB, UBR, URF, DFR, DBL, ULB, UFL, DLF},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {DR, BR, UR, FR, DL, BL, UL, FL, DB, UB, UF, DF},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {ULB, UFL, DLF, DBL, UBR, URF, DFR, DRB},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {BL, UL, FL, DL, BR, UR, FR, DR, UB, UF, DF, DB},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {UFL, ULB, DBL, DLF, URF, UBR, DRB, DFR},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {FL, UL, BL, DL, FR, UR, BR, DR, UF, UB, DB, DF},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {DBL, ULB, UFL, DLF, DRB, UBR, URF, DFR},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {DL, BL, UL, FL, DR, BR, UR, FR, DB, UB, UF, DF},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {ULB, DBL, DLF, UFL, UBR, DRB, DFR, URF},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {UL, BL, DL, FL, UR, BR, DR, FR, UB, DB, DF, UF},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {DLF, DBL, ULB, UFL, DFR, DRB, UBR, URF},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {FL, DL, BL, UL, FR, DR, BR, UR, DF, DB, UB, UF},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {DBL, DLF, UFL, ULB, DRB, DFR, URF, UBR},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {BL, DL, FL, UL, BR, DR, FR, UR, DB, DF, UF, UB},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {UFL, DLF, DBL, ULB, URF, DFR, DRB, UBR},
        .co = {1, 2, 1, 2, 2, 1, 2, 1},
        .ep = {UL, FL, DL, BL, UR, FR, DR, BR, UF, DF, DB, UB},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        .cp = {DLF, UFL, ULB, DBL, DFR, URF, UBR, DRB},
        .co = {2, 1, 2, 1, 1, 2, 1, 2},
        .ep = {DL, FL, UL, BL, DR, FR, UR, BR, DF, UF, UB, DB},
        .eo = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    }
};

#define DISTANCE_LENGTH 60
const int8_t distance[DISTANCE_LENGTH] = {
    0, 1, -1, 0, 1, 2, 3, 1, 2, 3,
    4, 2, 3, 4, 5, 6, 4, 5, 6, 7,
    5, 6, 7, 8, 9, 7, 8, 9, 10, 8,
    9, 10, 11, 12, 10, 11, 12, 13, 11, 12,
    13, 14, 15, 13, 14, 15, 16, 14, 15, 16,
    17, 18, 16, 17, 18, 19, 17, 18, 19, 20
};

#define CONJ_MOVE_LENGTH 864
const uint8_t conj_move[CONJ_MOVE_LENGTH] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 2, 1, 0, 14, 13, 12, 8, 7, 6, 11, 10, 9, 5, 4, 3, 17,
    16, 15, 0, 1, 2, 15, 16, 17, 3, 4, 5, 9, 10, 11, 6, 7, 8, 12, 13, 14, 2, 1, 0, 8, 7, 6, 5, 4, 3, 11, 10, 9, 17, 16,
    15, 14, 13, 12, 0, 1, 2, 12, 13, 14, 15, 16, 17, 9, 10, 11, 3, 4, 5, 6, 7, 8, 2, 1, 0, 5, 4, 3, 17, 16, 15, 11, 10,
    9, 14, 13, 12, 8, 7, 6, 0, 1, 2, 6, 7, 8, 12, 13, 14, 9, 10, 11, 15, 16, 17, 3, 4, 5, 2, 1, 0, 17, 16, 15, 14, 13,
    12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 9, 10, 11, 12, 13, 14, 6, 7, 8, 0, 1, 2, 3, 4, 5, 15, 16, 17, 11, 10, 9, 5, 4, 3,
    8, 7, 6, 2, 1, 0, 14, 13, 12, 17, 16, 15, 9, 10, 11, 15, 16, 17, 12, 13, 14, 0, 1, 2, 6, 7, 8, 3, 4, 5, 11, 10, 9,
    8, 7, 6, 14, 13, 12, 2, 1, 0, 17, 16, 15, 5, 4, 3, 9, 10, 11, 3, 4, 5, 15, 16, 17, 0, 1, 2, 12, 13, 14, 6, 7, 8, 11,
    10, 9, 14, 13, 12, 17, 16, 15, 2, 1, 0, 5, 4, 3, 8, 7, 6, 9, 10, 11, 6, 7, 8, 3, 4, 5, 0, 1, 2, 15, 16, 17, 12, 13,
    14, 11, 10, 9, 17, 16, 15, 5, 4, 3, 2, 1, 0, 8, 7, 6, 14, 13, 12, 6, 7, 8, 0, 1, 2, 3, 4, 5, 15, 16, 17, 9, 10, 11,
    12, 13, 14, 8, 7, 6, 11, 10, 9, 5, 4, 3, 17, 16, 15, 2, 1, 0, 14, 13, 12, 6, 7, 8, 12, 13, 14, 0, 1, 2, 15, 16, 17,
    3, 4, 5, 9, 10, 11, 8, 7, 6, 5, 4, 3, 2, 1, 0, 17, 16, 15, 14, 13, 12, 11, 10, 9, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 0, 1, 2, 3, 4, 5, 8, 7, 6, 2, 1, 0, 14, 13, 12, 17, 16, 15, 11, 10, 9, 5, 4, 3, 6, 7, 8, 3, 4, 5, 9, 10,
    11, 15, 16, 17, 12, 13, 14, 0, 1, 2, 8, 7, 6, 14, 13, 12, 11, 10, 9, 17, 16, 15, 5, 4, 3, 2, 1, 0, 15, 16, 17, 9,
    10, 11, 3, 4, 5, 6, 7, 8, 0, 1, 2, 12, 13, 14, 17, 16, 15, 2, 1, 0, 5, 4, 3, 8, 7, 6, 11, 10, 9, 14, 13, 12, 15, 16,
    17, 12, 13, 14, 9, 10, 11, 6, 7, 8, 3, 4, 5, 0, 1, 2, 17, 16, 15, 5, 4, 3, 11, 10, 9, 8, 7, 6, 14, 13, 12, 2, 1, 0,
    15, 16, 17, 0, 1, 2, 12, 13, 14, 6, 7, 8, 9, 10, 11, 3, 4, 5, 17, 16, 15, 11, 10, 9, 14, 13, 12, 8, 7, 6, 2, 1, 0,
    5, 4, 3, 15, 16, 17, 3, 4, 5, 0, 1, 2, 6, 7, 8, 12, 13, 14, 9, 10, 11, 17, 16, 15, 14, 13, 12, 2, 1, 0, 8, 7, 6, 5,
    4, 3, 11, 10, 9, 3, 4, 5, 6, 7, 8, 0, 1, 2, 12, 13, 14, 15, 16, 17, 9, 10, 11, 5, 4, 3, 17, 16, 15, 2, 1, 0, 14, 13,
    12, 8, 7, 6, 11, 10, 9, 3, 4, 5, 9, 10, 11, 6, 7, 8, 12, 13, 14, 0, 1, 2, 15, 16, 17, 5, 4, 3, 2, 1, 0, 8, 7, 6, 14,
    13, 12, 11, 10, 9, 17, 16, 15, 3, 4, 5, 15, 16, 17, 9, 10, 11, 12, 13, 14, 6, 7, 8, 0, 1, 2, 5, 4, 3, 8, 7, 6, 11,
    10, 9, 14, 13, 12, 17, 16, 15, 2, 1, 0, 3, 4, 5, 0, 1, 2, 15, 16, 17, 12, 13, 14, 9, 10, 11, 6, 7, 8, 5, 4, 3, 11,
    10, 9, 17, 16, 15, 14, 13, 12, 2, 1, 0, 8, 7, 6, 12, 13, 14, 15, 16, 17, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14,
    13, 12, 8, 7, 6, 2, 1, 0, 5, 4, 3, 17, 16, 15, 11, 10, 9, 12, 13, 14, 9, 10, 11, 15, 16, 17, 3, 4, 5, 0, 1, 2, 6, 7,
    8, 14, 13, 12, 2, 1, 0, 17, 16, 15, 5, 4, 3, 11, 10, 9, 8, 7, 6, 12, 13, 14, 6, 7, 8, 9, 10, 11, 3, 4, 5, 15, 16,
    17, 0, 1, 2, 14, 13, 12, 17, 16, 15, 11, 10, 9, 5, 4, 3, 8, 7, 6, 2, 1, 0, 12, 13, 14, 0, 1, 2, 6, 7, 8, 3, 4, 5, 9,
    10, 11, 15, 16, 17, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 17, 16, 15
};

// ecolor2ColorCube
ColorCube ec2cc(const EColor c)
{
    switch (c)
    {
    case White: return CU;
    case Orange: return CL;
    case Green: return CF;
    case Red: return CR;
    case Blue: return CB;
    case Yellow: return CD;
    case ECOLOR_LENGTH: return COLOR_CUBE_LENGTH;
    default: break;
    }

    TraceLog(LOG_FATAL, "Unknown conversion for ColorCube for EColor : %d", c); // Will abort with exit(EXIT_FAILURE)
    __builtin_unreachable();
}

void fillFaceCube(FaceCube* fc, const Cube* c, const EFace ef, size_t* i)
{
    const Face* f = &c->faces[f2i(ef)];

    fc->f[(*i)++] = ec2cc(getCell(f, F_U | F_L));
    fc->f[(*i)++] = ec2cc(getCell(f, F_U));
    fc->f[(*i)++] = ec2cc(getCell(f, F_U | F_R));
    fc->f[(*i)++] = ec2cc(getCell(f, F_L));
    fc->f[(*i)++] = ec2cc(f2c(ef));
    fc->f[(*i)++] = ec2cc(getCell(f, F_R));
    fc->f[(*i)++] = ec2cc(getCell(f, F_D | F_L));
    fc->f[(*i)++] = ec2cc(getCell(f, F_D));
    fc->f[(*i)++] = ec2cc(getCell(f, F_D | F_R));
}

FaceCube cubeToFaceCube(const Cube* c)
{
    FaceCube f;
    size_t i = 0;

    fillFaceCube(&f, c, F_U, &i);
    fillFaceCube(&f, c, F_R, &i);
    fillFaceCube(&f, c, F_F, &i);
    fillFaceCube(&f, c, F_D, &i);
    fillFaceCube(&f, c, F_L, &i);
    fillFaceCube(&f, c, F_B, &i);

    assert(i == FACELET_LENGTH);

    return f;
}

CubieCube identityCube(void)
{
    CubieCube cc = {0};

    for (int i = 0; i < CORNER_LENGTH; ++i)
        cc.cp[i] = i;

    for (int i = 0; i < EDGE_LENGTH; ++i)
        cc.ep[i] = i;

    return cc;
}

CubieCube faceToCubie(const FaceCube* f)
{
    CubieCube cc = {0};

    for (int i = 0; i < CORNER_LENGTH; ++i)
        cc.cp[i] = -1;

    for (int i = 0; i < EDGE_LENGTH; ++i)
        cc.ep[i] = -1;

    for (int i = 0; i < CORNER_LENGTH; ++i)
    {
        const Facelet* fac = cornerFacelet[i];
        int ori;
        for (ori = 0; ori < 3; ++ori)
        {
            if (f->f[fac[ori]] == CU || f->f[fac[ori]] == CD)
                break;
        }

        const ColorCube col1 = f->f[fac[(ori + 1) % 3]];
        const ColorCube col2 = f->f[fac[(ori + 2) % 3]];

        for (int j = 0; j < CORNER_LENGTH; ++j)
        {
            const ColorCube* col = cornerColor[j];
            if (col1 == col[1] && col2 == col[2])
            {
                cc.cp[i] = j;
                cc.co[i] = ori;
                break;
            }
        }
    }

    for (int i = 0; i < EDGE_LENGTH; ++i)
    {
        for (int j = 0; j < EDGE_LENGTH; ++j)
        {
            if (f->f[edgeFacelet[i][0]] == edgeColor[j][0] && f->f[edgeFacelet[i][1]] == edgeColor[j][1])
            {
                cc.ep[i] = j;
                cc.eo[i] = 0;
                break;
            }
            if (f->f[edgeFacelet[i][0]] == edgeColor[j][1] && f->f[edgeFacelet[i][1]] == edgeColor[j][0])
            {
                cc.ep[i] = j;
                cc.eo[i] = 1;
                break;
            }
        }
    }

    return cc;
}

// Rotate array arr left between l and r. r is included.
// void rotate_left(Corner* arr, const int l, const int r)
// {
//     const Corner temp = arr[l];
//     for (int i = l; i < r; ++i)
//         arr[i] = arr[i + 1];
//     arr[r] = temp;
// }

#define rotate_left(type, arr, l, r)        \
    do {                                    \
        const type temp = (arr)[(l)];     \
        for (int i = (l); i < (r); ++i)     \
            (arr)[i] = (arr)[i + 1];        \
        (arr)[(r)] = temp;                  \
    } while (0)

// Binomial coefficient [n choose k].
int32_t c_nk(const int32_t n, int32_t k)
{
    if (n < k) return 0;
    if (k > n / 2)
        k = n - k;

    int32_t s = 1;
    int32_t j = 1;
    int32_t i = n;

    while (i != n - k)
    {
        s *= i;
        s /= j;
        i -= 1;
        j += 1;
    }
    return s;
}

// Get the permutation of the 8 corners. 0 <= corners < 40320
int32_t get_corners(const CubieCube* cc)
{
    Corner perm[CORNER_LENGTH] = {0};
    memcpy(perm, cc->cp, CORNER_LENGTH * sizeof(Corner));

    int32_t b = 0;
    for (int j = DRB; j > URF; --j)
    {
        int32_t k = 0;
        while (j != (int)perm[j])
        {
            rotate_left(Corner, perm, 0, j);
            k += 1;
        }
        b = (j + 1) * b + k;
    }
    assert(b < 40320);
    return b;
}

// Get the twist of the 8 corners. 0 <= twist < 2187
int32_t get_twist(const CubieCube* cc)
{
    int32_t ret = 0;
    for (int i = URF; i < DRB; ++i)
        ret = 3 * ret + cc->co[i];

    assert(ret < 2187);
    return ret;
}

// Get the flip of the 12 edges. 0 <= flip < 2048
int32_t get_flip(const CubieCube* cc)
{
    int32_t ret = 0;
    for (int i = UR; i < BR; ++i)
        ret = 2 * ret + cc->eo[i];

    assert(ret < 2048);
    return ret;
}

// Get the permutation and location of the UD-slice edges FR,FL,BL and BR. 0 <= slice_sorted < 11880
int32_t get_slice_sorted(const CubieCube* cc)
{
    int32_t x;
    int32_t a = x = 0;
    Edge edge4[4] = {0};

    // First compute the index a < (12 choose 4) and the permutation array perm.
    for (int j = BR; j > UR - 1; --j)
    {
        if (FR <= cc->ep[j] && cc->ep[j] <= BR)
        {
            a += c_nk(BR - j, x + 1);
            edge4[3 - x] = cc->ep[j];
            x++;
        }
    }

    int32_t b = 0;
    for (int j = 3; j > 0; --j)
    {
        int32_t k = 0;
        while ((j + 8) != (int)edge4[j])
        {
            rotate_left(Edge, edge4, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }

    const int32_t ret = 24 * a + b;
    assert(ret < 11880);
    return ret;
}

// Multiply cubie ca with another cubie cube cb, restricted to the corners. Does not change b.
void corner_multiply(CubieCube* ca, const CubieCube* cb)
{
    Corner c_perm[CORNER_LENGTH] = {0};
    int32_t c_ori[CORNER_LENGTH] = {0};
    int32_t ori = 0;

    for (int c = 0; c < CORNER_LENGTH; ++c)
    {
        c_perm[c] = ca->cp[cb->cp[c]];
        const int32_t ori_a = ca->co[cb->cp[c]];
        const int32_t ori_b = cb->co[c];
#if 1
        ori = (ori_a + ori_b) % 3;
#else


        if (ori_a < 3 && ori_b < 3) // two regular cubes
        {
            ori = ori_a + ori_b;
            if (ori >= 3)
                ori -= 3;
        }
        else if (ori_a < 3 && 3 <= ori_b) // cube cb is in a mirrored state
        {
            ori = ori_a + ori_b;
            if (ori >= 6)
                ori -= 3; // the composition also is in a mirrored state
        }
        else if (ori_a >= 3 && 3 > ori_b) // cube ca is in a mirrored state
        {
            ori = ori_a - ori_b;
            if (ori < 3)
                ori += 3; // the composition is a mirrored cube
        }
        else if (ori_a >= 3 && ori_b >= 3) // if both cubes are in mirrored states
        {
            ori = ori_a - ori_b;
            if (ori < 0)
                ori += 3; // the composition is a regular cube
        }

#endif
        c_ori[c] = ori;
    }

    for (int c = 0; c < CORNER_LENGTH; ++c)
    {
        ca->cp[c] = c_perm[c];
        ca->co[c] = c_ori[c];
    }
}

// Multiply cubie ca with another cubiecube cb, restricted to the edges. Does not change cb.
void edge_multiply(CubieCube* ca, const CubieCube* cb)
{
    Edge e_perm[EDGE_LENGTH] = {0};
    int32_t e_ori[EDGE_LENGTH] = {0};

    for (int e = 0; e < EDGE_LENGTH; ++e)
    {
        e_perm[e] = ca->ep[cb->ep[e]];
        e_ori[e] = (cb->eo[e] + ca->eo[cb->ep[e]]) % 2;
    }

    for (int e = 0; e < EDGE_LENGTH; ++e)
    {
        ca->ep[e] = e_perm[e];
        ca->eo[e] = e_ori[e];
    }
}

void cubie_multiply(CubieCube* ca, const CubieCube* cb)
{
    corner_multiply(ca, cb);
    edge_multiply(ca, cb);
}

// get_fsst_depth3(ix) is *exactly* the number of moves % 3 to solve phase1x24 of a cube with index ix
// FIXME: [PERFORMANCE] This function get called so much that it simply destoys performance
//        It may be possible to split flipslicesorted_twist_depth3 into multiple arrays based on depth,
//          but at the end of the day is math I can't even start to grasp hf/gl doing that.
static int32_t get_flipslicesorted_twist_depth3(const uint32_t ix)
{
    uint32_t y = flipslicesorted_twist_depth3[ix / 16];
    y >>= (ix % 16) * 2;
    return y & 3;
}

// Compute the distance to the cube subgroup  where flip=slicesorted=twist=0
int32_t get_phasex24_depth(const CoordCube* coc, const int position)
{
    int32_t slicesorted, flip, twist;
    if (position == 0)
    {
        slicesorted = coc->UD_slice_sorted;
        flip = coc->UD_flip;
        twist = coc->UD_twist;
    }
    else if (position == 1)
    {
        slicesorted = coc->RL_slice_sorted;
        flip = coc->RL_flip;
        twist = coc->RL_twist;
    }
    else
    {
        slicesorted = coc->FB_slice_sorted;
        flip = coc->FB_flip;
        twist = coc->FB_twist;
    }

    const uint32_t flipslicesorted = N_FLIP * slicesorted + flip;
    const uint32_t classidx = flipslicesorted_classidx[flipslicesorted];
    int32_t sym = flipslicesorted_sym[flipslicesorted];
    int32_t depth_mod3 = get_flipslicesorted_twist_depth3(N_TWIST * classidx + twist_conj[(twist << 4) + sym]);

    int32_t depth = 0;
    while (flip != SOLVED || slicesorted != SOLVED || twist != SOLVED)
    {
        if (depth_mod3 == 0)
            depth_mod3 = 3;

        // We can use the same m in all 3 rotational positions
        for (int m = 0; m < FACE_TURN_MOVE_LENGTH; ++m)
        {
            const int32_t twist1 = twist_move[N_MOVE * twist + m];
            const int32_t flip1 = flip_move[N_MOVE * flip + m];
            const int32_t slicesorted1 = slice_sorted_move[N_MOVE * slicesorted + m];
            const int32_t flipslicesorted1 = N_FLIP * slicesorted1 + flip1;
            const uint32_t classidx1 = flipslicesorted_classidx[flipslicesorted1];
            sym = flipslicesorted_sym[flipslicesorted1];

            if ((depth_mod3 - 1) == get_flipslicesorted_twist_depth3(
                N_TWIST * classidx1 + twist_conj[(twist1 << 4) + sym]))
            {
                depth++;
                twist = twist1;
                flip = flip1;
                slicesorted = slicesorted1;
                depth_mod3--;
                break;
            }
        }
    }
    return depth;
}

CoordCube cubieToCoordCube(const CubieCube* cc)
{
    CubieCube rl = symCube[16]; // 120° rotation along URF-DBL axis
    cubie_multiply(&rl, cc);
    cubie_multiply(&rl, &symCube[32]); // ss = symCube[16]*cc*symCube[16]^-1

    CubieCube fb = symCube[32]; // -120° rotation along URF-DBL axis
    cubie_multiply(&fb, cc);
    cubie_multiply(&fb, &symCube[16]); // ss = symCube[32]*cc*symCube[32]^-1

    CoordCube coc = {
        .corners = get_corners(cc),
        .UD_twist = get_twist(cc),
        .UD_flip = get_flip(cc),
        .UD_slice_sorted = get_slice_sorted(cc),
        .RL_twist = get_twist(&rl),
        .RL_flip = get_flip(&rl),
        .RL_slice_sorted = get_slice_sorted(&rl),
        .FB_twist = get_twist(&fb),
        .FB_flip = get_flip(&fb),
        .FB_slice_sorted = get_slice_sorted(&fb)
    };

    coc.UD_phasex24_depth = get_phasex24_depth(&coc, 0); // since we store the depth mod 3, retrieving the
    coc.RL_phasex24_depth = get_phasex24_depth(&coc, 1); // initial absolute depth is a bit involved
    coc.FB_phasex24_depth = get_phasex24_depth(&coc, 2);

    return coc;
}

Move faceTurnMoveToMove(const FaceTurnMove f)
{
    switch (f)
    {
    case MU1: return U;
    case MU2: return UU;
    case MU3: return Up;

    case MR1: return R;
    case MR2: return RR;
    case MR3: return Rp;

    case MF1: return F;
    case MF2: return FF;
    case MF3: return Fp;

    case MD1: return D;
    case MD2: return DD;
    case MD3: return Dp;

    case ML1: return L;
    case ML2: return LL;
    case ML3: return Lp;

    case MB1: return B;
    case MB2: return BB;
    case MB3: return Bp;
    default: break;
    }

    TraceLog(LOG_FATAL, "Unknown conversion for FaceTurnMove to Move: %d", f); // Will abort with exit(EXIT_FAILURE)
    __builtin_unreachable();
}

const char* cornerToStr(const Corner c)
{
    switch (c)
    {
    case URF: return "URF";
    case UFL: return "UFL";
    case ULB: return "ULB";
    case UBR: return "UBR";
    case DFR: return "DFR";
    case DLF: return "DLF";
    case DBL: return "DBL";
    case DRB: return "DRB";
    default: break;
    }

    TraceLog(LOG_FATAL, "Unknown conversion for Corner to str : %d", c); // Will abort with exit(EXIT_FAILURE)
    __builtin_unreachable();
}

const char* edgeToStr(const Edge e)
{
    switch (e)
    {
    case UR: return "UR";
    case UF: return "UF";
    case UL: return "UL";
    case UB: return "UB";
    case DR: return "DR";
    case DF: return "DF";
    case DL: return "DL";
    case DB: return "DB";
    case FR: return "FR";
    case FL: return "FL";
    case BL: return "BL";
    case BR: return "BR";
    default: break;
    }

    TraceLog(LOG_FATAL, "Unknown conversion for Edge to str : %d", e); // Will abort with exit(EXIT_FAILURE)
    __builtin_unreachable();
}

const char* colorCubeToStr(const ColorCube c)
{
    switch (c)
    {
    case CU: return "U";
    case CF: return "F";
    case CR: return "R";
    case CL: return "L";
    case CD: return "D";
    case CB: return "B";
    default: break;
    }

    TraceLog(LOG_FATAL, "Unknown conversion for ColorCube to str : %d", c); // Will abort with exit(EXIT_FAILURE)
    __builtin_unreachable();
}

const char* faceTurnMoveToStr(const FaceTurnMove f)
{
    switch (f)
    {
    case MU1: return "U1";
    case MU2: return "U2";
    case MU3: return "U3";

    case MR1: return "R1";
    case MR2: return "R2";
    case MR3: return "R3";

    case MF1: return "F1";
    case MF2: return "F2";
    case MF3: return "F3";

    case MD1: return "D1";
    case MD2: return "D2";
    case MD3: return "D3";

    case ML1: return "L1";
    case ML2: return "L2";
    case ML3: return "L3";

    case MB1: return "B1";
    case MB2: return "B2";
    case MB3: return "B3";
    default: break;
    }

    TraceLog(LOG_FATAL, "Unknown conversion for FaceTurnMove to str : %d", f); // Will abort with exit(EXIT_FAILURE)
    __builtin_unreachable();
}

void dumpFaceCube(const FaceCube* fc)
{
    for (int i = 0; i < FACELET_LENGTH; i++)
    {
        printf("%s", colorCubeToStr(fc->f[i]));
    }
    printf("\n");
}

void dumpCubie(const CubieCube* cc)
{
    printf("(CubieCube){\n");
    printf("  .cp = {");
    for (int i = 0; i < CORNER_LENGTH; ++i)
    {
        printf("%s", cornerToStr(cc->cp[i]));
        if (i < CORNER_LENGTH - 1)
        {
            printf(", ");
        }
    }
    printf("},\n");

    printf("  .co = {");
    for (int i = 0; i < CORNER_LENGTH; ++i)
    {
        printf("%d", cc->co[i]);
        if (i < CORNER_LENGTH - 1)
        {
            printf(", ");
        }
    }
    printf("},\n");
    printf("  .ep = {");
    for (int i = 0; i < EDGE_LENGTH; ++i)
    {
        printf("%s", edgeToStr(cc->ep[i]));
        if (i < EDGE_LENGTH - 1)
        {
            printf(", ");
        }
    }
    printf("},\n");

    printf("  .eo = {");
    for (int i = 0; i < EDGE_LENGTH; ++i)
    {
        printf("%d", cc->eo[i]);
        if (i < EDGE_LENGTH - 1)
        {
            printf(", ");
        }
    }
    printf("},\n");
    printf("}");
}
