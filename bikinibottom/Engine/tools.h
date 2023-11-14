#pragma once

#include <array>
#include <string>

#ifndef TOOLS_H
#define TOOLS_H

struct Vec2 {
	Vec2(double x, double y) : X(x), Y(y) {};

	double X;
	double Y;
};

struct Vec2Int {
	Vec2Int(int x, int y) : X(x), Y(y) {};

	int X;
	int Y;
};

#endif // TOOLS_H