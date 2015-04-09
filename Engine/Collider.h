#pragma once

#include "Cmn.h"
#include "Vector3.h"
namespace talga
{
	class ACollider
	{
	public:
		ACollider();
		~ACollider();
	};

	class RectCollider : public ACollider
	{
	public:
		RectCollider(F32 x, F32 y, F32 w, F32 h);
		F32 x;
		F32 y;
		F32 w;
		F32 h;
	};

	class TriCollider : public ACollider
	{
		vec3 vert1;
		vec3 vert2;
		vec3 vert3;
	};

	class CircCollider : public ACollider
	{
		F32 radius;
		F32 x;
		F32 y;
	};
}

