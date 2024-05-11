#ifndef GSKIT_MESH_MESH_H
#define GSKIT_MESH_MESH_H

#include <stddef.h>
#include "MyTypes.h"
#include "../external/gsKit/include/gsKit.h"

typedef struct Vertex
{
    MyVector4 position;
    MyVector4 color;
} Vertex;

typedef struct Mesh
{
    Vertex *vertices;
    size_t verticesCount;

    MyVector3 position;
    MyVector3 rotation;
    MyVector3 scale;
} Mesh;

Mesh CreateMeshFromBuffers(const int *points, size_t pointsCount, const MyVector4 *positions, size_t positionsCount,
                           const MyVector4 *colors, size_t colorsCount); // Creates a mesh from composite buffers.

Mesh CreateMeshFromBuffer(const MyVector4 *positions,
                          size_t positionsCount); // Creates a mesh from just vertex positions.

Mesh CreateMeshFromVertexBuffer(const Vertex *vertices, size_t verticesCount); // Creates a mesh from ready vertices.

void CreateMeshTransformMatrix(PS2_MATRIX output, const Mesh *mesh); // Creates a model matrix from a mesh.

void AddMeshPosition(Mesh *mesh, const MyVector3 *position); // Adds position to a mesh.
void AddMeshRotation(Mesh *mesh, const MyVector3 *rotation); // Adds rotation to a mesh.

void DrawMesh(const Mesh *mesh, GSGLOBAL *gsGlobal, PS2_MATRIX mvp); // Draws the mesh to the screen.

void DeleteMesh(Mesh *mesh); // Deletes the mesh data from memory.

#endif
