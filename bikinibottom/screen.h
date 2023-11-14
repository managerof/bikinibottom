#include <vector>
#include "platform.h"
#include <string>
#include <thread>
#include <atomic>
#include "Engine/tools.h"

#pragma once

#ifndef SCREEN_H
#define SCREEN_H

#define DEFAULT_SCREEN_BUFFER_WIDTH 120
#define DEFAULT_SCREEN_BUFFER_HEIGHT 30

// Forward declaration of the Screen class
class Screen {
public:
    int Width;
    int Height;
    int BufferCapacity;

    Screen();
    ~Screen();

    void SetChar(int x, int y, char character);
    void ClearScreen();
    void Update();
    void SetTitle(const char* title);
    void SetCustomConsoleIcon(const char* iconFilePath);
    void Start();
    void ResizeBuffer(int width, int height);
    void DrawEntity(const std::string& entity, int X, int Y);
    Vec2Int GetConsoleBufferSize();
    int ToBufferCoords(int X, int Y);
    void PaintString(int x, int y, const std::string& text, WORD textColor, WORD backgroundColor);

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
    WORD originalAttributes;

    void ResizeConsoleBuffer(int columns, int rows);

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