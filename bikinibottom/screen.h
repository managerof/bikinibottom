#include <vector>
#include "platform.h"
#include <string>
#include <thread>
#include <atomic>

#pragma once

#ifndef SCREEN_H
#define SCREEN_H

// Forward declaration of the Screen class
class Screen {
public:
    int Width;
    int Height;

    Screen();
    ~Screen();

    void SetChar(int x, int y, char character);
    void ClearScreen();
    void Update();
    void SetTitle(const char* title);
    void SetCustomConsoleIcon(const char* iconFilePath);
    void Start();

// WINDOWS specific console handle functions
#ifdef _WIN32
    void ShowConsoleCursor(bool showFlag);
    void MoveCursor(int x, int y);

    bool ReadConsoleInput(INPUT_RECORD* irInBuf, DWORD nNumInputs, DWORD& cNumRead);
    void KeyEventProc(KEY_EVENT_RECORD ker);
    void MouseEventProc(MOUSE_EVENT_RECORD mer);
    void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr);

    void ProcessMouseEvent(MOUSE_EVENT_RECORD mer);

    void ErrorExit(LPCSTR lpszMessage);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdin;
    HWND hWindow = GetConsoleWindow();

    void StartBackgroundInputThread();

    void StopBackgroundInputThread();

    std::atomic<bool> stopThread{false};
    std::thread inputThread;
    void BackgroundInputThread();

    DWORD fdwSaveOldMode;
#endif
    
    std::string Buffer;
};

Screen* CreateScreen();

#endif // SCREEN_H

#ifdef _WIN32
BOOL CtrlHandler(DWORD ctrlType);
#endif