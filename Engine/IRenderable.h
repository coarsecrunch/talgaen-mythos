#pragma once

namespace talga
{
	class Renderer;
	class Camera;

	class IRenderable
	{
	public:
		virtual void render(Renderer* renderer, const Camera* camera) const = 0;

		virtual ~IRenderable() {}
	};
}