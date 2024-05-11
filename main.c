#include <kernel.h>

#include <math3d.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>

#include "external/gsKit/include/gsKit.h"
#include "external/gsKit/include/dmaKit.h"

#include "include/MeshData.h"
#include "include/MyTypes.h"
#include "include/Camera.h"
#include "include/Mesh.h"
#include "include/GamePad.h"

#define MAX_MOVEMENT_WAIT_TIME 32

GSGLOBAL *settings = NULL;

void InitScreenSettings()
{
    settings = gsKit_init_global();

    // We turn on the ZBuffer with resolution of 24. And we set the window offset to 0.
    settings->PSMZ = GS_PSMZ_24;
    settings->ZBuffering = GS_SETTING_ON;
    settings->OffsetX = 0;
    settings->OffsetY = 0;

    printf("PSMZ = %d, Screen Size = (%d, %d), Offset = (%d, %d)\n", settings->PSMZ, settings->Width, settings->Height,
           settings->OffsetX, settings->OffsetY);

    settings->PrimAlphaEnable = GS_SETTING_ON;
    settings->PrimAAEnable = GS_SETTING_ON; // Turn on Anti Aliasing.

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gsKit_set_primalpha(settings, GS_SETREG_ALPHA(0, 1, 0, 1, 0), 0);

    gsKit_set_clamp(settings, GS_CMODE_REPEAT);
    gsKit_vram_clear(settings);
    gsKit_init_screen(settings);
    gsKit_mode_switch(settings, GS_ONESHOT);
}

void FlipScreen(GSGLOBAL *gsGlobal)
{
    gsKit_queue_exec(gsGlobal);
    gsKit_sync_flip(gsGlobal);
}

void Render(const Camera *camera, Mesh *mesh)
{
    // Matrices to set up the 3D environment and camera
    MATRIX modelMatrix;
    MATRIX projMatrix;
    MATRIX viewMatrix;
    MATRIX modelViewProjMatrix;

    // Create the viewMatrix matrix.
    CreateViewMatrix(viewMatrix, camera);

    // Create the modelMatrix matrix.
    CreateMeshTransformMatrix(modelMatrix, mesh);

    // Create the projMatrix matrix.
    CreateProjMatrix(projMatrix, camera);

    // Create the modelViewProjMatrix matrix.
    create_local_screen(modelViewProjMatrix, modelMatrix, projMatrix, viewMatrix);

    // Finally draw the mesh.
    DrawMesh(mesh, settings, modelViewProjMatrix);
}

bool PadUpdate(Camera *camera)
{
    MyVector3 pos;

    PollPad();
    if (IsKeyPressedGamePad(GAMEPAD_START))
    {
        printf("Exiting...");
        return false;
    }

    if (IsKeyPressedGamePad(GAMEPAD_LEFT))
    {
        pos = GetCameraPosition(camera);
        pos.x -= 0.5f;
        SetCameraPosition(camera, &pos);
    }

    if (IsKeyPressedGamePad(GAMEPAD_RIGHT))
    {
        pos = GetCameraPosition(camera);
        pos.x += 0.5f;
        SetCameraPosition(camera, &pos);
    }

    if (IsKeyPressedGamePad(GAMEPAD_UP))
    {
        pos = GetCameraPosition(camera);
        pos.y -= 0.5f;
        SetCameraPosition(camera, &pos);
    }

    if (IsKeyPressedGamePad(GAMEPAD_DOWN))
    {
        pos = GetCameraPosition(camera);
        pos.y += 0.5f;
        SetCameraPosition(camera, &pos);
    }

    return true;
}

int main(void)
{
    const MyVector4 viewRect = {-0.5f, 0.5f, -0.5f, 0.5f};
    const MyVector3 camera_position = {0.00f, 0.00f, 100.00f};
    const MyVector3 camera_rotation = {0.00f, 0.00f, 0.00f};
    const MyVector3 rotationForce = {0.008f, 0.012f, 0.016f};
    const MyColor clearColor = CreateColorRGBA(0x00, 0x00, 0x3f, 0x80);

    int movementWaitTime = 0;

    InitScreenSettings();

    // Set up the camera
    Camera *cam = CreateCamera(4.0f / 3.0f, &viewRect, 1.00f, 2000.00f);
    SetCameraPosition(cam, &camera_position);
    SetCameraRotation(cam, &camera_rotation);

    // Load the mesh from 'MeshData.h'
    Mesh cubeMesh = CreateMeshFromBuffers(points, points_count, vertices,
                                          vertex_count, colours, vertex_count);
    cubeMesh.position = (MyVector3) {0, 0, 0};
    cubeMesh.rotation = (MyVector3) {0.00f, 105.00f, 0.00f};

    if (settings->ZBuffering == GS_SETTING_ON)
        gsKit_set_test(settings, GS_ZTEST_ON);

    srand(time(NULL));

    // Initialize the game pad, so we can check for user input.
    if (!InitPad())
    {
        printf("Couldn't initialize game pad!");
        goto end;
    }

    while (1)
    {
        if (!PadUpdate(cam)) // false is returned only if the user has pressed `start` -- then we exit.
            break;

        // Spin the cube a bit.
        AddMeshRotation(&cubeMesh, &rotationForce);

        // Check if the next movement is now, we move the cube randomly in space.
        if (movementWaitTime == 0)
        {
            cubeMesh.position = (MyVector3) {(float) ((float) rand() / (float)RAND_MAX) + rand() % 32,
                                             (float) ((float) rand() / (float)RAND_MAX) + rand() % 32,
                                             0.0f};
            printf("Position: %f, %f, %f\n", cubeMesh.position.x, cubeMesh.position.y, cubeMesh.position.z);
        }

        if (movementWaitTime >= MAX_MOVEMENT_WAIT_TIME)
            movementWaitTime = -1;

        movementWaitTime++;

        gsKit_clear(settings, clearColor);
        Render(cam, &cubeMesh);
        FlipScreen(settings);
    }

    EndPad();
    end:
    free(cam);
    DeleteMesh(&cubeMesh);
    return 0;
}
