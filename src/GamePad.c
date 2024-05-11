/// Modified from: github.com/phy1um/ps2-homebrew-livestreams/src/padlua.c in branch: tcp-experiment.

#include <libpad.h>
#include <loadfile.h>
#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include "../include/GamePad.h"

#define R_SIO2MAN "rom0:SIO2MAN"
#define R_PADMAN "rom0:PADMAN"

char *padBuffer;
struct padButtonStatus *padButtonStatus;
bool *btn_held;
int joysticks[GAMEPAD_AXIS_MAX];

bool InitPad()
{
    btn_held = memalign(256, GAMEPAD_MAX * sizeof(bool));
    padBuffer = memalign(256, 256);
    padButtonStatus = memalign(128, sizeof(struct padButtonStatus));

    int rc = SifLoadModule(R_SIO2MAN, 0, 0);
    if (!rc)
    {
        printf("Failed to load SIF: %s", R_SIO2MAN);
        return false;
    }
    rc = SifLoadModule(R_PADMAN, 0, 0);
    if (!rc)
    {
        printf("Failed to load SIF: %s", R_PADMAN);
        return false;
    }

    int padSetupStat;
    if ((padSetupStat = padInit(0)) != 1)
    {
        printf("Pad initialization failed! Status: %d", padSetupStat);
        return false;
    }

    if ((padSetupStat = padPortOpen(0, 0, padBuffer)) == 0)
    {
        printf("Pad open failed! Status: %d", padSetupStat);
        return false;
    }

    padSetMainMode(0, 0, 1, 3);

    int busy_loops = 100;
    while (busy_loops > 0)
    {
        int32_t state = padGetState(0, 0);
        if (state == PAD_STATE_STABLE || state == PAD_STATE_FINDCTP1)
        {
            int modes = padInfoMode(0, 0, PAD_MODETABLE, -1);
            for (int i = 0; i < modes; i++)
            {
                if (padInfoMode(0, 0, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
                {
                    printf("found dualshock controller in 0:0\n");
                }
            }
            padSetMainMode(0, 0, 1, 3);
            while (padGetReqState(0, 0) != PAD_RSTAT_COMPLETE)
            {
            }
            while (padGetState(0, 0) != PAD_STATE_STABLE)
            {
            }
            return true;
        }
        busy_loops -= 1;
    }

    return true;
}

void EndPad()
{
    padPortClose(0, 0);
    padEnd();
    free(padBuffer);
    free(padButtonStatus);
    free(btn_held);
}

bool PadWait(int port, int slot, int tries)
{
    int status = padGetState(port, slot);
    if (status == PAD_STATE_DISCONN)
        return false;

    while (status != PAD_STATE_STABLE)
    {
        status = padGetState(port, slot);
        tries--;
        if (tries == 0)
            return false;
    }

    return true;
}

#define TEST_BTN(EXPECTED, BUTTON, BUTTON_VALUE) if((EXPECTED) & (BUTTON)) { btn_held[BUTTON_VALUE] = true; }

void PollPad()
{
    if (!PadWait(0, 0, 10))
        return;

    for (int i = 0; i < GAMEPAD_MAX; i++)
        btn_held[i] = false;

    if (padRead(0, 0, padButtonStatus) != 0)
    {
        int pad = 0xffff ^ padButtonStatus->btns;

        TEST_BTN(pad, PAD_UP, GAMEPAD_UP);
        TEST_BTN(pad, PAD_RIGHT, GAMEPAD_RIGHT);
        TEST_BTN(pad, PAD_DOWN, GAMEPAD_DOWN);
        TEST_BTN(pad, PAD_LEFT, GAMEPAD_LEFT);
        TEST_BTN(pad, PAD_TRIANGLE, GAMEPAD_TRIANGLE);
        TEST_BTN(pad, PAD_CIRCLE, GAMEPAD_CIRCLE);
        TEST_BTN(pad, PAD_CROSS, GAMEPAD_CROSS);
        TEST_BTN(pad, PAD_SQUARE, GAMEPAD_SQUARE);
        TEST_BTN(pad, PAD_SELECT, GAMEPAD_SELECT);
        TEST_BTN(pad, PAD_START, GAMEPAD_START);
        TEST_BTN(pad, PAD_L1, GAMEPAD_L1);
        TEST_BTN(pad, PAD_L2, GAMEPAD_L2);
        TEST_BTN(pad, PAD_L3, GAMEPAD_L3);
        TEST_BTN(pad, PAD_R1, GAMEPAD_R1);
        TEST_BTN(pad, PAD_R2, GAMEPAD_R2);
        TEST_BTN(pad, PAD_R3, GAMEPAD_R3);

        joysticks[GAMEPAD_AXIS_LEFT_X] = (int) padButtonStatus->ljoy_h;
        joysticks[GAMEPAD_AXIS_LEFT_Y] = (int) padButtonStatus->ljoy_v;
        joysticks[GAMEPAD_AXIS_RIGHT_X] = (int) padButtonStatus->rjoy_h;
        joysticks[GAMEPAD_AXIS_RIGHT_Y] = (int) padButtonStatus->rjoy_v;
    }
}

bool IsKeyPressedGamePad(GamePadButtons button)
{
    return btn_held[button];
}

MyVector2 GetJoystickPositionGamePad(GamePadJoystick joystick)
{
    //TODO: Add dead zone.
    if(joystick == GAMEPAD_JOYSTICK_LEFT) {
        return (MyVector2) {(float)joysticks[GAMEPAD_AXIS_LEFT_X], (float)joysticks[GAMEPAD_AXIS_LEFT_X]};
    }

    return (MyVector2) {(float)joysticks[GAMEPAD_AXIS_RIGHT_X], (float)joysticks[GAMEPAD_AXIS_RIGHT_X]};
}

