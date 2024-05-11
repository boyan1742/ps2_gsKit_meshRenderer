#include <malloc.h>
#include <assert.h>
#include <draw.h>
#include "../include/Mesh.h"
#include "external/gsKit/include/gsInline.h"

void CreateMeshTransformMatrix(PS2_MATRIX output, const Mesh *mesh)
{
    PS2_VECTOR objPos, objRot, objScale;

    Vec3ToPS2Vector(&mesh->position, objPos);
    Vec3ToPS2Vector(&mesh->rotation, objRot);
    Vec3ToPS2Vector(&mesh->scale, objScale);

    matrix_unit(output);
    matrix_scale(output, output, objScale);
    matrix_rotate(output, output, objRot);
    matrix_translate(output, output, objPos);
}

Mesh CreateMeshFromBuffers(const int *points, size_t pointsCount, const MyVector4 *positions, size_t positionsCount,
                           const MyVector4 *colors, size_t colorsCount)
{
    assert(positionsCount == colorsCount);
    assert(points);
    assert(positions);
    assert(colors);
    assert(pointsCount > 0 && pointsCount > 0 && colorsCount > 0);

    Mesh mesh = {0};
    mesh.vertices = (Vertex *) memalign(128, sizeof(Vertex) * pointsCount);
    mesh.verticesCount = pointsCount;
    mesh.position = mesh.rotation = (MyVector3) {0, 0, 0};
    mesh.scale = (MyVector3) {1, 1, 1};

    for (size_t i = 0; i < pointsCount; i++)
    {
        mesh.vertices[i].position = positions[points[i]];
        mesh.vertices[i].color = colors[points[i]];
    }

    return mesh;
}

Mesh CreateMeshFromBuffer(const MyVector4 *positions, size_t positionsCount)
{
    Mesh mesh = {0};
    mesh.vertices = (Vertex *) malloc(sizeof(Vertex) * positionsCount);
    mesh.verticesCount = positionsCount;
    mesh.position = mesh.rotation = (MyVector3) {0, 0, 0};
    mesh.scale = (MyVector3) {1, 1, 1};

    for (size_t i = 0; i < positionsCount; i++)
    {
        mesh.vertices[i].position = positions[i];
        mesh.vertices[i].color = (MyVector4) {0, 0, 0, 1};
    }

    return mesh;
}

Mesh CreateMeshFromVertexBuffer(const Vertex *vertices, size_t verticesCount)
{
    Mesh mesh = {0};
    mesh.vertices = (Vertex *) malloc(sizeof(Vertex) * verticesCount);
    mesh.verticesCount = verticesCount;
    mesh.position = mesh.rotation = (MyVector3) {0, 0, 0};
    mesh.scale = (MyVector3) {1, 1, 1};

    memcpy(mesh.vertices, vertices, verticesCount);
    return mesh;
}

void AddMeshPosition(Mesh *mesh, const MyVector3 *position)
{
    mesh->position.x += position->x;
    mesh->position.y += position->y;
    mesh->position.z += position->z;
}

void AddMeshRotation(Mesh *mesh, const MyVector3 *rotation)
{
    mesh->rotation.x += rotation->x;
    mesh->rotation.y += rotation->y;
    mesh->rotation.z += rotation->z;
}

bool gsKitConvertXYZ(vertex_f_t *output, GSGLOBAL *gsGlobal, int count, vertex_f_t *meshVertices)
{
    int z;
    unsigned int max_z;

    switch (gsGlobal->PSMZ)
    {
        case GS_PSMZ_32:
            z = 32;
            break;

        case GS_PSMZ_24:
            z = 24;
            break;

        case GS_PSMZ_16:
        case GS_PSMZ_16S:
            z = 16;
            break;

        default:
            return false;
    }

    float center_x = (float) gsGlobal->Width / 2.f;
    float center_y = (float) gsGlobal->Height / 2.f;
    max_z = 1 << (z - 1);

    for (int i = 0; i < count; i++)
    {
        output[i].x = ((meshVertices[i].x + 1.0f) * center_x);
        output[i].y = ((meshVertices[i].y + 1.0f) * center_y);
        output[i].z = ((meshVertices[i].z + 1.0f) * max_z);
    }

    return true;
}

void DrawMesh(const Mesh *mesh, GSGLOBAL *gsGlobal, PS2_MATRIX mvp)
{
    GSPRIMPOINT *gs_vertices = (GSPRIMPOINT *) memalign(128, sizeof(GSPRIMPOINT) * mesh->verticesCount);
    VECTOR *ps2Vertices = (VECTOR *) memalign(128, sizeof(VECTOR) * mesh->verticesCount);
    VECTOR *ps2Colors = (VECTOR *) memalign(128, sizeof(VECTOR) * mesh->verticesCount);
    VECTOR *tempVertices = memalign(128, sizeof(VECTOR) * mesh->verticesCount);
    VECTOR *readyVertices = (VECTOR *) memalign(128, sizeof(VECTOR) * mesh->verticesCount);
    color_t *readyColors = (color_t *) memalign(128, sizeof(color_t) * mesh->verticesCount);

    for (size_t i = 0; i < mesh->verticesCount; i++)
    {
        ps2Vertices[i][0] = mesh->vertices[i].position.x;
        ps2Vertices[i][1] = mesh->vertices[i].position.y;
        ps2Vertices[i][2] = mesh->vertices[i].position.z;
        ps2Vertices[i][3] = mesh->vertices[i].position.w;

        ps2Colors[i][0] = mesh->vertices[i].color.x;
        ps2Colors[i][1] = mesh->vertices[i].color.y;
        ps2Colors[i][2] = mesh->vertices[i].color.z;
        ps2Colors[i][3] = mesh->vertices[i].color.w;
    }

    // Calculate the vertex values.
    calculate_vertices(tempVertices, (int) mesh->verticesCount, ps2Vertices, mvp);

    // Convert floating point vertices to fixed point and translate to center of screen.
    gsKitConvertXYZ((vertex_f_t *) readyVertices, gsGlobal,
                    (int) mesh->verticesCount, (vertex_f_t *) tempVertices);

    // Convert floating point colours to fixed point.
    draw_convert_rgbq(readyColors, (int) mesh->verticesCount,
                      (vertex_f_t *) tempVertices, (color_f_t *) ps2Colors, 0x80);

    for (size_t i = 0; i < mesh->verticesCount; i++)
    {
        gs_vertices[i].rgbaq = color_to_RGBAQ(readyColors[i].r, readyColors[i].g,
                                              readyColors[i].b, readyColors[i].a, 0.0f);
        gs_vertices[i].xyz2 = vertex_to_XYZ2(gsGlobal, readyVertices[i][0],
                                             readyVertices[i][1], (int) readyVertices[i][2]);
    }

    gsKit_prim_list_triangle_gouraud_3d(gsGlobal, (int) mesh->verticesCount, gs_vertices);

    free(ps2Vertices);
    free(ps2Colors);
    free(tempVertices);
    free(readyVertices);
    free(readyColors);
    free(gs_vertices);
}

void DeleteMesh(Mesh *mesh)
{
    free(mesh->vertices);
    mesh->verticesCount = 0;
    mesh->position = mesh->rotation = mesh->scale = (MyVector3) {0};
}