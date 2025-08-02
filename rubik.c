#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <raylib.h>
#define RL_MAX_MATRIX_STACK_SIZE 256
#include <rlgl.h>

#define Face uint32_t

typedef enum { White, Orange, Green, Red, Blue, Yellow, ECOLOR_LENGTH } EColor;

typedef enum
{
    U = 1,
    L = 2,
    F = 4,
    R = 8,
    B = 16,
    D = 32,
    EFACE_LENGTH
} EFace;

Vector3 FACE_CENTERS[EFACE_LENGTH] =
{
    (Vector3){0.f, 1.f, 0.f},
    (Vector3){-1.f, 0.f, 0.f},
    (Vector3){0.f, 0.f, 1.f},
    (Vector3){1.f, 0.f, 0.f},
    (Vector3){0.f, 0.f, -1.f},
    (Vector3){0.f, -1.f, 0.f}
};


// Face to index
int f2i(EFace f)
{
    switch (f)
    {
    case U: return 0;
    case L: return 1;
    case F: return 2;
    case R: return 3;
    case B: return 4;
    case D: return 5;
    default:
        {
            fprintf(stderr, "ERROR: Unknown face\n");
            exit(1);
        }
    }
}

typedef struct Rotation
{
    EFace face; // 0 for none
    float angle; // degrees
} Rotation;

typedef struct Cube
{
    Face faces[EFACE_LENGTH];
    Rotation rotation;
} Cube;

EColor getCell(Face* f, uint8_t cell)
{
    int width = 4; // bits required for a color
    int offset = 0;
    int mask = 0xF;

    if (cell & U && cell & L)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell == U)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell & U && cell & R)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell == L)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell == R)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell & D && cell & L)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell == D)
    {
        return (*f >> offset) & mask;
    }

    offset += width;
    if (cell & D && cell & R)
    {
        return (*f >> offset) & mask;
    }

    return White;
}

void paintCell(Face* f, uint8_t cell, EColor color)
{
    _Static_assert(ECOLOR_LENGTH <= 6, "check width is enough to fit all possible colors in a face");
    int width = 4; // bits required for a color
    int offset = 0;
    Face mask;

    mask = ~0 << (width + offset) | ~(~0 << offset);
    if (cell & U && cell & L)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0 << (width + offset) | ~(~0 << offset);
    if (cell == U)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0 << (width + offset) | ~(~0 << offset);
    if (cell & U && cell & R)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0 << (width + offset) | ~(~0 << offset);
    if (cell == L)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0 << (width + offset) | ~(~0 << offset);
    if (cell == R)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0 << (width + offset) | ~(~0 << offset);
    if (cell & D && cell & L)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0 << (width + offset) | ~(~0 << offset);
    if (cell == D)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = 0 | ~(~0 << offset);
    if (cell & D && cell & R)
    {
        *f = (*f & mask) | color << offset;
    }
}

void setFaceColor(Face* f, EColor color)
{
    *f = 0;

    paintCell(f, U | L, color);
    paintCell(f, U, color);
    paintCell(f, U | R, color);

    paintCell(f, L, color);
    paintCell(f, R, color);

    paintCell(f, D | L, color);
    paintCell(f, D, color);
    paintCell(f, D | R, color);
}

// Color to Raylib color
Color c2r(EColor color)
{
    switch (color)
    {
    case White: return RAYWHITE;
    case Orange: return ORANGE;
    case Green: return GREEN;
    case Red: return RED;
    case Blue: return BLUE;
    case Yellow: return YELLOW;
    default: return BLACK;
    }
}

Cube newCube()
{
    Cube c;

    setFaceColor(&c.faces[f2i(U)], White);
    setFaceColor(&c.faces[f2i(F)], Green);
    setFaceColor(&c.faces[f2i(B)], Blue);
    setFaceColor(&c.faces[f2i(D)], Yellow);
    setFaceColor(&c.faces[f2i(L)], Orange);
    setFaceColor(&c.faces[f2i(R)], Red);

    c.rotation.face = F;
    c.rotation.angle = 45.f;

    return c;
}

void DrawTopBottom(Vector3 position, Color top, Color bottom)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float width = 1.0f;
    float height = 1.0f;
    float length = 1.0f;

    rlPushMatrix();
    {
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        rlTranslatef(position.x, position.y, position.z);
        //rlRotatef(45, 0, 1, 0);
        //rlScalef(1.0f, 1.0f, 1.0f);   // NOTE: Vertices are directly scaled on definition

        rlBegin(RL_TRIANGLES);
        {
            rlColor4ub(top.r, top.g, top.b, top.a);

            // Top
            rlNormal3f(0.0f, 1.0f, 0.0f);
            rlVertex3f(x - width / 2, y + height / 2, z - length / 2); // Top Left
            rlVertex3f(x - width / 2, y + height / 2, z + length / 2); // Bottom Left
            rlVertex3f(x + width / 2, y + height / 2, z + length / 2); // Bottom Right

            rlVertex3f(x + width / 2, y + height / 2, z - length / 2); // Top Right
            rlVertex3f(x - width / 2, y + height / 2, z - length / 2); // Top Left
            rlVertex3f(x + width / 2, y + height / 2, z + length / 2); // Bottom Right

            rlColor4ub(bottom.r, bottom.g, bottom.b, bottom.a);

            // Bottom face
            rlNormal3f(0.0f, -1.0f, 0.0f);
            rlVertex3f(x - width / 2, y - height / 2, z - length / 2); // Top Left
            rlVertex3f(x + width / 2, y - height / 2, z + length / 2); // Bottom Right
            rlVertex3f(x - width / 2, y - height / 2, z + length / 2); // Bottom Left

            rlVertex3f(x + width / 2, y - height / 2, z - length / 2); // Top Right
            rlVertex3f(x + width / 2, y - height / 2, z + length / 2); // Bottom Right
            rlVertex3f(x - width / 2, y - height / 2, z - length / 2); // Top Left
        }
        rlEnd();

        rlBegin(RL_LINES);
        {
            rlColor4ub(BLACK.r, BLACK.g, BLACK.b, BLACK.a);

            // Top face
            //------------------------------------------------------------------
            // Left line
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top left front
            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top left back

            // Right line
            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top right front
            rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top right back

            // Bottom face
            //------------------------------------------------------------------
            // Left line
            rlVertex3f(x - width/2, y - height/2, z + length/2);  // Top left front
            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top left back

            // Right line
            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Top right front
            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top right back
        }
        rlEnd();
    }
    rlPopMatrix();
}

void DrawFrontBack(Vector3 position, Color front, Color back)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float width = 1.0f;
    float height = 1.0f;
    float length = 1.0f;

    rlPushMatrix();
    {
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        rlTranslatef(position.x, position.y, position.z);
        //rlRotatef(45, 0, 1, 0);
        //rlScalef(1.0f, 1.0f, 1.0f);   // NOTE: Vertices are directly scaled on definition

        rlBegin(RL_TRIANGLES);
        {
            rlColor4ub(front.r, front.g, front.b, front.a);

            // Front face
            rlNormal3f(0.0f, 0.0f, 1.0f);
            rlVertex3f(x - width / 2, y - height / 2, z + length / 2); // Bottom Left
            rlVertex3f(x + width / 2, y - height / 2, z + length / 2); // Bottom Right
            rlVertex3f(x - width / 2, y + height / 2, z + length / 2); // Top Left

            rlVertex3f(x + width / 2, y + height / 2, z + length / 2); // Top Right
            rlVertex3f(x - width / 2, y + height / 2, z + length / 2); // Top Left
            rlVertex3f(x + width / 2, y - height / 2, z + length / 2); // Bottom Right

            rlColor4ub(back.r, back.g, back.b, back.a);

            // Back face
            rlNormal3f(0.0f, 0.0f, -1.0f);
            rlVertex3f(x - width / 2, y - height / 2, z - length / 2); // Bottom Left
            rlVertex3f(x - width / 2, y + height / 2, z - length / 2); // Top Left
            rlVertex3f(x + width / 2, y - height / 2, z - length / 2); // Bottom Right

            rlVertex3f(x + width / 2, y + height / 2, z - length / 2); // Top Right
            rlVertex3f(x + width / 2, y - height / 2, z - length / 2); // Bottom Right
            rlVertex3f(x - width / 2, y + height / 2, z - length / 2); // Top Left
        }
        rlEnd();

        rlBegin(RL_LINES);
        {
            rlColor4ub(BLACK.r, BLACK.g, BLACK.b, BLACK.a);

            // Front face
            //------------------------------------------------------------------
            // Bottom line
            rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom left
            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom right

            // Left line
            rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom right
            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top right

            // Top line
            rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top right
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top left

            // Right line
            rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top left
            rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom left

            // Back face
            //------------------------------------------------------------------
            // Bottom line
            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom left
            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom right

            // Left line
            rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom right
            rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top right

            // Top line
            rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top right
            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top left

            // Right line
            rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top left
            rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom left
        }
        rlEnd();
    }
    rlPopMatrix();
}

void DrawLeftRight(Vector3 position, Color left, Color right)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float width = 1.0f;
    float height = 1.0f;
    float length = 1.0f;

    rlPushMatrix();
    {
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        rlTranslatef(position.x, position.y, position.z);
        //rlRotatef(45, 0, 1, 0);
        //rlScalef(1.0f, 1.0f, 1.0f);   // NOTE: Vertices are directly scaled on definition

        rlBegin(RL_TRIANGLES);
        {
            rlColor4ub(left.r, left.g, left.b, left.a);

            // Left face
            rlNormal3f(-1.0f, 0.0f, 0.0f);
            rlVertex3f(x - width / 2, y - height / 2, z - length / 2); // Bottom Right
            rlVertex3f(x - width / 2, y + height / 2, z + length / 2); // Top Left
            rlVertex3f(x - width / 2, y + height / 2, z - length / 2); // Top Right

            rlVertex3f(x - width / 2, y - height / 2, z + length / 2); // Bottom Left
            rlVertex3f(x - width / 2, y + height / 2, z + length / 2); // Top Left
            rlVertex3f(x - width / 2, y - height / 2, z - length / 2); // Bottom Right

            rlColor4ub(right.r, right.g, right.b, right.a);

            // Right face
            rlNormal3f(1.0f, 0.0f, 0.0f);
            rlVertex3f(x + width / 2, y - height / 2, z - length / 2); // Bottom Right
            rlVertex3f(x + width / 2, y + height / 2, z - length / 2); // Top Right
            rlVertex3f(x + width / 2, y + height / 2, z + length / 2); // Top Left

            rlVertex3f(x + width / 2, y - height / 2, z + length / 2); // Bottom Left
            rlVertex3f(x + width / 2, y - height / 2, z - length / 2); // Bottom Right
            rlVertex3f(x + width / 2, y + height / 2, z + length / 2); // Top Left
        }
        rlEnd();
    }
    rlPopMatrix();
}

void DrawUp(Face* up, Rotation r)
{
    EColor color;
    if (r.face == U || r.face == B || r.face == F || r.face == L || r.face == R )
        rlPushMatrix();

    if (r.face == U)
        rlRotatef(r.angle, FACE_CENTERS[f2i(U)].x, FACE_CENTERS[f2i(U)].y, FACE_CENTERS[f2i(U)].z);

    if (r.face == B)
        rlRotatef(r.angle, FACE_CENTERS[f2i(B)].x, FACE_CENTERS[f2i(B)].y, FACE_CENTERS[f2i(B)].z);

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(up, U | L);
    DrawTopBottom((Vector3){-1.f, 1.f, -1.f}, c2r(color), BLACK);
    if (r.face == L) rlPopMatrix();
    color = getCell(up, U);
    DrawTopBottom((Vector3){0.f, 1.f, -1.f}, c2r(color), BLACK);
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(up, U | R);
    DrawTopBottom((Vector3){1.f, 1.f, -1.f}, c2r(color), BLACK);
    if (r.face == R) rlPopMatrix();

    if (r.face == B)
        rlPopMatrix();

    if (r.face == L || r.face == R)
        rlPushMatrix();

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(up, L);
    DrawTopBottom((Vector3){-1.f, 1.f, 0.f}, c2r(color), BLACK);
    if (r.face == L) rlPopMatrix();
    color = White;
    DrawTopBottom(FACE_CENTERS[f2i(U)], c2r(color), BLACK);
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(up, R);
    DrawTopBottom((Vector3){1.f, 1.f, 0.f}, c2r(color), BLACK);
    if (r.face == R) rlPopMatrix();

    if (r.face == F)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F)].x, FACE_CENTERS[f2i(F)].y, FACE_CENTERS[f2i(F)].z);

    if (r.face == L || r.face == R)
        rlPushMatrix();

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(up, D | L);
    DrawTopBottom((Vector3){-1.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == L) rlPopMatrix();
    color = getCell(up, D);
    DrawTopBottom((Vector3){0.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(up, D | R);
    DrawTopBottom((Vector3){1.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == R) rlPopMatrix();

    if (r.face == U || r.face == F)
        rlPopMatrix();
}

void DrawDown(Face* down, Rotation r)
{
    EColor color;

    if (r.face == D || r.face == B || r.face == F || r.face == L || r.face == R )
        rlPushMatrix();

    if (r.face == D)
        rlRotatef(r.angle, FACE_CENTERS[f2i(D)].x, FACE_CENTERS[f2i(D)].y, FACE_CENTERS[f2i(D)].z);

    if (r.face == B)
        rlRotatef(r.angle, FACE_CENTERS[f2i(B)].x, FACE_CENTERS[f2i(B)].y, FACE_CENTERS[f2i(B)].z);

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(down, U | L);
    DrawTopBottom((Vector3){-1.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == L) rlPopMatrix();
    color = getCell(down, U);
    DrawTopBottom((Vector3){0.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(down, U | R);
    DrawTopBottom((Vector3){1.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == R) rlPopMatrix();

    if (r.face == B)
        rlPopMatrix();

    if (r.face == L || r.face == R)
        rlPushMatrix();

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(down, L);
    DrawTopBottom((Vector3){-1.f, -1.f, 0.f}, BLACK, c2r(color));
    if (r.face == L) rlPopMatrix();
    color = Yellow;
    DrawTopBottom(FACE_CENTERS[f2i(D)], BLACK, c2r(color));
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(down, R);
    DrawTopBottom((Vector3){1.f, -1.f, 0.f}, BLACK, c2r(color));
    if (r.face == R) rlPopMatrix();

    if (r.face == F)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F)].x, FACE_CENTERS[f2i(F)].y, FACE_CENTERS[f2i(F)].z);

    if (r.face == L || r.face == R)
        rlPushMatrix();

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(down, D | L);
    DrawTopBottom((Vector3){-1.f, -1.f, 1.f}, BLACK, c2r(color));
    if (r.face == L) rlPopMatrix();
    color = getCell(down, D);
    DrawTopBottom((Vector3){0.f, -1.f, 1.f}, BLACK, c2r(color));
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(down, D | R);
    DrawTopBottom((Vector3){1.f, -1.f, 1.f}, BLACK, c2r(color));
    if (r.face == R) rlPopMatrix();

    if (r.face == D || r.face == F)
        rlPopMatrix();
}

void DrawFront(Face* front, Rotation r)
{
    EColor color;

    if (r.face == F || r.face == U || r.face == D || r.face == L || r.face == R )
        rlPushMatrix();

    if (r.face == F)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F)].x, FACE_CENTERS[f2i(F)].y, FACE_CENTERS[f2i(F)].z);

    if (r.face == U)
        rlRotatef(r.angle, FACE_CENTERS[f2i(U)].x, FACE_CENTERS[f2i(U)].y, FACE_CENTERS[f2i(U)].z);

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(front, U | L);
    DrawFrontBack((Vector3){-1.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == L) rlPopMatrix();
    color = getCell(front, U);
    DrawFrontBack((Vector3){0.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(front, U | R);
    DrawFrontBack((Vector3){1.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == R) rlPopMatrix();

    if (r.face == U)
        rlPopMatrix();

    if (r.face == L || r.face == R)
        rlPushMatrix();

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(front, L);
    DrawFrontBack((Vector3){-1.f, 0.f, 1.f}, c2r(color), BLACK);
    if (r.face == L) rlPopMatrix();
    color = Green;
    DrawFrontBack(FACE_CENTERS[f2i(F)], c2r(color), BLACK);
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(front, R);
    DrawFrontBack((Vector3){1.f, 0.f, 1.f}, c2r(color), BLACK);
    if (r.face == R) rlPopMatrix();

    if (r.face == D)
        rlRotatef(r.angle, FACE_CENTERS[f2i(D)].x, FACE_CENTERS[f2i(D)].y, FACE_CENTERS[f2i(D)].z);

    if (r.face == L || r.face == R)
        rlPushMatrix();

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(front, D | L);
    DrawFrontBack((Vector3){-1.f, -1.f, 1.f}, c2r(color), BLACK);
    if (r.face == L) rlPopMatrix();
    color = getCell(front, D);
    DrawFrontBack((Vector3){0.f, -1.f, 1.f}, c2r(color), BLACK);
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(front, D | R);
    DrawFrontBack((Vector3){1.f, -1.f, 1.f}, c2r(color), BLACK);
    if (r.face == R) rlPopMatrix();

    if (r.face == F || r.face == D)
        rlPopMatrix();
}

void DrawBack(Face* back, Rotation r)
{
    EColor color;

    if (r.face == B || r.face == U || r.face == D || r.face == L || r.face == R )
        rlPushMatrix();

    if (r.face == B)
        rlRotatef(r.angle, FACE_CENTERS[f2i(B)].x, FACE_CENTERS[f2i(B)].y, FACE_CENTERS[f2i(B)].z);

    if (r.face == U)
        rlRotatef(r.angle, FACE_CENTERS[f2i(U)].x, FACE_CENTERS[f2i(U)].y, FACE_CENTERS[f2i(U)].z);

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(back, U | L);
    DrawFrontBack((Vector3){-1.f, 1.f, -1.f}, BLACK, c2r(color));
    if (r.face == L) rlPopMatrix();
    color = getCell(back, U);
    DrawFrontBack((Vector3){0.f, 1.f, -1.f}, BLACK, c2r(color));
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(back, U | R);
    DrawFrontBack((Vector3){1.f, 1.f, -1.f}, BLACK, c2r(color));
    if (r.face == R) rlPopMatrix();

    if (r.face == U)
        rlPopMatrix();

    if (r.face == L || r.face == R)
        rlPushMatrix();

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(back, L);
    DrawFrontBack((Vector3){-1.f, 0.f, -1.f}, BLACK, c2r(color));
    if (r.face == L) rlPopMatrix();
    color = Blue;
    DrawFrontBack(FACE_CENTERS[f2i(B)], BLACK, c2r(color));
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(back, R);
    DrawFrontBack((Vector3){1.f, 0.f, -1.f}, BLACK, c2r(color));
    if (r.face == R) rlPopMatrix();

    if (r.face == D)
        rlRotatef(r.angle, FACE_CENTERS[f2i(D)].x, FACE_CENTERS[f2i(D)].y, FACE_CENTERS[f2i(D)].z);

    if (r.face == L || r.face == R)
        rlPushMatrix();

    if (r.face == L) rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);
    color = getCell(back, D | L);
    DrawFrontBack((Vector3){-1.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == L) rlPopMatrix();
    color = getCell(back, D);
    DrawFrontBack((Vector3){0.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == R) rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);
    color = getCell(back, D | R);
    DrawFrontBack((Vector3){1.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == R) rlPopMatrix();

    if (r.face == B || r.face == D)
        rlPopMatrix();
}

void DrawLeft(Face* left, Rotation r)
{
    EColor color;

    if (r.face == L || r.face == U || r.face == D || r.face == B || r.face == F)
        rlPushMatrix();

    if (r.face == L)
        rlRotatef(r.angle, FACE_CENTERS[f2i(L)].x, FACE_CENTERS[f2i(L)].y, FACE_CENTERS[f2i(L)].z);

    if (r.face == U)
        rlRotatef(r.angle, FACE_CENTERS[f2i(U)].x, FACE_CENTERS[f2i(U)].y, FACE_CENTERS[f2i(U)].z);

    if (r.face == B) rlRotatef(r.angle, FACE_CENTERS[f2i(B)].x, FACE_CENTERS[f2i(B)].y, FACE_CENTERS[f2i(B)].z);
    color = getCell(left, U | L);
    DrawLeftRight((Vector3){-1.f, 1.f, -1.f}, c2r(color), BLACK);
    if (r.face == B) rlPopMatrix();
    color = getCell(left, U);
    DrawLeftRight((Vector3){-1.f, 1.f, 0.f}, c2r(color), BLACK);
    if (r.face == F) rlRotatef(r.angle, FACE_CENTERS[f2i(F)].x, FACE_CENTERS[f2i(F)].y, FACE_CENTERS[f2i(F)].z);
    color = getCell(left, U | R);
    DrawLeftRight((Vector3){-1.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F) rlPopMatrix();

    if (r.face == U)
        rlPopMatrix();

    if (r.face == B || r.face == F)
        rlPushMatrix();

    if (r.face == B) rlRotatef(r.angle, FACE_CENTERS[f2i(B)].x, FACE_CENTERS[f2i(B)].y, FACE_CENTERS[f2i(B)].z);
    color = getCell(left, L);
    DrawLeftRight((Vector3){-1.f, 0.f, -1.f}, c2r(color), BLACK);
    if (r.face == B) rlPopMatrix();
    color = Orange;
    DrawLeftRight(FACE_CENTERS[f2i(L)], c2r(color), BLACK);
    if (r.face == F) rlRotatef(r.angle, FACE_CENTERS[f2i(F)].x, FACE_CENTERS[f2i(F)].y, FACE_CENTERS[f2i(F)].z);
    color = getCell(left, R);
    DrawLeftRight((Vector3){-1.f, 0.f, 1.f}, c2r(color), BLACK);
    if (r.face == F) rlPopMatrix();

    if (r.face == D)
        rlRotatef(r.angle, FACE_CENTERS[f2i(D)].x, FACE_CENTERS[f2i(D)].y, FACE_CENTERS[f2i(D)].z);

    if (r.face == B || r.face == F)
        rlPushMatrix();

    if (r.face == B) rlRotatef(r.angle, FACE_CENTERS[f2i(B)].x, FACE_CENTERS[f2i(B)].y, FACE_CENTERS[f2i(B)].z);
    color = getCell(left, D | L);
    DrawLeftRight((Vector3){-1.f, -1.f, -1.f}, c2r(color), BLACK);
    if (r.face == B) rlPopMatrix();
    color = getCell(left, D);
    DrawLeftRight((Vector3){-1.f, -1.f, 0.f}, c2r(color), BLACK);
    if (r.face == F) rlRotatef(r.angle, FACE_CENTERS[f2i(F)].x, FACE_CENTERS[f2i(F)].y, FACE_CENTERS[f2i(F)].z);
    color = getCell(left, D | R);
    DrawLeftRight((Vector3){-1.f, -1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F) rlPopMatrix();

    if (r.face == L || r.face == D)
        rlPopMatrix();
}

void DrawRight(Face* right, Rotation r)
{
    EColor color;

    if (r.face == R || r.face == U || r.face == D || r.face == B || r.face == F)
        rlPushMatrix();

    if (r.face == R)
        rlRotatef(r.angle, FACE_CENTERS[f2i(R)].x, FACE_CENTERS[f2i(R)].y, FACE_CENTERS[f2i(R)].z);

    if (r.face == U)
        rlRotatef(r.angle, FACE_CENTERS[f2i(U)].x, FACE_CENTERS[f2i(U)].y, FACE_CENTERS[f2i(U)].z);

    if (r.face == B) rlRotatef(r.angle, FACE_CENTERS[f2i(B)].x, FACE_CENTERS[f2i(B)].y, FACE_CENTERS[f2i(B)].z);
    color = getCell(right, U | L);
    DrawLeftRight((Vector3){1.f, 1.f, -1.f}, BLACK, c2r(color));
    if (r.face == B) rlPopMatrix();
    color = getCell(right, U);
    DrawLeftRight((Vector3){1.f, 1.f, 0.f}, BLACK, c2r(color));
    if (r.face == F) rlRotatef(r.angle, FACE_CENTERS[f2i(F)].x, FACE_CENTERS[f2i(F)].y, FACE_CENTERS[f2i(F)].z);
    color = getCell(right, U | R);
    DrawLeftRight((Vector3){1.f, 1.f, 1.f}, BLACK, c2r(color));
    if (r.face == F) rlPopMatrix();

    if (r.face == U)
        rlPopMatrix();

    if (r.face == B || r.face == F)
        rlPushMatrix();

    if (r.face == B) rlRotatef(r.angle, FACE_CENTERS[f2i(B)].x, FACE_CENTERS[f2i(B)].y, FACE_CENTERS[f2i(B)].z);
    color = getCell(right, L);
    DrawLeftRight((Vector3){1.f, 0.f, -1.f}, BLACK, c2r(color));
    if (r.face == B) rlPopMatrix();
    color = Red;
    DrawLeftRight(FACE_CENTERS[f2i(R)], BLACK, c2r(color));
    if (r.face == F) rlRotatef(r.angle, FACE_CENTERS[f2i(F)].x, FACE_CENTERS[f2i(F)].y, FACE_CENTERS[f2i(F)].z);
    color = getCell(right, R);
    DrawLeftRight((Vector3){1.f, 0.f, 1.f}, BLACK, c2r(color));
    if (r.face == F) rlPopMatrix();

    if (r.face == D)
        rlRotatef(r.angle, FACE_CENTERS[f2i(D)].x, FACE_CENTERS[f2i(D)].y, FACE_CENTERS[f2i(D)].z);

    if (r.face == B || r.face == F)
        rlPushMatrix();

    if (r.face == B) rlRotatef(r.angle, FACE_CENTERS[f2i(B)].x, FACE_CENTERS[f2i(B)].y, FACE_CENTERS[f2i(B)].z);
    color = getCell(right, D | L);
    DrawLeftRight((Vector3){1.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == B) rlPopMatrix();
    color = getCell(right, D);
    DrawLeftRight((Vector3){1.f, -1.f, 0.f}, BLACK, c2r(color));
    if (r.face == F) rlRotatef(r.angle, FACE_CENTERS[f2i(F)].x, FACE_CENTERS[f2i(F)].y, FACE_CENTERS[f2i(F)].z);
    color = getCell(right, D | R);
    DrawLeftRight((Vector3){1.f, -1.f, 1.f}, BLACK, c2r(color));
    if (r.face == F) rlPopMatrix();

    if (r.face == R || r.face == D)
        rlPopMatrix();
}

// Will be drawn at 0.0 0.0 0.0
void DrawRubik(Cube* cube)
{
    DrawUp(&cube->faces[f2i(U)], cube->rotation);
    DrawDown(&cube->faces[f2i(D)], cube->rotation);

    DrawFront(&cube->faces[f2i(F)], cube->rotation);
    DrawBack(&cube->faces[f2i(B)], cube->rotation);

    DrawLeft(&cube->faces[f2i(L)], cube->rotation);
    DrawRight(&cube->faces[f2i(R)], cube->rotation);
}

int main()
{
    Face top = ~0;
    Cube cube = newCube();
    setFaceColor(&top, 5);
    //paintCell(&top, U, 5);

    printf("%d %04b\n", Yellow, Yellow);
    printf("%032b\n", top);
    printf("%ld\n", sizeof(Face));
    printf("%ld\n", sizeof(EColor));
    printf("%b\n", getCell(&top, U));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    InitWindow(800, 600, "Rubik's cube");

    SetExitKey(KEY_Q);

    Camera3D camera = {0};
    camera.position = (Vector3){10.0f, -10.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    const int cubeLength = 27;
    Vector3 cubePositions[cubeLength]; // 3x3x3 cube

    int idx = 0;
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; ++y)
        {
            for (int z = -1; z <= 1; z++)
            {
                cubePositions[idx++] = (Vector3){x, y, z};
            }
        }
    }

    while (!WindowShouldClose())
    {
        if (IsKeyReleased(KEY_UP))
        {
            camera.position.y += 1.f;
        }

        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawFPS(10, 10);
            BeginMode3D(camera);
            {
                DrawRubik(&cube);

                // for (int i = 0; i < cubeLength; i++)
                // {
                //     Vector3 cubePosition = cubePositions[i];
                //     //DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, RED);
                //     DrawCubeWires(cubePosition, 1.0f, 1.0f, 1.0f, BLACK);
                // }

                DrawGrid(10, 1.f);
            }
            EndMode3D();
        }
        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
