#include "renderableshapes.h"
#include "Renderer.h"

namespace talga
{
	RenderableRectangle::RenderableRectangle(F32 width, F32 height, F32 x, F32 y, vec4 color)
		: mBase(width, height, vec3(x,y))
		, mColor(color)
	{
	}

	void RenderableRectangle::render(Renderer* renderer, const Camera* camera) const
	{
		renderer->submit(mBase, mColor, 0.5f);
	}

	RenderableRectangle::~RenderableRectangle()
	{
	}

	RenderablePoint::RenderablePoint(fPnt point, F32 size, vec4 color)
		: mBase(size, size, vec3(point.x(), point.y()))
		, mColor(color)
	{
	}

	void RenderablePoint::render(Renderer* renderer, const Camera* camera) const
	{
		renderer->submit(mBase, mColor, 0.5f);
	}

	RenderablePoint::~RenderablePoint()
	{
	}

	RenderableTriangle::RenderableTriangle(TriVertList list, F32 x, F32 y, vec4 color)
		: mBase(list, x, y)
		, mColor(color)
	{
	}

	void RenderableTriangle::render(Renderer* renderer, const Camera* camera) const
	{
		renderer->submit(mBase, mColor, 0.5f);
	}

	RenderableTriangle::~RenderableTriangle()
	{
	}
}