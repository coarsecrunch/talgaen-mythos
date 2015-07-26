#include "renderableshapes.h"
#include "Renderer.h"

namespace talga
{
	RenderableRectangle::RenderableRectangle(F32 width, F32 height, F32 x, F32 y, vec4 color)
		: mColor(color)
	{
		box().setW(width);
		box().setH(height);
		box().setPosition(vec3(x, y));
		box().updateVertsPosition();
	}

	void RenderableRectangle::render(Renderer* renderer, const Camera* camera) const
	{
		renderer->submit(box(), mColor, 0.7f);
		IRenderable::render(renderer, camera);
	}

	RenderableRectangle::~RenderableRectangle()
	{
	}

	RenderablePoint::RenderablePoint(fPnt point, F32 size, vec4 color)
		: mBase(size, size, vec3(point.x(), point.y()))
		, mColor(color)
	{
		mBase.updateVertsPosition();
	}

	void RenderablePoint::render(Renderer* renderer, const Camera* camera) const
	{
		renderer->submit(mBase, mColor, 1.0f);
		IRenderable::render(renderer, camera);
	}

	RenderablePoint::~RenderablePoint()
	{
	}

	RenderableTriangle::RenderableTriangle(TriVertList list, F32 x, F32 y, vec4 color)
		: mBase(list, x, y)
		, mColor(color)
	{
		mBase.updateVerts();
	}

	void RenderableTriangle::render(Renderer* renderer, const Camera* camera) const
	{
		renderer->submit(mBase, mColor, 0.7f);
		renderer->tStackPush(mBase.getTransformationMatrix());
		IRenderable::render(renderer, camera);
		renderer->tStackPop();
	}

	RenderableTriangle::~RenderableTriangle()
	{
	}
}
