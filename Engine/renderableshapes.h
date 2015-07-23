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

		void setX(F32 value) { mBase.setX(value); mBase.updateVertsPosition(); }
		void setY(F32 value) { mBase.setY(value); mBase.updateVertsPosition(); }
		void setW(F32 value) { mBase.setW(value); mBase.updateVertsPosition(); }
		void setH(F32 value) { mBase.setH(value); mBase.updateVertsPosition(); }

		virtual void render(Renderer* renderer, const Camera* camera) const override;
	private:
		Rectangle mBase;
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
		RenderableTriangle(TriVertList list, F32 x = 0.0f, F32 y = 0.0f, vec4 color = vec4());
		virtual ~RenderableTriangle();
		void setX(F32 value) { mBase.setX(value); mBase.updateVertsPosition(); }
		void setY(F32 value) { mBase.setY(value); mBase.updateVertsPosition(); }

		virtual void render(Renderer* renderer, const Camera* camera) const override;

	private:
		Triangle mBase;
		vec4 mColor;
	};

	typedef RenderableRectangle RdrRect;

}