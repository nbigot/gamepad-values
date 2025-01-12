#include "gamepad.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <XInput.h>


// https://hardwaretester.com/gamepad
// https://en.wikipedia.org/wiki/Xbox_Controller
// https://en.wikipedia.org/wiki/Xbox_360_controller
// https://en.wikipedia.org/wiki/Xbox_One_Controller
// https://www.youtube.com/watch?v=e0s_UuZnt9s
// https://www.youtube.com/watch?v=ZPLkVEYgIJs


float sigmoid(float x, float b = 4.0f) {
    // Sigmoid function having a output range of 0.0 to 1.0
    // https://stats.stackexchange.com/questions/214877/is-there-a-formula-for-an-s-shaped-curve-with-domain-and-range-0-1
    if (x <= 0.0f) return 0.0f;
    if (x >= 1.0f) return 1.0f;
    return 1.0f / (1.0f + std::pow((x / (1.0f - x)), -b));
}

float normalize(float input, float min, float max)
{
    float average = (min + max) / 2;
    float range = (max - min) / 2;
    return (input - average) / range;
}

float applyResponseCurveOnControllerStick(SHORT value, float deadZone = 0.001f)
{
    // convert the short type value into a float with a range of -1.0 to 1.0 (linear interpolation)
    float fValue = normalize(static_cast<float>(value), -32767, 32767);

    // the output value range is [-1.0 to 1.0]
    // /!\ beware array 0 the value is approximative due to analog stick drift
    // you may want to use a dead zone to prevent the analog stick value from drifting
    // you can simply for value to 0 when the values is in range [-0.1 to 0.1]
    if (fValue >= 0.0f) {
        // analog stick toward the right side
        fValue = sigmoid(fValue);
        if (fValue < deadZone) {
            return 0.0f;
        }

        return fValue;
    }

    fValue = -sigmoid(-fValue);
    if (fValue > -deadZone) {
        return 0.0f;
    }

    return fValue;
}

Gamepad::Gamepad(DWORD controllerID) {
    this->controllerID = controllerID;
    this->lastUpdateStatus = ERROR_DEVICE_NOT_CONNECTED;
    this->bLX = 0;
    this->bLY = 0;
    this->bRX = 0;
    this->bRY = 0;
    this->stickDeadZone = 0.0f;
    ZeroMemory(&this->state, sizeof(XINPUT_STATE));
    ZeroMemory(&this->vibration, sizeof(XINPUT_VIBRATION));
}

bool Gamepad::initialize(float stickDeadZone) {
    ZeroMemory(&this->state, sizeof(XINPUT_STATE));
    this->lastUpdateStatus = XInputGetState(this->controllerID, &this->state);
    ZeroMemory(&this->vibration, sizeof(XINPUT_VIBRATION));
    this->bLX = 0;
    this->bLY = 0;
    this->bRX = 0;
    this->bRY = 0;
    this->stickDeadZone = stickDeadZone;

    if (!this->isConnected()) {
        // Gamepad is not connected
        return false;
    }

    // stop vibration
    XInputSetState(this->controllerID, &this->vibration);
    return true;
}

bool Gamepad::isConnected() {
    // Check if a gamepad is connected
    if (this->lastUpdateStatus == ERROR_SUCCESS) {
        // Gamepad is connected
        return true;
    } else {
        // Gamepad is not connected
        return false;
    }
}

void Gamepad::update() {
    this->lastUpdateStatus = XInputGetState(this->controllerID, &this->state);

    // Compute the left & right thumb stick values
    // https://learn.microsoft.com/fr-fr/windows/win32/xinput/getting-started-with-xinput
    // A dead zone is used to prevent the thumb stick from drifting (moving without user input)
    // and to prevent the thumb stick from being too sensitive. (it's analog and not very precise).
    // That why we convert the thumb stick original values to a byte value that has 4 possible values (range).
    // The thumb stick values are in the range of -32768 to 32767.

    // Iy you need you can update values to use 2 dead zones for the thumb sticks:
    // internal dead zone (inner circle/inner ring): every value below this value is considered as 0
    // external dead zone (outer circle/outer ring): every value above this value is considered as 1
    // example:
    // 0% - 20%: 0
    // 20% - 80%: sigmoid
    // 80% - 100%: 1

    this->bLX = applyResponseCurveOnControllerStick(this->state.Gamepad.sThumbLX);
    this->bLY = applyResponseCurveOnControllerStick(this->state.Gamepad.sThumbLY);
    this->bRX = applyResponseCurveOnControllerStick(this->state.Gamepad.sThumbRX);
    this->bRY = applyResponseCurveOnControllerStick(this->state.Gamepad.sThumbRY);
}

bool Gamepad::hasExitState() {
    // Check if the gamepad has an exit state.
    // The gamepad is considered to have an exit state
    // when both back & start buttons are pressed.
    if (this->state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK &&
        this->state.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
        return true;
    }

    return false;
}

void Gamepad::vibrate()
{
    if (this->vibration.wLeftMotorSpeed != 0 || this->vibration.wRightMotorSpeed != 0) {
        return;
    }
    this->vibration.wLeftMotorSpeed = 32768;
    this->vibration.wRightMotorSpeed = 16384;
    XInputSetState(this->controllerID, &this->vibration);
}

void Gamepad::resetVibration()
{
    if (this->vibration.wLeftMotorSpeed == 0 && this->vibration.wRightMotorSpeed == 0) {
        return;
    }
    this->vibration.wLeftMotorSpeed = 0;
    this->vibration.wRightMotorSpeed = 0;
    XInputSetState(this->controllerID, &this->vibration);
}


bool Gamepad::isButtonPressed(int button) {
    // Check if a button is pressed
    if (this->state.Gamepad.wButtons & button) {
        return true;
    }

    return false;
}

bool Gamepad::isButtonAPressed() {
    // Check if the A button is pressed
    return this->isButtonPressed(XINPUT_GAMEPAD_A);
}

void Gamepad::printState() {
    std::cout << "lastUpdateStatus: 0x" << std::hex << this->lastUpdateStatus
              << " UP: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? 1 : 0)
              << " DOWN: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? 1 : 0)
              << " LEFT: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? 1 : 0)
              << " RIGHT: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 0)
              << " START: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_START) ? 1 : 0)
              << " BACK: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? 1 : 0)
              << " LEFT_TH: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? 1 : 0)
              << " RIGHT_TH: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? 1 : 0)
              << " LEFT_SH: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1 : 0)
              << " RIGHT_SH: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1 : 0)
              << " Btn A: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? 1 : 0)
              << " Btn B: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? 1 : 0)
              << " Btn X: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? 1 : 0)
              << " Btn Y: " << ((this->state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? 1 : 0)
              << " LT: 0x" << std::hex << static_cast<unsigned int>(this->state.Gamepad.bLeftTrigger)
              << " RT: 0x" << std::hex << static_cast<unsigned int>(this->state.Gamepad.bRightTrigger)
              << std::setw(5) << std::setfill('0') 
              << " LX: " << static_cast<int>(this->state.Gamepad.sThumbLX)
              << " LY: " << static_cast<int>(this->state.Gamepad.sThumbLY)
              << " RX: " << static_cast<int>(this->state.Gamepad.sThumbRX)
              << " RY: " << static_cast<int>(this->state.Gamepad.sThumbRY)
              << std::fixed << std::setprecision(3) << std::showpos
              << " LXn: " << this->bLX
              << " LYn: " << this->bLY
              << " RXn: " << this->bRX
              << " RYn: " << this->bRY
              << std::endl;
}
