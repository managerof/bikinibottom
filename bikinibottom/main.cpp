#include "screen.h"


int main() {
    Screen* s = CreateScreen();
    s->SetTitle("Bikini Bottom");
    s->SetCustomConsoleIcon("icon.ico");

    s->Start();  // Set up the console and handle events
    s->StartBackgroundInputThread();  // Start background input thread

    while (true) {
        //s->Update();  // Display the Screen buffer
    }

    // Clean up
    s->StopBackgroundInputThread();  // Stop background input thread
    delete s; // Release the allocated Screen object

    return 0;
}
