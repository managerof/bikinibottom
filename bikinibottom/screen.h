#include <vector>
#include "platform.h"
#include <string>

#pragma once

#ifndef SCREEN_H
#define SCREEN_H

// Forward declaration of the Screen class
class Screen {
public:
    Screen(int width, int height);
    ~Screen();

    void SetChar(int x, int y, char character);
    //void SetBuffer(const std::vector<std::vector<char>>& characters);
    //void SetBuffer(const std::vector<char>& characters);
    void ClearScreen();
    void Update();

    int GetWidth() const {
        return Width;
    }

    int GetHeight() const {
        return Height;
    }

private:
#ifdef _WIN32
    HANDLE hConsole;
    void ShowConsoleCursor(bool showFlag);
    void MoveCursor(int x, int y);
#endif
    
    int Width;
    int Height;
    std::string Buffer;
};

Screen* CreateScreen(int width, int height);

#endif // SCREEN_H





/*
#include <iostream>
#include <Windows.h>

BOOL CtrlHandler(DWORD ctrlType) {
    switch (ctrlType) {
    case CTRL_C_EVENT:
        std::cout << "Ctrl+C received. Exiting..." << std::endl;
        // Handle Ctrl+C event
        return TRUE;

    case CTRL_BREAK_EVENT:
        std::cout << "Ctrl+Break received. Exiting..." << std::endl;
        // Handle Ctrl+Break event
        return TRUE;

    default:
        return FALSE;
    }
}

int main() {
    // Set the console control handler
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
        std::cout << "Press Ctrl+C or Ctrl+Break to trigger console events." << std::endl;

        // Keep the program running
        while (true) {
            // Wait for console input events
            INPUT_RECORD input;
            DWORD events;
            ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &input, 1, &events);

            // Process input events if needed
        }
    }
    else {
        std::cerr << "Error setting console control handler." << std::endl;
        return 1;
    }

    return 0;
}
*/