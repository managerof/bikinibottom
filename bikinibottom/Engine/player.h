#pragma once

#include "tools.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
	Player(Vec2 pos, double health) : Pos(pos), Health(health) {};

	std::string skin = {'0', '_', '0',		'\n',
						' ', '|', ' ',		'\n',
						'/', ' ', '\\',		'\n'};

	Vec2 GetCoords();
	Vec2Int GetIntCoords();
	void Move(Vec2 newPos);

private:
	double Health;
	Vec2 Pos;
};

#endif // PLAYER_H