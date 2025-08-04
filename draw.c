
#include <rlgl.h>

static const Vector3 FACE_CENTERS[EFACE_LENGTH] =
{ // This order is given by the f2i function
    { 0.f,  1.f,  0.f}, // UP
    {-1.f,  0.f,  0.f}, // LEFT
    { 0.f,  0.f,  1.f}, // FRONT
    { 1.f,  0.f,  0.f}, // RIGHT
    { 0.f,  0.f, -1.f}, // BACK
    { 0.f, -1.f,  0.f}, // DOWN
};

// Color to Raylib color
Color c2r(const EColor color)
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

void paintCell(Face* f, const uint8_t cell, const EColor color)
{
    _Static_assert(ECOLOR_LENGTH <= 6, "check width is enough to fit all possible colors in a face");
    const int width = 4; // bits required for a color
    int offset = 0;

    Face mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell & F_U && cell & F_L)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell == F_U)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell & F_U && cell & F_R)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell == F_L)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell == F_R)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell & F_D && cell & F_L)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = ~0U << (width + offset) | ~(~0U << offset);
    if (cell == F_D)
    {
        *f = (*f & mask) | color << offset;
    }

    offset += width;
    mask = 0 | ~(~0U << offset);
    if (cell & F_D && cell & F_R)
    {
        *f = (*f & mask) | color << offset;
    }
}

void setFaceColor(Face* f, const EColor color)
{
    *f = 0;

    paintCell(f, F_U | F_L, color);
    paintCell(f, F_U, color);
    paintCell(f, F_U | F_R, color);

    paintCell(f, F_L, color);
    paintCell(f, F_R, color);

    paintCell(f, F_D | F_L, color);
    paintCell(f, F_D, color);
    paintCell(f, F_D | F_R, color);
}

void DrawTopBottom(const Vector3 position, const Color top, const Color bottom)
{
    rlPushMatrix();
    {
        const float x = 0.0f;
        const float y = 0.0f;
        const float z = 0.0f;
        const float width = 1.0f;
        const float height = 1.0f;
        const float length = 1.0f;

        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        rlTranslatef(position.x, position.y, position.z);

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

void DrawFrontBack(const Vector3 position, const Color front, const Color back)
{
    rlPushMatrix();
    {
        const float x = 0.0f;
        const float y = 0.0f;
        const float z = 0.0f;
        const float width = 1.0f;
        const float height = 1.0f;
        const float length = 1.0f;

        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        rlTranslatef(position.x, position.y, position.z);

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

void DrawLeftRight(const Vector3 position, const Color left, const Color right)
{
    rlPushMatrix();
    {
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        rlTranslatef(position.x, position.y, position.z);

        rlBegin(RL_TRIANGLES);
        {
            const float x = 0.0f;
            const float y = 0.0f;
            const float z = 0.0f;
            const float width = 1.0f;
            const float height = 1.0f;
            const float length = 1.0f;

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

void DrawUp(const Face* up, const Rotation r)
{
    EColor color = {0};
    if (r.face == F_U || r.face == F_B || r.face == F_F || r.face == F_L || r.face == F_R )
        rlPushMatrix();

    if (r.face == F_U)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_U)].x, FACE_CENTERS[f2i(F_U)].y, FACE_CENTERS[f2i(F_U)].z);

    if (r.face == F_B)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_B)].x, FACE_CENTERS[f2i(F_B)].y, FACE_CENTERS[f2i(F_B)].z);

    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(up, F_U | F_L);
    DrawTopBottom((Vector3){-1.f, 1.f, -1.f}, c2r(color), BLACK);
    if (r.face == F_L) rlPopMatrix();
    color = getCell(up, F_U);
    DrawTopBottom((Vector3){0.f, 1.f, -1.f}, c2r(color), BLACK);
    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(up, F_U | F_R);
    DrawTopBottom((Vector3){1.f, 1.f, -1.f}, c2r(color), BLACK);
    if (r.face == F_R) rlPopMatrix();

    if (r.face == F_B)
        rlPopMatrix();

    if (r.face == F_L || r.face == F_R)
        rlPushMatrix();

    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(up, F_L);
    DrawTopBottom((Vector3){-1.f, 1.f, 0.f}, c2r(color), BLACK);
    if (r.face == F_L) rlPopMatrix();
    color = White;
    DrawTopBottom(FACE_CENTERS[f2i(F_U)], c2r(color), BLACK);
    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(up, F_R);
    DrawTopBottom((Vector3){1.f, 1.f, 0.f}, c2r(color), BLACK);
    if (r.face == F_R) rlPopMatrix();

    if (r.face == F_F)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_F)].x, FACE_CENTERS[f2i(F_F)].y, FACE_CENTERS[f2i(F_F)].z);

    if (r.face == F_L || r.face == F_R)
        rlPushMatrix();

    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(up, F_D | F_L);
    DrawTopBottom((Vector3){-1.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F_L) rlPopMatrix();
    color = getCell(up, F_D);
    DrawTopBottom((Vector3){0.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(up, F_D | F_R);
    DrawTopBottom((Vector3){1.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F_R) rlPopMatrix();

    if (r.face == F_U || r.face == F_F)
        rlPopMatrix();
}

void DrawDown(const Face* down, const Rotation r)
{
    EColor color = {0};

    if (r.face == F_D || r.face == F_B || r.face == F_F || r.face == F_L || r.face == F_R )
        rlPushMatrix();

    if (r.face == F_D)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_D)].x, FACE_CENTERS[f2i(F_D)].y, FACE_CENTERS[f2i(F_D)].z);

    if (r.face == F_F)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_F)].x, FACE_CENTERS[f2i(F_F)].y, FACE_CENTERS[f2i(F_F)].z);

    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(down, F_U | F_L);
    DrawTopBottom((Vector3){-1.f, -1.f, 1.f}, BLACK, c2r(color));
    if (r.face == F_L) rlPopMatrix();
    color = getCell(down, F_U);
    DrawTopBottom((Vector3){0.f, -1.f, 1.f}, BLACK, c2r(color));
    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(down, F_U | F_R);
    DrawTopBottom((Vector3){1.f, -1.f, 1.f}, BLACK, c2r(color));
    if (r.face == F_R) rlPopMatrix();

    if (r.face == F_F)
        rlPopMatrix();

    if (r.face == F_L || r.face == F_R)
        rlPushMatrix();

    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(down, F_L);
    DrawTopBottom((Vector3){-1.f, -1.f, 0.f}, BLACK, c2r(color));
    if (r.face == F_L) rlPopMatrix();
    color = Yellow;
    DrawTopBottom(FACE_CENTERS[f2i(F_D)], BLACK, c2r(color));
    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(down, F_R);
    DrawTopBottom((Vector3){1.f, -1.f, 0.f}, BLACK, c2r(color));
    if (r.face == F_R) rlPopMatrix();

    if (r.face == F_B)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_B)].x, FACE_CENTERS[f2i(F_B)].y, FACE_CENTERS[f2i(F_B)].z);

    if (r.face == F_L || r.face == F_R)
        rlPushMatrix();

    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(down, F_D | F_L);
    DrawTopBottom((Vector3){-1.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == F_L) rlPopMatrix();
    color = getCell(down, F_D);
    DrawTopBottom((Vector3){0.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(down, F_D | F_R);
    DrawTopBottom((Vector3){1.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == F_R) rlPopMatrix();

    if (r.face == F_D || r.face == F_B)
        rlPopMatrix();
}

void DrawFront(const Face* front, const Rotation r)
{
    EColor color = {0};

    if (r.face == F_F || r.face == F_U || r.face == F_D || r.face == F_L || r.face == F_R)
        rlPushMatrix();

    if (r.face == F_F)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_F)].x, FACE_CENTERS[f2i(F_F)].y, FACE_CENTERS[f2i(F_F)].z);

    if (r.face == F_U)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_U)].x, FACE_CENTERS[f2i(F_U)].y, FACE_CENTERS[f2i(F_U)].z);

    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(front, F_U | F_L);
    DrawFrontBack((Vector3){-1.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F_L) rlPopMatrix();
    color = getCell(front, F_U);
    DrawFrontBack((Vector3){0.f, 1.f, 1.f}, c2r(color), BLACK);
    DrawLeftRight((Vector3){0.f, 1.f, 1.f}, BLACK, BLACK);
    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(front, F_U | F_R);
    DrawFrontBack((Vector3){1.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F_R) rlPopMatrix();

    if (r.face == F_U)
        rlPopMatrix();

    if (r.face == F_L || r.face == F_R)
        rlPushMatrix();

    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(front, F_L);
    DrawFrontBack((Vector3){-1.f, 0.f, 1.f}, c2r(color), BLACK);
    DrawTopBottom((Vector3){-1.f, 0.f, 1.f}, BLACK, BLACK);
    if (r.face == F_L) rlPopMatrix();
    color = Green;
    DrawFrontBack(FACE_CENTERS[f2i(F_F)], c2r(color), BLACK);
    DrawLeftRight(FACE_CENTERS[f2i(F_F)], BLACK, BLACK);
    DrawTopBottom(FACE_CENTERS[f2i(F_F)], BLACK, BLACK);
    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(front, F_R);
    DrawFrontBack((Vector3){1.f, 0.f, 1.f}, c2r(color), BLACK);
    DrawTopBottom((Vector3){1.f, 0.f, 1.f}, BLACK, BLACK);
    if (r.face == F_R) rlPopMatrix();

    if (r.face == F_D)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_D)].x, FACE_CENTERS[f2i(F_D)].y, FACE_CENTERS[f2i(F_D)].z);

    if (r.face == F_L || r.face == F_R)
        rlPushMatrix();

    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(front, F_D | F_L);
    DrawFrontBack((Vector3){-1.f, -1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F_L) rlPopMatrix();
    color = getCell(front, F_D);
    DrawFrontBack((Vector3){0.f, -1.f, 1.f}, c2r(color), BLACK);
    DrawLeftRight((Vector3){0.f, -1.f, 1.f}, BLACK, BLACK);
    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(front, F_D | F_R);
    DrawFrontBack((Vector3){1.f, -1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F_R) rlPopMatrix();

    if (r.face == F_F || r.face == F_D)
        rlPopMatrix();
}

void DrawBack(const Face* back, const Rotation r)
{
    EColor color = {0};

    if (r.face == F_B || r.face == F_U || r.face == F_D || r.face == F_L || r.face == F_R)
        rlPushMatrix();

    if (r.face == F_B)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_B)].x, FACE_CENTERS[f2i(F_B)].y, FACE_CENTERS[f2i(F_B)].z);

    if (r.face == F_U)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_U)].x, FACE_CENTERS[f2i(F_U)].y, FACE_CENTERS[f2i(F_U)].z);

    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(back, F_U | F_L);
    DrawFrontBack((Vector3){1.f, 1.f, -1.f}, BLACK, c2r(color));
    if (r.face == F_R) rlPopMatrix();
    color = getCell(back, F_U);
    DrawFrontBack((Vector3){0.f, 1.f, -1.f}, BLACK, c2r(color));
    DrawLeftRight((Vector3){0.f, 1.f, -1.f}, BLACK, BLACK);
    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(back, F_U | F_R);
    DrawFrontBack((Vector3){-1.f, 1.f, -1.f}, BLACK, c2r(color));
    if (r.face == F_L) rlPopMatrix();

    if (r.face == F_U)
        rlPopMatrix();

    if (r.face == F_R || r.face == F_L)
        rlPushMatrix();

    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(back, F_L);
    DrawFrontBack((Vector3){1.f, 0.f, -1.f}, BLACK, c2r(color));
    DrawTopBottom((Vector3){1.f, 0.f, -1.f}, BLACK, BLACK);
    if (r.face == F_R) rlPopMatrix();
    color = Blue;
    DrawFrontBack(FACE_CENTERS[f2i(F_B)], BLACK, c2r(color));
    DrawLeftRight(FACE_CENTERS[f2i(F_B)], BLACK, BLACK);
    DrawTopBottom(FACE_CENTERS[f2i(F_B)], BLACK, BLACK);
    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(back, F_R);
    DrawFrontBack((Vector3){-1.f, 0.f, -1.f}, BLACK, c2r(color));
    DrawTopBottom((Vector3){-1.f, 0.f, -1.f}, BLACK, BLACK);
    if (r.face == F_L) rlPopMatrix();

    if (r.face == F_D)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_D)].x, FACE_CENTERS[f2i(F_D)].y, FACE_CENTERS[f2i(F_D)].z);

    if (r.face == F_R || r.face == F_L)
        rlPushMatrix();

    if (r.face == F_R) rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);
    color = getCell(back, F_D | F_L);
    DrawFrontBack((Vector3){1.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == F_R) rlPopMatrix();
    color = getCell(back, F_D);
    DrawFrontBack((Vector3){0.f, -1.f, -1.f}, BLACK, c2r(color));
    DrawLeftRight((Vector3){0.f, -1.f, -1.f}, BLACK, BLACK);
    if (r.face == F_L) rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);
    color = getCell(back, F_D | F_R);
    DrawFrontBack((Vector3){-1.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == F_L) rlPopMatrix();

    if (r.face == F_B || r.face == F_D)
        rlPopMatrix();
}

void DrawLeft(const Face* left, const Rotation r)
{
    EColor color = {0};

    if (r.face == F_L || r.face == F_U || r.face == F_D || r.face == F_B || r.face == F_F)
        rlPushMatrix();

    if (r.face == F_L)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_L)].x, FACE_CENTERS[f2i(F_L)].y, FACE_CENTERS[f2i(F_L)].z);

    if (r.face == F_U)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_U)].x, FACE_CENTERS[f2i(F_U)].y, FACE_CENTERS[f2i(F_U)].z);

    if (r.face == F_B) rlRotatef(r.angle, FACE_CENTERS[f2i(F_B)].x, FACE_CENTERS[f2i(F_B)].y, FACE_CENTERS[f2i(F_B)].z);
    color = getCell(left, F_U | F_L);
    DrawLeftRight((Vector3){-1.f, 1.f, -1.f}, c2r(color), BLACK);
    if (r.face == F_B) rlPopMatrix();
    color = getCell(left, F_U);
    DrawLeftRight((Vector3){-1.f, 1.f, 0.f}, c2r(color), BLACK);
    DrawFrontBack((Vector3){-1.f, 1.f, 0.f}, BLACK, BLACK);
    if (r.face == F_F) rlRotatef(r.angle, FACE_CENTERS[f2i(F_F)].x, FACE_CENTERS[f2i(F_F)].y, FACE_CENTERS[f2i(F_F)].z);
    color = getCell(left, F_U | F_R);
    DrawLeftRight((Vector3){-1.f, 1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F_F) rlPopMatrix();

    if (r.face == F_U)
        rlPopMatrix();

    if (r.face == F_B || r.face == F_F)
        rlPushMatrix();

    if (r.face == F_B) rlRotatef(r.angle, FACE_CENTERS[f2i(F_B)].x, FACE_CENTERS[f2i(F_B)].y, FACE_CENTERS[f2i(F_B)].z);
    color = getCell(left, F_L);
    DrawLeftRight((Vector3){-1.f, 0.f, -1.f}, c2r(color), BLACK);
    if (r.face == F_B) rlPopMatrix();
    color = Orange;
    DrawLeftRight(FACE_CENTERS[f2i(F_L)], c2r(color), BLACK);
    DrawFrontBack(FACE_CENTERS[f2i(F_L)], BLACK, BLACK);
    DrawTopBottom(FACE_CENTERS[f2i(F_L)], BLACK, BLACK);
    if (r.face == F_F) rlRotatef(r.angle, FACE_CENTERS[f2i(F_F)].x, FACE_CENTERS[f2i(F_F)].y, FACE_CENTERS[f2i(F_F)].z);
    color = getCell(left, F_R);
    DrawLeftRight((Vector3){-1.f, 0.f, 1.f}, c2r(color), BLACK);
    if (r.face == F_F) rlPopMatrix();

    if (r.face == F_D)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_D)].x, FACE_CENTERS[f2i(F_D)].y, FACE_CENTERS[f2i(F_D)].z);

    if (r.face == F_B || r.face == F_F)
        rlPushMatrix();

    if (r.face == F_B) rlRotatef(r.angle, FACE_CENTERS[f2i(F_B)].x, FACE_CENTERS[f2i(F_B)].y, FACE_CENTERS[f2i(F_B)].z);
    color = getCell(left, F_D | F_L);
    DrawLeftRight((Vector3){-1.f, -1.f, -1.f}, c2r(color), BLACK);
    if (r.face == F_B) rlPopMatrix();
    color = getCell(left, F_D);
    DrawLeftRight((Vector3){-1.f, -1.f, 0.f}, c2r(color), BLACK);
    DrawFrontBack((Vector3){-1.f, -1.f, 0.f}, BLACK, BLACK);
    if (r.face == F_F) rlRotatef(r.angle, FACE_CENTERS[f2i(F_F)].x, FACE_CENTERS[f2i(F_F)].y, FACE_CENTERS[f2i(F_F)].z);
    color = getCell(left, F_D | F_R);
    DrawLeftRight((Vector3){-1.f, -1.f, 1.f}, c2r(color), BLACK);
    if (r.face == F_F) rlPopMatrix();

    if (r.face == F_L || r.face == F_D)
        rlPopMatrix();
}

void DrawRight(const Face* right, const Rotation r)
{
    EColor color = {0};

    if (r.face == F_R || r.face == F_U || r.face == F_D || r.face == F_B || r.face == F_F)
        rlPushMatrix();

    if (r.face == F_R)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_R)].x, FACE_CENTERS[f2i(F_R)].y, FACE_CENTERS[f2i(F_R)].z);

    if (r.face == F_U)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_U)].x, FACE_CENTERS[f2i(F_U)].y, FACE_CENTERS[f2i(F_U)].z);

    if (r.face == F_F) rlRotatef(r.angle, FACE_CENTERS[f2i(F_F)].x, FACE_CENTERS[f2i(F_F)].y, FACE_CENTERS[f2i(F_F)].z);
    color = getCell(right, F_U | F_L);
    DrawLeftRight((Vector3){1.f, 1.f, 1.f}, BLACK, c2r(color));
    if (r.face == F_F) rlPopMatrix();
    color = getCell(right, F_U);
    DrawLeftRight((Vector3){1.f, 1.f, 0.f}, BLACK, c2r(color));
    DrawFrontBack((Vector3){1.f, 1.f, 0.f}, BLACK, BLACK);
    if (r.face == F_B) rlRotatef(r.angle, FACE_CENTERS[f2i(F_B)].x, FACE_CENTERS[f2i(F_B)].y, FACE_CENTERS[f2i(F_B)].z);
    color = getCell(right, F_U | F_R);
    DrawLeftRight((Vector3){1.f, 1.f, -1.f}, BLACK, c2r(color));
    if (r.face == F_B) rlPopMatrix();

    if (r.face == F_U)
        rlPopMatrix();

    if (r.face == F_B || r.face == F_F)
        rlPushMatrix();

    if (r.face == F_F) rlRotatef(r.angle, FACE_CENTERS[f2i(F_F)].x, FACE_CENTERS[f2i(F_F)].y, FACE_CENTERS[f2i(F_F)].z);
    color = getCell(right, F_L);
    DrawLeftRight((Vector3){1.f, 0.f, 1.f}, BLACK, c2r(color));
    if (r.face == F_F) rlPopMatrix();
    color = Red;
    DrawLeftRight(FACE_CENTERS[f2i(F_R)], BLACK, c2r(color));
    DrawFrontBack(FACE_CENTERS[f2i(F_R)], BLACK, BLACK);
    DrawTopBottom(FACE_CENTERS[f2i(F_R)], BLACK, BLACK);
    if (r.face == F_B) rlRotatef(r.angle, FACE_CENTERS[f2i(F_B)].x, FACE_CENTERS[f2i(F_B)].y, FACE_CENTERS[f2i(F_B)].z);
    color = getCell(right, F_R);
    DrawLeftRight((Vector3){1.f, 0.f, -1.f}, BLACK, c2r(color));
    if (r.face == F_B) rlPopMatrix();

    if (r.face == F_D)
        rlRotatef(r.angle, FACE_CENTERS[f2i(F_D)].x, FACE_CENTERS[f2i(F_D)].y, FACE_CENTERS[f2i(F_D)].z);

    if (r.face == F_B || r.face == F_F)
        rlPushMatrix();

    if (r.face == F_F) rlRotatef(r.angle, FACE_CENTERS[f2i(F_F)].x, FACE_CENTERS[f2i(F_F)].y, FACE_CENTERS[f2i(F_F)].z);
    color = getCell(right, F_D | F_L);
    DrawLeftRight((Vector3){1.f, -1.f, 1.f}, BLACK, c2r(color));
    if (r.face == F_F) rlPopMatrix();
    color = getCell(right, F_D);
    DrawLeftRight((Vector3){1.f, -1.f, 0.f}, BLACK, c2r(color));
    DrawFrontBack((Vector3){1.f, -1.f, 0.f}, BLACK, BLACK);
    if (r.face == F_B) rlRotatef(r.angle, FACE_CENTERS[f2i(F_B)].x, FACE_CENTERS[f2i(F_B)].y, FACE_CENTERS[f2i(F_B)].z);
    color = getCell(right, F_D | F_R);
    DrawLeftRight((Vector3){1.f, -1.f, -1.f}, BLACK, c2r(color));
    if (r.face == F_B) rlPopMatrix();

    if (r.face == F_R || r.face == F_D)
        rlPopMatrix();
}

// Will be drawn at 0.0 0.0 0.0
void DrawRubik(const Cube* cube)
{
    DrawUp(&cube->faces[f2i(F_U)], cube->rotation);
    DrawDown(&cube->faces[f2i(F_D)], cube->rotation);

    DrawFront(&cube->faces[f2i(F_F)], cube->rotation);
    DrawBack(&cube->faces[f2i(F_B)], cube->rotation);

    DrawLeft(&cube->faces[f2i(F_L)], cube->rotation);
    DrawRight(&cube->faces[f2i(F_R)], cube->rotation);
}
