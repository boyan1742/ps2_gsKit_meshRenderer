#include <malloc.h>
#include "../include/Camera.h"

struct Camera
{
    MyVector3 position;
    MyVector3 rotation;
    float aspectRatio;
    MyVector4 viewRect;
    float near;
    float far;
};

Camera *CreateCamera(float aspectRatio, const MyVector4 *viewRect, float near, float far)
{
    Camera *camera = (Camera *) malloc(sizeof(Camera));

    camera->position = camera->rotation = (MyVector3) {0, 0, 0};
    camera->aspectRatio = aspectRatio;
    camera->viewRect = *viewRect;
    camera->near = near;
    camera->far = far;

    return camera;
}

void CreateViewMatrix(PS2_MATRIX output, const Camera *camera)
{
    create_view_screen(output, camera->aspectRatio,
                       camera->viewRect.x, camera->viewRect.y, camera->viewRect.z, camera->viewRect.w,
                       camera->near, camera->far);
}

void CreateProjMatrix(PS2_MATRIX output, const Camera *camera)
{
    PS2_VECTOR camPos, camRot;

    Vec3ToPS2Vector(&camera->position, camPos);
    Vec3ToPS2Vector(&camera->rotation, camRot);
    create_world_view(output, camPos, camRot);
}

void SetCameraPosition(Camera *camera, const MyVector3 *position)
{
    camera->position = *position;
}

void SetCameraRotation(Camera *camera, const MyVector3 *rotation)
{
    camera->rotation = *rotation;
}

MyVector3 GetCameraPosition(Camera *camera)
{
    return camera->position;
}

MyVector3 GetCameraRotation(Camera *camera)
{
    return camera->rotation;
}

void SetCameraAspectRatio(Camera *camera, float aspectRatio)
{
    camera->aspectRatio = aspectRatio;
}

void SetCameraRect(Camera *camera, const MyVector4 *viewRect)
{
    camera->viewRect = *viewRect;
}

void SetCameraNear(Camera *camera, float near)
{
    camera->near = near;
}

void SetCameraFar(Camera *camera, float far)
{
    camera->far = far;
}

