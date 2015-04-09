#pragma once

#include "PhysSprite.h"
#include <vector>

namespace talga
{

	class Character;

	enum class SIDE { NONE, RIGHT, LEFT, TOP, BOTTOM, TOPRIGHT, TOPRIGHT_X, TOPRIGHT_Y, TOPLEFT, TOPLEFT_X, TOPLEFT_Y, BOTRIGHT, BOTRIGHT_X, BOTRIGHT_Y, BOTLEFT, BOTLEFT_X, BOTLEFT_Y };

	struct Collision
	{
		float xOverlap;
		float yOverlap;
		SIDE type;
	};

	Collision OverlapMovingStillCol(PhysSprite& moving, RectCollider& still);
	float OverlapArea(const Collision& col);
	bool ResMovingStillCol(PhysSprite& moving, const Collision& col);
	void ResMovingMovingCol(PhysSprite& moving1, PhysSprite& moving2);
	void SortCollisions(std::vector<Collision>& list);
	void FindResCollision(PhysSprite& moving, RectCollider& still);
	//std::vector<Collision> GetTileCols(PhysSprite& object);
	//void ResloveMultipleCol(PhysSprite& object);

}