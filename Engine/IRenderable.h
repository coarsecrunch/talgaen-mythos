#pragma once

namespace talga
{
	class Renderer;

	class IRenderable
	{
	public:
		virtual void render(Renderer* renderer) const = 0;

		virtual ~IRenderable() {}
	};
}