#include <iostream>
#include "../screen.h"


int TransformCoord(int x, int y, int numCols) {
    return y * numCols + x;
}

Screen::Screen(int width, int height) : Width(width), Height(height) {
    // Initialize the buffer with spaces
    Buffer.resize((width + 1) * height, ' ');

    for (int i = 0; i < height; i++) {
        Buffer[TransformCoord(width, i, width+1)] = '\n';
    }

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ShowConsoleCursor(false);
}

Screen::~Screen() {
    // Destructor - clean up resources if needed
}

void Screen::SetChar(int x, int y, char character) {
    if (x >= 0 && x < Width && y >= 0 && y < Height) {
        Buffer[TransformCoord(x, y, Width + 1)] = character;
    }
}

void Screen::ShowConsoleCursor(bool showFlag) {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = showFlag; // Set the cursor visibility
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void Screen::MoveCursor(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Screen::Update() {
    MoveCursor(0, 0);

    std::cout << Buffer;
}

void Screen::ClearScreen() {
    // Clear the buffer with spaces
    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < Width; ++j) {
            Buffer[i * Width + j] = ' ';
        }
    }
}

Screen* CreateScreen(int width, int height) {
    return new Screen(width, height);
}
