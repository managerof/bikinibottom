#include "screen.h"
#include "Engine/engine.h"
#include <iostream>

int main() {
    Screen* s = CreateScreen();
    s->SetTitle("Bikini Bottom");
    s->SetCustomConsoleIcon("icon.ico");

    s->Start();  // Set up the console and handle events
    s->StartBackgroundInputThread();  // Start background input thread

    //s->ResizeConsoleBuffer(80, 25);

    Player player(Vec2(0, 0), 100);

    Vec2Int playerPos = player.GetIntCoords();

    s->DrawEntity(player.skin, 0, 0);

    std::string penis = { 'O', '\n',
                         ' ', '=', '=', '=', '=', '8', '\n',
                         'O', '\n' };

    s->DrawEntity(penis, 20, 9);

    while (1) {
        s->Update();
        Sleep(50);
    }

    // Clean up
    s->StopBackgroundInputThread();  // Stop background input thread
    delete s; // Release the allocated Screen object

    return 0;
}
