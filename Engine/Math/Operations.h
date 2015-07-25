#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "../Point.h"
namespace talga
{
	class Rectangle;
	class Triangle;
	class IRenderable;

	bool pointInRect(const Rectangle& rect, vec2 pnt);
	bool pointInRenderableRect(const IRenderable* rect, vec3 pnt);
	bool pointIsInTri(const Triangle& tri, vec2 pnt);
  
	
	template<typename T>
	T abs(T num)
	{
		if (num >= 0)
		{
			return num;
		}

		return -num;
	}

  vec2 GET_UV(float x, float xMax, float y, float yMax);

  
}

