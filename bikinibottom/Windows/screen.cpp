#include <iostream>
#include "windows_mode.h"

int TransformCoord(int x, int y, int numCols) {
    return y * numCols + x;
}

Screen::Screen() {
    // Initialize the buffer with spaces
    Buffer.resize((20 + 1) * 9, ' ');

    for (int i = 0; i < 9; i++) {
        Buffer[TransformCoord(20, i, 20 + 1)] = '\n';
    }

    // Obtain the standard input handle
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle");

    // Save the current input mode, to be restored on exit. 
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
        ErrorExit("GetConsoleMode");

    ShowConsoleCursor(false);

    // Enable the window and mouse input events. 
    DWORD fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS;
    if (!SetConsoleMode(hStdin, fdwMode))
        ErrorExit("SetConsoleMode");
}


Screen::~Screen() {
    // Destructor - clean up resources if needed
}

void Screen::StartBackgroundInputThread() {
    inputThread = std::thread(&Screen::BackgroundInputThread, this);
}

void Screen::StopBackgroundInputThread() {
    stopThread.store(true);
    if (inputThread.joinable()) {
        inputThread.join();
    }
}
void Screen::BackgroundInputThread() {
    while (!stopThread.load()) {
        DWORD cNumRead;
        INPUT_RECORD irInBuf[128];

        if (ReadConsoleInput(irInBuf, 128, cNumRead)) {
            for (DWORD i = 0; i < cNumRead; i++) {
                switch (irInBuf[i].EventType) {
                case KEY_EVENT:
                    if (irInBuf[i].Event.KeyEvent.bKeyDown) {
                        // Process key press event
                        std::cout << "Key pressed: " << irInBuf[i].Event.KeyEvent.uChar.AsciiChar << std::endl;
                        // Update Screen's internal buffer as needed
                    }
                    break;

                case MOUSE_EVENT:
                    // Process mouse event
                    ProcessMouseEvent(irInBuf[i].Event.MouseEvent);
                    // Update Screen's internal buffer as needed
                    break;

                    // Handle other event types if needed

                default:
                    break;
                }
            }
        }
    }
}

void Screen::ProcessMouseEvent(MOUSE_EVENT_RECORD mer) {
    // Process mouse events based on mer structure
    // Update the Screen's internal buffer as needed

    // Example: Print mouse position and button state
    std::cout << "Mouse event at (" << mer.dwMousePosition.X << ", " << mer.dwMousePosition.Y << "): ";
    if (mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
        std::cout << "Left button pressed ";
    }
    if (mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
        std::cout << "Right button pressed ";
    }
    std::cout << std::endl;
}


void Screen::SetCustomConsoleIcon(const char* iconFilePath) {
    HICON hIcon = static_cast<HICON>(LoadImageA(nullptr, iconFilePath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE));

    if (hIcon != nullptr) {
        // Set the console window icon
        SendMessage(GetConsoleWindow(), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));

        // Free the icon resource
        DestroyIcon(hIcon);
    }
    else {
        std::cerr << "Failed to load the icon." << std::endl;
    }
}

void Screen::SetTitle(const char* title) {
    SetConsoleTitleA(title);
}

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

void Screen::Start() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    COORD newSize;
    newSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    newSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), newSize);

    // Get the current window style
    LONG style = GetWindowLong(hWindow, GWL_STYLE);

    // Disable resizing by removing WS_THICKFRAME and WS_MAXIMIZEBOX styles
    style &= ~WS_THICKFRAME;
    style &= ~WS_MAXIMIZEBOX;

    // Apply the modified style
    SetWindowLong(hWindow, GWL_STYLE, style);

    // Set the console control handler
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {

        /*std::string msg = "Press Ctrl+C or Ctrl+Break to trigger console events.";

        MoveCursor(newSize.X / 2 - msg.length() / 2, newSize.Y / 2 - 2);

        std::cout << msg;*/
    }
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
    SetConsoleCursorPosition(hConsole, coord);
}

void Screen::Update() {
    MoveCursor(0, 0);

    std::cout << Buffer;
}
bool Screen::ReadConsoleInput(INPUT_RECORD* irInBuf, DWORD nNumInputs, DWORD& cNumRead)
{
    return ::ReadConsoleInput(hStdin, irInBuf, nNumInputs, &cNumRead);
}

void Screen::ErrorExit(LPCSTR lpszMessage)
{
    DWORD error = GetLastError();  // Get the last error code
    std::cerr << lpszMessage << " (Error code: " << error << ")" << std::endl;

    // Restore input mode on exit.
    SetConsoleMode(hConsole, fdwSaveOldMode);

    ExitProcess(error);
}

void Screen::KeyEventProc(KEY_EVENT_RECORD ker)
{
    MoveCursor(0, 0);

    std::cout << "Key event: ";

    if (ker.bKeyDown)
        std::cout << "key pressed\n";
    else
        std::cout << "key released\n";
}

void Screen::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
    MoveCursor(0, 0);

    printf("Mouse event: ");

    switch (mer.dwEventFlags)
    {
    case 0:
        if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            printf("left button press at (%d, %d)\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
        }
        else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
        {
            printf("right button press at (%d, %d)\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
        }
        else
        {
            printf("button press at (%d, %d)\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
        }
        break;

    case DOUBLE_CLICK:
        printf("double click at (%d, %d)\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
        break;

    case MOUSE_HWHEELED:
        printf("horizontal mouse wheel\n");
        break;

    case MOUSE_MOVED:
        printf("mouse moved to (%d, %d)\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
        break;

    case MOUSE_WHEELED:
        printf("vertical mouse wheel\n");
        break;

    default:
        printf("unknown\n");
        break;
    }
}

void Screen::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
    MoveCursor(0, 0);

    std::cout << "Resize event\n";
    std::cout << "Console screen buffer is " << wbsr.dwSize.X << " columns by " << wbsr.dwSize.Y << " rows.\n";
}


void Screen::ClearScreen() {
    // Clear the buffer with spaces
    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < Width; ++j) {
            Buffer[i * Width + j] = ' ';
        }
    }
}

Screen* CreateScreen() {
    return new Screen();
}
