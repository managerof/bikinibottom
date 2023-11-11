#pragma once
#include <vector>

#ifndef SCREEN_H
#define SCREEN_H

// Forward declaration of the Screen class
class Screen {
public:
	Screen(int width, int height);
	~Screen();

	void SetChar(int x, int y, char character);
	void ClearScreen();

private:
	int Width;
	int Height;
	std::vector<std::vector<char>> Buffer;
};

#endif // SCREEN_H
