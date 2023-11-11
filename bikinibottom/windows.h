// screen.cpp
#include "screen.h"

Screen::Screen(int width, int height) : Width(width), Height(height) {
    // Initialize the buffer with spaces
    Buffer.resize(Height, std::vector<char>(Width, ' '));
}

Screen::~Screen() {
    // Destructor - clean up resources if needed
}

void Screen::SetChar(int x, int y, char character) {
    if (x >= 0 && x < Width && y >= 0 && y < Height) {
        Buffer[y][x] = character;
    }
}

//char Screen::GetChar(int x, int y) const {
//    if (x >= 0 && x < Width && y >= 0 && y < Height) {
//        return Buffer[y][x];
//    }
//    return ' ';  // Return space for out-of-bounds access
//}

void Screen::ClearScreen() {
    // Clear the buffer with spaces
    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < Width; ++j) {
            Buffer[i][j] = ' ';
        }
    }
}

//int Screen::GetWidth() const {
//    return Width;
//}
//
//int Screen::GetHeight() const {
//    return Height;
//}
