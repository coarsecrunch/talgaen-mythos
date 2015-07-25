#include "Operations.h"
#include "../Rectangle.h"
#include "../Triangle.h"
#include "../IRenderable.h"

namespace talga
{
	vec2 GET_UV(F32 x, F32 xMax, F32 y, F32 yMax)
	{
		return vec2{x / xMax, (y / yMax) };
	}

	static float sign(vec2 p1, vec2 p2, vec2 p3)
	{
		return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
	}

	//for now assumes that rectangle is not oriented
	bool pointInRect(const Rectangle& rect, vec2 pnt)
	{
		if (pnt.x() < rect.getX() - rect.getW() * 0.5f)
			return false;
		if (pnt.x() > rect.getX() + rect.getW() * 0.5f)
			return false;
		if (pnt.y() < rect.getY() - rect.getH() * 0.5f)
			return false;
		if (pnt.y() > rect.getY() + rect.getH() * 0.5f)
			return false;

		return true;
	}

	bool pointInRenderableRect(const IRenderable* rect, vec3 pnt)
	{
		vec3 globalPos = rect->getGlobalPosition();
		if (pnt.x() < globalPos.x() - rect->box().getW() * 0.5f)
			return false;
		if (pnt.x() > globalPos.x() + rect->box().getW() * 0.5f)
			return false;
		if (pnt.y() < globalPos.y() - rect->box().getH() * 0.5f)
			return false;
		if (pnt.y() > globalPos.y() + rect->box().getH() * 0.5f)
			return false;

		return true;
	}

	bool pointIsInTri(const Triangle& tri, vec2 pnt)
	{
		bool b1, b2, b3;

		b1 = sign(pnt, tri.getVerts()[0], tri.getVerts()[1]) < 0.0f;
		b2 = sign(pnt, tri.getVerts()[1], tri.getVerts()[2]) < 0.0f;
		b3 = sign(pnt, tri.getVerts()[2], tri.getVerts()[0]) < 0.0f;

		return ((b1 == b2) && (b2 == b3));
	}
}
