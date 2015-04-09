#include "Collider.h"

namespace talga
{
	ACollider::ACollider()
	{
	}


	ACollider::~ACollider()
	{
	}


	RectCollider::RectCollider(F32 x, F32 y, F32 w, F32 h) :
		x(x), y(y), w(w), h(h)
	{

	}
}