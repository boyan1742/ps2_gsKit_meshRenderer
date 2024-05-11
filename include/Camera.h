#ifndef GSKIT_MESH_CAMERA_H
#define GSKIT_MESH_CAMERA_H

#include "MyTypes.h"

typedef struct Camera Camera;

Camera* CreateCamera(float aspectRatio, const MyVector4 *viewRect, float near, float far); // Creates a camera.

void CreateViewMatrix(PS2_MATRIX output, const Camera *camera); // Creates a view matrix from a camera.
void CreateProjMatrix(PS2_MATRIX output, const Camera *camera); // Creates a projection matrix from a camera.

void SetCameraPosition(Camera* camera, const MyVector3* position); // Sets a camera's position.
void SetCameraRotation(Camera* camera, const MyVector3* rotation); // Sets a camera's rotation.

MyVector3 GetCameraPosition(Camera* camera); // Gets a camera's position.
MyVector3 GetCameraRotation(Camera* camera); // Gets a camera's rotation.

void SetCameraAspectRatio(Camera* camera, float aspectRatio); // Sets a camera's aspect ratio.
void SetCameraRect(Camera* camera, const MyVector4* viewRect); // Sets a camera's view rectangle.
void SetCameraNear(Camera* camera, float near); // Sets a camera's near plane.
void SetCameraFar(Camera* camera, float far); // Sets a camera's far plane.

#endif
