// screen.cpp
#include "Screen.h"
#include "Platform.h"  // Include the header file that defines the platform macros

#ifdef PLATFORM_WINDOWS
#include "windows.h" // Include Windows-specific implementation

#elif PLATFORM_LINUX
#include "linux.h"   // Include Linux-specific implementation

#elif PLATFORM_MACOS
#include "macos.h"   // Include macOS-specific implementation

#else
#error "Unsupported platform"
#endif
