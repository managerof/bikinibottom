#ifndef MY_CROSS_PLATFORM_HEADER_H
#define MY_CROSS_PLATFORM_HEADER_H

// Common code that is platform-independent goes here

#ifdef _WIN32 // Windows specific code
#define PLATFORM_WINDOWS
// Include Windows-specific headers or define platform-specific macros
#include <Windows.h>
#elif __linux__ // Linux specific code
#define PLATFORM_LINUX
// Include Linux-specific headers or define platform-specific macros
#include <unistd.h>
#elif __APPLE__ // macOS specific code
#define PLATFORM_MACOS
// Include macOS-specific headers or define platform-specific macros
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

// Common code that uses platform-specific features goes here

#endif // MY_CROSS_PLATFORM_HEADER_H
