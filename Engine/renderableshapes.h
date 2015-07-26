#pragma once

#include "IRenderable.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Point.h"

namespace talga
{
	class RenderableRectangle : public IRenderable
	{
	public:
		RenderableRectangle(F32 width, F32 height, F32 x = 0.0f, F32 y = 0.0f, vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f));
		virtual ~RenderableRectangle();

		void setX(F32 value) { box().setX(value); box().updateVertsPosition(); }
		void setY(F32 value) { box().setY(value); box().updateVertsPosition(); }
		void setW(F32 value) { box().setW(value); box().updateVertsPosition(); }
		void setH(F32 value) { box().setH(value); box().updateVertsPosition(); }

		virtual void render(Renderer* renderer, const Camera* camera) const override;
	private:
		vec4 mColor;
	};

	class RenderablePoint :public IRenderable
	{
	public:
		RenderablePoint(fPnt point, F32 size, vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f));
		virtual ~RenderablePoint();

		void setX(F32 value) { mBase.setX(value); mBase.updateVertsPosition(); }
		void setY(F32 value) { mBase.setY(value); mBase.updateVertsPosition(); }
		void setSize(F32 value) { mBase.setW(value); mBase.setH(value); mBase.updateVertsPosition(); }
		virtual void render(Renderer* renderer, const Camera* camera) const override;
	private:
		Rectangle mBase;
		vec4 mColor;
	};

	class RenderableTriangle : public IRenderable
	{
	public:
		RenderableTriangle(TriVertList list, F32 x = 0.0f, F32 y = 0.0f, vec4 color = vec4());
		virtual ~RenderableTriangle();
		void setX(F32 value) { mBase.setX(value); mBase.updateVerts(); }
		void setY(F32 value) { mBase.setY(value); mBase.updateVerts(); }

		const Triangle& getBase() const { return mBase; }
		virtual void render(Renderer* renderer, const Camera* camera) const override;
		Triangle& getBase() { return mBase; }
	private:
		Triangle mBase;
		vec4 mColor;
	};

	typedef RenderableRectangle RdrRect;
	typedef RenderableTriangle RdrTri;
	typedef RenderablePoint RdrPnt;

}