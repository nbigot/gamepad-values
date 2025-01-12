#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <windows.h>
#include <XInput.h>


class Gamepad {
public:
    Gamepad(DWORD controllerID = 0);
    bool initialize(float stickDeadZone = 0.001f);
    bool isConnected();
    bool hasExitState();
    bool isButtonAPressed();
    bool isButtonPressed(int button);
    void update();
    void vibrate();
    void resetVibration();
    void printState();

private:
    // state variables
    DWORD controllerID;
    DWORD lastUpdateStatus;
    XINPUT_STATE state;
    XINPUT_VIBRATION vibration;
    float bLX;
    float bLY;
    float bRX;
    float bRY;

    // config variables
    float stickDeadZone;
};

#endif // GAMEPAD_H
