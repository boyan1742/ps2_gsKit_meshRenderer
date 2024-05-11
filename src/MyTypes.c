#include "../include/MyTypes.h"

MyVector2 ToVec2FromVec3(const MyVector3 *vec)
{
    return (MyVector2) {.x = vec->x, .y = vec->y};
}
MyVector2 ToVec2FromVec4(const MyVector4 *vec)
{
    return (MyVector2) {.x = vec->x, .y = vec->y};
}

MyVector3 ToVec3FromVec2(const MyVector2 *vec)
{
    return (MyVector3) {.x = vec->x, .y = vec->y, .z = 0};
}

MyVector3 ToVec3FromVec4(const MyVector4 *vec)
{
    return (MyVector3) {.x = vec->x, .y = vec->y, .z = vec->z};
}

MyVector4 ToVec4FromVec2(const MyVector2 *vec)
{
    return (MyVector4) {.x = vec->x, .y = vec->y, .z = 0, .w = 0};
}

MyVector4 ToVec4FromVec3(const MyVector3 *vec)
{
    return (MyVector4) {.x = vec->x, .y = vec->y, .z = vec->z, .w = 0};
}

MyColor CreateColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
    return CreateColorRGBQ(r, g, b, 0x80, 0);
}

MyColor CreateColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return CreateColorRGBQ(r, g, b, a, 0);
}

MyColor CreateColorRGBQ(uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t q)
{
    return ((uint64_t) (r) | ((uint64_t) (g) << 8) | ((uint64_t) (b) << 16) |
            ((uint64_t) (a) << 24) | ((uint64_t) (q) << 32));
}

void Vec2ToPS2Vector(const MyVector2 *vec, VECTOR output)
{
    output[0] = vec->x;
    output[1] = vec->y;
    output[2] = 0;
    output[3] = 1;
}

void Vec3ToPS2Vector(const MyVector3 *vec, VECTOR output)
{
    output[0] = vec->x;
    output[1] = vec->y;
    output[2] = vec->z;
    output[3] = 1;
}

void Vec4ToPS2Vector(const MyVector4 *vec, VECTOR output)
{
    output[0] = vec->x;
    output[1] = vec->y;
    output[2] = vec->z;
    output[3] = vec->w;
}