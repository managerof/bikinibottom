#include <iostream>
#include <Windows.h>

#include <iostream>
#include <Windows.h>

BOOL CtrlHandler(DWORD ctrlType) {
    switch (ctrlType) {
    case CTRL_C_EVENT:
        std::cout << "Ctrl+C received. Exiting..." << std::endl;
        return TRUE;

    case CTRL_BREAK_EVENT:
        std::cout << "Ctrl+Break received. Exiting..." << std::endl;
        return TRUE;

    default:
        return FALSE;
    }
}

int main() {
    // Set the console control handler
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
        std::cout << "Press Ctrl+C or Ctrl+Break to trigger console events." << std::endl;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Keep the program running
        while (true) {
            CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
            GetConsoleScreenBufferInfo(hConsole, &bufferInfo);

            COORD newSize = { bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1,
                              bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1 };

            // Print if the console size changes
            static COORD lastSize = newSize;
            if (newSize.X != lastSize.X || newSize.Y != lastSize.Y) {
                std::cout << "Console resized to (" << newSize.X << ", " << newSize.Y << ")." << std::endl;
                lastSize = newSize;
            }

            // Sleep for a short time to avoid high CPU usage
            Sleep(1000);
        }
    }
    else {
        std::cerr << "Error setting console control handler." << std::endl;
        return 1;
    }

    return 0;
}
