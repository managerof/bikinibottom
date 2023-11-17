#include <iostream>
#include "windows_mode.h"

int TransformCoord(int x, int y, int numCols) {
    return y * numCols + x;
}

Screen::Screen() : Buffer((DEFAULT_SCREEN_BUFFER_WIDTH+1) * DEFAULT_SCREEN_BUFFER_HEIGHT, ' ') {
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

    // wives are for breeding

    COORD bufferSize = { DEFAULT_SCREEN_BUFFER_WIDTH, DEFAULT_SCREEN_BUFFER_HEIGHT };  // Set the desired buffer size
    if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) {
        std::cerr << "Error setting console buffer size" << std::endl;
    }

    //Buffer.resize((Width + 1) * Height, ' ');
    //Buffer.reserve((Width + 1) * Height);
    // 
    // Populate the buffer with newline characters at appropriate positions
    for (int i = 0; i < DEFAULT_SCREEN_BUFFER_HEIGHT; i++) {
        //Buffer[(DEFAULT_SCREEN_BUFFER_WIDTH + 1) * i + DEFAULT_SCREEN_BUFFER_WIDTH] = '\n';
    }

    Width = DEFAULT_SCREEN_BUFFER_WIDTH-1;
    Height = DEFAULT_SCREEN_BUFFER_HEIGHT;

    BufferCapacity = Buffer.length();
}

void Screen::ResizeBuffer(int width, int height)
{
    Buffer.resize((width + 1) * height, ' ');

    for (int i = 0; i < height; i++) {
        //Buffer[TransformCoord(width, i, width + 1)] = '\n';
    }
}

Screen::~Screen() {
    // Destructor - clean up resources if needed
}

int Screen::ToBufferCoords(int X, int Y) {
    return Y * (Width+1) + X;
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

void Screen::DrawEntity(const std::string& entity, int X, int Y) {
    if (BufferCapacity < entity.length()) {
        std::cerr << "Entity length is greater than the buffer length.\n";
        return;
    }

    int row = 0;
    int column = 0;

    for (char ch : entity) {
        if (ch == '\n') {
            row++;
            column = 0;  // Reset column position for the next row
            continue;
        }

        if (X + column >= Width || Y + row >= Height) {
            // Skip drawing if it goes beyond the screen boundaries
            column++;
            continue;
        }

        Buffer[ToBufferCoords(X + column, Y + row)] = ch;
        column++;
    }
}


void Screen::PaintString(int x, int y, const std::string& text, WORD textColor, WORD backgroundColor) {
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, coord);

    WORD attributes = (backgroundColor << 4) | textColor | 0x2;
    SetConsoleTextAttribute(hConsole, attributes);
    
    std::cout << text;

    // Reset text attribute to default
    SetConsoleTextAttribute(hConsole, originalAttributes);
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
                        //std::cout << "Key pressed: " << irInBuf[i].Event.KeyEvent.uChar.AsciiChar << std::endl;
                    }
                    break;

                case MOUSE_EVENT:
                    //ProcessMouseEvent(irInBuf[i].Event.MouseEvent);
                    break;

                    // Handle other event types
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
        Buffer[TransformCoord(x, y, Width)] = character;
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
    DWORD lpNumberOfCharsWritten;
        
    WriteConsoleOutputCharacterA(hConsole, Buffer.c_str(), static_cast<DWORD>(Buffer.length()), { 0, 0 }, &lpNumberOfCharsWritten);
}

void Screen::ResizeConsoleBuffer(int columns, int rows) {
    COORD newSize = { static_cast<SHORT>(columns), static_cast<SHORT>(rows) };
    SetConsoleScreenBufferSize(hConsole, newSize);

    SMALL_RECT sr = { 0, 0, static_cast<SHORT>(columns - 1), static_cast<SHORT>(rows - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &sr);
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
    //std::cout << "Key event: ";

    if (ker.bKeyDown)
    {
        //std::cout << "key pressed\n"
    }
    else {
        //std::cout << "key released\n"
    }
}

void Screen::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
    //printf("Mouse event: ");

    switch (mer.dwEventFlags)
    {
    case 0:
        if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            //printf("left button press at (%d, %d)\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
        }
        else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
        {
            //printf("right button press at (%d, %d)\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
        }
        else
        {
            //printf("button press at (%d, %d)\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
        }
        break;

    case DOUBLE_CLICK:
        //printf("double click at (%d, %d)\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
        break;

    case MOUSE_HWHEELED:
        //printf("horizontal mouse wheel\n");
        break;

    case MOUSE_MOVED:
        //printf("mouse moved to (%d, %d)\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
        break;

    case MOUSE_WHEELED:
        //printf("vertical mouse wheel\n");
        break;

    default:
        //printf("unknown\n");
        break;
    }
}

void Screen::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
    if (wbsr.dwSize.X != Width || wbsr.dwSize.Y != Height) {
        ResizeConsoleBuffer(wbsr.dwSize.X, wbsr.dwSize.Y);
        Width = wbsr.dwSize.X;
        Height = wbsr.dwSize.Y;
    }
    std::cout << "Resize event\n";
    //std::cout << "Console screen buffer is " << wbsr.dwSize.X << " columns by " << wbsr.dwSize.Y << " rows.\n";
}


void Screen::ClearBuffer() {
    // Clear the buffer with spaces
    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < Width; ++j) {
            Buffer[ToBufferCoords(j, i)] = ' ';
        }
    }
}

Screen* CreateScreen() {
    return new Screen();
}
