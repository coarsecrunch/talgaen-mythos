#include "Collision.h"
#include "Rect.h"
#include <iostream>
#include "Point.h"
#include "Game.h"
#include "Map.h"

using namespace std;
#if 0
namespace talga
{

	Collision OverlapMovingStillCol(PhysSprite& moving, RectCollider& still)
	{
		F32 xProj1 = still.w;
		F32 xProj2 = moving.getCollider().w;

		F32 xDistance = abs((moving.getCollider().x + moving.getW() / 2) - (still.x + still.w / 2));
		F32 compareX = (xProj1 + xProj2) / 2;

		if (xDistance < compareX) // overlap x-axis
		{
			F32 yProj1 = still.h;
			F32 yProj2 = moving.getCollider().h;

			F32 yDistance = abs((moving.getCollider().y + moving.getH() / 2) - (still.y + still.h / 2));
			F32 compareY = (yProj1 + yProj2) / 2;
			if (yDistance < compareY)// overlap y-axis
			{
				F32 xOverlap = compareX - xDistance;
				F32 yOverlap = compareY - yDistance;

				if (xOverlap > yOverlap) //y-axis collision
				{
					if (moving.getVy() >= 0) //bottom
					{
						return Collision{ xOverlap, yOverlap, SIDE::BOTTOM };
					}
					else//top
					{
						return Collision{ xOverlap, yOverlap, SIDE::TOP };
					}
				}
				else if (yOverlap > xOverlap) //x-axis collision
				{
					if (moving.getVx() > 0) //RIGHT
					{
						return Collision{ xOverlap, yOverlap, SIDE::RIGHT };
					}
					else //right
					{
						return Collision{ xOverlap, yOverlap, SIDE::LEFT };
					}
				}
			}
		}

		return Collision{ 0, 0, SIDE::NONE };
	}

	F32 OverlapArea(const Collision& col)
	{
		return col.xOverlap * col.yOverlap;
	}

	bool ResMovingStillCol(PhysSprite& moving, const Collision& col)
	{
		if (col.type != SIDE::NONE)
		{

			moving.setHorizontalWalled(false);
			if (col.type == SIDE::RIGHT)
			{
				moving.setHorizontalWalled(true);
				moving.setX(moving.getX() - col.xOverlap);
			}
			else if (col.type == SIDE::LEFT)
			{
				moving.setHorizontalWalled(true);
				moving.setX(moving.getX() + col.xOverlap);
			}
			else if (col.type == SIDE::TOP)
			{
				moving.setVerticalWalled(true);
				moving.setY(moving.getY() + col.yOverlap);
			}
			else if (col.type == SIDE::BOTTOM)
			{
				moving.setVerticalWalled(true);
				moving.setHasJumped(false);
				moving.setVy(0);
				moving.setY(moving.getY() - col.yOverlap);
			}
			moving.setIsColliding(true);
			return true;
		}
		else
		{
			return false;
		}
		moving.setIsColliding(false);
		moving.setVerticalWalled(false);
	}

	void ResMovingMovingCol(PhysSprite& moving1, PhysSprite& moving2)
	{
		F32 xProj1 = moving1.getCollider().w;
		F32 xProj2 = moving2.getCollider().w;

		F32 xDistance = abs((moving1.getCollider().x + moving1.getCollider().w / 2) - (moving2.getCollider().x + moving2.getCollider().w / 2));
		F32 compareX = (xProj1 + xProj2) / 2;



		if (xDistance < compareX) // overlap x-axis
		{
			F32 yProj1 = moving1.getCollider().h;
			F32 yProj2 = moving2.getCollider().h;

			F32 yDistance = abs((moving1.getCollider().y + moving1.getCollider().h / 2) - (moving2.getCollider().y + moving2.getCollider().h / 2));
			F32 compareY = (yProj1 + yProj2) / 2;
			if (yDistance < compareY)// overlap y-axis
			{
				moving1.setIsColliding(true);
				moving2.setIsColliding(true);
				F32 xOverlap = compareX - xDistance;
				F32 yOverlap = compareY - yDistance;

				if (xOverlap > yOverlap) //y-axis collision
				{
					float ratio = 0.0f;
					float leftOver = 0.0f;
					if (abs(moving1.getVy()) > abs(moving2.getVy()))
					{
						ratio = abs(moving2.getVy() / moving1.getVy());
						leftOver = 1.0f - ratio;
						if (moving1.getVy() > 0 && moving2.getVy() > 0) // 1
						{
							moving1.setY(moving1.getY() + ratio * yOverlap);
							moving2.setY(moving2.getY() - leftOver * yOverlap);
						}
						else if (moving1.getVy() < 0 && moving2.getVy() < 0) // 2
						{
							moving1.setY(moving1.getY() - ratio * yOverlap);
							moving2.setY(moving2.getY() + leftOver * yOverlap);
						}
						else if (moving1.getVy() > 0 && moving2.getVy() < 0) //3
						{
							moving1.setY(moving1.getY() - ratio * yOverlap);
							moving2.setY(moving2.getY() + leftOver * yOverlap);
						}
						else if (moving1.getVy() < 0 && moving2.getVy() > 0) //4
						{
							moving1.setY(moving1.getY() + ratio * yOverlap);
							moving2.setY(moving2.getY() - leftOver * yOverlap);
						}
						else if (moving1.getVy() < 0 && moving2.getVy() == 0)//5
						{
							/*	if (moving2.isVerticalWalled())
								{
								moving1.setY(moving1.getY() + yOverlap);
								moving1.setHasJumped(false);
								moving1.setVy(0.0f);
								}
								else*/
							{
								moving1.setY(moving1.getY() + ratio * yOverlap);
								//moving1.setHasJumped(false);
								//moving2.setY(moving2.getY() - leftOver * yOverlap);
							}

						}
						else if (moving1.getVy() > 0 && moving2.getVy() == 0) //6
						{
							if (moving2.isVerticalWalled())
							{
								moving1.setY(moving1.getY() - yOverlap);
								moving1.setHasJumped(false);
								moving1.setVy(0.0f);
							}
							else
							{
								moving1.setHasJumped(false);
								moving1.setY(moving1.getY() - ratio * yOverlap);
								moving2.setY(moving2.getY() + leftOver * yOverlap);
							}

						}
					}
					else
					{
						ratio = abs(moving1.getVy() / moving2.getVy());
						leftOver = 1.0f - ratio;
						if (moving1.getVy() > 0 && moving2.getVy() > 0) // 1
						{
							moving1.setY(moving1.getY() + leftOver * yOverlap);
							moving2.setY(moving2.getY() - ratio * yOverlap);

						}
						else if (moving1.getVy() < 0 && moving2.getVy() < 0) // 2
						{
							moving1.setY(moving1.getY() - leftOver * yOverlap);
							moving2.setY(moving2.getY() + ratio * yOverlap);
						}
						else if (moving1.getVy() > 0 && moving2.getVy() < 0) //3
						{
							moving1.setY(moving1.getY() - leftOver * yOverlap);
							moving2.setY(moving2.getY() + ratio * yOverlap);
						}
						else if (moving1.getVy() < 0 && moving2.getVy() > 0) //4
						{
							moving1.setY(moving1.getY() + leftOver * yOverlap);
							moving2.setY(moving2.getY() - ratio * yOverlap);
						}
						else if (moving1.getVy() == 0 && moving2.getVy() < 0)//5
						{
							/*
							if (moving1.isVerticalWalled())
							{
							moving2.setY(moving2.getY() + yOverlap);
							moving2.setVy(0.0f);
							}
							else*/
							{
								//moving1.setY(moving1.getY() - leftOver * yOverlap);
								moving2.setY(moving2.getY() + ratio * yOverlap);
							}

						}
						else if (moving1.getVy() == 0 && moving2.getVy() > 0) //6
						{
							if (moving1.isVerticalWalled())
							{
								moving2.setY(moving2.getY() - yOverlap);
								moving2.setVy(0.0f);
								moving2.setHasJumped(false);
							}
							else
							{
								moving1.setY(moving1.getY() + leftOver * yOverlap);
								moving2.setY(moving2.getY() - ratio * yOverlap);
								moving2.setHasJumped(false);
							}

						}
					}
				}
				else if (yOverlap > xOverlap) //x-axis collision
				{
					float ratio = 0.0f;
					float leftOver = 0.0f;
					if (abs(moving1.getVx()) > abs(moving2.getVx()))
					{
						ratio = abs(moving2.getVx() / moving1.getVx());
						leftOver = 1.0f - ratio;
						if (moving1.getVx() > 0 && moving2.getVx() > 0) // 1
						{
							moving1.setX(moving1.getX() - ratio * xOverlap);
							moving2.setX(moving2.getX() + leftOver * xOverlap);
						}
						else if (moving1.getVx() < 0 && moving2.getVx() < 0) // 2
						{
							moving1.setX(moving1.getX() + ratio * xOverlap);
							moving2.setX(moving2.getX() - leftOver * xOverlap);
						}
						else if (moving1.getVx() > 0 && moving2.getVx() < 0) //3
						{
							moving1.setX(moving1.getX() - ratio * xOverlap);
							moving2.setX(moving2.getX() + leftOver * xOverlap);
						}
						else if (moving1.getVx() < 0 && moving2.getVx() > 0) //4
						{
							moving1.setX(moving1.getX() + ratio * xOverlap);
							moving2.setX(moving2.getX() - leftOver * xOverlap);
						}
						else if (moving1.getVx() < 0 && moving2.getVx() == 0)//5
						{
							moving1.setX(moving1.getX() + ratio * xOverlap);
							moving2.setX(moving2.getX() - leftOver * xOverlap);
						}
						else if (moving1.getVx() > 0 && moving2.getVx() == 0) //6
						{
							moving1.setX(moving1.getX() - ratio * xOverlap);
							moving2.setX(moving2.getX() + leftOver * xOverlap);
						}
					}
					else
					{
						ratio = abs(moving1.getVx() / moving2.getVx());
						leftOver = 1.0f - ratio;
						if (moving1.getVx() > 0 && moving2.getVx() > 0) // 1
						{
							moving1.setX(moving1.getX() - leftOver * xOverlap);
							moving2.setX(moving2.getX() + ratio * xOverlap);
						}
						else if (moving1.getVx() < 0 && moving2.getVx() < 0) // 2
						{
							moving1.setX(moving1.getX() + leftOver * xOverlap);
							moving2.setX(moving2.getX() - ratio * xOverlap);
						}
						else if (moving1.getVx() > 0 && moving2.getVx() < 0) //3
						{
							moving1.setX(moving1.getX() - leftOver * xOverlap);
							moving2.setX(moving2.getX() + ratio * xOverlap);
						}
						else if (moving1.getVx() < 0 && moving2.getVx() > 0) //4
						{
							moving1.setX(moving1.getX() + leftOver * xOverlap);
							moving2.setX(moving2.getX() - ratio * xOverlap);
						}
						else if (moving1.getVx() == 0 && moving2.getVx() < 0)//5
						{
							moving1.setX(moving1.getX() - leftOver * xOverlap);
							moving2.setX(moving2.getX() + ratio * xOverlap);
						}
						else if (moving1.getVx() == 0 && moving2.getVx() > 0) //6
						{
							moving1.setX(moving1.getX() + leftOver * xOverlap);
							moving2.setX(moving2.getX() - ratio * xOverlap);
						}
					}
				}

				return; // must return so that the isColliding is set to false
			}
		}

		moving1.setIsColliding(false);
		moving2.setIsColliding(false);
	}*/

	void FindResCollision(PhysSprite& moving, RectCollider& still)
	{
		Collision col = OverlapMovingStillCol(moving, still);
		ResMovingStillCol(moving, col);
	}
	/*
	std::vector<Collision> GetTileCols(PhysSprite& object)
	{
	Point player_pos = object.getGame()->currentMap()->CartToTile(static_cast<int>(object.getX() + object.getW() * 0.5), static_cast<int>(object.getY() + object.getH() *0.5));
	//Tile check;
	Rect colrect;
	std::vector<Collision> collisions;
	Point pos;
	int numChecks = 0;
	for (int x = -1, i = 0; x < 2; ++x)
	{
	for (int y = -1; y < 2; ++y, ++i)
	{
	Tile* check = object.getGame()->currentMap()->TileAt(player_pos.x + x, player_pos.y + y);
	if (check == nullptr) continue;
	if (check->solid == true)
	{
	pos = object.getGame()->currentMap()->TileToCart(player_pos.x + x, player_pos.y + y);
	colrect.x = pos.x;
	colrect.y = pos.y;
	colrect.w = object.getGame()->currentMap()->tileWidth();
	colrect.h = object.getGame()->currentMap()->tileHeight();
	numChecks++;

	Collision col = OverlapMovingStillCol(object, colrect);
	if (col.type == SIDE::NONE) continue;
	collisions.push_back(col);
	}
	}
	}
	SortCollisions(collisions);
	return collisions;
	}*/

	void SortCollisions(std::vector<Collision>& list)
	{
		for (int i = list.size() - 1; i >= 0; --i)
		{
			for (int j = 1; j <= i; ++j)
			{
				if (OverlapArea(list[j - 1]) > OverlapArea(list[j]))
				{
					Collision temp = list[j];
					list[j] = list[j - 1];
					list[j - 1] = temp;
				}
			}
		}
	}

	/*
	void ResloveMultipleCol(PhysSprite& object)
	{
	std::vector<Collision> collisions = GetTileCols(object);
	int indexLargestHorizontal = 0;
	int numHorizontalCol = 0;

	while (collisions.size() > 0)
	{
	if (collisions[collisions.size() - 1].type == SIDE::BOTTOM)
	{
	object.setVy(0);
	//object.ResetJumps();
	}
	else if (collisions[collisions.size() - 1].type == SIDE::TOP)
	{
	//object.setVy(0);
	}

	ResMovingStillCol(object, collisions[collisions.size() - 1]);
	collisions = GetTileCols(object);
	}

	}*/

}
#endif