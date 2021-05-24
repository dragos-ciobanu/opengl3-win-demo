#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
struct Vertex
{
    glm::vec3 position;
    //glm::vec3 color;
    glm::vec2 texCoords;
    float texID;
};

static glm::vec3 red = { 1.0f, 0.0f, 0.0f };
static glm::vec3 green = { 0.0f, 1.0f, 0.0f };
static glm::vec3 blue = { 0.0f, 0.0f, 1.0f };

static Vertex* CreateCube(Vertex* target, float x, float y, float z, float lenght = 50.0f, glm::vec3 color = red, float texId = 0)
{
    target->position = { x, y, z };
    //target->color = color;
    target->texCoords = { 0.0f, 0.0f };
    target->texID = texId;
    target++;
    target->position = { x + lenght, y, z };
    //target->color = color;
    target->texCoords = { 1.0f, 0.0f };
    target->texID = texId;
    target++;
    target->position = { x + lenght, y, z + lenght };
    //target->color = color;
    target->texCoords = { 0.0f, 0.0f };
    target->texID = texId;
    target++;
    target->position = { x, y, z + lenght };
    //target->color = color;
    target->texCoords = { 1.0f, 0.0f };
    target->texID = texId;
    target++;
    target->position = { x, y + lenght, z };
    //target->color = color;
    target->texCoords = { 0.0f, 1.0f };
    target->texID = texId;
    target++;
    target->position = { x + lenght, y + lenght, z };
    //target->color = color;
    target->texCoords = { 1.0f, 1.0f };
    target->texID = texId;
    target++;
    target->position = { x + lenght, y + lenght, z + lenght };
    //target->color = color;
    target->texCoords = { 0.0f, 1.0f };
    target->texID = texId;
    target++;
    target->position = { x, y + lenght, z + lenght };
    //target->color = color;
    target->texCoords = { 1.0f, 1.0f };
    target->texID = texId;
    target++;

    return target;
}

static Vertex* CreateCuboid(Vertex* target, glm::vec3 p1, glm::vec3 p2, glm::vec3 color = red, float texId = 0)
{
    target->position = { p1.x, p1.y, p1.z };
    //target->color = color;
    target->texCoords = { 0.0f, 0.0f };
    target->texID = texId;
    target++;

    target->position = { p2.x, p1.y, p1.z };
    //target->color = color;
    target->texCoords = { 1.0f, 0.0f };
    target->texID = texId;
    target++;

    target->position = { p2.x, p1.y, p2.z };
    //target->color = color;
    target->texCoords = { 0.0f, 0.0f };
    target->texID = texId;
    target++;

    target->position = { p1.x, p1.y, p2.z };
    //target->color = color;
    target->texCoords = { 1.0f, 0.0f };
    target->texID = texId;
    target++;

    target->position = { p1.x, p2.y, p1.z };
    //target->color = color;
    target->texCoords = { 0.0f, 10.0f };
    target->texID = texId;
    target++;

    target->position = { p2.x, p2.y, p1.z };
    //target->color = color;
    target->texCoords = { 1.0f, 10.0f };
    target->texID = texId;
    target++;

    target->position = { p2.x, p2.y, p2.z };
    //target->color = color;
    target->texCoords = { 0.0f, 10.0f };
    target->texID = texId;
    target++;

    target->position = { p1.x, p2.y, p2.z };
    //target->color = color;
    target->texCoords = { 1.0f, 10.0f };
    target->texID = texId;
    target++;


    return target;
}

static unsigned int* CreateCubeIndices(unsigned int* target, unsigned int offset)
{
    unsigned int quadIndices[36] = {
        // draw quad
        /* face 1 */   0, 1, 2,    0, 2, 3,
        /* face 2 */   0, 1, 5,    0, 5, 4,
        /* face 3 */   1, 2, 6,    1, 6, 5,
        /* face 4 */   2, 3, 7,    2, 7, 6,
        /* face 5 */   0, 4, 7,    0, 7, 3,
        /* face 6 */   4, 5, 6,    4, 6, 7,
    };

    for (size_t i = 0; i < 36; i++) {
        *target = quadIndices[i] += offset;
        target++;
    }

    return target;
}

static Vertex* CreatePyramid(Vertex* target, float x, float y, float z, float lenght = 50.0f, float height = 100.0f, glm::vec3 color = red, float texId = 0.0f)
{
    target->position = { x, y, z };
    //target->color = color;
    target->texCoords = { 0.0f, 0.0f };
    target->texID = texId;
    target++;

    target->position = { x + lenght, y, z };
    //target->color = color;
    target->texCoords = { 5.0f, 0.0f };
    target->texID = texId;
    target++;

    target->position = { x + lenght, y, z - lenght };
    //target->color = color;
    target->texCoords = { 0.0f, 0.0f };
    target->texID = texId;
    target++;

    target->position = { x, y, z - lenght };
    //target->color = color;
    target->texCoords = { 5.0f, 0.0f };
    target->texID = texId;
    target++;

    target->position = { x + (lenght / 2), y + height, z - (lenght / 2) };
    //target->color = color;
    target->texCoords = { 2.5f, 5.0f };
    target->texID = texId;
    target++;

    return target;
}

static unsigned int* CreatePyramidIndices(unsigned int* target, unsigned int offset)
{
    unsigned int indices[18] = {
        // draw pyramid
        /* base 1 */        0, 1, 2,
        /* base 1 */        0, 2, 3,
        /* face 1 */        0, 1, 4,
        /* face 2 */        1, 2, 4,
        /* face 3 */        2, 3, 4,
        /* face 4 */        3, 0, 4,
    };

    for (size_t i = 0; i < 18; i++) {
        *target = indices[i] += offset;
        target++;
    }

    return target;
}

static Vertex* CreatePlane(Vertex* target, float x, float y, float z, float lenght = 50.0f, glm::vec3 color = red, float texId = 0.0f)
{
    target->position = { x, y, z };
    //target->color = color;
    target->texCoords = { 0.0f, 0.0f };
    target->texID = texId;
    target++;

    target->position = { x + lenght, y, z };
    //target->color = color;
    target->texCoords = { 10.0f, 0.0f };
    target->texID = texId;
    target++;

    target->position = { x + lenght, y, z - lenght };
    //target->color = color;
    target->texCoords = { 10.0f, 10.0f };
    target->texID = texId;
    target++;

    target->position = { x, y, z - lenght };
    //target->color = color;
    target->texCoords = { 0.0f, 10.0f };
    target->texID = texId;
    target++;

    return target;
}


static unsigned int* CreatePlaneIndices(unsigned int* target, unsigned int offset)
{
    unsigned int indices[6] = {
        // draw plane
        /* base 1 */        0, 1, 2,
        /* base 1 */        0, 2, 3,
    };

    for (size_t i = 0; i < 6; i++) {
        *target = indices[i] += offset;
        target++;
    }

    return target;
}
