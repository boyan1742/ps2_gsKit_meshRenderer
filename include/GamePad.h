/// Modified from: github.com/phy1um/ps2-homebrew-livestreams/src/pad.h in branch: tcp-experiment.

#ifndef GSKIT_MESH_GAMEPAD_H
#define GSKIT_MESH_GAMEPAD_H

#include <stdbool.h>
#include "MyTypes.h"

typedef enum GamePadButtons
{
    GAMEPAD_LEFT = 0,
    GAMEPAD_DOWN,
    GAMEPAD_RIGHT,
    GAMEPAD_UP,
    GAMEPAD_START,
    GAMEPAD_R3,
    GAMEPAD_L3,
    GAMEPAD_SELECT,
    GAMEPAD_SQUARE,
    GAMEPAD_CROSS,
    GAMEPAD_CIRCLE,
    GAMEPAD_TRIANGLE,
    GAMEPAD_R1,
    GAMEPAD_L1,
    GAMEPAD_R2,
    GAMEPAD_L2,
    GAMEPAD_MAX
} GamePadButtons;

typedef enum GamePadJoystickAxis
{
    GAMEPAD_AXIS_LEFT_X = 0,
    GAMEPAD_AXIS_LEFT_Y,
    GAMEPAD_AXIS_RIGHT_X,
    GAMEPAD_AXIS_RIGHT_Y,
    GAMEPAD_AXIS_MAX
} GamePadJoystickAxis;

typedef enum GamePadJoystick
{
    GAMEPAD_JOYSTICK_LEFT = 0,
    GAMEPAD_JOYSTICK_RIGHT
} GamePadJoystick;

bool InitPad();
void EndPad();
void PollPad();

bool IsKeyPressedGamePad(GamePadButtons button);
MyVector2 GetJoystickPositionGamePad(GamePadJoystick joystick);

#endif
