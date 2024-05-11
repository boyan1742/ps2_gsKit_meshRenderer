#ifndef GSKIT_MESH_MYTYPES_H
#define GSKIT_MESH_MYTYPES_H

#include <math3d.h>
#include <stdint.h>

typedef struct MyVector2
{
    float x;
    float y;
} MyVector2;

typedef struct MyVector3
{
    float x;
    float y;
    float z;
} MyVector3;

typedef struct MyVector4
{
    float x;
    float y;
    float z;
    float w;
} MyVector4;

typedef VECTOR PS2_VECTOR;
typedef MATRIX PS2_MATRIX;

typedef uint64_t MyColor;
typedef MyVector4 MyColorRGBA;

MyVector2 ToVec2FromVec3(const MyVector3 *vec); // Converts from Vec3 to Vec2
MyVector2 ToVec2FromVec4(const MyVector4 *vec); // Converts from Vec4 to Vec2

MyVector3 ToVec3FromVec2(const MyVector2 *vec); // Converts from Vec2 to Vec3
MyVector3 ToVec3FromVec4(const MyVector4 *vec); // Converts from Vec4 to Vec3

MyVector4 ToVec4FromVec2(const MyVector2 *vec); // Converts from Vec2 to Vec4
MyVector4 ToVec4FromVec3(const MyVector3 *vec); // Converts from Vec3 to Vec4

MyColor CreateColorRGB(uint8_t r, uint8_t g, uint8_t b); // Creates a RGBAQ color from just RGB data.
MyColor CreateColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a); // Creates a RGBAQ color from just RGBA data.
MyColor CreateColorRGBQ(uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t q); // Creates a RGBAQ color.

void Vec2ToPS2Vector(const MyVector2* vec, VECTOR output); // Converts from Vec2 to PS2 Vector impl.
void Vec3ToPS2Vector(const MyVector3* vec, VECTOR output); // Converts from Vec3 to PS2 Vector impl.
void Vec4ToPS2Vector(const MyVector4* vec, VECTOR output); // Converts from Vec4 to PS2 Vector impl.

#endif
