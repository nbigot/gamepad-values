#include <iostream>
#include "gamepad.h"


// Global variable to indicate if Ctrl+C has been received
volatile bool ctrlCReceived = false;

// Control handler function
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
            ctrlCReceived = true;
            return TRUE;
        default:
            return FALSE;
    }
}

int main() {
    // Register the control handler function
    if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
        std::cerr << "Failed to set control handler." << std::endl;
        return -1;
    }

    Gamepad gamepad;

    if (!gamepad.initialize()) {
        std::cerr << "Failed to initialize gamepad." << std::endl;
        return -1;
    }

    while (true) {
        if (ctrlCReceived) {
            std::cout << "Ctrl+C received. Exiting..." << std::endl;
            break;
        }

        gamepad.update();
        gamepad.printState();

        if (gamepad.hasExitState()) {
            std::cout << "Exit state detected. Exiting..." << std::endl;
            break;
        }

        if (gamepad.isButtonAPressed()) {
            gamepad.vibrate();
        } else {
            gamepad.resetVibration();
        }

        Sleep(5);
    }

    return 0;
}