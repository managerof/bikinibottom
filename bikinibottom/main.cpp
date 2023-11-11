//#include <iostream>
//#include <Windows.h>
//#include <string>
//
//const int CONSOLE_WIDTH = 80;
//const int CONSOLE_HEIGHT = 30;
//
//// Function to show/hide the console cursor
//void ShowConsoleCursor(bool showFlag) {
//    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
//
//    CONSOLE_CURSOR_INFO cursorInfo;
//    GetConsoleCursorInfo(out, &cursorInfo);
//    cursorInfo.bVisible = showFlag; // Set the cursor visibility
//    SetConsoleCursorInfo(out, &cursorInfo);
//}
//
//void gotoxy(int x, int y) {
//    COORD coord = { x, y };
//    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
//}
//// Store the original cursor position
//COORD originalCursorPosition;
//
//// Create a buffer to store console data
//std::string consoleBuffer[CONSOLE_HEIGHT];
//
//// Function to clear and update the console buffer
//void clearConsoleBuffer() {
//    for (int i = 0; i < CONSOLE_HEIGHT; ++i) {
//        consoleBuffer[i] = std::string(CONSOLE_WIDTH, ' ') + '\n';  // Initialize buffer with spaces and a newline character
//    }
//}
//
//// Function to render the console buffer to the console
//void renderConsoleBuffer() {
//    // Set cursor position to the top-left corner
//    gotoxy(0, 0);
//
//    // Output the entire console buffer
//    for (int i = 0; i < CONSOLE_HEIGHT; ++i) {
//        std::cout << consoleBuffer[i];
//    }
//}
//
//
//int main() {
//    // Set console size using constants
//    std::string consoleSizeCommand = "mode con cols=" + std::to_string(CONSOLE_WIDTH) + " lines=" + std::to_string(CONSOLE_HEIGHT);
//    system(consoleSizeCommand.c_str());
//
//    // Store the original cursor position
//    CONSOLE_SCREEN_BUFFER_INFO csbi;
//    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
//    originalCursorPosition = csbi.dwCursorPosition;
//
//    ShowConsoleCursor(false);
//
//    // Main loop
//    for (int frame = 0; frame < 1000; ++frame) {
//        // Clear and update the console buffer
//        clearConsoleBuffer();
//
//        // Example: Write symbols to the console buffer
//        consoleBuffer[5][10] = '*';
//        consoleBuffer[10][20] = '#';
//
//        // Render the console buffer
//        renderConsoleBuffer();
//    }
//
//    // Restoring cursor position
//    gotoxy(originalCursorPosition.X, originalCursorPosition.Y);
//
//    return 0;
//}

#include "screen.h"

#define PLATFORM_WINDOWS

int main() {
	Screen CreateScreen(20, 9);

	return 0;
}