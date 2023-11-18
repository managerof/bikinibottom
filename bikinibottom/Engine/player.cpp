#include "player.h"

Vec2Int Player::GetIntCoords() {
	return Vec2Int(static_cast<int>(Pos.X), static_cast<int>(Pos.Y));
}

Player::Player(Vec2 pos, double health) : Pos(pos), Health(health) {}